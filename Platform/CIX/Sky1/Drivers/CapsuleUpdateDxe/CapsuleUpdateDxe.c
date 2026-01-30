/** @file

  Copyright 2025 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/FileHandleLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CixGPNVLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/WatchdogTimer.h>

#define MAX_CAPSULE_NUM  10

//
// Define how many block descriptors we want to test with.
//
UINTN  NumberOfDescriptors = 1;

STATIC
EFI_STATUS
BeforeCapsuleUpdateHook(
  VOID
)
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  UINTN                             VarSize;
  UINT32                            VarAttr;
  UINT8                             CapsuleUpdateEnabled;
  EFI_WATCHDOG_TIMER_ARCH_PROTOCOL  *Wdt = NULL;

  VarSize = sizeof (UINT8);
  Status = gRT->GetVariable (
                  L"CapsuleUpdate",
                  &gCixGPNVGuid,
                  &VarAttr,
                  &VarSize,
                  &CapsuleUpdateEnabled
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: get capsule update variable failed: %r\n", __FUNCTION__, Status));
  } else {
    CapsuleUpdateEnabled = 0;

    Status = gRT->SetVariable (
                    L"CapsuleUpdate",
                    &gCixGPNVGuid,
                    VarAttr,
                    sizeof (UINT8),
                    &CapsuleUpdateEnabled
                    );
  }

#ifdef CIX_GPNV_ENABLE
  CixGPNVSync(
    &gCixGPNVGuid,
    FixedPcdGet32(PcdNorFlashVarSyncRegionBase),
    FixedPcdGet32(PcdNorFlashVarSyncRegionSize),
    SIZE_4KB
    );
 #endif

  Status = gBS->LocateProtocol (
                  &gEfiWatchdogTimerArchProtocolGuid,
                  NULL,
                  (VOID **)&Wdt
                  );
  if (!EFI_ERROR (Status)) {
    Wdt->SetTimerPeriod (Wdt, 0);
  }

  return Status;
}

STATIC
EFI_STATUS
AfterCapsuleUpdateHook(
  VOID
)
{

  return EFI_SUCCESS;
}

/**
  Validate if it is valid capsule header

  This function assumes the caller provided correct CapsuleHeader pointer
  and CapsuleSize.

  This function validates the fields in EFI_CAPSULE_HEADER.

  @param[in] CapsuleHeader  Points to a capsule header.
  @param[in] CapsuleSize    Size of the whole capsule image.

**/
BOOLEAN
IsValidCapsuleHeader (
  IN EFI_CAPSULE_HEADER  *CapsuleHeader,
  IN UINT64              CapsuleSize
  )
{
  if (CapsuleSize < sizeof (EFI_CAPSULE_HEADER)) {
    return FALSE;
  }

  if (CapsuleHeader->CapsuleImageSize != CapsuleSize) {
    return FALSE;
  }

  if (CapsuleHeader->HeaderSize > CapsuleHeader->CapsuleImageSize) {
    return FALSE;
  }

  if (CapsuleHeader->HeaderSize < sizeof (EFI_CAPSULE_HEADER)) {
    return FALSE;
  }

  return TRUE;
}

/**
  Build Gather list for a list of capsule images.

  @param[in]  CapsuleBuffer    An array of pointer to capsule images
  @param[in]  FileSize         An array of UINTN to capsule images size
  @param[in]  CapsuleNum       The count of capsule images
  @param[out] BlockDescriptors The block descriptors for the capsule images

  @retval EFI_SUCCESS The block descriptors for the capsule images are constructed.
**/
EFI_STATUS
BuildGatherList (
  IN VOID                           **CapsuleBuffer,
  IN UINTN                          *FileSize,
  IN UINTN                          CapsuleNum,
  OUT EFI_CAPSULE_BLOCK_DESCRIPTOR  **BlockDescriptors
  )
{
  EFI_STATUS                    Status;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors1;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors2;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptorPre;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptorsHeader;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *TempBlockPtr;
  UINT8                         *TempDataPtr;
  UINTN                         SizeLeft;
  UINTN                         Size;
  INT32                         Count;
  INT32                         Number;
  UINTN                         Index;

  TempBlockPtr           = NULL;
  BlockDescriptors1      = NULL;
  BlockDescriptors2      = NULL;
  BlockDescriptorPre     = NULL;
  BlockDescriptorsHeader = NULL;

  for (Index = 0; Index < CapsuleNum; Index++) {
    //
    // Allocate memory for the descriptors.
    //
    if (NumberOfDescriptors == 1) {
      Count = 2;
    } else {
      Count = (INT32)(NumberOfDescriptors + 2) / 2;
    }

    Size              = Count * sizeof (EFI_CAPSULE_BLOCK_DESCRIPTOR);
    BlockDescriptors1 = AllocateRuntimeZeroPool (Size);
    if (BlockDescriptors1 == NULL) {
      DEBUG ((DEBUG_ERROR, "%a: failed to allocate memory for descriptors\n", __FUNCTION__));
      Status = EFI_OUT_OF_RESOURCES;
      goto ERREXIT;
    } else {
      DEBUG ((DEBUG_INFO, "%a: creating capsule descriptors at 0x%X\n", __FUNCTION__, (UINTN)BlockDescriptors1));
      DEBUG ((DEBUG_INFO, "%a: capsule data starts          at 0x%X with size 0x%X\n", __FUNCTION__, (UINTN)CapsuleBuffer[Index], FileSize[Index]));
    }

    //
    // Record descriptor header
    //
    if (Index == 0) {
      BlockDescriptorsHeader = BlockDescriptors1;
    }

    if (BlockDescriptorPre != NULL) {
      BlockDescriptorPre->Union.ContinuationPointer = (UINTN)BlockDescriptors1;
      BlockDescriptorPre->Length                    = 0;
    }

    //
    // Fill them in
    //
    TempBlockPtr = BlockDescriptors1;
    TempDataPtr  = CapsuleBuffer[Index];
    SizeLeft     = FileSize[Index];
    for (Number = 0; (Number < Count - 1) && (SizeLeft != 0); Number++) {
      //
      // Divide remaining data in half
      //
      if (NumberOfDescriptors != 1) {
        if (SizeLeft == 1) {
          Size = 1;
        } else {
          Size = SizeLeft / 2;
        }
      } else {
        Size = SizeLeft;
      }

      TempBlockPtr->Union.DataBlock = (UINTN)TempDataPtr;
      TempBlockPtr->Length          = Size;
      DEBUG ((DEBUG_INFO, "%a: capsule block/size              0x%X/0x%X\n", __FUNCTION__, (UINTN)TempDataPtr, Size));
      SizeLeft    -= Size;
      TempDataPtr += Size;
      TempBlockPtr++;
    }

    //
    // Allocate the second list, point the first block's last entry to point
    // to this one, and fill this one in. Worst case is that the previous
    // list only had one element that pointed here, so we need at least two
    // elements -- one to point to all the data, another to terminate the list.
    //
    if ((NumberOfDescriptors != 1) && (SizeLeft != 0)) {
      Count = (INT32)(NumberOfDescriptors + 2) - Count;
      if (Count == 1) {
        Count++;
      }

      Size              = Count * sizeof (EFI_CAPSULE_BLOCK_DESCRIPTOR);
      BlockDescriptors2 = AllocateRuntimeZeroPool (Size);
      if (BlockDescriptors2 == NULL) {
        DEBUG ((DEBUG_ERROR, "%a: failed to allocate memory for descriptors\n", __FUNCTION__));
        Status = EFI_OUT_OF_RESOURCES;
        goto ERREXIT;
      }

      //
      // Point the first list's last element to point to this second list.
      //
      TempBlockPtr->Union.ContinuationPointer = (UINTN)BlockDescriptors2;

      TempBlockPtr->Length = 0;
      TempBlockPtr         = BlockDescriptors2;
      for (Number = 0; Number < Count - 1; Number++) {
        //
        // If second-to-last one, then dump rest to this element
        //
        if (Number == (Count - 2)) {
          Size = SizeLeft;
        } else {
          //
          // Divide remaining data in half
          //
          if (SizeLeft == 1) {
            Size = 1;
          } else {
            Size = SizeLeft / 2;
          }
        }

        TempBlockPtr->Union.DataBlock = (UINTN)TempDataPtr;
        TempBlockPtr->Length          = Size;
        DEBUG ((DEBUG_INFO, "%a: capsule block/size              0x%X/0x%X\n", __FUNCTION__ , (UINTN)TempDataPtr, Size));
        SizeLeft    -= Size;
        TempDataPtr += Size;
        TempBlockPtr++;
        if (SizeLeft == 0) {
          break;
        }
      }
    }

    BlockDescriptorPre = TempBlockPtr;
    BlockDescriptors1  = NULL;
  }

  //
  // Null-terminate.
  //
  if (TempBlockPtr != NULL) {
    TempBlockPtr->Union.ContinuationPointer = (UINTN)NULL;
    TempBlockPtr->Length                    = 0;
    *BlockDescriptors                       = BlockDescriptorsHeader;
  }

  return EFI_SUCCESS;

ERREXIT:
  if (BlockDescriptors1 != NULL) {
    FreePool (BlockDescriptors1);
  }

  if (BlockDescriptors2 != NULL) {
    FreePool (BlockDescriptors2);
  }

  return Status;
}

/**
  Clear the Gather list for a list of capsule images.

  @param[in]  BlockDescriptors The block descriptors for the capsule images
  @param[in]  CapsuleNum       The count of capsule images
**/
VOID
CleanGatherList (
  IN EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors,
  IN UINTN                         CapsuleNum
  )
{
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *TempBlockPtr;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *TempBlockPtr1;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *TempBlockPtr2;
  UINTN                         Index;

  if (BlockDescriptors != NULL) {
    TempBlockPtr1 = BlockDescriptors;
    while (1) {
      TempBlockPtr = TempBlockPtr1;
      for (Index = 0; Index < CapsuleNum; Index++) {
        if (TempBlockPtr[Index].Length == 0) {
          break;
        }
      }

      if (TempBlockPtr[Index].Union.ContinuationPointer == (UINTN)NULL) {
        break;
      }

      TempBlockPtr2 = (VOID *)((UINTN)TempBlockPtr[Index].Union.ContinuationPointer);
      FreePool (TempBlockPtr1);
      TempBlockPtr1 = TempBlockPtr2;
    }
  }
}

BOOLEAN
IsEfiSysPartitionDevicePath (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  HARDDRIVE_DEVICE_PATH     *Hd;

  //
  // Check if the device path contains GPT/MBR node
  //
  TempDevicePath = DevicePath;

  while (!IsDevicePathEnd (TempDevicePath)) {
    if ((DevicePathType (TempDevicePath) == MEDIA_DEVICE_PATH) &&
        (DevicePathSubType (TempDevicePath) == MEDIA_HARDDRIVE_DP))
    {
      Hd = (HARDDRIVE_DEVICE_PATH *)TempDevicePath;
      if ((Hd->MBRType == MBR_TYPE_EFI_PARTITION_TABLE_HEADER) || (Hd->MBRType == MBR_TYPE_PCAT)) {
        break;
      }
    }

    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }

  if (!IsDevicePathEnd (TempDevicePath)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

EFI_STATUS
ReadUpdateFile (
  IN OUT  VOID                             *Buffer,
  IN OUT  UINTN                            *BufferSize,
  IN      CHAR16                           *FileName,
  IN      EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs
  )
{
  EFI_STATUS         Status;
  EFI_FILE           *Root;
  EFI_FILE           *FileHandle;
  EFI_FILE_PROTOCOL  *DirHandle;
  UINT64             FileSize;

  DirHandle  = NULL;
  FileHandle = NULL;

  //
  // Open Root from SFS
  //
  Status = Fs->OpenVolume (Fs, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: cannot open volume. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = Root->Open (Root, &DirHandle, EFI_CAPSULE_FILE_DIRECTORY, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: unable to locate %s directory\n", __FUNCTION__, EFI_CAPSULE_FILE_DIRECTORY));
    return Status;
  }

  //
  // Open UpdateCapsule file
  //
  Status = DirHandle->Open (DirHandle, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: unable to read %s file\n", __FUNCTION__, FileName));
    return Status;
  }

  Status = FileHandleGetSize (FileHandle, &FileSize);

  if (*BufferSize < FileSize) {
    *BufferSize = FileSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = FileHandleRead (FileHandle, BufferSize, Buffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: unable to read capsule file to %s, Status = %r\n", __FUNCTION__, FileName, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: succeed to read %s\n", __FUNCTION__, FileName));
  FileHandleClose (FileHandle);

  return EFI_SUCCESS;
}

EFI_STATUS
AuthenticateAndUpdate (
  IN OUT  VOID   *CapsuleBuffer,
  IN OUT  UINTN  CapsuleBufferSize
  )
{
  EFI_STATUS                    Status;
  EFI_CAPSULE_BLOCK_DESCRIPTOR  *BlockDescriptors = NULL;
  EFI_CAPSULE_HEADER            *CapsuleHeaderArray[MAX_CAPSULE_NUM + 1];
  UINT64                        MaxCapsuleSize;
  EFI_RESET_TYPE                ResetType;

  if (!IsValidCapsuleHeader (CapsuleBuffer, CapsuleBufferSize)) {
    DEBUG ((DEBUG_ERROR, "%a: capsule image is not a valid capsule.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Every capsule use 2 descriptor 1 for data 1 for end
  //
  Status = BuildGatherList (&CapsuleBuffer, &CapsuleBufferSize, 1, &BlockDescriptors);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  BeforeCapsuleUpdateHook();
  //
  // Call the runtime service capsule.
  //
  CapsuleHeaderArray[0] = (EFI_CAPSULE_HEADER *)CapsuleBuffer;
  CapsuleHeaderArray[1] = NULL;
  //
  // Inquire platform capability of UpdateCapsule.
  //
  Status = gRT->QueryCapsuleCapabilities (CapsuleHeaderArray, 1, &MaxCapsuleSize, &ResetType);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to query capsule capability - %r\n", __FUNCTION__, Status));
    goto Done;
  }
  if (CapsuleBufferSize > MaxCapsuleSize) {
    DEBUG ((DEBUG_ERROR, "%a: capsule is too large to update, %ld is allowed\n", __FUNCTION__, MaxCapsuleSize));
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  Status = gRT->UpdateCapsule (CapsuleHeaderArray, 1, (UINTN)BlockDescriptors);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: failed to update capsule - %r\n", __FUNCTION__, Status));
    goto Done;
  }

  AfterCapsuleUpdateHook();

  gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, 0, NULL);  //always to reset

Done:
  CleanGatherList (BlockDescriptors, 1);
  return Status;
}

VOID
EFIAPI
ProcessCapsuleOnDisk (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs;
  CHAR16                           *FileName;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;
  VOID                             *CapsuleBuffer = NULL;
  UINTN                            CapsuleBufferSize = 0;
  EFI_HANDLE                       *Handle;
  UINTN                            HandleNum;
  UINT8                            Index;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleNum,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gEfiSimpleFileSystemProtocolGuid not found yet, status: %r\n", Status));
    return;
  }

  //
  // Get a valid file system from boot path
  //
  Fs = NULL;
  FileName = AllocateCopyPool (StrnSizeS(L"BIOS.cap", MAX_UINT16), L"BIOS.cap");

  for (Index = 0; Index < HandleNum; Index++) {
    DevicePath = DevicePathFromHandle (Handle[Index]);
    if (IsEfiSysPartitionDevicePath (DevicePath)) {
      Status = gBS->HandleProtocol (Handle[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&Fs);
      if (!EFI_ERROR (Status)) {
        //
        // Copy capsule image from '\efi\UpdateCapsule\'
        //
        Status = ReadUpdateFile (CapsuleBuffer, &CapsuleBufferSize, FileName, Fs);
        if (Status == EFI_BUFFER_TOO_SMALL) {
          CapsuleBuffer = AllocateZeroPool(CapsuleBufferSize);
          Status = ReadUpdateFile (CapsuleBuffer, &CapsuleBufferSize, FileName, Fs);
        }

        if (!EFI_ERROR (Status)) {
          break;
        } else {
          DEBUG ((DEBUG_ERROR, \
                 "%a: capsule image could not be copied for update at %s\n", \
                 __FUNCTION__, \
                 ConvertDevicePathToText(DevicePath, TRUE,TRUE)));
        }
      }
    }
  }

  if (EFI_ERROR (Status) || (Index >= HandleNum)) {
    DEBUG ((DEBUG_ERROR, "%a: cannot find capsule files for BIOS update!\n", __FUNCTION__));
    return;
  }

  AuthenticateAndUpdate(CapsuleBuffer, CapsuleBufferSize);
}

EFI_STATUS
EFIAPI
CapsuleUpdateDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  VOID                   *Registration;
  UINTN                  VarSize;
  UINT8                  CapsuleUpdateEnabled;

  VarSize = sizeof (UINT8);
  Status = gRT->GetVariable (
                  L"CapsuleUpdate",
                  &gCixGPNVGuid,
                  NULL,
                  &VarSize,
                  &CapsuleUpdateEnabled
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: get capsule update variable failed: %r\n", __FUNCTION__, Status));
  } else {
    if(CapsuleUpdateEnabled == 1) {
      EfiCreateProtocolNotifyEvent (
            &gEfiSimpleFileSystemProtocolGuid,
            TPL_CALLBACK,
            ProcessCapsuleOnDisk,
            NULL,
            &Registration
            );
    }
  }

  return EFI_SUCCESS;
}
