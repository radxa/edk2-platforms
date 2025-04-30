/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/NetworkStackSetup.h>

#define PXE_DRIVER_INSTALL_PROTOCOL_GUID  {0x1898e36b, 0xd507, 0x490b, {0x86, 0xda, 0xdb, 0x73, 0xb2, 0x2d, 0x76, 0x8b}}
EFI_GUID  gPxeDriverInstallProtocolGuid = PXE_DRIVER_INSTALL_PROTOCOL_GUID;

EFI_STATUS
EFIAPI
RealtekLanDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  UINTN          VarSize;
  NETWORK_STACK  NetworkStack;

  VarSize = sizeof (NETWORK_STACK);

  Status = gRT->GetVariable (
                  NETWORK_STACK_VAR,
                  &gEfiNetworkStackSetupGuid,
                  NULL,
                  &VarSize,
                  &NetworkStack
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (NetworkStack.Enable) {
    Status = gBS->InstallProtocolInterface (
                    &ImageHandle,
                    &gPxeDriverInstallProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
  }

  return Status;
}
