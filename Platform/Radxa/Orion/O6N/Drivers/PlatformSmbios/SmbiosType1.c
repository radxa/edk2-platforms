/*
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/GpioLib.h>
#include "PlatformSmbios.h"

#define FIXED_STRINGS                                         \
  "Radxa Computer (Shenzhen) Co., Ltd.\0" /* Manufacturer */  \
  "Radxa Orion O6N\0"                     /* Product Name */  \
  "1.0\0"                                 /* Version */       \
  "Radxa System Serial Number\0"          /* Serial number */ \
  "Orion O6 Series\0"                     /* System Family */

#define DEFAULT_VERSION                                       \
  "Undefined\0"                           /* SKUNumber */

#define VERSION_MAX_SIZE 256

#pragma pack(1)
typedef union {
  struct {
    CHAR8 FixedStrings[sizeof (FIXED_STRINGS) - 1];
    CHAR8 Version[VERSION_MAX_SIZE];
  } Components;
  CHAR8 RawData[sizeof (FIXED_STRINGS) - 1 + VERSION_MAX_SIZE];
} TYPE1_STRINGS;

typedef struct {
  SMBIOS_TABLE_TYPE1  Base;
  TYPE1_STRINGS       Strings;
} PLATFORM_SMBIOS_TYPE1;
#pragma pack()

// System information (section 7.2)
STATIC PLATFORM_SMBIOS_TYPE1  mPlatformDefaultType1 = {
  {
    {
      // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE1),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,     // Manufacturer
    2,     // Product Name
    3,     // Version
    4,     // Serial
    { 0xFFFFFFFF,
        0xFFFF,
        0xFFFF,
        { 0xFF,      0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
    },                                                      // UUID
    SystemWakeupTypePowerSwitch,                            // Wakeup type
    6,                                                      // SKU
    5,                                                      // Family
  },
  // Text strings (unformatted),
  {
    .RawData = FIXED_STRINGS DEFAULT_VERSION,
  }
};

/**
  GPIOs is ordered from the lowest bit to the highest bit.
**/
EFI_STATUS
GpioGetMultiple (
  IN UINT32   *GPIOs,
  IN UINT32   Count,
  OUT UINT32  *Output
  )
{
  UINT32              Index;
  EFI_STATUS          Status;
  IO_INOUT_VALUE_SEL  Value;

  *Output = 0;

  for (Index = 0; Index < Count; Index++) {
    Status = GpioGet (GPIOs[Index], &Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGet failed for %d: %r\n", __FUNCTION__, Index, Status));
      *Output = -1;
      return Status;
    } else if (Value == INOUT_VALUE_DEFAULT) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGet returned default value for %d\n", __FUNCTION__, Index));
      *Output = -1;
      return EFI_DEVICE_ERROR;
    }

    DEBUG ((DEBUG_INFO, "%a: GPIOs[%d] = %d\n", __FUNCTION__, Index, Value));
    *Output |= (Value << Index);
    DEBUG ((DEBUG_INFO, "%a: Output = %d\n", __FUNCTION__, *Output));
  }

  DEBUG ((DEBUG_INFO, "%a: final Output = %d\n", __FUNCTION__, *Output));
  return EFI_SUCCESS;
}

EFI_STATUS
AddSmbiosType1 (
  IN EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  UINT32                   MemType;

  Status = GpioGetMultiple ((UINT32[]){49, 50, 57, 60}, 4, &MemType);
  if (!EFI_ERROR (Status)) {
    AsciiSPrint (
      mPlatformDefaultType1.Strings.Components.Version,
      sizeof (mPlatformDefaultType1.Strings.Components.Version),
      "%d",
      MemType
      );
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add (
                           Smbios,
                           NULL,
                           &SmbiosHandle,
                           (EFI_SMBIOS_TABLE_HEADER *)&mPlatformDefaultType1
                           );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "[%a]:[%dL] Smbios Type1 Table Log Failed! %r \n",
       __FUNCTION__,
       DEBUG_LINE_NUMBER,
       Status
      )
      );
  }

  return EFI_SUCCESS;
}
