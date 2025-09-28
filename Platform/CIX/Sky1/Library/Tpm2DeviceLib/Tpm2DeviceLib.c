/*++

Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

--*/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/DTpmDeviceLib.h>
#include <Library/FTpmDeviceLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/Tpm2CommandLib.h>
#include <string.h>
#include <PlatformSetupVar.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "Tpm2Fifo.h"

STATIC UINT8  TPMDeviceSelect = 255;

/**
  The constructor function caches the pointer to PEI services.

  The constructor function caches the pointer to PEI services.
  It will always return EFI_SUCCESS.

  @param  FfsHeader   Pointer to FFS header the loaded driver.
  @param  PeiServices Pointer to the PEI services.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
Tpm2DeviceLibConstructor (
  VOID
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  PLATFORM_SETUP_DATA  PlatformSetupVar;
  UINTN                VarSize = sizeof (PLATFORM_SETUP_DATA);

  if (TPMDeviceSelect == 255) {
    Status = gRT->GetVariable (
                    PLATFORM_SETUP_VAR,
                    &gPlatformSetupVariableGuid,
                    NULL,
                    &VarSize,
                    &PlatformSetupVar
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a EfiGetVariable failed: %r\n", __FUNCTION__, Status));
      return Status;
    }

    TPMDeviceSelect = PlatformSetupVar.TPMDeviceSelect;
  }

  return Status;
}

EFI_STATUS
EFIAPI
Tpm2SubmitCommand (
  IN UINT32      InputParameterBlockSize,
  IN UINT8       *InputParameterBlock,
  IN OUT UINT32  *OutputParameterBlockSize,
  IN OUT UINT8   *OutputParameterBlock
  )
{
  switch (TPMDeviceSelect) {
    case DTPM_ENABLE:
      return DTpmSubmitCommand (
               InputParameterBlockSize,
               InputParameterBlock,
               OutputParameterBlockSize,
               OutputParameterBlock
               );
    case FTPM_ENABLE:
      return FTpmSubmitCommand (
               InputParameterBlockSize,
               InputParameterBlock,
               OutputParameterBlockSize,
               OutputParameterBlock
               );
    default:
      return EFI_UNSUPPORTED;
  }
}

EFI_STATUS
EFIAPI
DTpmSwitchFTpm (
  VOID
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  PLATFORM_SETUP_DATA  PlatformSetupVar;
  UINTN                VarSize = sizeof (PLATFORM_SETUP_DATA);

  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PlatformSetupVar
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a EfiGetVariable failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  PlatformSetupVar.TPMDeviceSelect = FTPM_ENABLE;

  Status = gRT->SetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (PLATFORM_SETUP_DATA),
                  &PlatformSetupVar
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a EfiSetVariable failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  TPMDeviceSelect = FTPM_ENABLE;
  return Status;
}

/**
  This service requests use TPM2.

  @retval EFI_SUCCESS      Get the control of TPM2 chip.
  @retval EFI_NOT_FOUND    TPM2 not found.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
Tpm2RequestUseTpm (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  switch (TPMDeviceSelect) {
    case DTPM_ENABLE:
      Status = DTpmRequestUseTpm ();
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "DTPM not detect, change to FTPM interface\n"));
        DTpmSwitchFTpm ();
        return FTpmRequestUseTpm ();
      }

      break;
    case FTPM_ENABLE:
      return FTpmRequestUseTpm ();
    default:
      return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  This service register TPM2 device.

  @Param Tpm2Device  TPM2 device

  @retval EFI_SUCCESS          This TPM2 device is registered successfully.
  @retval EFI_UNSUPPORTED      System does not support register this TPM2 device.
  @retval EFI_ALREADY_STARTED  System already register this TPM2 device.
**/
EFI_STATUS
EFIAPI
Tpm2RegisterTpm2DeviceLib (
  IN TPM2_DEVICE_INTERFACE  *Tpm2Device
  )
{
  switch (TPMDeviceSelect) {
    case DTPM_ENABLE:
      return DTpmRegisterTpm2DeviceLib ();
    case FTPM_ENABLE:
      return FTpmRegisterTpm2DeviceLib ();
    default:
      return EFI_UNSUPPORTED;
  }
}
