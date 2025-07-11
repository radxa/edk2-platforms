/** @file
 *
 *  Copyright 2025 Cix Technology Group Co., Ltd. All Rights Reserved.
 **/

#include <libfdt.h>
#include <Library/PlatformDtbUpdateLib.h>
#include <Protocol/ConfigParamsManageProtocol.h>
#include <Protocol/PlatformConfigParamsManageProtocol.h>

STATIC
EFI_STATUS
PlatformUpdateDtb (
  VOID   *FdtImage,
  UINTN  *ImageSize
  )
{
  EFI_STATUS                         Status;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *Config;
  INT32                              ret;
  UINT32                             PaddSize;

  ret = fdt_check_header (FdtImage) || fdt_check_header_ext (FdtImage);
  if (ret) {
    DebugPrint (EFI_D_ERROR, "ERROR: Invalid device tree header ...\n");
    return EFI_NOT_FOUND;
  }

  /* Add padding to make space for new nodes and properties. */
  PaddSize = ADD_OF (fdt_totalsize (FdtImage), DTB_PAD_SIZE);

  ret = fdt_open_into (FdtImage, FdtImage, PaddSize);
  if (ret != 0) {
    DebugPrint (DEBUG_ERROR, "ERROR: Failed to move/resize dtb buffer ...\n");
    return EFI_BAD_BUFFER_SIZE;
  }

  Status = gBS->LocateProtocol (&gCixConfigParamsManageProtocolGuid, NULL, (VOID **)&Config);
  if (EFI_ERROR (Status)) {
    DebugPrint (DEBUG_ERROR, "Locate %g %r\n", &gCixConfigParamsManageProtocolGuid, Status);
  } else {
    if (Config->Data->UsbCDrd[0].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_0);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_0_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_0);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_0_CONTROLLER);
    }

    if (Config->Data->UsbC[0].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_1);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_1_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_1);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_1_CONTROLLER);
    }

    if (Config->Data->UsbC[1].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_2);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_2_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_2);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_2_CONTROLLER);
    }

    if (Config->Data->UsbC[2].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_3);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_3_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_3);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_3_CONTROLLER);
    }

    if (Config->Data->Usb32Drd[0].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_4);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_4_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_4);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_4_CONTROLLER);
    }

    if (Config->Data->Usb32Drd[1].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBSS_5);
      EnableDtbNode (FdtImage, DT_NODE_USBSS_5_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBSS_5);
      DisableDtbNode (FdtImage, DT_NODE_USBSS_5_CONTROLLER);
    }

    if (Config->Data->Usb20[0].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBHS_0);
      EnableDtbNode (FdtImage, DT_NODE_USBHS_0_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBHS_0);
      DisableDtbNode (FdtImage, DT_NODE_USBHS_0_CONTROLLER);
    }

    if (Config->Data->Usb20[1].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBHS_1);
      EnableDtbNode (FdtImage, DT_NODE_USBHS_1_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBHS_1);
      DisableDtbNode (FdtImage, DT_NODE_USBHS_1_CONTROLLER);
    }

    if (Config->Data->Usb20[2].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBHS_2);
      EnableDtbNode (FdtImage, DT_NODE_USBHS_2_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBHS_2);
      DisableDtbNode (FdtImage, DT_NODE_USBHS_2_CONTROLLER);
    }

    if (Config->Data->Usb20[3].Enable == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_USBHS_3);
      EnableDtbNode (FdtImage, DT_NODE_USBHS_3_CONTROLLER);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_USBHS_3);
      DisableDtbNode (FdtImage, DT_NODE_USBHS_3_CONTROLLER);
    }

    if (Config->Data->Pcie.PcieRpEnable[0] == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_PCIEX8_RC);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_PCIEX8_RC);
    }

    if (Config->Data->Pcie.PcieRpEnable[1] == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_PCIEX4_RC);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_PCIEX4_RC);
    }

    if (Config->Data->Pcie.PcieRpEnable[2] == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_PCIEX2_RC);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_PCIEX2_RC);
    }

    if (Config->Data->Pcie.PcieRpEnable[3] == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_PCIEX1_1_RC);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_PCIEX1_1_RC);
    }

    if (Config->Data->Pcie.PcieRpEnable[4] == TRUE) {
      EnableDtbNode (FdtImage, DT_NODE_PCIEX1_0_RC);
    } else {
      DisableDtbNode (FdtImage, DT_NODE_PCIEX1_0_RC);
    }
  }

  ret = fdt_pack (FdtImage);
  if (ret != 0) {
    DebugPrint (DEBUG_ERROR, "ERROR: Failed to pack dtb ...\n");
    return EFI_ABORTED;
  }

  *ImageSize = fdt_totalsize (FdtImage);
  return EFI_SUCCESS;
}

STATIC
VOID
PlatformDtsImageUpdateCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  EFI_HANDLE                       *pSimpleFileHandle = NULL;
  EFI_FILE_PROTOCOL                *pFs               = NULL;
  EFI_FILE_PROTOCOL                *pFile             = NULL;
  EFI_FILE_INFO                    *FileInfo          = NULL;
  UINTN                            FileInfoSize       = 0;
  UINTN                            HandleNum;
  UINT32                           Index;
  UINT64                           OpenMode;
  VOID                             *ImageBuffer            = NULL;
  UINTN                            ImageSize               = 0;
  CHAR16                           *FileName               = NULL;
  BOOLEAN                          FileFound               = FALSE;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *pSimpleFileSysProtocol = NULL;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleNum,
                  &pSimpleFileHandle
                  );
  if (EFI_ERROR (Status)) {
    DebugPrint (DEBUG_ERROR, "%a: Locate gEfiSimpleFileSystemProtocolGuid failed, %r\n", __FUNCTION__, Status);
    return;
  }

  FileName = (CHAR16 *)PcdGetPtr (PcdSiliconDtbUpdateFileName);
  if (NULL == FileName) {
    return;
  }

  DEBUG ((DEBUG_INFO, "%a: DTB file name: %s\n", __FUNCTION__, FileName));
  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->OpenProtocol (
                    pSimpleFileHandle[Index],
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID **)&pSimpleFileSysProtocol,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "%a: open gEfiSimpleFileSystemProtocolGuid failed, %r\n", __FUNCTION__, Status);
      return;
    }

    Status = pSimpleFileSysProtocol->OpenVolume (
                                       pSimpleFileSysProtocol,
                                       &pFs
                                       );
    if (EFI_ERROR (Status)) {
      continue;
    }

    OpenMode = EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE;
    Status   = pFs->Open (
                      pFs,
                      &pFile,
                      FileName,
                      OpenMode,
                      0
                      );
    if (EFI_ERROR (Status)) {
      continue;
    } else {
      FileFound = TRUE;
      break;
    }
  }

  if (FileFound) {
    Status = pFile->GetInfo (pFile, &gEfiFileInfoGuid, &FileInfoSize, NULL);
    if (EFI_ERROR (Status) && (Status == EFI_BUFFER_TOO_SMALL)) {
      FileInfo = AllocatePool (FileInfoSize);
      if (NULL == FileInfo) {
        DebugPrint (DEBUG_ERROR, "%a: Allocate memory failed\n", __FUNCTION__);
        goto exit;
      }

      Status = pFile->GetInfo (pFile, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
      if (EFI_ERROR (Status)) {
        DebugPrint (DEBUG_ERROR, "%a: Get file info failed, %r\n", __FUNCTION__, Status);
        goto exit;
      }
    } else {
      DebugPrint (DEBUG_ERROR, "%a: Get file info size failed, %r\n", __FUNCTION__, Status);
      goto exit;
    }

    ImageSize   = FileInfo->FileSize;
    ImageBuffer = AllocatePool (ImageSize + 2048);
    if (NULL == ImageBuffer) {
      DebugPrint (DEBUG_ERROR, "%a: Allocate memory failed\n", __FUNCTION__);
      goto exit;
    }

    Status = pFile->Read (pFile, &ImageSize, ImageBuffer);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "%a: Read file %a faild", __FUNCTION__, FileName);
      goto exit;
    }

    DEBUG ((DEBUG_INFO, "Before UpdateDeviceTree Data size: %d\n", ImageSize));

    Status = PlatformUpdateDtb (ImageBuffer, &ImageSize);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "Update device tree failed, %r\n", Status);
      goto exit;
    }

    DEBUG ((DEBUG_INFO, "After update device tree, data size: %d\n", ImageSize));

    Status = pFile->SetPosition (pFile, 0);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "Cannot set file position to first byte: %r\n", Status);
      goto exit;
    }

    FileInfo->FileSize = ImageSize;
    Status             = pFile->SetInfo (pFile, &gEfiFileInfoGuid, FileInfoSize, FileInfo);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "%a %d Set file Info fail Status: %r\n", __FUNCTION__, __LINE__, Status);
      goto exit;
    }

    Status = pFile->Write (pFile, &ImageSize, ImageBuffer);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "%a %d Write file fail Status: %r\n", __FUNCTION__, __LINE__, Status);
      goto exit;
    }

    DEBUG ((DEBUG_INFO, "%a %d Update device tree completed %r\n", __FUNCTION__, __LINE__, Status));
  } else {
    DebugPrint (DEBUG_ERROR, "Dtb file not found\n");
    return;
  }

exit:
  if (FileInfo) {
    FreePool (FileInfo);
  }

  if (ImageBuffer) {
    FreePool (ImageBuffer);
  }

  if (pFile) {
    pFile->Flush (pFile);
    pFile->Close (pFile);
  }

  DEBUG ((DEBUG_INFO, "%a: Exit\n", __FUNCTION__));

  return;
}

EFI_STATUS
EFIAPI
PlatformDtbUpdateEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_EVENT   Event;

  DEBUG ((DEBUG_INFO, "%a: platform DTS updated enabled: %d\n", __FUNCTION__, PcdGetBool (PcdPlatformDtbUpdateEnable)));

  if (!PcdGetBool (PcdPlatformDtbUpdateEnable)) {
    return Status;
  }

  //
  // Register notify function to update platform DTB on ReadyToBoot Event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PlatformDtsImageUpdateCallBack,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
