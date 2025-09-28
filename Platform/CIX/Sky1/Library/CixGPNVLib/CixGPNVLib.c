/**
  @file CixGPNVLib.c

  Copyright 2025 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved. *

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "Base.h"
#include "Guid/GlobalVariable.h"
#include "ProcessorBind.h"
#include "Uefi/UefiBaseType.h"
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Library/CixGPNVLib.h>

#include <Protocol/DiskIo.h>
#include <Protocol/DevicePath.h>
/**
  Get the Nor Flash Disk Io Protocol

  @param  NorFlashDiskIo   Disk Io Protocol
  @param  MediaId          Media Id

  @retval EFI_SUCCESS      Get Nor Flash Disk Io Protocol success
  @retval EFI_NOT_FOUND    Not found NorFlash device
**/
STATIC
EFI_STATUS
EFIAPI
GetNorFlashDiskIo (
  IN OUT EFI_DISK_IO_PROTOCOL **NorFlashDiskIo,
  IN     UINT8                *MediaId
  )
{
  EFI_HANDLE                *DiskIoHandles;
  UINTN                     NumberDiskIoHandles;
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  VOID                      *TempPtr;
  EFI_STATUS                Status;
  // find DiskIoProtocol
  Status = gBS->LocateHandleBuffer (
                                    ByProtocol,
                                    &gEfiDiskIoProtocolGuid,
                                    NULL,
                                    &NumberDiskIoHandles,
                                    &DiskIoHandles
                                    );
  if(EFI_ERROR(Status)) {
    Status = EFI_NOT_FOUND;
  } else {
    // DEBUG ((DEBUG_VERBOSE, "[FwU] NumberDiskIoHandles %d\n", NumberDiskIoHandles));
    for (Index = 0; Index < NumberDiskIoHandles; Index++) {
      Status = gBS->HandleProtocol (DiskIoHandles[Index], &gCixFlashBlockIoProtocolGuid, (VOID **)&TempPtr);
      if(EFI_ERROR(Status)){
        continue;
      }
      DevicePath = DevicePathFromHandle (DiskIoHandles[Index]);
      if (!DevicePath) {
        continue;
      }
      *MediaId = ((NOR_FLASH_DEVICE_PATH *)DevicePath)->Index;
      Status  = gBS->HandleProtocol (DiskIoHandles[Index], &gEfiDiskIoProtocolGuid, (VOID **)NorFlashDiskIo);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_VERBOSE, "Locate NorFlash DiskIoProtocol success\n"));
        break;
      }
    }
    if (Index == NumberDiskIoHandles) {
      DEBUG ((DEBUG_VERBOSE, "Not found NorFlash device.\n"));
      Status = EFI_NOT_FOUND;
    }
  }
  return Status;
}

/**
  Dump the record Information for Debug

  @param  RecordHeader     comment
  @param  RecordCount      comment
**/
STATIC
VOID
DumpRecordHeader(
  IN     VARIABLE_RECORD_HEADER **RecordHeader,
  IN     UINTN                 RecordCount
  )
{
  DEBUG((DEBUG_INFO, "DumpRecordHeader: %p %d\n", RecordHeader, RecordCount));
  if(RecordHeader == NULL) {
    DEBUG((DEBUG_INFO, "RecordHeader is NULL\n"));
    return;
  }
  for(UINTN Index = 0; Index < RecordCount; Index++) {
    DEBUG((DEBUG_INFO, "Index: %04X %p ", Index, RecordHeader[Index]));
    DEBUG((DEBUG_INFO, "VariableNameSize: %04x ", RecordHeader[Index]->VariableNameSize));
    DEBUG((DEBUG_INFO, "VariableSize: %04x ", RecordHeader[Index]->VariableSize));
    DEBUG((DEBUG_INFO, "RecordSize: %04x ", RecordHeader[Index]->RecordSize));
    DEBUG((DEBUG_INFO, "Data: %s\n", RecordHeader[Index]->Data));
  }
}

/**
  Write a record to the flash region. The record is in the format of VARIABLE_RECORD_HEADER.

  @param  FlashRegionBase  Flash Region Base Address
  @param  FlashRegionSize  Flash Region Size
  @param  RecordIndex      Record Index in the flash region.
  @param  Record           Record Header Pointer

  @retval EFI_SUCCESS      Write record to flash success
  @retval EFI_NOT_FOUND    Get DiskIoProtocol failed
  @retval EFI_OUT_OF_RESOURCES  Flash Region is Full
  @retval Others           Write record to flash failed
**/
STATIC
EFI_STATUS
EFIAPI
WriteRecord2FlashRegion (
  IN     UINTN FlashRegionBase,
  IN     UINTN FlashRegionSize,
  IN     UINTN RecordIndex,
  IN     VARIABLE_RECORD_HEADER *Record
  )
{
  EFI_STATUS Status;
  EFI_DISK_IO_PROTOCOL *NorFlashDiskIo;
  UINT8 MediaId;
  UINTN Offset;
  // Get the Disk IO protocol for the NOR flash device
  Status = GetNorFlashDiskIo (&NorFlashDiskIo, &MediaId);
  if (EFI_ERROR(Status)) {
    DebugPrint (DEBUG_ERROR, "GetNorFlashDiskIo failed: %r\n", Status);
    return Status;
  }

  ASSERT((FlashRegionBase & (SIZE_4KB-1)) == 0 && (FlashRegionSize & (SIZE_4KB-1)) == 0);
  if(RecordIndex == 0xFFFF) {
    DebugPrint (DEBUG_ERROR, "RecordIndex is invalid\n");
    return EFI_INVALID_PARAMETER;
  } else {
    Offset = FlashRegionBase + RecordIndex * SIZE_4KB;
    if(Offset > FlashRegionSize + FlashRegionBase) {
      DebugPrint (DEBUG_ERROR, "Flash Region Full!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    Status = NorFlashDiskIo->WriteDisk (NorFlashDiskIo,
                                        MediaId,
                                        Offset,
                                        Record->RecordSize,
                                        Record);
  }
  return Status;
}

/**
  Free the record Pointer array & record resources.

  @param  RecordArray      Record Header Pointer List
  @param  RecordCount      Record Count
**/
STATIC
VOID
FreeRecordArray(
  IN OUT VARIABLE_RECORD_HEADER **RecordArray,
  IN     UINTN RecordCount
  ){
  DEBUG((DEBUG_VERBOSE, "FreeRecordArray: %p %d\n", RecordArray, RecordCount));
  if(RecordArray != NULL){
    for (UINTN Index = 0; Index<RecordCount; Index++) {
      DEBUG((DEBUG_VERBOSE, "FreeRecordArray: %p %d\n", RecordArray[Index], Index));
      if(RecordArray[Index] != NULL){
        FreePool(RecordArray[Index]);
      }
    }
    FreePool(RecordArray);
    RecordArray = NULL;
  }
}
/**
  Get the All Vendor Guid Var and save to Record array.
  The record is in the format of VARIABLE_RECORD_HEADER.

  @param  VendorGuid       Vendor Specific Guid
  @param  Record           Address of A Pointer which Point to Record Header Pointer
  @param  RecordCount      Record Count

  @retval EFI_SUCCESS      Get all vendor guid var success
  @retval Other            Get all vendor guid var failed
**/
STATIC
EFI_STATUS
GetAllVendorGuidVar (
  IN     EFI_GUID *VendorGuid,
  IN OUT VARIABLE_RECORD_HEADER ***Record,
  IN OUT UINTN *RecordCount
  )
{
  EFI_STATUS               Status;
  EFI_GUID                 GPGuid;
  UINTN                    VariableNameBufferSize;
  UINTN                    NewVariableNameBufferSize;
  CHAR16                   *VariableName;
  UINTN                    VariableDataSize;
  VOID                     *VariableData;
  UINT32                   VariableAttributes;
  VARIABLE_RECORD_HEADER   *RecordHeader;
  VARIABLE_RECORD_HEADER   **RecordArray;
  UINTN                    VariableCount;
  VariableCount = 0;
  CopyGuid(&GPGuid, VendorGuid);
  // First, get the number of variables in the gVendorGuid namespace
  RecordArray = AllocateZeroPool(sizeof(VARIABLE_RECORD_HEADER *) * (*RecordCount));
  if (RecordArray == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG((DEBUG_VERBOSE, "RecordArray:%p Variable \n", RecordArray));
  *Record = (VOID*)RecordArray;


  // Then, save all variables to flash
  VariableNameBufferSize = 128;
  VariableName = AllocateZeroPool(VariableNameBufferSize);
  if (VariableName == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG((DEBUG_VERBOSE, "GetGuid:%g Variable \n", &GPGuid));
    for (;;) {
      if(VariableCount >= *RecordCount) {
        FreePool(VariableName);
        return EFI_OUT_OF_RESOURCES;
      }
      NewVariableNameBufferSize = VariableNameBufferSize;
      Status = gRT->GetNextVariableName(&NewVariableNameBufferSize, VariableName, &GPGuid);

      if(Status == EFI_SUCCESS && (!CompareGuid(&GPGuid, VendorGuid))) {
        continue;
      }
      DEBUG ((DEBUG_VERBOSE, "GetNextVariableName %g %s Sta:%r \n", &GPGuid, VariableName, Status));
      if (Status == EFI_NOT_FOUND) {
        Status = EFI_SUCCESS;
        break;
      } else if (Status == EFI_BUFFER_TOO_SMALL) {
        VariableName = ReallocatePool(VariableNameBufferSize, NewVariableNameBufferSize, VariableName);
        if (VariableName == NULL) {
          Status = EFI_OUT_OF_RESOURCES;
          break;
        }
        Status = gRT->GetNextVariableName(&VariableNameBufferSize, VariableName, &GPGuid);
        VariableNameBufferSize = NewVariableNameBufferSize;
        DEBUG ((DEBUG_VERBOSE, "Re GetVarName %d Sta:%r \n", VariableNameBufferSize, Status));
      }

      if (EFI_ERROR(Status)) {
        break;
      }

      // Get variable data
      VariableDataSize = 0;
      Status = gRT->GetVariable(VariableName, &GPGuid, &VariableAttributes, &VariableDataSize, NULL);

      if (Status != EFI_BUFFER_TOO_SMALL) {
        break;
      }

      VariableData = AllocatePool(VariableDataSize);
      if (VariableData == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }

      Status = gRT->GetVariable(VariableName, &GPGuid, &VariableAttributes, &VariableDataSize, VariableData);
      DEBUG ((DEBUG_VERBOSE, "Get Var:%g %s Sta:%r \n", &GPGuid, VariableName, Status));

      if (EFI_ERROR(Status)) {
        break;
      }

      RecordHeader = AllocateZeroPool(sizeof(VARIABLE_RECORD_HEADER) + StrSize(VariableName) + VariableDataSize);
      if(RecordHeader == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }

      RecordHeader->Signature = CIX_VAR_SYNC_SIGNATURE;
      RecordHeader->VariableNameSize = StrSize(VariableName);
      RecordHeader->VariableSize = VariableDataSize;
      RecordHeader->RecordSize = sizeof(VARIABLE_RECORD_HEADER) + RecordHeader->VariableNameSize + VariableDataSize;
      CopyMem(RecordHeader->Data, VariableName, RecordHeader->VariableNameSize);
      CopyMem((UINT8 *)RecordHeader->Data + RecordHeader->VariableNameSize, VariableData, VariableDataSize);
      *RecordArray = RecordHeader;
      RecordArray ++;
      VariableCount++;
      FreePool(VariableData);
    }
  FreePool(VariableName);
  *RecordCount = VariableCount;
  DEBUG((DEBUG_VERBOSE, "GetAllVendorGuidVar count: %d %p %r\n", VariableCount, Record, Status));
  if (VariableCount == 0 || EFI_ERROR(Status)) {
    FreeRecordArray(*Record, VariableCount);
    *Record = NULL;
  }
  return Status;
}

/**
  Get the All Record From Flash Record

  @param  FlashRegionBase  Flash Region Base Address
  @param  FlashRegionSize  Flash Region Size
  @param  Record           Address of A Pointer which Point to Record Header Pointer
  @param  Count            Record Count

  @retval EFI_SUCCESS      Get all vendor guid var success
  @retval Other            Get all vendor guid var failed
**/
STATIC
EFI_STATUS
GetAllRecordFromFlash(
  IN     UINTN FlashRegionBase,
  IN     UINTN FlashRegionSize,
  IN OUT VARIABLE_RECORD_HEADER ***Record,
  IN OUT UINTN *Count
  )
{
  EFI_STATUS Status;
  EFI_DISK_IO_PROTOCOL *NorFlashDiskIo;
  UINT8 MediaId;
  VARIABLE_RECORD_HEADER *TempRecord;
  VARIABLE_RECORD_HEADER *RecordHeader;
  VARIABLE_RECORD_HEADER **RecordArray;
  UINTN Offset;
  UINTN FlashRecordCount;

  ASSERT( (SIZE_4KB * (*Count)) == FlashRegionSize);
  ASSERT((FlashRegionBase & (SIZE_4KB-1)) == 0 && (FlashRegionSize & (SIZE_4KB-1)) == 0);
  RecordArray = AllocateZeroPool(sizeof(VARIABLE_RECORD_HEADER *) * (*Count));
  if (RecordArray == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *Record = RecordArray;


  Status = GetNorFlashDiskIo (&NorFlashDiskIo, &MediaId);
  if (EFI_ERROR(Status)) {
    DebugPrint (DEBUG_ERROR, "GetNorFlashDiskIo failed: %r\n", Status);
    return Status;
  }

  TempRecord = AllocatePages(SIZE_4KB / EFI_PAGE_SIZE);
  if (TempRecord == NULL) {
    DebugPrint (DEBUG_ERROR, "AllocatePages failed\n");
    return EFI_OUT_OF_RESOURCES;
  }
  FlashRecordCount = 0;
  Offset = FlashRegionBase;
  for(; Offset < FlashRegionSize + FlashRegionBase; Offset += SIZE_4KB) {
    ZeroMem(TempRecord, SIZE_4KB);
    Status = NorFlashDiskIo->ReadDisk(NorFlashDiskIo,
                                      MediaId,
                                      Offset,
                                      SIZE_4KB,
                                      TempRecord);
    DEBUG((DEBUG_VERBOSE, "Read: %08X Status:%r\n", Offset, Status));
    if(Status == EFI_SUCCESS && TempRecord->Signature == CIX_VAR_SYNC_SIGNATURE) {
      RecordHeader = AllocatePool(TempRecord->RecordSize);
      if(RecordHeader == NULL) {
        FreePages(TempRecord, SIZE_4KB / EFI_PAGE_SIZE);
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }
      CopyMem(RecordHeader, TempRecord, TempRecord->RecordSize);
      *RecordArray = RecordHeader;
      RecordArray++;
      FlashRecordCount++;
    } else {
      break;
    }
  }
  *Count = FlashRecordCount;
  if(FlashRecordCount==0){
    if(*Record != NULL) {
      FreePool(*Record);
      *Record = NULL;
    }
    if(TempRecord != NULL) {
      FreePages(TempRecord, SIZE_4KB / EFI_PAGE_SIZE);
    }
  }
  return Status;
}

/**
  Synchronize variables between gVendorGuid Variable and FlashRegionBase storage.
  This function will check the records in the flash region and the gVendorGuid Variable.
  If the record is not in the flash region, it will be added to the flash region.
  If the record is not in the gVendorGuid Variable, it will be added to the gVendorGuid Variable.
  If the record is different, it will be updated in the flash region or gVendorGuid Variable.
  @param  Guid             Vendor Guid
  @param  FlashRegionBase  Flash Region Base Address
  @param  FlashRegionSize  Flash Region Size
  @param  RecordSize       Record Size

  @retval EFI_SUCCESS      Get all vendor guid var success
  @retval Other            Get all vendor guid var failed
**/
EFI_STATUS
EFIAPI
CixGPNVSync(
  EFI_GUID *Guid,
  UINTN    FlashRegionBase,
  UINTN    FlashRegionSize,
  UINTN    RecordSize
){
  EFI_STATUS          Status;
  VARIABLE_RECORD_HEADER **FlashRecordArray,**VariableRecordArray;
  UINTN                  FlashRecCount,VariableRecCount;

  BOOLEAN             IsFlashRecordExist;
  BOOLEAN             IsVariableExist;
  UINTN               NewFlashRecCount;
  DEBUG((DEBUG_INFO, "SyncVarRegion: %g %08X %d %d\n", Guid, FlashRegionBase, FlashRegionSize, RecordSize));
  FlashRecordArray = NULL;
  VariableRecordArray = NULL;
  FlashRecCount = FlashRegionSize/RecordSize;
  VariableRecCount = FlashRecCount;
  // Get all record from flash region
  Status = GetAllRecordFromFlash(FlashRegionBase, FlashRegionSize, &FlashRecordArray, &FlashRecCount);
  if (EFI_ERROR(Status)) {
    DebugPrint (DEBUG_ERROR, "GetAllRecordFromFlash failed: %r\n", Status);
  }
  DEBUG((DEBUG_VERBOSE, "GetAllRecordFromFlash count: %d\n", FlashRecCount));
  DumpRecordHeader(FlashRecordArray, FlashRecCount);
  // Get all record from variable
  Status = GetAllVendorGuidVar(Guid, &VariableRecordArray, &VariableRecCount);
  if (EFI_ERROR(Status)) {
    DebugPrint (DEBUG_ERROR, "GetAllVendorGuidVar failed: %r\n", Status);
  }
  DumpRecordHeader(VariableRecordArray, VariableRecCount);
 if(FlashRecCount > 0 || VariableRecCount > 0) {
    // Add new record to variable which record in flash region but not in variable
    // Sync variable record from flash region if record is different
    for(UINTN Index = 0; Index < FlashRecCount; Index++) {
      IsVariableExist = FALSE;
      for(UINTN VarIndex = 0; VarIndex < VariableRecCount; VarIndex++) {
        if(FlashRecordArray[Index]->VariableNameSize == VariableRecordArray[VarIndex]->VariableNameSize &&
          CompareMem(FlashRecordArray[Index]->Data,VariableRecordArray[VarIndex]->Data,FlashRecordArray[Index]->VariableNameSize) == 0
        ) {
          IsVariableExist = TRUE;
          DEBUG((DEBUG_VERBOSE, "Record SameName: %s\n", VariableRecordArray[VarIndex]->Data));
          // Sync flash record from variable if record is different
          if(CompareMem(VariableRecordArray[VarIndex],
                        FlashRecordArray[Index],
                        VariableRecordArray[VarIndex]->RecordSize) != 0) {
            Status = WriteRecord2FlashRegion(FlashRegionBase,
                                    FlashRegionSize,
                                    Index,
                                    VariableRecordArray[VarIndex]);
            if(EFI_ERROR(Status)) {
              DebugPrint (DEBUG_ERROR, "WriteRecord2FlashRegion failed: %r\n", Status);
            }
            DebugPrint(DEBUG_INFO, "Sync FlashRecord:%s From Variable.\n", VariableRecordArray[VarIndex]->Data);
          }
          break;
        }
      }
      // Add to variable if record not exist in variable but exist in flash region
      if(!IsVariableExist) {
        DEBUG((DEBUG_VERBOSE, "Record Only in Flash: %s\n", FlashRecordArray[Index]->Data));
        // Delete record from flash region
        Status = gRT->SetVariable((CHAR16*)FlashRecordArray[Index]->Data,
                                  Guid,
                                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                                  FlashRecordArray[Index]->VariableSize,
                                  (UINT8*)(FlashRecordArray[Index]->Data) + FlashRecordArray[Index]->VariableNameSize);
        if(EFI_ERROR(Status)) {
          DebugPrint (DEBUG_ERROR, "Delete variable failed: %r\n", Status);
          break;
        }
        DebugPrint(DEBUG_INFO, "Add  Variable:%s From Flash.\n", FlashRecordArray[Index]->Data);
      }
    }
    // Add new record to Flash region which record in variable but not in flash region
    NewFlashRecCount = FlashRecCount;
    for(UINTN VarIndex = 0; VarIndex < VariableRecCount; VarIndex++) {
      IsFlashRecordExist = FALSE;
      for(UINTN Index = 0; Index < FlashRecCount; Index++) {
        if(CompareMem(VariableRecordArray[VarIndex]->Data,
          FlashRecordArray[Index]->Data,
          VariableRecordArray[VarIndex]->VariableNameSize) == 0) {
          IsFlashRecordExist = TRUE;
          break;
        }
      }
      if(!IsFlashRecordExist) {
        DEBUG((DEBUG_VERBOSE, "Record Only in Variable: %s\n", VariableRecordArray[VarIndex]->Data));
        Status = WriteRecord2FlashRegion(FlashRegionBase,
                                    FlashRegionSize,
                                    NewFlashRecCount++,
                                    VariableRecordArray[VarIndex]);
        if(EFI_ERROR(Status)) {
          DebugPrint (DEBUG_ERROR, "WriteRecord2FlashRegion failed: %r\n", Status);
          break;
        }
        DebugPrint(DEBUG_INFO, "Add  FlashRecord:%s From Variable.\n", VariableRecordArray[VarIndex]->Data);
      }
    }
    FreeRecordArray(FlashRecordArray, FlashRecCount);
    FreeRecordArray(VariableRecordArray, VariableRecCount);
  }
  return Status;
}
