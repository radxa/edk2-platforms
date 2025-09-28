/*
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include "PlatformSmbios.h"

#define TYPE0_STRINGS                                                                    \
  "Cix Technology Group Co., Ltd.\0" /* Vendor */                                        \
  "1.0\0"                            /* BiosVersion */                                   \
  RELEASE_DATE_TEMPLATE              /* BiosReleaseDate */

#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE0    Base;
  INT8                  Strings[sizeof (TYPE0_STRINGS)];
} PLATFORM_SMBIOS_TYPE0;
#pragma pack()

// BIOS information (section 7.1)
STATIC PLATFORM_SMBIOS_TYPE0  mPlatformDefaultType0 = {
  {
    {
      // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_BIOS_INFORMATION,       // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE0),            // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,          // SMBIOS_TABLE_STRING       Vendor
    2,          // SMBIOS_TABLE_STRING       BiosVersion
    0,          // UINT16                    BiosSegment
    3,          // SMBIOS_TABLE_STRING       BiosReleaseDate
    0x7F,       // UINT8                     BiosSize
    {
      0, 0, 0, 0, 0, 0,
      1,       // PCI supported
      0,
      1,       // PNP supported
      0,
      1,       // BIOS upgradable
      0, 0, 0,
      0,       // Boot from CD
      1,       // selectable boot
    },         // MISC_BIOS_CHARACTERISTICS BiosCharacteristics
    {
      // BIOSCharacteristicsExtensionBytes[2]
      0x1,       // Acpi supported
      0xC,       // TargetContentDistribution, UEFI
    },
    0,       // UINT8                     SystemBiosMajorRelease
    1,       // UINT8                     SystemBiosMinorRelease
    0xFF,    // UINT8                     EmbeddedControllerFirmwareMajorRelease
    0xFF     // UINT8                     EmbeddedControllerFirmwareMinorRelease
  },
  // Text strings (unformatted area)
  TYPE0_STRINGS
};

typedef struct {
  CHAR8    MonthNameStr[4]; // example "Jan", Compiler build date, month
  CHAR8    DigitStr[3];     // example "01", Smbios date format, month
} MonthStringDig;

STATIC MonthStringDig  MonthMatch[12] = {
  { "Jan", "01" },
  { "Feb", "02" },
  { "Mar", "03" },
  { "Apr", "04" },
  { "May", "05" },
  { "Jun", "06" },
  { "Jul", "07" },
  { "Aug", "08" },
  { "Sep", "09" },
  { "Oct", "10" },
  { "Nov", "11" },
  { "Dec", "12" }
};

STATIC
VOID
ConstructBuildDate (
  OUT CHAR8  *DateBuf
  )
{
  UINTN  i;

  // GCC __DATE__ format is "Feb  2 1996"
  // If the day of the month is less than 10, it is padded with a space on the left
  CHAR8  *BuildDate = __DATE__;

  // SMBIOS spec date string: MM/DD/YYYY
  CHAR8  SmbiosDateStr[sizeof (RELEASE_DATE_TEMPLATE)] = { 0 };

  SmbiosDateStr[sizeof (RELEASE_DATE_TEMPLATE) - 1] = '\0';

  SmbiosDateStr[2] = '/';
  SmbiosDateStr[5] = '/';

  // Month
  for (i = 0; i < sizeof (MonthMatch) / sizeof (MonthMatch[0]); i++) {
    if (AsciiStrnCmp (&BuildDate[0], MonthMatch[i].MonthNameStr, AsciiStrLen (MonthMatch[i].MonthNameStr)) == 0) {
      CopyMem (&SmbiosDateStr[0], MonthMatch[i].DigitStr, AsciiStrLen (MonthMatch[i].DigitStr));
      break;
    }
  }

  // Day
  CopyMem (&SmbiosDateStr[3], &BuildDate[4], 2);
  if (BuildDate[4] == ' ') {
    // day is less then 10, SAPCE filed by compiler, SMBIOS requires 0
    SmbiosDateStr[3] = '0';
  }

  // Year
  CopyMem (&SmbiosDateStr[6], &BuildDate[7], 4);

  CopyMem (DateBuf, SmbiosDateStr, AsciiStrLen (RELEASE_DATE_TEMPLATE));
}

EFI_STATUS
AddSmbiosType0 (
  IN EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  EFI_STATUS         Status;
  EFI_SMBIOS_HANDLE  SmbiosHandle;
  UINTN              StrSize, StringNumber;
  CHAR8              *ReleaseDateBuf = NULL;

  mPlatformDefaultType0.Base.SystemBiosMajorRelease =
    (PcdGet32 (PcdFirmwareRevision) >> 16) & 0xFF;
  mPlatformDefaultType0.Base.SystemBiosMinorRelease =
    PcdGet32 (PcdFirmwareRevision) & 0xFF;

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add (
                           Smbios,
                           NULL,
                           &SmbiosHandle,
                           (EFI_SMBIOS_TABLE_HEADER *)&mPlatformDefaultType0
                           );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a]:[%dL] Smbios Type0 Table Log Failed! %r \n",
      __FUNCTION__,
      DEBUG_LINE_NUMBER,
      Status
      ));

    return Status;
  }

  StrSize        = AsciiStrSize (RELEASE_DATE_TEMPLATE);
  ReleaseDateBuf = AllocateZeroPool (StrSize);
  StringNumber   = 3;// BIOS release date

  if (ReleaseDateBuf != NULL) {
    ConstructBuildDate (ReleaseDateBuf);
    Status = Smbios->UpdateString (Smbios, &SmbiosHandle, &StringNumber, ReleaseDateBuf);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to update BIOS release date."));
    }

    FreePool (ReleaseDateBuf);
  }

  return Status;
}
