/** @file
  Null discrete TPM device library. O6N has no SPI TPM.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/DTpmDeviceLib.h>

EFI_STATUS
EFIAPI
DTpmSubmitCommand (
  IN UINT32      InputParameterBlockSize,
  IN UINT8       *InputParameterBlock,
  IN OUT UINT32  *OutputParameterBlockSize,
  IN UINT8       *OutputParameterBlock
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
DTpmRequestUseTpm (
  VOID
  )
{
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
DTpmRegisterTpm2DeviceLib (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}
