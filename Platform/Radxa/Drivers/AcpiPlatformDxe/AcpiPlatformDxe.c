/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/AcpiLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/PlatformConfigParamsManageProtocol.h>
#include <RadxaSetupVar.h>

EFI_GUID  pAcpiPlatformTableStorageGuid = {
  0xc1bb2ead, 0xc76a, 0x45dd, { 0x90, 0xb5, 0xd4, 0x02, 0x55, 0x17, 0x0a, 0x9c }
};

VOID
EFIAPI
AcpiHookFunctionOnReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *PlatformConfigManage;
  CHAR8               *SsdtTableId = "ORIONO6";
  CHAR16 *            SystemProductName;
  RADXA_SETUP_DATA    RadxaSetupVar;
  UINTN               VarSize;

  DEBUG ((DEBUG_INFO, "Enter %a\n", __func__));

  VarSize = sizeof (RADXA_SETUP_DATA);
  Status = gRT->GetVariable (
                  RADXA_SETUP_VAR,
                  &gRadxaSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &RadxaSetupVar
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: EfiGetVariable failed for gRadxaSetupVariableGuid - %r\n", __FUNCTION__, Status));
  }

  Status = gBS->LocateProtocol (&gCixPlatformConfigParamsManageProtocolGuid, NULL, (VOID **)&PlatformConfigManage);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: LocateProtocol failed: %r\n", __FUNCTION__, Status));
  }

  SystemProductName = (CHAR16 *)FixedPcdGetPtr (PcdSystemProductName);

  if (!StrCmp (L"Radxa Orion O6", SystemProductName)) {
    Status = UpdateSsdtNameAslCode ((UINT8 *) SsdtTableId, AsciiStrLen (SsdtTableId), SIGNATURE_32 ('E', 'C', 'F', 'M'), &(PlatformConfigManage->Data->EcFanMode), sizeof (PlatformConfigManage->Data->EcFanMode));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Update ECFM failed, Status=%r\n", __FUNCTION__, Status));
    }
  }

  Status = UpdateNameAslCode (SIGNATURE_32 ('S','C','M','S'), &(RadxaSetupVar.EnableAcpiScmi), sizeof (RadxaSetupVar.EnableAcpiScmi));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Update SCMS failed, Status=%r\n", __FUNCTION__, Status));
  }

  //
  // Close the event, so it will not be signalled again.
  //
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  DEBUG ((DEBUG_INFO, "Exit %a\n", __func__));
}

EFI_STATUS
EFIAPI
AcpiPlatformDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "Acpi Platform Dxe Entry\n"));

  Status = LocateAndInstallAcpiFromFv (&pAcpiPlatformTableStorageGuid);
  DEBUG ((DEBUG_INFO, "Install Acpi table status: %r\n", Status));

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiHookFunctionOnReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "HACK: disable PcdTestKeyUsed FrontPage warning\n"));
  PcdSetBoolS (PcdTestKeyUsed, FALSE);

  DEBUG ((DEBUG_INFO, "Acpi Platform Dxe Exit\n"));

  return EFI_SUCCESS;
}
