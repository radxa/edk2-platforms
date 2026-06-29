/** @file

  Copyright 2026 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "TrustZoneConfigUpdateDxe.h"
#include <PlatformSetupVar.h>
#include "TFAConfigurationTable.h"

#define SIZE_OF_TZCONFIG                                (sizeof(CixTrustZoneConfig))

static CixTrustZoneConfig mTFAConfig = {0};
static UINT8              mVarSPEEnable = 0xFF;

/**
 * Check CRC checksum
 */
STATIC BOOLEAN CheckCrc(const CixTrustZoneConfig *config)
{
    UINT32 *ptr = (UINT32 *)config;
    UINT32 length = config->length;
    UINT32 cka = 0, ckb = 0;

    if (length > 4096) {
        return FALSE;
    }

    /* Set CRC fields to zero for calculation */
    UINT32 bin_cka = config->crc1;
    UINT32 bin_ckb = config->crc2;

    /* Temporary clear CRC fields for calculation */
    CixTrustZoneConfig temp_config = *config;
    temp_config.crc1 = temp_config.crc2 = 0;
    ptr = (UINT32 *)&temp_config;

    for (UINT32 i = 0; i * 4 < length; i++) {
        cka += ptr[i];
        ckb += cka;
    }

    return (bin_cka == cka && bin_ckb == ckb);
}

/**
 * Calculate CRC checksum
 */
static VOID GenCrc(CixTrustZoneConfig *config)
{
    uint32_t *ptr = (uint32_t *)config;
    uint32_t length = config->length;
    uint32_t cka = 0, ckb = 0;

    if (length > 4096) {
        return;
    }

    config->crc1 = 0;
    config->crc2 = 0;

    for (uint32_t i = 0; i * 4 < length; i++) {
        cka += ptr[i];
        ckb += cka;
    }
    config->crc1 = cka;
    config->crc2 = ckb;
}

static BOOLEAN CheckVer(const CixTrustZoneConfig *config)
{
    return (config->versionMajor == TRUSTZONE_CONFIG_VERSION_MAJOR &&
            config->versionMinor == TRUSTZONE_CONFIG_VERSION_MINOR);
}

static BOOLEAN CheckSig(const CixTrustZoneConfig *config)
{
    return (config->signature == (UINT32)TRUSTZONE_CONFIG_SIGNATURE);
}

static BOOLEAN CheckTFAConfigValid (const CixTrustZoneConfig *config)
{
    return (CheckSig(config) && CheckVer(config) && CheckCrc(config));
}

static EFI_STATUS GetOrSetDataFromFlash(VOID** pData, BOOLEAN isWrite)
{
    EFI_STATUS             Status = EFI_SUCCESS;
    UINT8*                 pImage = NULL;
    CIX_FW_UPDATE_PROTOCOL *FlashUpdateProtocol = NULL;
    UINTN                  Length = 0x1000;

    pImage = AllocateZeroPool(Length);
    if (pImage == NULL) {
        DEBUG ((DEBUG_ERROR, "[%a] AllocateZeroPool for Flash Image failed\n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
    }

    *pData = pImage;

    Status = gBS->LocateProtocol (&gCixFirmwareUpdateProtocolGuid, NULL, (VOID **)&FlashUpdateProtocol);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] Locate FlashUpdateProtocol failed: %r\n", __FUNCTION__, Status));
        return Status;
    }

    Status = FlashUpdateProtocol->FirmwareRawEntryUpdate (FIRMWARE_TYPE_TFA_CONF, pImage, Length, ENTRY_READ, NULL);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] FirmwareRawEntryUpdate Read failed: %r\n", __FUNCTION__, Status));
        return Status;
    }

    if (isWrite) {
        CopyMem(pImage, &mTFAConfig, SIZE_OF_TZCONFIG);
        Status = FlashUpdateProtocol->FirmwareRawEntryUpdate (FIRMWARE_TYPE_TFA_CONF, (UINT8*)pImage, Length, ENTRY_WRITE, NULL);
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "[%a] FirmwareRawEntryUpdate Write failed: %r\n", __FUNCTION__, Status));
            return Status;
        } else {
            DEBUG ((DEBUG_INFO, "[%a] TFA Configuration updated successfully\n", __FUNCTION__));
        }
    }

    return EFI_SUCCESS;
}

VOID
TrustZoneConfigUpdateCallback (
    IN EFI_EVENT  Event,
    IN VOID       *Context
    )
{
    EFI_STATUS                Status;
    PLATFORM_SETUP_DATA       PlatformSetupVar;
    UINT8*                    pImage = NULL;
    UINTN                     mVarSize = sizeof (PLATFORM_SETUP_DATA);

    DEBUG ((DEBUG_INFO, "[%a] Entering Callback\n", __FUNCTION__));

    Status = gRT->GetVariable (
                    PLATFORM_SETUP_VAR,
                    &gPlatformSetupVariableGuid,
                    NULL,
                    &mVarSize,
                    &PlatformSetupVar
                    );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] GetVariable failed: %r\n", __FUNCTION__, Status));
        return;
    }

    if (mVarSPEEnable == PlatformSetupVar.SPEEnable) {
        return;
    }

    mVarSPEEnable = PlatformSetupVar.SPEEnable;
    mTFAConfig.config.data.SPEFeatureEn = mVarSPEEnable;

    if (CheckTFAConfigValid(&mTFAConfig) == FALSE) {
        GenCrc(&mTFAConfig);
    }

    Status = GetOrSetDataFromFlash((VOID**)&pImage, TRUE);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] GetDataFromFlash failed: %r\n", __FUNCTION__, Status));
    }

    if (pImage != NULL) {
        FreePool(pImage);
        pImage = NULL;
    }
}

EFI_STATUS
EFIAPI
TrustZoneConfigUpdateDxeEntryPoint (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
    )
{
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_EVENT           Event;
    PLATFORM_SETUP_DATA PlatformSetupVar;
    UINT8*              pImage = NULL;
    BOOLEAN             isNeedUpdate = TRUE;
    UINTN               mVarSize = sizeof (PLATFORM_SETUP_DATA);

    mVarSize = sizeof (PLATFORM_SETUP_DATA);

    Status = gRT->GetVariable (
                PLATFORM_SETUP_VAR,
                &gPlatformSetupVariableGuid,
                NULL,
                &mVarSize,
                &PlatformSetupVar
                );
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] GetVariable failed: %r\n", __FUNCTION__, Status));
        goto Done;
    }

    Status = GetOrSetDataFromFlash((VOID**)&pImage, FALSE);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] GetDataFromFlash failed: %r\n", __FUNCTION__, Status));
        goto Done;
    }

    CopyMem(&mTFAConfig, pImage, SIZE_OF_TZCONFIG);
    if (CheckTFAConfigValid(&mTFAConfig) == FALSE) {
      DEBUG((DEBUG_ERROR, "TF-A config is invalid.\n"));
      goto Done;
    }

    if (PlatformSetupVar.SPEEnable != mTFAConfig.config.data.SPEFeatureEn) {
        mVarSPEEnable = mTFAConfig.config.data.SPEFeatureEn;
    } else {
        isNeedUpdate = FALSE;
    }
    //
    // Register event for Setup Variable Save
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    TrustZoneConfigUpdateCallback,
                    NULL,
                    &gSetupSaveVariableCallBackGuid,
                    &Event
                    );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[%a] CreateEventEx failed: %r\n", __FUNCTION__, Status));
    }

    Done:
    if (pImage != NULL) {
        FreePool(pImage);
        pImage = NULL;
    }

    // Update Setup Variable if needed
    // if error, update to 0xff
    // otherwise, update to the value in flash
    if (isNeedUpdate) {
        PlatformSetupVar.SPEEnable = mVarSPEEnable;
        Status = gRT->SetVariable (
                        PLATFORM_SETUP_VAR,
                        &gPlatformSetupVariableGuid,
                        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof (PLATFORM_SETUP_DATA),
                        &PlatformSetupVar
                        );
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "TFA config can't set variable!\n"));
        }
    }

    return Status;
}
