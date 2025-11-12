/*
  Copyright 2024-2025 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/Cs32g051EcLib.h>
#include "PlatformSmbios.h"

#define FIXED_STRINGS                                           \
  "Radxa Computer (Shenzhen) Co., Ltd.\0" /* Manufacturer */    \
  "PD    Firmware\0"                      /* Component Name */  \
  "NULL\0"                                /* Firmware ID & RELEASE Date & LowestSupportedVersion == NULL */

#define DEFAULT_VERSION                                         \
  "Undefined\0"                           /* Default FirmwareVersion */

#define VERSION_MAX_SIZE 256

#pragma pack(1)
typedef union {
  struct {
    CHAR8 FixedStrings[sizeof (FIXED_STRINGS) - 1];
    CHAR8 Version[VERSION_MAX_SIZE];
  } Components;
  CHAR8 RawData[sizeof (FIXED_STRINGS) - 1 + VERSION_MAX_SIZE];
} TYPE45_STRINGS;

typedef struct {
  SMBIOS_TABLE_TYPE45 Base;
  TYPE45_STRINGS      Strings;
} PLATFORM_SMBIOS_TYPE45;
#pragma pack()

STATIC PLATFORM_SMBIOS_TYPE45 mPlatformDefaultType45 = {
  {
    {                                   // SMBIOS_STRUCTURE Hdr
      SMBIOS_TYPE_FIRMWARE_INVENTORY_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE45),      // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    0x02,                          //FirmwareComponentName;
    0x04,                          //FirmwareVersion;
    VersionFormatTypeFreeForm,     //FirmwareVersionFormat;    ///< The enumeration value from FIRMWARE_INVENTORY_VERSION_FORMAT_TYPE
    0x03,                          //FirmwareId;
    FirmwareIdFormatTypeFreeForm,  //FirmwareIdFormat;         ///< The enumeration value from FIRMWARE_INVENTORY_FIRMWARE_ID_FORMAT_TYPE.
    0x03,                          //ReleaseDate;
    0x01,                          //Manufacturer;
    0x03,                          //LowestSupportedVersion;
    SIZE_256KB,                    //ImageSize;
    {
      .Updatable = 0,
      .WriteProtected = 0,
      .Reserved = 0
    },                     //Characteristics; Updatable & WriteProtected
    FirmwareInventoryStateEnabled, //State;                    ///< The enumeration value from FIRMWARE_INVENTORY_STATE.
    1                   //AssociatedComponentCount; Max Firmware components that can be associated with this firmware inventory information structure.
  },
  {
    .RawData = FIXED_STRINGS DEFAULT_VERSION,
  }
};

EFI_STATUS
AddSmbiosType45 (
  IN EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  EFI_STATUS         Status;
  EFI_SMBIOS_HANDLE  SmbiosHandle;
  CS32G051_RESULT_CMD_GET_IC_STATUS IcStatus;

  Status = PdGetIcStatus(0, &IcStatus);
  if (!EFI_ERROR (Status)) {
    AsciiSPrint (
      mPlatformDefaultType45.Strings.Components.Version,
      sizeof (mPlatformDefaultType45.Strings.Components.Version),
      "Boot: v%d.%d.%d\nApp: v%d.%d.%d",
      IcStatus.BootVersionMajor,
      IcStatus.BootVersionMinor,
      IcStatus.BootVersionPatch,
      IcStatus.AppVersionMajor,
      IcStatus.AppVersionMinor,
      IcStatus.AppVersionPatch
      );
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add (
                           Smbios,
                           NULL,
                           &SmbiosHandle,
                           (EFI_SMBIOS_TABLE_HEADER *)&mPlatformDefaultType45
                           );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "[%a]:[%dL] Smbios Type45 Table Log Failed! %r \n",
       __FUNCTION__,
       DEBUG_LINE_NUMBER,
       Status
      )
      );
  }

  return EFI_SUCCESS;
}
