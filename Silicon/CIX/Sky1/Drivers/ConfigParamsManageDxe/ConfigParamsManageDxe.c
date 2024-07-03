/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ConfigParamsManageDxe.h"

EFI_STATUS
EFIAPI
ConfigDataGetEntry (
  IN     CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *This,
  IN     UINT32                             Index,
  IN OUT CONFIG_PARAMS_DATA_ENTRY           *Entry,
  IN OUT VOID                               *Buffer,
  IN OUT UINTN                              *BufferSize

  )
{
  if (Index > This->EntryNum) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BufferSize < This->Entry[Index].Size) {
    *BufferSize = This->Entry[Index].Size;
    return EFI_BUFFER_TOO_SMALL;
  }

  if ((Buffer == NULL) || (Entry == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem ((VOID *)Buffer, (VOID *)(((UINT8 *)(This->Data)) + This->Entry[Index].Offset), This->Entry[Index].Size);
  CopyMem ((VOID *)Entry, (VOID *)(&This->Entry[Index]), sizeof (CONFIG_PARAMS_DATA_ENTRY));

  *BufferSize = This->Entry[Index].Size;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ConfigDataFindEntry (
  IN     CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *This,
  IN     UINT32                             Id,
  IN OUT CONFIG_PARAMS_DATA_ENTRY           *Entry,
  IN OUT VOID                               *Buffer,
  IN OUT UINTN                              *BufferSize

  )
{
  UINT32  Index;

  if (BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < This->EntryNum; Index++) {
    if (Id == This->Entry[Index].Id) {
      if (*BufferSize < This->Entry[Index].Size) {
        *BufferSize = This->Entry[Index].Size;
        return EFI_BUFFER_TOO_SMALL;
      }

      if ((Buffer == NULL) || (Entry == NULL)) {
        return EFI_INVALID_PARAMETER;
      }

      CopyMem ((VOID *)Buffer, (VOID *)(((UINT8 *)(This->Data)) + This->Entry[Index].Offset), This->Entry[Index].Size);
      CopyMem ((VOID *)Entry, (VOID *)(&This->Entry[Index]), sizeof (CONFIG_PARAMS_DATA_ENTRY));
      *BufferSize = This->Entry[Index].Size;

      break;
    }
  }

  if (Index == This->EntryNum) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Entrypoint of Config Params Manage Dxe Driver

  @param  ImageHandle[in]      The firmware allocated handle for the EFI image.
  @param  SystemTable[in]      A pointer to the EFI System Table.

  @retval EFI_SUCCESS          The config params manage protocol has been registered.
  @retval Others               Unexpected error happened.

**/
EFI_STATUS
EFIAPI
ConfigParamsManageDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status           = EFI_SUCCESS;
  CONFIG_PARAMS_DATA_BLOCK           *ConfigData      = NULL;
  CONFIG_PARAMS_DATA_ENTRY           *ConfigDataEntry = NULL;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage    = NULL;

  POST_CODE (ConfigParamsManageDxeStart);

  ConfigData      = AllocateCopyPool (sizeof (CONFIG_PARAMS_DATA_BLOCK), &mConfigParamsDataBlock);
  ConfigDataEntry = AllocateCopyPool (sizeof (CONFIG_PARAMS_DATA_ENTRY) * mConfigDataEntryNum, &mConfigDataEntryTable);

  // hook routines for configuration parameters initialization
  ConfigParamsHook (ConfigData);

  ConfigManage = AllocateZeroPool (sizeof (CIX_CONFIG_PARAMS_MANAGE_PROTOCOL));
  if (ConfigManage != NULL) {
    ConfigManage->Data      = ConfigData;
    ConfigManage->Entry     = ConfigDataEntry;
    ConfigManage->EntryNum  = mConfigDataEntryNum;
    ConfigManage->GetEntry  = ConfigDataGetEntry;
    ConfigManage->FindEntry = ConfigDataFindEntry;

    gBS->InstallProtocolInterface (
           &ImageHandle,
           &gCixConfigParamsManageProtocolGuid,
           EFI_NATIVE_INTERFACE,
           ConfigManage
           );
  }

  POST_CODE (ConfigParamsManageDxeEnd);

  return Status;
}
