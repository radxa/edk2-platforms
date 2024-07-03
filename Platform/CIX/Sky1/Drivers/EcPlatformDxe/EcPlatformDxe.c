#/** @file EcPlatformDxe.c
#
#  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
#
#**/

#include <EcPlatformDxe.h>

EFI_I2C_MASTER_PROTOCOL  *mI2cMaster = NULL;

STATIC
EFI_STATUS
PlatformGetEcVersion (
  IN     EC_PLATFORM_PROTOCOL         *This,
  IN OUT EC_RESPONSE_EC_VERSION_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetEcVersion (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetBoardId (
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_BOARD_ID_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetBoardId (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetBatteryStaticInfo (
  IN     EC_PLATFORM_PROTOCOL             *This,
  IN OUT EC_RESPONSE_BATTERY_STATIC_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetBatteryStaticInfo (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetBatteryDynamicInfo (
  IN     EC_PLATFORM_PROTOCOL              *This,
  IN OUT EC_RESPONSE_BATTERY_DYNAMIC_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetBatteryDynamicInfo (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetFarmId (
  IN     EC_PLATFORM_PROTOCOL      *This,
  IN OUT EC_RESPONSE_FRAM_ID_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetFarmId (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetGpio (
  IN     EC_PLATFORM_PROTOCOL  *This,
  IN OUT EC_W_GPIO_INFO        *Info
  )
{
  EFI_STATUS  Status;

  Status = SetGpio (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetGpio (
  IN     EC_PLATFORM_PROTOCOL  *This,
  IN OUT EC_R_GPIO_INFO        *Info
  )
{
  EFI_STATUS  Status;

  Status = GetGpio (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetPmicVersion (
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_PMIC_VER_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetPmicVersion (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetPdVersion (
  IN     EC_PLATFORM_PROTOCOL     *This,
  IN OUT EC_RESPONSE_PD_VER_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetPdVersion (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetGreenPakVersion (
  IN     EC_PLATFORM_PROTOCOL           *This,
  IN OUT EC_RESPONSE_GREENPAK_VER_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetGreenPakVersion (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetChagerInfo (
  IN     EC_PLATFORM_PROTOCOL      *This,
  IN OUT EC_RESPONSE_CHARGER_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetChagerInfo (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetFanRpm (
  IN     EC_PLATFORM_PROTOCOL  *This,
  IN OUT EC_FAN_SET_RPM        *Info
  )
{
  EFI_STATUS  Status;

  Status = SetFanRpm (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetFanRpm (
  IN     EC_PLATFORM_PROTOCOL     *This,
  IN OUT EC_RESPONSE_FAN_GET_RPM  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetFanRpm (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGSetPwmDuty (
  IN     EC_PLATFORM_PROTOCOL  *This,
  IN OUT EC_PWM_SET_DUTY       *Info
  )
{
  EFI_STATUS  Status;

  Status = SetPwmDuty (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetPwmDuty (
  IN     EC_PLATFORM_PROTOCOL   *This,
  IN  EC_PWM_GET_DUTY           *Param,
  OUT EC_RESPONSE_PWM_GET_DUTY  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetPwmDuty (Param, Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetThermalFanAutoCtl (
  IN     EC_PLATFORM_PROTOCOL          *This,
  IN OUT EC_THERMAL_AUTO_FAN_CTL_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = SetThermalFanAutoCtl (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetPvtTemp (
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_PVT_TEMP_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetPvtTemp (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetPoweroffRsn (
  IN     EC_PLATFORM_PROTOCOL            *This,
  IN OUT EC_RESPONSE_POWER_OFF_RSN_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetPoweroffRsn (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetECAutoPowerOn (
  IN     EC_PLATFORM_PROTOCOL               *This,
  IN OUT EC_RESPONSE_EC_AUTO_POWER_ON_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = SetECAutoPowerOn (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformGetAcpiIntEvent (
  IN     EC_PLATFORM_PROTOCOL             *This,
  IN OUT EC_RESPONSE_ACPI_INT_EVENT_INFO  *Info
  )
{
  EFI_STATUS  Status;

  Status = GetAcpiIntEvent (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetPdFwUpdateInfo (
  IN     EC_PLATFORM_PROTOCOL         *This,
  IN     EC_PARAMS_PD_FW_UPDATE_INFO  *Info
  )
{
  return SetPdFwUpdateInfo (Info);
}

STATIC
EFI_STATUS
PlatformGetPdFwUpdateState (
  IN     EC_PLATFORM_PROTOCOL                 *This,
  IN     EC_PARAMS_PD_FW_UPDATE_STATE_INFO    *Param,
  OUT    EC_RESPONSE_PD_FW_UPDATE_STATE_INFO  *Info
  )
{
  return GetPdFwUpdateState (Param, Info);
}

STATIC
EFI_STATUS
PlatformTransPdFwBin (
  IN  EC_PLATFORM_PROTOCOL            *This,
  IN  EC_PARAMS_PD_FW_BIN_TRANS_INFO  *Param
  )
{
  return TransPdFwBin (Param);
}

STATIC
EFI_STATUS
PlatformForceECMirror (
  IN     EC_PLATFORM_PROTOCOL    *This,
  IN     EC_PARAMS_FORCE_MIRROR  *Info
  )
{
  EFI_STATUS  Status;

  Status = ForceECMirror (Info);

  return Status;
}

STATIC
EFI_STATUS
PlatformSetAlsMode (
  IN     EC_PLATFORM_PROTOCOL    *This,
  IN     EC_PARAMS_ALS_MODE_CTL  *Info
  )
{
  EFI_STATUS  Status;

  Status = SetAlsMode (Info);

  return Status;
}

GLOBAL_REMOVE_IF_UNREFERENCED EC_PLATFORM_PROTOCOL  mEcPlatformProtocolInstance = {
  NULL,
  EC_PROTOCOL_VERSION,
  PlatformGetEcVersion,
  PlatformGetBoardId,
  PlatformGetBatteryStaticInfo,
  PlatformGetBatteryDynamicInfo,
  PlatformGetFarmId,
  PlatformSetGpio,
  PlatformGetGpio,
  PlatformGetPmicVersion,
  PlatformGetPdVersion,
  PlatformGetGreenPakVersion,
  PlatformGetChagerInfo,
  PlatformSetFanRpm,
  PlatformGetFanRpm,
  PlatformGSetPwmDuty,
  PlatformGetPwmDuty,
  PlatformSetThermalFanAutoCtl,
  PlatformGetPvtTemp,
  PlatformGetPoweroffRsn,
  PlatformSetECAutoPowerOn,
  PlatformGetAcpiIntEvent,
  PlatformSetPdFwUpdateInfo,
  PlatformGetPdFwUpdateState,
  PlatformTransPdFwBin,
  PlatformForceECMirror,
  PlatformSetAlsMode,
};

/**
    This function is the entry point for this DXE.
    @param ImageHandle Image handle
    @param SystemTable Pointer to the system table

    @retval Return Status based on errors

**/
EFI_STATUS
EFIAPI
EcPlatformDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  EFI_HANDLE                         *Handle;
  UINTN                              HandleNum;
  UINTN                              Index;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  UINTN                              BusClockHertz = 100000; // 100KHz by default
  CONFIG_PARAMS_DATA_BLOCK           *ConfigData;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;

  if (PcdGetBool (PcdEcAcpiI2cEn) == TRUE) {
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiI2cMasterProtocolGuid,
                    NULL,
                    &HandleNum,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "gEfiI2cMasterProtocolGuid not found yet, status: %r\n", Status));
    } else {
      for (Index = 0; Index < HandleNum; Index++) {
        Status = gBS->HandleProtocol (
                        Handle[Index],
                        &gEfiDevicePathProtocolGuid,
                        (VOID **)&DevicePath
                        );
        if (EFI_ERROR (Status)) {
          continue;
        } else {
          if ((DevicePath->Type == HARDWARE_DEVICE_PATH) && (DevicePath->SubType == HW_VENDOR_DP)) {
            if (((I2C_DEVICE_PATH *)DevicePath)->Bus == FixedPcdGet8 (PcdEcI2cBus)) {
              Status = gBS->LocateProtocol (
                              &gCixConfigParamsManageProtocolGuid,
                              NULL,
                              (VOID **)&ConfigManage
                              );

              if (!EFI_ERROR (Status)) {
                ConfigData    = ConfigManage->Data;
                BusClockHertz = ConfigData->Fch.I2c[FixedPcdGet8 (PcdEcI2cBus)].BusFreq;
              } else {
                DEBUG ((DEBUG_ERROR, "%a: config parameters is invalid %r\n", __FUNCTION__, Status));
              }

              Status = gBS->HandleProtocol (
                              Handle[Index],
                              &gEfiI2cMasterProtocolGuid,
                              (VOID **)&mI2cMaster
                              );
              Status = mI2cMaster->SetBusFrequency (mI2cMaster, &BusClockHertz);

              mEcPlatformProtocolInstance.Handle = Handle[Index];
              Status                             = gBS->InstallProtocolInterface (
                                                          &Handle[Index],
                                                          &gCixEcPlatformProtocolGuid,
                                                          EFI_NATIVE_INTERFACE,
                                                          &mEcPlatformProtocolInstance
                                                          );
            }
          }
        }
      }

      gBS->FreePool (Handle);
    }
  }

  return Status;
}
