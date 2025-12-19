/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/AcpiLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PlatformConfigParamsManageProtocol.h>

EFI_GUID  pAcpiPlatformTableStorageGuid = {
  0xc1bb2ead, 0xc76a, 0x45dd, { 0x90, 0xb5, 0xd4, 0x02, 0x55, 0x17, 0x0a, 0x9c }
};

EFI_STATUS
EFIAPI
UpdateSsdt (
  VOID
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *PlatformConfigManage;
  CHAR8               *SsdtTableId = "ORIONO6";
  CHAR16 *            SystemProductName;

  DEBUG ((DEBUG_INFO, "Enter %a\n", __func__));

  Status = gBS->LocateProtocol (&gCixPlatformConfigParamsManageProtocolGuid, NULL, (VOID **)&PlatformConfigManage);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a LocateProtocol failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  SystemProductName = (CHAR16 *)FixedPcdGetPtr (PcdSystemProductName);

  if (!StrCmp (L"Radxa Orion O6", SystemProductName)) {
    Status = UpdateSsdtNameAslCode ((UINT8 *) SsdtTableId, AsciiStrLen (SsdtTableId), SIGNATURE_32 ('E', 'C', 'F', 'M'), &(PlatformConfigManage->Data->EcFanMode), sizeof (UINT8));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Update ECFM failed, Status=%r\n", Status));
      return Status;
    }
  }

  DEBUG ((DEBUG_INFO, "Exit %a\n", __func__));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AcpiPlatformDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Acpi Platform Dxe Entry\n"));

  Status = LocateAndInstallAcpiFromFv (&pAcpiPlatformTableStorageGuid);
  DEBUG ((DEBUG_INFO, "Install Acpi table status: %r\n", Status));

  UpdateSsdt();
  DEBUG ((DEBUG_INFO, "UpdateSsdt status: %r\n", Status));

  DEBUG ((DEBUG_INFO, "HACK: disable PcdTestKeyUsed FrontPage warning\n"));
  PcdSetBoolS (PcdTestKeyUsed, FALSE);

  DEBUG ((DEBUG_INFO, "Acpi Platform Dxe Exit\n"));

  return EFI_SUCCESS;
}
