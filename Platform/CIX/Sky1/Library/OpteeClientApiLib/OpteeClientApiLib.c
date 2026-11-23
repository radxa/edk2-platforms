/** @file
  Initialize CIX OpteeLib. The in-tree header only exposes init/finalize;
  command submission uses OpteeLib directly.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/OpteeLib.h>
#include <Library/OpteeClientApiLib.h>

EFI_STATUS
OpteeClientApiInitialize (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;

  if (!IsOpteePresent ()) {
    DEBUG ((DEBUG_ERROR, "%a: OP-TEE not present\n", __func__));
    return EFI_NOT_FOUND;
  }

  Status = OpteeInit ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: OpteeInit failed: %r\n", __func__, Status));
  }

  return Status;
}

BOOLEAN
OpteeClientApiFinalize (
  VOID
  )
{
  return TRUE;
}
