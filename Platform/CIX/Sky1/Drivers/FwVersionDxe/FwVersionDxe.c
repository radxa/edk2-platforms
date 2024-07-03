/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ShMemLib.h>
#include <Protocol/FwVersionProtocol.h>
#include <Protocol/EcPlatformProtocol.h>

VOID
AsciiToUnicode (
  IN    CHAR8   *AsciiString,
  IN    CHAR16  *UnicodeString
  )
{
  UINT8  Index;

  Index = 0;
  while (AsciiString[Index] != 0) {
    UnicodeString[Index] = (CHAR16)AsciiString[Index];
    Index++;
  }
}

UINTN
UnicodeToAscii (
  IN CONST CHAR16  *UStr,
  IN CONST UINTN   Length,
  OUT CHAR8        *AStr
  )
{
  UINTN  Index;

  //
  // just buffer copy, not character copy
  //
  for (Index = 0; Index < Length; Index++) {
    *AStr++ = (CHAR8)*UStr++;
  }

  return Index;
}

STATIC
EFI_STATUS
EFIAPI
FwGetEcVersion (
  IN OUT CHAR16  **FwVerBuff,
  IN OUT UINT32  *FwVerSize
  )
{
  EFI_STATUS  Status;

  EC_RESPONSE_EC_VERSION_INFO  EcVersion;
  EC_PLATFORM_PROTOCOL         *Ec;

  Status = gBS->LocateProtocol (&gCixEcPlatformProtocolGuid, NULL, (VOID **)&Ec);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return EFI_NOT_FOUND;
  }

  Status = Ec->GetEcVersion (Ec, &EcVersion);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return EFI_NOT_FOUND;
  }

  *FwVerSize = AsciiStrLen ((CHAR8 *)EcVersion.Version);
  if (*FwVerSize == 0) {
    return EFI_NOT_FOUND;
  }

  *FwVerBuff = AllocateZeroPool ((*FwVerSize + 1) * 2);

  AsciiToUnicode ((CHAR8 *)EcVersion.Version, (CHAR16 *)*FwVerBuff);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
FwVerGetBoardId (
  IN OUT CHAR16  **FwVerBuff,
  IN OUT UINT32  *FwVerSize
  )
{
  EFI_STATUS            Status;
  EC_PLATFORM_PROTOCOL  *Ec;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gCixEcPlatformProtocolGuid, NULL, (VOID **)&Ec);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return Status;
  }

  *FwVerSize = sizeof (EC_RESPONSE_BOARD_ID_INFO);
  *FwVerBuff = AllocateZeroPool (*FwVerSize);

  Status = Ec->GetBoardId (Ec, (EC_RESPONSE_BOARD_ID_INFO *)(*FwVerBuff));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
GetFwVersion (
  FW_VERSION_TYPE  FwVerType,
  IN OUT CHAR16    **FwVerBuff,
  IN OUT UINT32    *FwVerSize
  )
{
  void        *FwVerAddr = NULL;
  EFI_STATUS  Status;
  UINT32      SmemSize;

  if ((FwVerBuff == NULL) || (FwVerSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  switch (FwVerType) {
    case FwVerSE:
      FwVerAddr = SmemGetAddr (SMEM_VER_SE, &SmemSize);
      if (FwVerAddr == NULL) {
        return EFI_NOT_FOUND;
      }

      // DEBUG ((DEBUG_INFO, "[VER] PBL:%s\n", FwVerAddr));
      break;
    case FwVerPM:
      FwVerAddr = SmemGetAddr (SMEM_VER_PM, &SmemSize);
      if (FwVerAddr == NULL) {
        return EFI_NOT_FOUND;
      }

      // DEBUG ((DEBUG_INFO, "[VER] PBL:%s\n", FwVerAddr));
      break;
    case FwVerTEE:
      FwVerAddr = SmemGetAddr (SMEM_VER_TEE, &SmemSize);
      if (FwVerAddr == NULL) {
        return EFI_NOT_FOUND;
      }

      // DEBUG ((DEBUG_INFO, "[VER] PBL:%s\n", FwVerAddr));
      break;
    case FwVerPBL:
      FwVerAddr = SmemGetAddr (SMEM_VER_PBL, &SmemSize);
      if (FwVerAddr == NULL) {
        return EFI_NOT_FOUND;
      }

      // DEBUG ((DEBUG_INFO, "[VER] PBL:%s\n", FwVerAddr));
      break;
    case FwVerATF:
      FwVerAddr = SmemGetAddr (SMEM_VER_ATF, &SmemSize);
      if (FwVerAddr == NULL) {
        return EFI_NOT_FOUND;
      }

      // DEBUG ((DEBUG_INFO, "[VER] ATF:%s\n", FwVerAddr));
      break;
    case FwVerUEFI:
      *FwVerBuff = (CHAR16 *)PcdGetPtr (PcdFirmwareVersionString);
      *FwVerSize = StrLen (*FwVerBuff);
      return EFI_SUCCESS;

    case FwVerEC:
      Status = FwGetEcVersion (FwVerBuff, FwVerSize);
      if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
      }
      return EFI_SUCCESS;
    case FwVerBoardId:
      Status = FwVerGetBoardId (FwVerBuff, FwVerSize);
      if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
      }
      return EFI_SUCCESS;

    default:
      break;
  }

  if (FwVerAddr == NULL) {
    return EFI_NOT_FOUND;
  }

  if(*(UINT32 *)FwVerAddr == 0xffffffff) {
    return EFI_NOT_FOUND;
  }

  *FwVerSize = AsciiStrLen ((CHAR8 *)FwVerAddr);
  if (*FwVerSize == 0) {
    return EFI_NOT_FOUND;
  }

  *FwVerBuff = AllocateZeroPool ((*FwVerSize + 1) * 2);

  AsciiToUnicode ((CHAR8 *)FwVerAddr, (CHAR16 *)*FwVerBuff);
  // CopyMem (*FwVerBuff, p, *FwVerSize);
  return EFI_SUCCESS;
}

STATIC CIX_FW_VERSION_PROTOCOL  CixFwVerProtocol = {
  GetFwVersion
};

STATIC
VOID
EFIAPI
SetUefiVersion (

  )
{
  void    *FwVerAddrUefi;
  UINT32  SmemSize;
  CHAR16  *FwVerUefiBuff;
  UINTN   FwVerUefiSize;

  FwVerUefiBuff = (CHAR16 *)PcdGetPtr (PcdFirmwareVersionString);
  FwVerUefiSize = StrLen (FwVerUefiBuff) * 2;

  FwVerAddrUefi = SmemGetAddr (SMEM_VER_UEFI, &SmemSize);
  ZeroMem (FwVerAddrUefi, SmemSize);

  UnicodeToAscii(FwVerUefiBuff,StrLen(FwVerUefiBuff),(CHAR8 *)FwVerAddrUefi);
  //CopyMem (FwVerAddrUefi, FwVerUefiBuff, FwVerUefiSize);

  return;
}

STATIC
VOID
EFIAPI
SetBoardInfo (
  )
{
  EFI_STATUS            Status;
  void                  *FwVerAddrBoard;
  UINT32                SmemSize;
  UINT32                BoardId;
  EC_PLATFORM_PROTOCOL  *Ec;
  EC_RESPONSE_EC_VERSION_INFO  EcVersion;
  UINT32                EcVersionSize;
  void    *FwVerAddrEc;


  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gCixEcPlatformProtocolGuid, NULL, (VOID **)&Ec);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    // Write Default Version info
    FwVerAddrBoard = SmemGetAddr (SMEM_INFO_BOARD, &SmemSize);
    MmioWrite32 ((UINTN)FwVerAddrBoard, 0xFFFFFFFF);
    FwVerAddrEc = SmemGetAddr (SMEM_VER_EC, &SmemSize);
    ZeroMem (FwVerAddrEc, SmemSize);
    CopyMem (FwVerAddrEc, "NULL", sizeof("NULL"));
    return;
  }

  Status = Ec->GetBoardId (Ec, (EC_RESPONSE_BOARD_ID_INFO *)&BoardId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return;
  }

  FwVerAddrBoard = SmemGetAddr (SMEM_INFO_BOARD, &SmemSize);
  MmioWrite32 ((UINTN)FwVerAddrBoard, BoardId);

  Status = Ec->GetEcVersion (Ec, &EcVersion);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC platform not ready.\n"));
    return;
  }

  EcVersionSize = AsciiStrLen ((CHAR8 *)EcVersion.Version);
  if (EcVersionSize == 0) {
    return;
  }

  FwVerAddrEc = SmemGetAddr (SMEM_VER_EC, &SmemSize);
  ZeroMem (FwVerAddrEc, SmemSize);

  //UnicodeToAscii(FwVerUefiBuff,StrLen(FwVerUefiBuff),(CHAR8 *)FwVerAddrUefi);
  CopyMem (FwVerAddrEc, (CHAR8 *)EcVersion.Version, EcVersionSize);


  return;
}

VOID
EFIAPI
UpdateBoardInfo (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  SetBoardInfo();

}

EFI_STATUS
EFIAPI
FwVersionDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   BoardInfoUpdateEvent;
  VOID        *Registration;

  DEBUG ((DEBUG_INFO, "%a start.\n", __FUNCTION__));

  SetUefiVersion ();
  SetBoardInfo ();

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gCixFwVersionProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &CixFwVerProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG (
      (DEBUG_ERROR,
       "%a: failed to install fw version protocol (Status == %r)\n",
       __FUNCTION__, Status)
      );
  }


  BoardInfoUpdateEvent = EfiCreateProtocolNotifyEvent (
                                &gCixEcPlatformProtocolGuid,
                                TPL_CALLBACK,
                                UpdateBoardInfo,
                                NULL,
                                &Registration
                                );
  if(BoardInfoUpdateEvent == NULL) {
    DEBUG ((DEBUG_ERROR, "%a UpdateBoardInfo Event Create Failed.  \n", __FUNCTION__));
  }

  return Status;
}
