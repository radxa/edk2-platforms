/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MISC_LIB_H_
#define _MISC_LIB_H_
#include <Uefi.h>

#pragma pack(1)
typedef struct {
  UINT32    Votlage0;
  UINT32    Votlage1;
  UINT32    Votlage2;
  UINT32    Votlage3;
  UINT32    Votlage4;
  UINT32    Votlage5;
  UINT32    Votlage6;
  UINT32    Votlage7;
} VOLTAGE_INFO;

typedef struct {
  UINT8    ManufacturerId; // 制造商ID (如: 0xEF for Winbond, 0xC2 for Macronix)
  UINT8    MemoryType;     // 存储器类型 (如: 0x40 for SPI NOR Flash)
  UINT8    Capacity;       // 容量标识 (如: 0x17 for 128Mb)
} FLASH_JEDEC_ID;
#pragma pack()

EFI_STATUS
FctMemoryTestEntry (
  IN UINT8              TestAlgorithm,
  OUT PHYSICAL_ADDRESS  *Address
  );

EFI_STATUS
FctGetVoltageInfoFromPmic (
  VOLTAGE_INFO  *pVoltageInfo
  );

EFI_STATUS
FctGetSpiNorFlashInfo (
  OUT FLASH_JEDEC_ID  *FlashJedecId,
  OUT UINT8           *FlashSize
  );

#endif
