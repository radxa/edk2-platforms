/** @file

  Copyright 2025 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _FASTBOOT_INFO_H_
#define _FASTBOOT_INFO_H_

#include <Uefi.h>

typedef struct _CIX_FASTBOOT_INFO_PROTOCOL CIX_FASTBOOT_INFO_PROTOCOL;

struct _CIX_FASTBOOT_INFO_PROTOCOL {
  CHAR8    *StrSerialNumber;
};

#endif
