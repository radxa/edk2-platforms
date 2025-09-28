/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <SeConfigUpdateDxe.h>
#include <SeConfigSetupVar.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>

EFI_GUID  gSeConfigFormSetGuid = {
  0x45f0def4, 0xde9a, 0x449e, { 0x85, 0x2, 0x49, 0x48, 0xc4, 0xf4, 0x30, 0xe0 }
};

EFI_GUID  gSeConfigSetupVariableGuid = {
  0x5c0efc49, 0xe1b2, 0x451a, { 0xac, 0x69, 0x88, 0x4d, 0xef, 0xc6, 0x19, 0x56 }
};

extern  UINT8  SeConfigUpdateHiiBin[];
extern  UINT8  SeConfigUpdateDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH  mSeConfigVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SE_CONFIG_FORMSET_GUID
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
SeConfigUpdateCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS           Status;
  UINTN                VarSize            = 0;
  SE_CONFIG_SETUP_VAR  *pSeConfigSetupVar = NULL;
  UINT8                MemTagExtension;
  UINT16               Param;

  DEBUG ((DEBUG_INFO, "SeConfigUpdateCallback\n"));

  Status = gRT->GetVariable (
                  SE_CONFIG_SETUP_VAR_NAME,
                  &gSeConfigSetupVariableGuid,
                  NULL,
                  &VarSize,
                  pSeConfigSetupVar
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    pSeConfigSetupVar = AllocateZeroPool (VarSize);
    if (pSeConfigSetupVar == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a][%d]: AllocateZeroPool failed\n", __FUNCTION__, __LINE__));
      goto Done;
    }
  } else {
    goto Done;
  }

  Status = gRT->GetVariable (
                  SE_CONFIG_SETUP_VAR_NAME,
                  &gSeConfigSetupVariableGuid,
                  NULL,
                  &VarSize,
                  pSeConfigSetupVar
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: GetVariable failed, Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto Done;
  }

  // Read Memory Tagging Extension State
  Param  = 0x0000;
  Status = MboxMemTagExtensionControl (&Param, &MemTagExtension);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a][%d]: MboxGetMemTagExtension Status = %r\n", __FUNCTION__, __LINE__, Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "[%a][%d]: Current MemTagExtension = %x\n", __FUNCTION__, __LINE__, MemTagExtension));
  if (MemTagExtension != pSeConfigSetupVar->MemTagExtension) {
    Param  = pSeConfigSetupVar->MemTagExtension;
    Param |= 0x0100;
    Status = MboxMemTagExtensionControl (&Param, &MemTagExtension);
    DEBUG ((DEBUG_INFO, "[%a][%d]: MboxSetMemTagExtension Status = %r\n", __FUNCTION__, __LINE__, Status));
  }

Done:
  if (pSeConfigSetupVar != NULL) {
    FreePool (pSeConfigSetupVar);
  }

  return;
}

EFI_STATUS
SeConfigSetupVariableInit (
  VOID
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  UINTN                VarSize;
  SE_CONFIG_SETUP_VAR  pSeConfigSetupVar;

  VarSize = sizeof (SE_CONFIG_SETUP_VAR);

  Status = gRT->GetVariable (
                  SE_CONFIG_SETUP_VAR_NAME,
                  &gSeConfigSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &pSeConfigSetupVar
                  );
  if (EFI_ERROR (Status)) {
    //
    // Variable does not exist yet - create it
    //
    ZeroMem (&pSeConfigSetupVar, VarSize);
    pSeConfigSetupVar.MemTagExtension = 0;

    Status = gRT->SetVariable (
                    SE_CONFIG_SETUP_VAR_NAME,
                    &gSeConfigSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (SE_CONFIG_SETUP_VAR),
                    &pSeConfigSetupVar
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: EfiSetVariable failed - %r\n", __FUNCTION__, Status));
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: EfiGetVariable Success - %r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS
EFIAPI
SeConfigUpdateDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  EFI_EVENT       Event;
  EFI_HII_HANDLE  HiiHandle;
  EFI_HANDLE      DriverHandle;

  // Init SE config setup variable
  Status = SeConfigSetupVariableInit ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: SeConfigSetupVariableInit Status - %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SeConfigUpdateCallback,
                  NULL,
                  &gSetupSaveVariableCallBackGuid,
                  &Event
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SeConfigUpdateDxeEntryPoint: CreateEvent failed: %r\n", Status));
    return Status;
  }

  DriverHandle = NULL;
  Status       = gBS->InstallMultipleProtocolInterfaces (
                        &DriverHandle,
                        &gEfiDevicePathProtocolGuid,
                        &mSeConfigVendorDevicePath,
                        NULL
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (
                &gSeConfigFormSetGuid,
                DriverHandle,
                SeConfigUpdateDxeStrings,
                SeConfigUpdateHiiBin,
                NULL
                );
  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mSeConfigVendorDevicePath,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
