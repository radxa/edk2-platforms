/*
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include "PlatformSmbios.h"
#include <Protocol/FwVersionProtocol.h>
#include <Protocol/EcPlatformProtocol.h>
#include <Protocol/SocInfoProtocol.h>
#include <Library/PrintLib.h>

#define TYPE1_STRINGS                                                                    \
  "Cix Technology Group Co., Ltd.\0"   /* Manufacturer */                                \
  "CIX Board\0"                        /* Product Name */                                \
  "1.0\0"                              /* Version */                                     \
  "Cix System Serial Numbe\0"          /* Serial number */                               \
  "Default\0"                          /* SKUNumber */                                   \
  "Default\0"                          /* System Family */                               \
  "CIX Merak Board\0"                  /* Product Name 0*/                               \
  "CIX Phecda Board\0"                 /* Product Name 1*/

#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE1    Base;
  UINT8                 Strings[sizeof (TYPE1_STRINGS)];
} PLATFORM_SMBIOS_TYPE1;
#pragma pack()

// System information (section 7.2)
STATIC PLATFORM_SMBIOS_TYPE1  mPlatformDefaultType1 = {
  {
    {
      // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE1),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,     // Manufacturer
    2,     // Product Name
    3,     // Version
    4,     // Serial
    { 0xFFFFFFFF,
        0xFFFF,
        0xFFFF,
        { 0xFF,      0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
    },                                                      // UUID
    SystemWakeupTypePowerSwitch,                            // Wakeup type
    5,                                                      // SKU
    6,                                                      // Family
  },
  // Text strings (unformatted)
  TYPE1_STRINGS
};

EFI_STATUS
GetSerialNumStr (
  IN OUT CHAR16  **StrSerialNum
  )
{
  EFI_STATUS             Status;
  CIX_SOC_INFO_PROTOCOL  *pSocInfoProtocol;
  UINT32                 *pSocInfo;
  UINT32                 SocInfoSize;
  UINTN                  BufferSize;

  Status = gBS->LocateProtocol (
                  &gCixSocInfoProtocolGuid,
                  NULL,
                  (VOID **)&pSocInfoProtocol
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: soc info protocol not found\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  Status = pSocInfoProtocol->GetSocInfo (SerialNum, &pSocInfo, &SocInfoSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: GetSerialNum failed\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  BufferSize    = (SocInfoSize*2+1)*sizeof (CHAR16);
  *StrSerialNum = AllocateZeroPool (BufferSize);
  UnicodeSPrint (*StrSerialNum, BufferSize, L"%08x%08x", pSocInfo[0], pSocInfo[1]);
  return EFI_SUCCESS;
}

EFI_STATUS
AddSmbiosType1 (
  IN EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  CIX_FW_VERSION_PROTOCOL  *pFwVerProtocol;
  CHAR16                   *pFwVerBuff;
  UINT32                   FwVerSize;
  EC_RESPONSE_BOARD_ID     *pBoardId;
  UINT16                   Sku;
  UINTN                    StringNumber, SysSnSize, SysUuidSize;
  CHAR16                   *SysSnPtr = NULL;
  CHAR8                    *SysSnBuf, *SysUuidPtr;

  Status = gBS->LocateProtocol (
                  &gCixFwVersionProtocolGuid,
                  NULL,
                  (VOID **)&pFwVerProtocol
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fw ver protocol not found\n", __FUNCTION__));
  } else {
    Status = pFwVerProtocol->GetFwVersion (FwVerBoardId, &pFwVerBuff, &FwVerSize);
    if (!EFI_ERROR (Status)) {
      pBoardId = (EC_RESPONSE_BOARD_ID *)pFwVerBuff;
      Sku      = (pBoardId->Id.SkuExt << 3) + pBoardId->Id.Sku;
      switch (Sku) {
        case 0:
          mPlatformDefaultType1.Base.ProductName = 7;
          break;
        case 4:
          mPlatformDefaultType1.Base.ProductName = 8;
          break;
        default:
          break;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "%a: Board Id:NULL\n", __FUNCTION__));
    }
  }

  // Update UUID
  Status = GetVariable2 (
             L"SystemUUID",
             &gCixGPNVGuid,
             (VOID **)&SysUuidPtr,
             &SysUuidSize
             );
  if (!EFI_ERROR (Status)) {
    CopyMem ((CHAR8 *)&(mPlatformDefaultType1.Base.Uuid), SysUuidPtr, 16);
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add (
                           Smbios,
                           NULL,
                           &SmbiosHandle,
                           (EFI_SMBIOS_TABLE_HEADER *)&mPlatformDefaultType1
                           );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "[%a]:[%dL] Smbios Type1 Table Log Failed! %r \n",
       __FUNCTION__,
       DEBUG_LINE_NUMBER,
       Status
      )
      );
  }

  // update serial number
  Status = GetSerialNumStr (&SysSnPtr);
  if (!EFI_ERROR (Status)) {
    SysSnSize = StrLen (SysSnPtr);
    SysSnBuf  = AllocateZeroPool (SysSnSize+1);
    UnicodeToAscii (SysSnPtr, SysSnSize, SysSnBuf);
    SysSnBuf[SysSnSize] = 0;
    StringNumber        = 4;
    Status              = Smbios->UpdateString (Smbios, &SmbiosHandle, &StringNumber, SysSnBuf);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "Fail to update serial number.\n");
    }

    FreePool (SysSnBuf);
    FreePool (SysSnPtr);
  }

  // override SystemSN with variable
  Status = GetVariable2 (
             L"SystemSN",
             &gCixGPNVGuid,
             (VOID **)&SysSnBuf,
             &SysSnSize
             );
  if (!EFI_ERROR (Status)) {
    StringNumber = 4;
    Status = Smbios->UpdateString (Smbios, &SmbiosHandle, &StringNumber, SysSnBuf);
    if (EFI_ERROR (Status)) {
      DebugPrint (DEBUG_ERROR, "Fail to update serial number.\n");
    }

    FreePool (SysSnBuf);
  }

  return EFI_SUCCESS;
}
