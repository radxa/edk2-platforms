/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _OPN_LIB_H_
#define _OPN_LIB_H_

#include <Uefi.h>

EFIAPI EFI_STATUS
GetProcessVersion (
  IN OUT CHAR16  **ProcessVersion
  );

#endif
