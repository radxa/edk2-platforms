/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Protocol/Tcg2Protocol.h>
#include <Protocol/VariableLock.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Guid/EventGroup.h>
#include <Guid/Tcg2PhysicalPresenceData.h>
#include <Library/Tcg2PhysicalPresenceLib.h>
#include <Library/Tcg2PpVendorLib.h>

/**
    This is the entry point of the TPM variable DXE driver, which is responsible for initializing and locking the physical presence flag variable.
    The main steps include:
    1. Initialize the physical presence flags.
    2. Check and set the physical presence flag variable.
    3. Ensure that the physical presence flag variable is read-only to prevent it from being tampered with by malicious software.
**/
EFI_STATUS
EFIAPI
TPMVariableDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  UINTN                             DataSize;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLockProtocol;
  EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  PpiFlags;

  //
  // Initialize physical presence flags.
  //
  DataSize = sizeof (EFI_TCG2_PHYSICAL_PRESENCE_FLAGS);
  Status   = gRT->GetVariable (
                    TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
                    &gEfiTcg2PhysicalPresenceGuid,
                    NULL,
                    &DataSize,
                    &PpiFlags
                    );
  if (EFI_ERROR (Status)) {
    PpiFlags.PPFlags = PcdGet32 (PcdTcg2PhysicalPresenceFlags);
    Status           = gRT->SetVariable (
                              TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
                              &gEfiTcg2PhysicalPresenceGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              sizeof (EFI_TCG2_PHYSICAL_PRESENCE_FLAGS),
                              &PpiFlags
                              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TPM2] Set physical presence flag failed, Status = %r\n", Status));
      return Status;
    }

    DEBUG ((DEBUG_INFO, "[TPM2] Initial physical presence flags value is 0x%x\n", PpiFlags.PPFlags));
  }

  // This flags variable controls whether physical presence is required for TPM command.
  // It should be protected from malicious software. We set it as read-only variable here.

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (!EFI_ERROR (Status)) {
    Status = VariableLockProtocol->RequestToLock (
                                     VariableLockProtocol,
                                     TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
                                     &gEfiTcg2PhysicalPresenceGuid
                                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TPM2] Error when lock variable %s, Status = %r\n", TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE, Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  DEBUG ((DEBUG_INFO, "TPMVariableDxe Success.\n"));
  return Status;
}
