/** I2cDxe.c
  I2c driver APIs for read, write, initialize, set speed and reset

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/I2cLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PostCodeLib.h>
#include <Protocol/ConfigParamsManageProtocol.h>
#include "I2cDxe.h"

STATIC CONST EFI_I2C_CONTROLLER_CAPABILITIES  mI2cControllerCapabilities = {
  0,
  0,
  0,
  0
};

STATIC I2C_DEVICE_PATH  mI2cDevicePathProtocolTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(OFFSET_OF (I2C_DEVICE_PATH, End)),
        (UINT8)(OFFSET_OF (I2C_DEVICE_PATH, End) >> 8),
      },
    },
    {
      0  // GUID
    },
  },
  0,    // MmioBase
  0xFF, // I2c Bus
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

/**
  Function to set i2c bus frequency

  @param   This            Pointer to I2c master protocol
  @param   BusClockHertz   value to be set

  @retval EFI_SUCCESS      Operation successfull
**/
STATIC
EFI_STATUS
EFIAPI
SetBusFrequency (
  IN CONST EFI_I2C_MASTER_PROTOCOL  *This,
  IN OUT UINTN                      *BusClockHertz
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  I2C_CONTROLLER_CONTEXT  *I2c;

  I2c = I2C_CONTROLLER_FROM_THIS (This);

  I2c->Descriptor.BusClk = *BusClockHertz;

  Status = I2cSetClk (&I2c->Descriptor);

  return Status;
}

/**
  Function to reset I2c Controller

  @param  This             Pointer to I2c master protocol

  @return EFI_SUCCESS      Operation successfull
**/
STATIC
EFI_STATUS
EFIAPI
Reset (
  IN CONST EFI_I2C_MASTER_PROTOCOL  *This
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
StartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL  *This,
  IN UINTN                          SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET         *RequestPacket,
  IN EFI_EVENT                      Event            OPTIONAL,
  OUT EFI_STATUS                    *I2cStatus       OPTIONAL
  )
{
  EFI_STATUS              Status;
  I2C_CONTROLLER_CONTEXT  *I2c;

  I2c = I2C_CONTROLLER_FROM_THIS (This);

  Status = I2cMasterXfer (&I2c->Descriptor, SlaveAddress, RequestPacket);

  return Status;
}

EFI_STATUS
I2cHostInit (
  IN UINT32  I2cBus,
  IN UINT32  Clock
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  I2C_CONTROLLER_CONTEXT  *I2c   = NULL;

  I2c = AllocateZeroPool (sizeof (I2C_CONTROLLER_CONTEXT));
  if (I2c != NULL) {
    I2c->Signature                           = I2C_CONTROLLER_SIGNATURE;
    I2c->Descriptor.Bus                      = I2cBus;
    I2c->Descriptor.MemBase                  = I2cGetMemBase (I2cBus);
    I2c->Descriptor.BusClk                   = Clock;
    I2c->I2cMaster.SetBusFrequency           = SetBusFrequency;
    I2c->I2cMaster.Reset                     = Reset;
    I2c->I2cMaster.StartRequest              = StartRequest;
    I2c->I2cMaster.I2cControllerCapabilities = &mI2cControllerCapabilities;

    Status = I2cInitialize (&I2c->Descriptor);

    CopyMem ((VOID *)(UINTN)&(I2c->DevicePath), (VOID *)(UINTN)&mI2cDevicePathProtocolTemplate, sizeof (I2C_DEVICE_PATH));
    CopyGuid (&I2c->DevicePath.Vendor.Guid, &gCixI2cDevicePathGuid);
    I2c->DevicePath.MmioBase = I2c->Descriptor.MemBase;
    I2c->DevicePath.Bus      = I2c->Descriptor.Bus;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &I2c->Controller,
                    &gEfiI2cMasterProtocolGuid,
                    (VOID **)&I2c->I2cMaster,
                    &gEfiDevicePathProtocolGuid,
                    &I2c->DevicePath,
                    NULL
                    );

    if (EFI_ERROR (Status)) {
      FreePool (I2c);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a failed to allocate buffer(size %x) for I2C host(base %x)\n", __FUNCTION__, sizeof (I2C_CONTROLLER_CONTEXT), I2c->Descriptor.MemBase));
    Status = EFI_OUT_OF_RESOURCES;
  }

  return Status;
}

EFI_STATUS
EFIAPI
I2cDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status;
  CONFIG_PARAMS_DATA_BLOCK           *ConfigData;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;

  POST_CODE (I2cInitDxeStart);

  Status = gBS->LocateProtocol (
                  &gCixConfigParamsManageProtocolGuid,
                  NULL,
                  (VOID **)&ConfigManage
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: config parameters invalid %r\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  ConfigData = ConfigManage->Data;

  for (UINT8 I2cBus = 0; I2cBus < I2C_CTRL_NUM; I2cBus++) {
    if (!ConfigData->Fch.I2c[I2cBus].Enable) {
      continue;
    }

    I2cEnvInit (I2cBus);
    Status = I2cHostInit (I2cBus, ConfigData->Fch.I2c[I2cBus].BusFreq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to initialize I2C[%d], status: %r\n", I2cBus, Status));
      // ASSERT_EFI_ERROR(Status);
    }
  }

  POST_CODE (I2cInitDxeEnd);

  return EFI_SUCCESS;
}
