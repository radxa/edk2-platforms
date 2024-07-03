#/**
#
#  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
#
#**/

#ifndef _EC_PLATFORM_PROTOCOL_H_
#define _EC_PLATFORM_PROTOCOL_H_

  #include <Library/EcLib.h>
  #include <Protocol/I2cDevicePath.h>

typedef struct _EC_PLATFORM_PROTOCOL EC_PLATFORM_PROTOCOL;

typedef
  EFI_STATUS
(EFIAPI *GET_EC_VERSION)(
  IN     EC_PLATFORM_PROTOCOL         *This,
  IN OUT EC_RESPONSE_EC_VERSION_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_BOARD_ID)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_BOARD_ID_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_BATTTERY_STATIC_INFO)(
  IN     EC_PLATFORM_PROTOCOL             *This,
  IN OUT EC_RESPONSE_BATTERY_STATIC_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_BATTTERY_DYNAMIC_INFO)(
  IN     EC_PLATFORM_PROTOCOL              *This,
  IN OUT EC_RESPONSE_BATTERY_DYNAMIC_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_FARM_ID)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_FRAM_ID_INFO   *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_GPIO)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_W_GPIO_INFO             *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_GPIO)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_R_GPIO_INFO             *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_PMIC_VERSION)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_PMIC_VER_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_PD_VERSION)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_PD_VER_INFO    *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_GREENPAK_VERSION)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_GREENPAK_VER_INFO    *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_CHARGER_INFO)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_CHARGER_INFO   *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_FAN_RPM)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_FAN_SET_RPM             *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_FAN_RPM)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_FAN_GET_RPM    *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_FAN_DUTY)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_PWM_SET_DUTY            *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_FAN_DUTY)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN     EC_PWM_GET_DUTY            *Param,
  OUT    EC_RESPONSE_PWM_GET_DUTY   *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_AUTO_FAN_CTL)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_THERMAL_AUTO_FAN_CTL_INFO            *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_PVT_TEMP)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_PVT_TEMP_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_POWER_OFF_RSN)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_POWER_OFF_RSN_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_AUTO_POWER_ON)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_EC_AUTO_POWER_ON_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_ACPI_INT_EVENT)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN OUT EC_RESPONSE_ACPI_INT_EVENT_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_PD_FW_UPDATE_INFO)(
  IN     EC_PLATFORM_PROTOCOL         *This,
  IN     EC_PARAMS_PD_FW_UPDATE_INFO  *Info
  );

typedef
  EFI_STATUS
(EFIAPI *GET_PD_FW_UPDATE_STATE)(
  IN    EC_PLATFORM_PROTOCOL                *This,
  IN    EC_PARAMS_PD_FW_UPDATE_STATE_INFO   *Param,
  OUT   EC_RESPONSE_PD_FW_UPDATE_STATE_INFO *Info
  );

typedef
  EFI_STATUS
(EFIAPI *TRANS_PD_FW_BIN)(
  IN    EC_PLATFORM_PROTOCOL                *This,
  IN    EC_PARAMS_PD_FW_BIN_TRANS_INFO      *Param
  );

typedef
  EFI_STATUS
(EFIAPI *FORCE_EC_MIRROR)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN     EC_PARAMS_FORCE_MIRROR     *Info
  );

typedef
  EFI_STATUS
(EFIAPI *SET_ALS_MODE)(
  IN     EC_PLATFORM_PROTOCOL       *This,
  IN     EC_PARAMS_ALS_MODE_CTL     *Info
  );

struct _EC_PLATFORM_PROTOCOL {
  EFI_HANDLE                   Handle;
  UINT32                       Version;
  GET_EC_VERSION               GetEcVersion;
  GET_BOARD_ID                 GetBoardId;
  GET_BATTTERY_STATIC_INFO     GetBatteryStaticInfo;
  GET_BATTTERY_DYNAMIC_INFO    GetBatteryDynamicInfo;
  GET_FARM_ID                  GetFarmId;
  SET_GPIO                     SetGpio;
  GET_GPIO                     GetGpio;
  GET_PMIC_VERSION             GetPmicVersion;
  GET_PD_VERSION               GetPdVersion;
  GET_GREENPAK_VERSION         GetGreenPakVersion;
  GET_CHARGER_INFO             GetChagerInfo;
  SET_FAN_RPM                  SetFanRpm;
  GET_FAN_RPM                  GetFanRpm;
  SET_FAN_DUTY                 SetPwmDuty;
  GET_FAN_DUTY                 GetPwmDuty;
  SET_AUTO_FAN_CTL             SetThermalFanAutoCtl;
  GET_PVT_TEMP                 GetPvtTemp;
  GET_POWER_OFF_RSN            GetPoweroffRsn;
  SET_AUTO_POWER_ON            SetECAutoPowerOn;
  GET_ACPI_INT_EVENT           GetAcpiIntEvent;
  SET_PD_FW_UPDATE_INFO        SetPdFwUpdateInfo;
  GET_PD_FW_UPDATE_STATE       GetPdFwUpdateState;
  TRANS_PD_FW_BIN              TransPdFwBin;
  FORCE_EC_MIRROR              ForceECMirror;
  SET_ALS_MODE                 SetAlsMode;
};

extern EFI_GUID  gCixEcPlatformProtocolGuid;
#endif
