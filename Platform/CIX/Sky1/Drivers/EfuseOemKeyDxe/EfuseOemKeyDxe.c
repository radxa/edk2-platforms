/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "EfuseOemKeyDxe.h"

UINT8  OemKey[32] = { };
EFI_STATUS
EFIAPI
EfuseOemKeyDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  INT32       ErrCode = 0;
  VOID        *Buffer = NULL;
  UINTN       Size    = 0;

  Status = GetSectionFromAnyFv (
             PcdGetPtr (PcdOemPublicKeyFileGuid),
             EFI_SECTION_RAW,
             0,
             &Buffer,
             &Size
             );
  if (EFI_ERROR (Status)) {
    DebugPrint (DEBUG_INFO, "Get Oem Public Key fail:%r\n", Status);
    return Status;
  }

  DebugPrint (DEBUG_INFO, "Get Oem Public Key Success Size:%x\n", Size);

  PrivilegedGetEfusePublicKeyStatus (&ErrCode);

  if (ErrCode == 3) {
    DebugPrint (DEBUG_INFO, "Oem Public Key fuse start\n");
    PrivilegedBurnEfusePublicKey ((UINT8 *)Buffer, Size, &ErrCode);
    DebugPrint (DEBUG_INFO, "Oem Public Key fuse end\n");
  }

  return EFI_SUCCESS;
}
