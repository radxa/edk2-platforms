/** @file
  Send TPM 2.0 commands to the Microsoft OP-TEE fTPM TA
  (GUID bc50d971-d4c9-42c4-82cb-343fb7f37896).

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FTpmDeviceLib.h>
#include <Library/OpteeClientApiLib.h>
#include <Library/OpteeLib.h>
#include <Library/OpteeTrustedAppGuids.h>

#define FTPM_OPTEE_TA_SUBMIT_COMMAND  0
#define FTPM_MAX_COMMAND_SIZE         4096
#define FTPM_MAX_RESPONSE_SIZE        4096

STATIC BOOLEAN  mSessionOpen = FALSE;
STATIC BOOLEAN  mUseFwPath   = TRUE;
STATIC UINT32   mSessionId   = 0;

STATIC
EFI_STATUS
OpenFtpmSession (
  VOID
  )
{
  EFI_STATUS              Status;
  OPTEE_OPEN_SESSION_ARG  OpenArg;

  if (mSessionOpen) {
    return EFI_SUCCESS;
  }

  Status = OpteeClientApiInitialize (NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (&OpenArg, sizeof (OpenArg));
  CopyMem (&OpenArg.Uuid, &gOpteeFtpmTaGuid, sizeof (EFI_GUID));

  Status = OpteeOpenFwSession (&OpenArg);
  if (EFI_ERROR (Status) || (OpenArg.Return != OPTEE_SUCCESS)) {
    DEBUG ((
      DEBUG_INFO,
      "FTpm: Fw session failed (st=%r tee=0x%x), trying NS path\n",
      Status,
      OpenArg.Return
      ));
    ZeroMem (&OpenArg, sizeof (OpenArg));
    CopyMem (&OpenArg.Uuid, &gOpteeFtpmTaGuid, sizeof (EFI_GUID));
    Status      = OpteeOpenSession (&OpenArg);
    mUseFwPath  = FALSE;
  } else {
    mUseFwPath = TRUE;
  }

  if (EFI_ERROR (Status) || (OpenArg.Return != OPTEE_SUCCESS)) {
    DEBUG ((
      DEBUG_ERROR,
      "FTpm: open session failed st=%r tee=0x%x origin=%u\n",
      Status,
      OpenArg.Return,
      OpenArg.ReturnOrigin
      ));
    return EFI_NOT_FOUND;
  }

  mSessionId   = OpenArg.Session;
  mSessionOpen = TRUE;
  DEBUG ((DEBUG_INFO, "FTpm: session %u (%a)\n", mSessionId, mUseFwPath ? "Fw" : "NS"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FTpmSubmitCommand (
  IN UINT32      InputParameterBlockSize,
  IN UINT8       *InputParameterBlock,
  IN OUT UINT32  *OutputParameterBlockSize,
  IN UINT8       *OutputParameterBlock
  )
{
  EFI_STATUS                 Status;
  OPTEE_INVOKE_FUNCTION_ARG  InvokeArg;
  UINT32                     ResponseSize;

  if ((InputParameterBlock == NULL) || (OutputParameterBlock == NULL) ||
      (OutputParameterBlockSize == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  if (InputParameterBlockSize > FTPM_MAX_COMMAND_SIZE) {
    return EFI_BAD_BUFFER_SIZE;
  }

  Status = OpenFtpmSession ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (&InvokeArg, sizeof (InvokeArg));
  InvokeArg.Function = FTPM_OPTEE_TA_SUBMIT_COMMAND;
  InvokeArg.Session  = mSessionId;

  InvokeArg.Params[0].Attribute                   = OPTEE_MESSAGE_ATTRIBUTE_TYPE_MEMORY_INPUT;
  InvokeArg.Params[0].Union.Memory.BufferAddress  = (UINT64)(UINTN)InputParameterBlock;
  InvokeArg.Params[0].Union.Memory.Size           = InputParameterBlockSize;

  InvokeArg.Params[1].Attribute                   = OPTEE_MESSAGE_ATTRIBUTE_TYPE_MEMORY_INOUT;
  InvokeArg.Params[1].Union.Memory.BufferAddress  = (UINT64)(UINTN)OutputParameterBlock;
  InvokeArg.Params[1].Union.Memory.Size           = *OutputParameterBlockSize;
  if (InvokeArg.Params[1].Union.Memory.Size > FTPM_MAX_RESPONSE_SIZE) {
    InvokeArg.Params[1].Union.Memory.Size = FTPM_MAX_RESPONSE_SIZE;
  }

  if (mUseFwPath) {
    Status = OpteeFwInvokeFunction (&InvokeArg);
  } else {
    Status = OpteeInvokeFunction (&InvokeArg);
  }

  if (EFI_ERROR (Status) || (InvokeArg.Return != OPTEE_SUCCESS)) {
    DEBUG ((
      DEBUG_ERROR,
      "FTpm: invoke failed st=%r tee=0x%x origin=%u\n",
      Status,
      InvokeArg.Return,
      InvokeArg.ReturnOrigin
      ));
    return EFI_DEVICE_ERROR;
  }

  ResponseSize = (UINT32)InvokeArg.Params[1].Union.Memory.Size;
  if (ResponseSize > *OutputParameterBlockSize) {
    *OutputParameterBlockSize = ResponseSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  *OutputParameterBlockSize = ResponseSize;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FTpmRequestUseTpm (
  VOID
  )
{
  return OpenFtpmSession ();
}

EFI_STATUS
EFIAPI
FTpmRegisterTpm2DeviceLib (
  VOID
  )
{
  return EFI_SUCCESS;
}
