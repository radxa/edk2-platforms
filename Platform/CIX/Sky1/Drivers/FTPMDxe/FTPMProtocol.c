/** @file
  Publish CIX FTPM_INIT_PROTOCOL and initialize the TPM2 CRB control area
  used by the ACPI TPM2 table (start method 9 / TrEE).

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <IndustryStandard/Tpm2Acpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FTpmDeviceLib.h>
#include <Library/OpteeClientApiLib.h>
#include <Library/OpteeLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/FTPMInitProtocol.h>

STATIC TEEC_Context   mContext;
STATIC TEEC_Session   mSession;
STATIC BOOLEAN        mReady = FALSE;
STATIC FTPM_INIT_PROTOCOL  mFtpmInitProtocol;

STATIC
EFI_STATUS
EFIAPI
GetFTPMSession (
  IN FTPM_INIT_PROTOCOL  *This,
  OUT TEEC_Context       *Context,
  OUT TEEC_Session       *Session
  )
{
  if ((This == NULL) || (Context == NULL) || (Session == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mReady) {
    return EFI_NOT_READY;
  }

  *Context = mContext;
  *Session = mSession;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
FTPMOpenSession (
  IN FTPM_INIT_PROTOCOL  *This
  )
{
  EFI_STATUS  Status;

  Status = FTpmRequestUseTpm ();
  if (EFI_ERROR (Status)) {
    mReady = FALSE;
    return Status;
  }

  mReady = TRUE;
  return EFI_SUCCESS;
}

STATIC
BOOLEAN
EFIAPI
GetFTPMStatus (
  IN FTPM_INIT_PROTOCOL  *This
  )
{
  return mReady;
}

STATIC
VOID
InitCrbControlArea (
  VOID
  )
{
  EFI_TPM2_ACPI_CONTROL_AREA  *Control;
  UINT8                       *Base;
  UINT32                      Size;
  UINT32                      CmdOff;
  UINT32                      RspOff;
  UINT32                      BufSize;

  Base = (UINT8 *)(UINTN)PcdGet32 (PcdTpm2AcpiBufferBase);
  Size = PcdGet32 (PcdTpm2AcpiBufferSize);
  if ((Base == NULL) || (Size < 0x80 + 0x1000 + 0x1000)) {
    DEBUG ((DEBUG_ERROR, "FTPM: invalid CRB buffer PCD\n"));
    return;
  }

  ZeroMem (Base, Size);

  CmdOff  = 0x80;
  BufSize = 0x1000;
  RspOff  = CmdOff + BufSize;

  Control                   = (EFI_TPM2_ACPI_CONTROL_AREA *)Base;
  Control->CommandSize      = BufSize;
  Control->Command          = (UINT64)(UINTN)(Base + CmdOff);
  Control->ResponseSize     = BufSize;
  Control->Response         = (UINT64)(UINTN)(Base + RspOff);
}

EFI_STATUS
EFIAPI
FTPMProtocolEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  InitCrbControlArea ();

  Status = OpteeClientApiInitialize (ImageHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FTPM: OP-TEE init failed: %r (table still published)\n", Status));
  } else {
    Status = FTpmRequestUseTpm ();
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "FTPM: TA session failed: %r (fTPM.bin is in the FV; OP-TEE may need the TA loaded)\n",
        Status
        ));
    } else {
      mReady = TRUE;
    }
  }

  mFtpmInitProtocol.GetFTPMSession  = GetFTPMSession;
  mFtpmInitProtocol.FTPMOpenSession = FTPMOpenSession;
  mFtpmInitProtocol.GetFTPMStatus   = GetFTPMStatus;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gCixFtpmInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mFtpmInitProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FTPM: install protocol failed: %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
