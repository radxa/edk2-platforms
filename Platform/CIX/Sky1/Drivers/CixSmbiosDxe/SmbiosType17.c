/*
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include "CixSmbiosDxe.h"
#include <Protocol/MemOutputBuffer.h>

#define TYPE17_STRINGS                                                         \
  "Top - on board\0" /* location */                                            \
  "BANK 0\0"         /* bank description */                                    \
  "BANK 1\0"         /* bank description */                                    \
  "BANK 2\0"         /* bank description */                                    \
  "BANK 3\0"         /* bank description */

#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE17    Base;
  UINT8                  Strings[sizeof (TYPE17_STRINGS)];
} CIX_TYPE17;
#pragma pack()

// Memory device
STATIC CIX_TYPE17  mCixDefaultType17 = {
  {
    {
      // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_MEMORY_DEVICE,       // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE17),        // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    SMBIOS_HANDLE_MEMORY,                            // array to which this module belongs
    0xFFFE,                                          // no errors
    32,                                              // unknown total width
    32,                                              // unknown data width
    0,                                               // Size
    0x0b,                                            // Row of chips
    0,                                               // not part of a set
    1,                                               // Device locator
    2,                                               // bank 0
    MemoryTypeUnknown,                               // memory type
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, // unbuffered
    0,                                               // speed
    0,                                               // varies between diffrent production runs
    0,                                               // serial
    0,                                               // asset tag
    0,                                               // part number
    0,                                               // rank
  },
  TYPE17_STRINGS
};

EFI_STATUS
AddSmbiosType17 (
  IN EFI_SMBIOS_PROTOCOL           *Smbios,
  IN const MEM_INIT_OUTPUT_BUFFER  *MemoryInfo
  )
{
  EFI_STATUS         Status;
  EFI_SMBIOS_HANDLE  SmbiosHandle;
  UINT8              MemoryDevNum;
  UINT8              i;
  UINT16             MemorySize;

  MemoryDevNum = COUNT_MEMORY_DEVICE_NUMBER (MemoryInfo->ChannelMask);
  MemorySize   = MemoryInfo->TotalSize/MemoryDevNum;
  if (MemorySize >= 0x7fff) {
    mCixDefaultType17.Base.Size         = 0x7fff;
    mCixDefaultType17.Base.ExtendedSize = MemorySize;
  } else {
    mCixDefaultType17.Base.Size = MemorySize;
  }

  if (MemoryInfo->DdrType == DDR_TYPE_LPDDR4X) {
    mCixDefaultType17.Base.MemoryType = MemoryTypeLpddr4;
  } else if (MemoryInfo->DdrType == DDR_TYPE_LPDDR5) {
    mCixDefaultType17.Base.MemoryType = MemoryTypeLpddr5;
  }

  mCixDefaultType17.Base.Speed                      = MemoryInfo->MaxFreq*2;
  mCixDefaultType17.Base.Attributes                 = MemoryInfo->RanksPerChannel;
  mCixDefaultType17.Base.ConfiguredMemoryClockSpeed = MemoryInfo->MaxFreq*2;
  mCixDefaultType17.Base.MemoryTechnology           = 0x03;

  for (i = 0; i < MemoryDevNum; i++) {
    mCixDefaultType17.Base.BankLocator = 2+i;
    SmbiosHandle                       = SMBIOS_HANDLE_PI_RESERVED;
    Status                             = Smbios->Add (
                                                   Smbios,
                                                   NULL,
                                                   &SmbiosHandle,
                                                   (EFI_SMBIOS_TABLE_HEADER *)&mCixDefaultType17
                                                   );
    if (EFI_ERROR (Status)) {
      DEBUG (
        (
         DEBUG_ERROR,
         "[%a]:[%dL] Smbios Type17 Table Log Failed! %r \n",
         __FUNCTION__,
         DEBUG_LINE_NUMBER,
         Status
        )
        );
    }
  }

  return EFI_SUCCESS;
}
