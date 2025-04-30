/** @file

  Update the patched PCDs to their correct value

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright (c) 2020, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/**
 * Patch the relevant PCDs of the NOR flash driver with the correct address of the
 * allocated memory
 *
**/
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MmServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Guid/NvramInformation.h>

#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>

#include "Library/NorFlashLib.h"

/**
  Fixup the Pcd values for variable storage

  Since the upper layers of EDK2 expect a memory mapped interface and we can't
  offer that from an RPMB or Nor Flash, the driver allocates memory on init and passes that
  on the upper layers. Since the memory is dynamically allocated and we can't set the
  PCD in StMM context, we need to patch it correctly on each access

  @retval EFI_SUCCESS            Protocol was found and PCDs patched up
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
VariablePcdLibConstructor (
  VOID
  )
{
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *FvbProtocol;
  NOR_FLASH_INSTANCE                  *Instance;
  EFI_STATUS                          Status;
  EFI_CONFIGURATION_TABLE             *ConfigurationTable;
  UINTN                               Index;
  VOID                                *HobList;
  NVRAM_INFORMATION_HOB_DATA          *NvramInformationHobData;
  EFI_HOB_GUID_TYPE                   *Hob;

  //
  // Locate SmmFirmwareVolumeBlockProtocol
  //
  Status = gMmst->MmLocateProtocol (
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    NULL,
                    (VOID **)&FvbProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  Instance = INSTANCE_FROM_FVB_THIS (FvbProtocol);

  ConfigurationTable = gMmst->MmConfigurationTable;
  for (Index = 0; Index < gMmst->NumberOfTableEntries; Index++) {
    if (CompareGuid (&gEfiHobListGuid, &(ConfigurationTable[Index].VendorGuid))) {
      break;
    }
  }

  // Bail out if the HobList could not be found
  if (Index >= gMmst->NumberOfTableEntries) {
    DEBUG ((DEBUG_ERROR, "%a: HobList not found - 0x%x\n", __FUNCTION__, Index));
    return EFI_OUT_OF_RESOURCES;
  }

  HobList = ConfigurationTable[Index].VendorTable;
  if (HobList == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NvramInformation hob list not found\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  Hob = GetNextGuidHob (&gNvramInformationHobGuid, HobList);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NvramInformation hob not found\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  NvramInformationHobData = GET_GUID_HOB_DATA (Hob);
  if (NvramInformationHobData == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NvramInformationHob extraction failed - 0x%x\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  // Patch PCDs with the correct values
  PatchPcdSet32 (PcdFlashNvStorageVariableSize, NvramInformationHobData->VariableSize);
  PatchPcdSet32 (PcdFlashNvStorageFtwWorkingSize, NvramInformationHobData->FtwWorkingSize);
  PatchPcdSet32 (PcdFlashNvStorageFtwSpareSize, NvramInformationHobData->FtwSpareSize);

  PatchPcdSet64 (PcdFlashNvStorageVariableBase64, Instance->RegionBaseAddress);
  PatchPcdSet64 (PcdFlashNvStorageFtwWorkingBase64, Instance->RegionBaseAddress + NvramInformationHobData->VariableSize);
  PatchPcdSet64 (PcdFlashNvStorageFtwSpareBase64, Instance->RegionBaseAddress + NvramInformationHobData->VariableSize + NvramInformationHobData->FtwWorkingSize);

  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageVariableBase64: 0x%lx\n", __FUNCTION__, PcdGet64 (PcdFlashNvStorageVariableBase64)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwWorkingBase64: 0x%lx\n", __FUNCTION__, PcdGet64 (PcdFlashNvStorageFtwWorkingBase64)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwSpareBase64: 0x%lx\n", __FUNCTION__, PcdGet64 (PcdFlashNvStorageFtwSpareBase64)));

  return Status;
}
