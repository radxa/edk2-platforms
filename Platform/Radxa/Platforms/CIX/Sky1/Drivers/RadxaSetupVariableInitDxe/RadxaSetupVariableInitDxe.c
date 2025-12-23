/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved

**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include "../../Include/RadxaSetupVar.h"

EFI_STATUS
EFIAPI
RadxaSetupVariableInitDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINTN             VarSize;
  RADXA_SETUP_DATA  RadxaSetupVar;

  VarSize = sizeof (RADXA_SETUP_DATA);

  Status = gRT->GetVariable (
                  RADXA_SETUP_VAR,
                  &gRadxaSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &RadxaSetupVar
                  );
  if (EFI_ERROR (Status)) {
    ZeroMem (&RadxaSetupVar, VarSize);
    //
    // Variable does not exist yet - create it
    //
    Status = gRT->SetVariable (
                    RADXA_SETUP_VAR,
                    &gRadxaSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (RADXA_SETUP_DATA),
                    &RadxaSetupVar
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: EfiSetVariable failed - %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a: EfiGetVariable Success - %r\n", __FUNCTION__, Status));
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gRadxaSetupVariableGuid,
                  NULL,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: InstallMultipleProtocolInterfaces failed - %r\n", __FUNCTION__, Status));
    return Status;
  }

  return Status;
}
