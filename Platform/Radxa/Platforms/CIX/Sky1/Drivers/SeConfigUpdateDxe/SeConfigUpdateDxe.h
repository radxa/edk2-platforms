/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SE_CONFIG_UPDATE_DXE_H_
#define _SE_CONFIG_UPDATE_DXE_H_
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MailBoxLib.h>

#define SE_CONFIG_FORMSET_GUID \
 {0x45f0def4, 0xde9a, 0x449e, {0x85, 0x2, 0x49, 0x48, 0xc4, 0xf4, 0x30, 0xe0}}

#endif
