/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PlatformDxe.h"

VOID
EFIAPI
UpdateDtsProperty
 (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  #define DEVICE_MODE 0
  #define USB_MODE    1
  #define DP_MODE     2
  #define DP_USB_MODE 3

  #define KERNEL_HOST_MODE   0
  #define KERNEL_DP_USB_MODE 1
  #define KERNEL_DEVICE_MODE 2
  #define KERNEL_DP_MODE     3

  BOOT_HOOK_PARAMS_DATA_BLOCK  *ConfigData;
  MEM_SHARE_DTS_PROPERTY  *MemShareDtsProperty;
  UINT32  MemShareDtsPropertyLen;
  UINT8   Index;

  if (Context == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: failed to get config data\n", __FUNCTION__));
    return;
  }

  ConfigData = (BOOT_HOOK_PARAMS_DATA_BLOCK *)Context;

  DEBUG ((DEBUG_INFO, "%a: GopDisplayPort %d\n", __FUNCTION__, ConfigData->PlatConfig->GopDisplayPort));

  MemShareDtsProperty = SmemGetAddr (SMEM_DTS_PROPERTY, &MemShareDtsPropertyLen);
  ZeroMem (MemShareDtsProperty, MemShareDtsPropertyLen);

  for (Index = 0; Index < 4; Index++) {
    UINT8 DisplayPortIdx = Index;
    // skip display port 2, fixed at edp
    if(Index > 1) {
      DisplayPortIdx = Index + 1;
    }

    // if pd report dp or usb+dp, but no display on it, set usb mode
    if (ConfigData->PlatConfig->TypecMode[Index] > USB_MODE &&
        ConfigData->PlatConfig->GopDisplayPort != DisplayPortIdx) {
      MemShareDtsProperty->TypecPortState[Index] = KERNEL_HOST_MODE;
    } else {
      switch (ConfigData->PlatConfig->TypecMode[Index]) {
      case DEVICE_MODE:
        MemShareDtsProperty->TypecPortState[Index] = KERNEL_DEVICE_MODE;
        break;
      case DP_MODE:
        MemShareDtsProperty->TypecPortState[Index] = KERNEL_DP_MODE;
        break;
      case DP_USB_MODE:
        MemShareDtsProperty->TypecPortState[Index] = KERNEL_DP_USB_MODE;
        break;
      case USB_MODE:
      default:
        MemShareDtsProperty->TypecPortState[Index] = KERNEL_HOST_MODE;
        break;
      }
    }
  }

  MemShareDtsProperty->GopDisplayPort = ConfigData->PlatConfig->GopDisplayPort;

  UINT8 *pMemShareData = (UINT8 *)MemShareDtsProperty;
  DEBUG ((DEBUG_INFO, "%a: Dts Share Memory Data\n", __FUNCTION__));
  for (UINT8 i = 0; i < sizeof(MEM_SHARE_DTS_PROPERTY); i++) {
    DEBUG ((DEBUG_INFO, " %02X", *pMemShareData));
    pMemShareData++;
  }
  DEBUG ((DEBUG_INFO, "\n"));

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
  Entrypoint of Platform Dxe Driver

  @param  ImageHandle[in]      The firmware allocated handle for the EFI image.
  @param  SystemTable[in]      A pointer to the EFI System Table.

  @retval EFI_SUCCESS          The RAM disk has been registered.
  @retval EFI_NOT_FOUND        No RAM disk protocol instances were found.
  @retval EFI_UNSUPPORTED      The ImageType is not supported.
  @retval Others               Unexpected error happened.

**/
EFI_STATUS
EFIAPI
PlatformDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                  Status;
  EFI_EVENT                                   Event;
  EFI_HANDLE                                  Handle;
  ENV_HOOK_PARAMS_DATA_BLOCK                  *EnvConfigData  = NULL;
  BOOT_HOOK_PARAMS_DATA_BLOCK                 *BootConfigData = NULL;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL           *ConfigManage;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *PlatformConfigManage;

  POST_CODE (PlatformDxeStart);

  EnvConfigData  = AllocateZeroPool (sizeof (ENV_HOOK_PARAMS_DATA_BLOCK));
  BootConfigData = AllocateZeroPool (sizeof (BOOT_HOOK_PARAMS_DATA_BLOCK));

  Status = gBS->LocateProtocol (
                  &gCixConfigParamsManageProtocolGuid,
                  NULL,
                  (VOID **)&ConfigManage
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: config parameters invalid %r\n", __FUNCTION__, Status));
  } else {
    if (EnvConfigData != NULL) {
      EnvConfigData->SocConfig = ConfigManage->Data;
    }

    if (BootConfigData != NULL) {
      BootConfigData->SocConfig = ConfigManage->Data;
    }
  }

  Status = gBS->LocateProtocol (
                  &gCixPlatformConfigParamsManageProtocolGuid,
                  NULL,
                  (VOID **)&PlatformConfigManage
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: platform config parameters invalid %r\n", __FUNCTION__, Status));
  } else {
    if (EnvConfigData != NULL) {
      EnvConfigData->PlatConfig = PlatformConfigManage->Data;
    }

    if (BootConfigData != NULL) {
      BootConfigData->PlatConfig = PlatformConfigManage->Data;
    }
  }

  // Platform ENV phase routines
  PlatformEnvHook (EnvConfigData);

  // Platform BOOT phase routines register
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             PlatformBootHook,
             BootConfigData,
             &Event
             );

  // Update Dts Property to Memory
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             UpdateDtsProperty,
             BootConfigData,
             &Event
             );

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gCixPlatformInitCompleteProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );

  POST_CODE (PlatformDxeEnd);
  return Status;
}
