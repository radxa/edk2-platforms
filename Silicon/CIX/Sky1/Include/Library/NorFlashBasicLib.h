/** @file  NorFlashBasicLib.h

  Copyright 2024 - 2025 Cix Technology Group Co., Ltd. All Rights Reserved
  Copyright (c) 2022-2023, CIX, Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __NOR_FLASH_BASIC_LIB_H__
#define __NOR_FLASH_BASIC_LIB_H__

#define GENMASK(high, low)  (((~(0)) - ((1) << (low)) + 1) & (~(0) >> (32 - 1 - (high))))

/* Supported SPI protocols */
#define SNOR_MODE_IS_DTR      BIT24                        /* Double Transfer Rate */
#define SNOR_MODE_INST_MASK   GENMASK(23, 16)
#define SNOR_MODE_INST_SHIFT  16
#define SNOR_MODE_INST(bits)  ((((UINT32)(bits)) << SNOR_MODE_INST_SHIFT) & SNOR_MODE_INST_MASK)

#define SNOR_MODE_ADDR_MASK   GENMASK(15, 8)
#define SNOR_MODE_ADDR_SHIFT  8
#define SNOR_MODE_ADDR(bits)  ((((UINT32)(bits)) << SNOR_MODE_ADDR_SHIFT) & SNOR_MODE_ADDR_MASK)

#define SNOR_MODE_DATA_MASK   GENMASK(7, 0)
#define SNOR_MODE_DATA_SHIFT  0
#define SNOR_MODE_DATA(bits)  ((((UINT32)(bits)) << SNOR_MODE_DATA_SHIFT) & SNOR_MODE_DATA_MASK)

#define SNOR_MODE_STR(inst_bits, addr_bits, data_bits)  \
                                    (SNOR_MODE_INST(inst_bits) | SNOR_MODE_ADDR(addr_bits) | SNOR_MODE_DATA(data_bits))
#define SNOR_MODE_DTR(inst_bits, addr_bits, data_bits)  \
                                    (SNOR_MODE_IS_DTR | SNOR_MODE_STR(inst_bits, addr_bits, data_bits))

typedef enum _SPI_NOR_MODE {
  SNOR_1_1_1_MODE = SNOR_MODE_STR (1, 1, 1),
  SNOR_1_1_2_MODE = SNOR_MODE_STR (1, 1, 2),
  SNOR_1_1_4_MODE = SNOR_MODE_STR (1, 1, 4),
  SNOR_1_1_8_MODE = SNOR_MODE_STR (1, 1, 8),
  SNOR_1_2_2_MODE = SNOR_MODE_STR (1, 2, 2),
  SNOR_1_4_4_MODE = SNOR_MODE_STR (1, 4, 4),
  SNOR_1_8_8_MODE = SNOR_MODE_STR (1, 8, 8),
  SNOR_2_2_2_MODE = SNOR_MODE_STR (2, 2, 2),
  SNOR_4_4_4_MODE = SNOR_MODE_STR (4, 4, 4),
  SNOR_8_8_8_MODE = SNOR_MODE_STR (8, 8, 8),

  SNOR_1_1_1_DTR_MODE = SNOR_MODE_DTR (1, 1, 1),
  SNOR_1_2_2_DTR_MODE = SNOR_MODE_DTR (1, 2, 2),
  SNOR_1_4_4_DTR_MODE = SNOR_MODE_DTR (1, 4, 4),
  SNOR_1_8_8_DTR_MODE = SNOR_MODE_DTR (1, 8, 8),
} SPI_NOR_MODE;

EFI_STATUS
NorFlashWaitFlashIdle (
  VOID
  );

EFI_STATUS
NorFlashReadEnhVolConfigReg (
  OUT UINT8  *ConfigReg
  );

EFI_STATUS
NorFlashWriteEnhVolConfigReg (
  IN UINT8  *ConfigReg
  );

EFI_STATUS
NorFlashWriteEnable (
  VOID
  );

EFI_STATUS
NorFlashWriteDisable (
  VOID
  );

EFI_STATUS
NorFlashReadId (
  OUT UINT8  *Id
  );

EFI_STATUS
NOrFlashReadStatusReg (
  IN  UINT8  Idx,
  OUT UINT8  *StatReg
  );

EFI_STATUS
NorFlashWriteStatusReg (
  IN UINT8  Idx,
  IN UINT8  *StatusReg
  );

EFI_STATUS
NorFlashEraseChip (
  VOID
  );

EFI_STATUS
NorFlashDualEnable (
  IN UINT8  *JedecId
  );

EFI_STATUS
NorFlashQuadEnable (
  IN UINT8  *JedecId
  );

EFI_STATUS
NOrFlashBasicRead (
  IN UINTN   Address,
  IN UINT32  Length,
  OUT UINT8  *Buffer
  );

EFI_STATUS
NorFlashBasicWrite (
  IN UINTN   Address,
  IN UINT32  Length,
  IN UINT8   *Buffer
  );

EFI_STATUS
NorFlashBasicErase (
  IN UINTN    Address,
  IN UINT32   Length,
  IN BOOLEAN  WholeChip
  );

EFI_STATUS
NorFlashBasicWriteReadTransaction (
  IN SPI_NOR_MODE  Format,
  IN BOOLEAN       DualRate,
  IN UINT8         Command,
  IN UINT32        Address,
  IN UINT32        AddressSize,
  IN UINT8         *DataOut,
  IN UINT32        DataOutSize,
  IN UINT32        DummyCycles,
  OUT UINT8        *DataIn,
  IN UINT32        DataInSize
  );

#endif /* __NOR_FLASH_BASIC_LIB_H__ */
