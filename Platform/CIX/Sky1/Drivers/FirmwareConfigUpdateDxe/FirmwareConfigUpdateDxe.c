/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "FirmwareConfigUpdateDxe.h"
#include <PlatformSetupVar.h>
#include <MemConfigurationTable.h>

EFI_STATUS
GetMemConfigAndUpdate (
  PLATFORM_SETUP_DATA  *pPlatformSetupData,
  UINT8                *pImage,
  BOOLEAN              *Update
  )
{
  MEM_QUICK_CONFIG  *MemQuickCfg = (MEM_QUICK_CONFIG*)pImage;

  *Update = FALSE;

  if(MemQuickCfg->cpufmax != pPlatformSetupData->CpuFMax) {
    DEBUG ((DEBUG_INFO, "Update CpuFMax %x\n", pPlatformSetupData->CpuFMax));
    MemQuickCfg->cpufmax = pPlatformSetupData->CpuFMax;
    *Update = TRUE;
  }

  if(MemQuickCfg->gpufmax != pPlatformSetupData->GpuFMax) {
    DEBUG ((DEBUG_INFO, "Update GpuFMax %x\n", pPlatformSetupData->GpuFMax));
    MemQuickCfg->gpufmax = pPlatformSetupData->GpuFMax;
    *Update = TRUE;
  }

  return EFI_SUCCESS;
}

VOID
FwConfigUpdateCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS              Status;
  UINT8                   *pImage;
  UINTN                   Length = 0x1000;
  BOOLEAN                 Update = FALSE;
  UINTN                   VarSize;
  PLATFORM_SETUP_DATA     *pPlatformSetupVar;
  CIX_FW_UPDATE_PROTOCOL  *FlashUpdateProtocol = NULL;


  DEBUG ((DEBUG_INFO, "FwConfigUpdateCallback\n"));
  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));

  VarSize           = sizeof (PLATFORM_SETUP_DATA);
  pPlatformSetupVar = AllocateZeroPool (VarSize);
  if (pPlatformSetupVar == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: AllocateZeroPool failed\n", __FUNCTION__, __LINE__));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  pPlatformSetupVar
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: GetVariable failed, Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  pImage = AllocateZeroPool (Length);
  if (pImage == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: AllocateZeroPool failed\n", __FUNCTION__, __LINE__));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  Status = gBS->LocateProtocol (&gCixFirmwareUpdateProtocolGuid, NULL, (VOID **)&FlashUpdateProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: LocateProtocol failed: %r\n", __FUNCTION__, __LINE__, Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  Status = FlashUpdateProtocol->FirmwareRawEntryUpdate (FIRMWARE_TYPE_MEM_CONF, pImage, Length, ENTRY_READ, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: FirmwareRawEntryUpdate Read failed: %r\n", __FUNCTION__, __LINE__, Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  DEBUG ((DEBUG_INFO, "[%a][%d]: FirmwareRawEntryUpdate Read success\n", __FUNCTION__, __LINE__));
  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  Status = GetMemConfigAndUpdate (pPlatformSetupVar, pImage, &Update);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "GetMemConfigBiosBlockAndUpdate Status:%r\n", Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
  if (Update) {
    // UpdateCheckSum (pPmConfig);
    Status = FlashUpdateProtocol->FirmwareRawEntryUpdate (FIRMWARE_TYPE_MEM_CONF, pImage, Length, ENTRY_WRITE, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a][%d]: FirmwareRawEntryUpdate write failed: %r\n", __FUNCTION__, __LINE__, Status));
      goto Done;
    }
  }

  DEBUG ((DEBUG_INFO, "Debug-------------------------------------------------------[%a][%d]\n", __FUNCTION__, __LINE__));
Done:
  if (pImage != NULL) {
    FreePool (pImage);
  }

  if (pPlatformSetupVar != NULL) {
    FreePool (pPlatformSetupVar);
  }

  return;
}

EFI_STATUS
EFIAPI
FirmwareConfigUpdateDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_EVENT   Event;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  FwConfigUpdateCallback,
                  NULL,
                  &gSetupSaveVariableCallBackGuid,
                  &Event
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FirmwareConfigUpdateDxeEntryPoint: CreateEvent failed: %r\n", Status));
    return Status;
  }

  return Status;
}
