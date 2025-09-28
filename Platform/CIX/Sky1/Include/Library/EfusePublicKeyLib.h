/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFUSE_PUBLIC_KEY_LIB_H_
#define _EFUSE_PUBLIC_KEY_LIB_H_
#include <Uefi.h>

EFI_STATUS
PrivilegedBurnEfusePublicKey (
  UINT8  *pData,
  UINTN  Length,
  INT32  *ErrCode
  );

EFI_STATUS
PrivilegedGetEfusePublicKeyStatus (
  INT32  *ErrCode
  );

#endif
