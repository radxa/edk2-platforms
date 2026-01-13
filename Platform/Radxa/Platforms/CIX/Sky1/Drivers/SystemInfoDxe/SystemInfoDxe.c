/** @file
*
*  Copyright 2024 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.
*  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/GpioLib.h>
#include <Library/Cs32g051EcLib.h>

#include "SystemInfoDxe.h"

extern  UINT8  SystemInfoHiiBin[];
extern  UINT8  SystemInfoDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH  mSystemInfoDxeVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SYSTEM_INFO_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

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
  UnicodeString[Index] = 0;
}

VOID
Decode4BitMemType (
  EFI_HII_HANDLE           HiiHandle,
  UINT16                   MemType
  )
{
  CHAR8   DateBuf[128]   = { 0 };
  CHAR16  NewString[128] = { 0 };
  CHAR8   *pFmtStr = NULL;

  switch (MemType) {
    case 0:
      pFmtStr = "4 GiB";
      break;
    case 1:
      pFmtStr = "4 GiB (Hynix)";
      break;
    case 2:
      pFmtStr = "2 GiB";
      break;
    case 3:
      pFmtStr = "12 GiB (Hynix)";
      break;
    case 5:
      pFmtStr = "3 GiB";
      break;
    case 6:
      pFmtStr = "8 GiB (Hive)";
      break;
    case 7:
      pFmtStr = "6 GiB (Hive)";
      break;
    case 8:
      pFmtStr = "8 GiB (V1.1)";
      break;
    case 9:
      pFmtStr = "12 GiB (Hive)";
      break;
    case 10:
      pFmtStr = "16 GiB (Hynix)";
      break;
    case 11:
      pFmtStr = "8 GiB (Samsung)";
      break;
    case 12:
      pFmtStr = "16 GiB (Rayson)";
      break;
    case 13:
      pFmtStr = "4 GiB (Hive)";
      break;
    case 14:
      pFmtStr = "8 GiB (Hynix)";
      break;
    case 15:
      pFmtStr = "Development Sample";
      break;
    case (UINT16) -1:
      pFmtStr = "Internal Error";
      break;
    default:
      pFmtStr = "Undefined";
      break;
  }
  AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "%a | (Raw Value: %d)", pFmtStr, MemType);
  AsciiToUnicode (DateBuf, NewString);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_TYPE_VALUE), NewString, NULL);
}

VOID
Decode2BitMemType (
  EFI_HII_HANDLE           HiiHandle,
  UINT16                   MemType
  )
{
  CHAR8   DateBuf[128]   = { 0 };
  CHAR16  NewString[128] = { 0 };
  CHAR8   *pFmtStr = NULL;

  switch (MemType) {
    case 3:
      pFmtStr = "Development Sample";
      break;
    case (UINT16) -1:
      pFmtStr = "Internal Error";
      break;
    default:
      pFmtStr = "Undefined";
      break;
  }
  AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "%a | (Raw Value: %d)", pFmtStr, MemType);
  AsciiToUnicode (DateBuf, NewString);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_TYPE_VALUE), NewString, NULL);
}

VOID
InitializeHardwareInfoWithEC (
  EFI_HII_HANDLE           HiiHandle
  )
{
  EFI_STATUS                 Status;
  BOARD_ID_INFO              BoardID;
  EC_PLATFORM_PROTOCOL       *pEcPlatformProtocol = NULL;
  EC_RESPONSE                EcResponse;
  CHAR8                      DateBuf[128]   = { 0 };
  CHAR16                     NewString[128] = { 0 };
  PMIC_VERSION_INFO          PmicVerInfo;
  UINT8                      Index;

  Status = gBS->LocateProtocol (&gCixEcPlatformProtocolGuid, NULL, (VOID **)&pEcPlatformProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate Protocol failed for %g\n", &gCixEcPlatformProtocolGuid));
    return;
  }

  Status = pEcPlatformProtocol->Transfer (pEcPlatformProtocol, EC_COMMAND_GET_BOARD_ID, NULL, &EcResponse);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Board ID failed\n"));
    return;
  }

  BoardID.Bits.PcbSku = EcResponse.BoardId.Id.Sku + (EcResponse.BoardId.Id.SkuExt << 3);
  switch (BoardID.Bits.PcbSku) {
    case 4:
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PCB_SKU_VALUE), L"Radxa Orion O6", NULL);
      break;
    default:
      AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "Undefined (%d)", BoardID.Bits.PcbSku);
      AsciiToUnicode (DateBuf, NewString);
      HiiSetString (HiiHandle, STRING_TOKEN (STR_PCB_SKU_VALUE), NewString, NULL);
      break;
  }

  BoardID.Bits.MemType = EcResponse.BoardId.Id.Memory + (EcResponse.BoardId.Id.MemExt << 3);
  Decode4BitMemType (HiiHandle, BoardID.Bits.MemType);

  BoardID.Bits.BoardRev = EcResponse.BoardId.Id.Rev;
  switch (BoardID.Bits.BoardRev) {
    default:
      AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "%d", BoardID.Bits.BoardRev);
      AsciiToUnicode (DateBuf, NewString);
      HiiSetString (HiiHandle, STRING_TOKEN (STR_BOARD_REV_VALUE), NewString, NULL);
      break;
  }

  Status = pEcPlatformProtocol->Transfer (pEcPlatformProtocol, EC_COMMAND_GET_PD_VERSION, NULL, &EcResponse);
  if (!EFI_ERROR (Status)) {
    AsciiSPrint (
      (CHAR8 *)DateBuf,
      sizeof (DateBuf),
      "%d.%d %d.%d",
      EcResponse.PdVer.Pd2Ver & 0xFF,
      (EcResponse.PdVer.Pd2Ver & 0xFF00) >> 8,
      EcResponse.PdVer.Pd1Ver & 0xFF,
      (EcResponse.PdVer.Pd1Ver & 0xFF00) >> 8
      );
    AsciiToUnicode (DateBuf, NewString);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PD_VER_VALUE), NewString, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PD_VER_VALUE), L"Undefined", NULL);
  }

  Status = CsuPmMsgGetPmicVersion (&PmicVerInfo);
  if (!EFI_ERROR (Status)) {
    ZeroMem (NewString, sizeof (NewString));
    for (Index = 0; Index < PmicVerInfo.Count; Index++) {
      UnicodeSPrint ((CHAR16 *)NewString, sizeof (NewString), L"%s%x.%x ", NewString, (PmicVerInfo.Version[Index] >> 8) & 0xFF, PmicVerInfo.Version[Index] & 0xFF);
    }
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PMIC_VER_VALUE), NewString, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PMIC_VER_VALUE), L"Undefined", NULL);
  }
}

/**
  GPIOs is ordered from the lowest bit to the highest bit.
**/
EFI_STATUS
GpioGetMultiple (
  IN UINT32   *GPIOs,
  IN UINT32   Count,
  OUT UINT32  *Output
  )
{
  UINT32              Index;
  EFI_STATUS          Status;
  IO_INOUT_VALUE_SEL  Value;

  *Output = 0;

  for (Index = 0; Index < Count; Index++) {
    Status = GpioGet (GPIOs[Index], &Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGet failed for %d: %r\n", __FUNCTION__, Index, Status));
      *Output = -1;
      return Status;
    } else if (Value == INOUT_VALUE_DEFAULT) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGet returned default value for %d\n", __FUNCTION__, Index));
      *Output = -1;
      return EFI_DEVICE_ERROR;
    }

    DEBUG ((DEBUG_INFO, "%a: GPIOs[%d] = %d\n", __FUNCTION__, Index, Value));
    *Output |= (Value << Index);
    DEBUG ((DEBUG_INFO, "%a: Output = %d\n", __FUNCTION__, *Output));
  }

  DEBUG ((DEBUG_INFO, "%a: final Output = %d\n", __FUNCTION__, *Output));
  return EFI_SUCCESS;
}

VOID
InitializeHardwareInfoWithGPIO (
  EFI_HII_HANDLE           HiiHandle
  )
{
  EFI_STATUS          Status;
  CHAR16 *            SystemProductName;
  UINT32              MemType;
  CHAR8               AsciiBuf[128]   = { 0 };
  CHAR16              UnicodeBuf[128] = { 0 };
  // FIXME: This is not the type defined in PdEcLib.h`
  CS32G051_RESULT_CMD_GET_IC_STATUS IcStatus;

  SystemProductName = (CHAR16 *)FixedPcdGetPtr (PcdSystemProductName);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_PCB_SKU_VALUE), SystemProductName, NULL);

  // FIXME: We might have more than 1 PD devices
  Status = PdGetIcStatus(0, &IcStatus);
  if (!EFI_ERROR (Status)) {
    AsciiSPrint (
      (CHAR8 *)AsciiBuf,
      sizeof (AsciiBuf),
      "Boot: v%d.%d.%d\nApp: v%d.%d.%d",
      IcStatus.BootVersionMajor,
      IcStatus.BootVersionMinor,
      IcStatus.BootVersionPatch,
      IcStatus.AppVersionMajor,
      IcStatus.AppVersionMinor,
      IcStatus.AppVersionPatch
      );
    AsciiToUnicode (AsciiBuf, UnicodeBuf);
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PD_VER_VALUE), UnicodeBuf, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PD_VER_VALUE), L"Undefined", NULL);
  }

  if (!StrCmp (L"Radxa Orion O6T", SystemProductName)) {
    Status = GpioGetMultiple ((UINT32[]){49, 50}, 2, &MemType);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGetMultiple failed: %r\n", __FUNCTION__, Status));
      return;
    }
    Decode2BitMemType (HiiHandle, MemType);
  } else if (!StrCmp (L"Radxa Orion O6N", SystemProductName)) {
    Status = GpioGetMultiple ((UINT32[]){49, 50, 57, 60}, 4, &MemType);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: GpioGetMultiple failed: %r\n", __FUNCTION__, Status));
      return;
    }
    Decode4BitMemType (HiiHandle, MemType);
  } else {
    DEBUG ((DEBUG_ERROR, "%a: Unknown product: %a\n", __FUNCTION__, SystemProductName));
    HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_TYPE_VALUE), L"Internal Error", NULL);
  }
}

VOID
InitializeHardwareInfo (
  EFI_HII_HANDLE           HiiHandle
  )
{
  EFI_STATUS                 Status;
  CIX_SOC_INFO_PROTOCOL      *pCixSocInfoProtocol = NULL;
  CHAR8                      DateBuf[128]   = { 0 };
  CHAR16                     NewString[128] = { 0 };

  Status = gBS->LocateProtocol (&gCixSocInfoProtocolGuid, NULL, (VOID **)&pCixSocInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate Protocol failed for %g\n", &gCixSocInfoProtocolGuid));
    return;
  }

  AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "%d GiB", pCixSocInfoProtocol->MemInfo->TotalSize/1024);
  AsciiToUnicode (DateBuf, NewString);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_SIZE_VALUE), NewString, NULL);

  // Double Data Rate so we x2 the frequency
  AsciiSPrint ((CHAR8 *)DateBuf, sizeof (DateBuf), "%d MT/s", pCixSocInfoProtocol->MemInfo->MaxFreq * 2);
  AsciiToUnicode (DateBuf, NewString);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_FREQ_VALUE), NewString, NULL);

  if (FixedPcdGetBool (PcdEcAcpiI2cEn)) {
    InitializeHardwareInfoWithEC (HiiHandle);
  } else {
    InitializeHardwareInfoWithGPIO (HiiHandle);
  }
}

VOID
EFIAPI
GetFirmwareSystemInfo (
  EFI_HII_HANDLE           HiiHandle
  )
{
  EFI_STATUS               Status;
  CIX_FW_VERSION_PROTOCOL  *pFwVerProtocol;
  CHAR16                   *pFwVerBuff;
  UINT32                   FwVerSize;

  Status    = gBS->LocateProtocol (
                     &gCixFwVersionProtocolGuid,
                     NULL,
                     (VOID **)&pFwVerProtocol
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate CIX_FW_VERSION_PROTOCOL\n"));
    return;
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerSE, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_SE_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_SE_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerPBL, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PBL_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PBL_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerATF, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_ATF_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_ATF_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerPM, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PM_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_PM_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerTEE, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_TEE_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_TEE_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerUEFI, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_UEFI_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_UEFI_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerSTMM, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_STMM_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_STMM_VER_VALUE), L"N/A", NULL);
  }

  Status = pFwVerProtocol->GetFwVersion (FwVerEC, &pFwVerBuff, &FwVerSize);
  if (!EFI_ERROR (Status)) {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_EC_VER_VALUE), pFwVerBuff, NULL);
  } else {
    HiiSetString (HiiHandle, STRING_TOKEN (STR_EC_VER_VALUE), L"N/A", NULL);
  }
}

VOID
EFIAPI
GetSourceCodeInfo (
  EFI_HII_HANDLE           HiiHandle
  )
{
  CHAR16      *NewString;

  NewString = (CHAR16 *)FixedPcdGetPtr (PcdEdk2CixCommitHash);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_SOURCE_CODE_EDK2_CIX_VALUE), NewString, NULL);

  NewString = (CHAR16 *)FixedPcdGetPtr (PcdEdk2CommitHash);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_SOURCE_CODE_EDK2_VALUE), NewString, NULL);

  NewString = (CHAR16 *)FixedPcdGetPtr (PcdEdk2NonOSICommitHash);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_SOURCE_CODE_EDK2_NON_OSI_VALUE), NewString, NULL);

  NewString = (CHAR16 *)FixedPcdGetPtr (PcdEdk2PlatformsCommitHash);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_SOURCE_CODE_EDK2_PLATFORMS_VALUE), NewString, NULL);
}

STATIC
EFI_STATUS
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS      Status;
  EFI_HII_HANDLE  HiiHandle;
  EFI_HANDLE      DriverHandle;

  DriverHandle = NULL;
  Status       = gBS->InstallMultipleProtocolInterfaces (
                        &DriverHandle,
                        &gEfiDevicePathProtocolGuid,
                        &mSystemInfoDxeVendorDevicePath,
                        NULL
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (
                &gSystemInfoFormSetGuid,
                DriverHandle,
                SystemInfoDxeStrings,
                SystemInfoHiiBin,
                NULL
                );

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mSystemInfoDxeVendorDevicePath,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  InitializeHardwareInfo (HiiHandle);
  GetFirmwareSystemInfo (HiiHandle);
  GetSourceCodeInfo (HiiHandle);

  return EFI_SUCCESS;
}

/**
  The entry point for SystemInfoDxe driver.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_ALREADY_STARTED     The driver already exists in system.
  @retval EFI_OUT_OF_RESOURCES    Fail to execute entry point due to lack of
                                  resources.
  @retval EFI_SUCCESS             All the related protocols are installed on
                                  the driver.

**/
EFI_STATUS
EFIAPI
SystemInfoDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return InstallHiiPages();
}
