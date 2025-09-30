/**

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PinMuxLib.h>
#include <Library/PinMuxTableLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioTableLib.h>
#include <Library/PlatformEnvHookLib.h>
#include <Protocol/I2cDevicePath.h>
#include <Library/CixSipLib.h>
#include <Library/MailBoxLib.h>
#include <Guid/NetworkStackSetup.h>
#include <PlatformSetupVar.h>

// {3F7B73C7-FB70-4e91-86E7-34EAD76AC74D}
EFI_GUID  gEfiFarmEnableFlagGuid = {
  0x3f7b73c7, 0xfb70, 0x4e91, { 0x86, 0xe7, 0x34, 0xea, 0xd7, 0x6a, 0xc7, 0x4d }
};

GPIO_CFG  GpioCfgTable[] = {
  { 1,   OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO001_MKEY_EN
  { 2,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SLT_X8_RST_L
  { 3,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // PERSTB2
  { 4,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // M2_NVME_RST_L
  { 5,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // M2_WLAN_RST_L
  { 6,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // LOM_RST_L
  { 7,   OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // PD_RESET
  { 9,   INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // PD_INT_L
  { 10,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // LOM_PWREN
  { 11,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // RTC_ALARM_L
  { 13,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // WLAN_PWREN
  { 14,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // UFS_5V_EN
  { 15,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // PWR_LED_EN
  { 16,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_DISABLE,        EDGE_LOW               }, // PWRBTN_L
  { 19,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // WL_RADIO_DISABLE_L
  { 20,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // BT_RADIO_DISABLE_L
  { 22,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // PHY2_PWR_EN
  { 23,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // TPM_PWR_EN
  { 30,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // GBE1_PEWAKE_L
  { 31,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // MKEY_WAKE
  { 32,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // WLAN_WAKE_L
  { 33,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // GBE2_PEWAKE_L
  { 40,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // USB_DRIVE_VBUS0
  { 43,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // BOOT_STRAP
  // { 44,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // GPIO44 (DEFAULT LOW)
  { 44,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // GPIO44 (DEFAULT LOW)
  { 45,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO45 (DEFAULT LOW)
  { 46,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO46 (DEFAULT LOW)
  { 49,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_DISABLE       , INTERRUPT_TYPE_DEFAULT }, // BOARD_ID1
  { 50,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_DISABLE       , INTERRUPT_TYPE_DEFAULT }, // BOARD_ID2
  { 71,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO71 (DEFAULT LOW)
  { 73,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // BKEY_PWR_EN
  { 74,  OUTPUT,            INOUT_HIGH,          INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // BKEY_RESET
  { 76,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO76 (DEFAULT LOW)
  { 78,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO78 (DEFAULT LOW)
  { 79,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO79 (DEFAULT LOW)
  { 80,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO80 (DEFAULT LOW)
  { 81,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM_PWREN
  { 82,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM2_PWDN
  { 84,  INPUT,             INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, LEVEL_LOW              }, // TPM_INT_L
  { 85,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM2_RST_L
  { 86,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM1_PWDN
  { 87,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM3_PWDN
  { 88,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM0_RST_L
  { 89,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // CAM0_PWDN
  { 90,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // I2S4_MCLK (DEFAULT LOW)
  { 91,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // I2S4_SCK (DEFAULT LOW)
  { 92,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // I2S4_TWS (DEFAULT LOW)
  { 93,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // I2S4_DATA_IN (DEFAULT LOW)
  { 94,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // I2S4_DATA_OUT (DEFAULT LOW)
  { 95,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO95_PWM0 (DEFAULT LOW)
  { 96,  OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // GPIO96_PWM1 (DEFAULT LOW)
  { 138, OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SPI2_MISO (DEFAULT LOW)
  { 139, OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SPI2_CS0_L (DEFAULT LOW)
  { 140, OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SPI2_CS1_L (DEFAULT LOW)
  { 141, OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SPI2_MOSI (DEFAULT LOW)
  { 142, OUTPUT,            INOUT_LOW,           INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }, // SPI2_CLK (DEFAULT LOW)

  // the end of GpioCfgTable, no more item allowed to add afterward
  { 0,   DIRECTION_DEFAULT, INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE_DEFAULT, INTERRUPT_TYPE_DEFAULT }
};

PINMUX_CFG  PinMuxCfgTable[] = {
  { S0_DOMAIN,       IO_S0_I2C2_SCL,         IO_FUNC02,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SOC_I2C2_SCL
  { S0_DOMAIN,       IO_S0_I2C2_SDA,         IO_FUNC02,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SOC_I2C2_SDA
  { S0_DOMAIN,       IO_S0_I2C4_CLK,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SOC_I2C4_SCL
  { S0_DOMAIN,       IO_S0_I2C4_SDA,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SOC_I2C4_SDA
  { S0_DOMAIN,       IO_S0_I2S2_RSCK,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO71
  { S0_DOMAIN,       IO_S0_I2S2_TSCK,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // BKEY_PWR_EN
  { S0_DOMAIN,       IO_S0_I2S2_TWS,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // BKEY_RESET
  { S0_DOMAIN,       IO_S0_I2S2_DATA_IN1,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO76
  { S0_DOMAIN,       IO_S0_I2S2_DATA_OUT1,   IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO78
  { S0_DOMAIN,       IO_S0_I2S2_DATA_OUT2,   IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO79
  { S0_DOMAIN,       IO_S0_I2S2_DATA_OUT3,   IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO80
  { S0_DOMAIN,       IO_S0_I2S3_MCLK,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM_PWREN
  { S0_DOMAIN,       IO_S0_I2S3_RSCK,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM2_PWDN
  { S0_DOMAIN,       IO_S0_I2S3_TSCK,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // TPM_INT_L
  { S0_DOMAIN,       IO_S0_I2S3_TWS,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM2_RST_L
  { S0_DOMAIN,       IO_S0_I2S3_DATA_IN0,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM1_PWDN
  { S0_DOMAIN,       IO_S0_I2S3_DATA_IN1,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM3_PWDN
  { S0_DOMAIN,       IO_S0_I2S3_DATA_OUT0,   IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM0_RST_L
  { S0_DOMAIN,       IO_S0_I2S3_DATA_OUT1,   IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // CAM0_PWDN
  { S0_DOMAIN,       IO_S0_UART0_TXD,        IO_FUNC02,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO95_PWM0
  { S0_DOMAIN,       IO_S0_UART0_RXD,        IO_FUNC02,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO96_PWM1
  { S0_DOMAIN,       IO_S0_UART1_TXD,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // FAN_OUT
  { S0_DOMAIN,       IO_S0_UART1_RXD,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // FAN_TACH
  { S0_DOMAIN,       IO_S0_PM_GPIO0,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // PMIC0_I2C_INT_L
  { S0_DOMAIN,       IO_S0_PM_GPIO1,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // PMIC1_I2C_INT_L
  { S0_DOMAIN,       IO_S0_PM_GPIO2,         IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // GPIO153/PM_GPIO2

  { S5_DOMAIN,       IO_S5_SFI_GPIO0,        IO_FUNC02,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // PWR_LED_EN
  { S5_DOMAIN,       IO_S5_SFI_GPIO1,        IO_FUNC00,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // PWRBTN_L
  { S5_DOMAIN,       IO_S5_SFI_GPIO4,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // WL_RADIO_DISABLE_L
  { S5_DOMAIN,       IO_S5_SFI_GPIO5,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // BT_RADIO_DISABLE_L
  { S5_DOMAIN,       IO_S5_SFI_GPIO7,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // PHY2_PWR_EN
  { S5_DOMAIN,       IO_S5_SFI_GPIO8,        IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // TPM_PWR_EN
  { S5_DOMAIN,       IO_S5_USB_DRIVE_VBUS0,  IO_FUNC00,   PU_DEFAULT, PD_DISABLE, ST_DEFAULT, DRV_STREN_DEFAULT }, // USB_DRIVE_VBUS0
  { S5_DOMAIN,       IO_S5_USB_DRIVE_VBUS4,  IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // USB_DRIVE_VBUS4
  { S5_DOMAIN,       IO_S5_USB_DRIVE_VBUS5,  IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // USB_DRIVE_VBUS5
  { S5_DOMAIN,       IO_S5_SE_QSPI_CLK,      IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_CLK
  { S5_DOMAIN,       IO_S5_SE_QSPI_CS_L,     IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_CS_L
  { S5_DOMAIN,       IO_S5_SE_QSPI_DATA0,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_D0
  { S5_DOMAIN,       IO_S5_SE_QSPI_DATA1,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_D1
  { S5_DOMAIN,       IO_S5_SE_QSPI_DATA2,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_D2
  { S5_DOMAIN,       IO_S5_SE_QSPI_DATA3,    IO_FUNC01,   PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }, // SE_QSPI_D3

  // the end of PinMuxCfgTable, no more item allowed to add afterward
  { UNDEFINE_DOMAIN, IO_S0_S5_UNDEFINED,     IO_UNDEFINE, PU_DEFAULT, PD_DEFAULT, ST_DEFAULT, DRV_STREN_DEFAULT }
};

EFI_STATUS
EFIAPI
PlatformEnvHookLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  return Status;
}

EFI_STATUS
EFIAPI
InitGpio (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  GpioInit (GpioCfgTable, ARRAY_SIZE (GpioCfgTable));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitPinmux (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  PinMuxInit (PinMuxCfgTable, ARRAY_SIZE (PinMuxCfgTable));

  return EFI_SUCCESS;
}

VOID
EFIAPI
InstallRtcProtocol (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_HANDLE                *Handle;
  UINTN                     HandleNum;
  UINTN                     Index;

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
          if (CompareGuid (&(((VENDOR_DEVICE_PATH *)DevicePath)->Guid), &gCixI2cDevicePathGuid) &&
              (((I2C_DEVICE_PATH *)DevicePath)->Bus == FixedPcdGet8 (PcdRtc8563I2cBus)))
          {
            //
            // Install the ra8900ce I2C Master protocol on this handle so the RTC driver
            // can identify it as the I2C master it can invoke directly, rather than
            // through the I2C driver stack (which cannot be used at runtime)
            //
            Status = gBS->InstallProtocolInterface (
                            &Handle[Index],
                            &gHym8563RealTimeClockLibI2cMasterProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            NULL
                            );
            ASSERT_EFI_ERROR (Status);
            if (Event != NULL) {
              gBS->CloseEvent (Event);
            }
          }
        }
      }
    }

    gBS->FreePool (Handle);
  }

  return;
}

EFI_STATUS
EFIAPI
UpdatePcdDmaDeviceLimit (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS  Status;
  UINT64      SystemMemSize  = 0;
  UINT64      DmaDeviceLimit = 0;

 #ifdef UPDATE_MEMORY_TOP
  UINT32  *pMemSizeAddr = (UINT32 *)(FixedPcdGet64 (PcdMemInfoBaseAddr) + 0x10);
  SystemMemSize   = *pMemSizeAddr;
  SystemMemSize <<= 20;
 #else
  SystemMemSize = FixedPcdGet64 (PcdSystemMemorySize);
 #endif
  DEBUG ((DEBUG_INFO, "[%a] SystemMemSize :%lx\n", __FUNCTION__, SystemMemSize));
  DmaDeviceLimit += (SystemMemSize + 0x80000000);
  DmaDeviceLimit -= 1;
  Status          = PcdSet64S (PcdDmaDeviceLimit, DmaDeviceLimit);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PcdDmaDeviceLimit set fail status:%r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "PcdDmaDeviceLimit set success\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
WakeupSourceInit (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a\n",__func__));

  /*usb3 typec ports*/
  WakeupCfg(USB_C_SSP_0_HOST_IRQ,FALSE);
  WakeupCfg(USB_C_SSP_1_HOST_IRQ,FALSE);
  WakeupCfg(USB_C_SSP_2_HOST_IRQ,FALSE);
  WakeupCfg(USB_C_SSP_3_HOST_IRQ,FALSE);

  /*usb3 typea ports*/
  WakeupCfg(USB_SSP_0_HOST_IRQ,FALSE);
  WakeupCfg(USB_SSP_1_HOST_IRQ,FALSE);

  /*usb2 ports*/
  WakeupCfg(USB2_0_HOST_IRQ,FALSE);
  WakeupCfg(USB2_1_HOST_IRQ,FALSE);
  WakeupCfg(USB2_2_HOST_IRQ,FALSE);
  WakeupCfg(USB2_3_HOST_IRQ,FALSE);
  return Status;
}

EFI_STATUS
EFIAPI
OnboardDevicePowerOff (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "[%a] EntryPoint\n", __FUNCTION__));

  if (ConfigData == NULL) {
    DebugPrint (DEBUG_ERROR, "ConfigData point is NULL");
    return EFI_OUT_OF_RESOURCES;
  }

  if (ConfigData->PlatConfig->GfxPower == 0) {
    // GPIO001
    DEBUG ((DEBUG_INFO, "GPIO001 Output Low GFX Power Off\n"));
    GpioConfig (1, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  } else {
    // GPIO001
    DEBUG ((DEBUG_INFO, "GPIO001 Output Low GFX Power On\n"));
    GpioConfig (1, OUTPUT, INOUT_HIGH, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  }

  if (ConfigData->PlatConfig->OnBoardLanPower == 0) {
    // GPIO010
    DEBUG ((DEBUG_INFO, "GPIO010 Output Low OnBoardLan0Power Power Off\n"));
    GpioConfig (10, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
    // GPIO022
    DEBUG ((DEBUG_INFO, "GPIO022 Output Low OnBoardLan1Power Power Off\n"));
    GpioConfig (22, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  } else {
    // GPIO010
    DEBUG ((DEBUG_INFO, "GPIO010 Output High OnBoardLan0Power Power On\n"));
    GpioConfig (10, OUTPUT, INOUT_HIGH, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
    // GPIO022
    DEBUG ((DEBUG_INFO, "GPIO022 Output High OnBoardLan1Power Power On\n"));
    GpioConfig (22, OUTPUT, INOUT_HIGH, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  }

  if (ConfigData->PlatConfig->WlanPower == 0) {
    // GPIO13
    DEBUG ((DEBUG_INFO, "GPIO13 Output Low WlanPower Power Off\n"));
    GpioConfig (13, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  } else {
    // GPIO13
    DEBUG ((DEBUG_INFO, "GPIO13 Output High WlanPower Power On\n"));
    GpioConfig (13, OUTPUT, INOUT_HIGH, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  }

  if (ConfigData->PlatConfig->M2SsdPower == 0) {
    // GPIOXX1
    DEBUG ((DEBUG_INFO, "GPIOXX1 Output Low M2SsdPower Power Off\n"));
  } else {
    // GPIOXX1
    DEBUG ((DEBUG_INFO, "GPIOXX1 Output High M2SsdPower Power On\n"));
  }

  if (ConfigData->PlatConfig->IspCamera0Power == 0) {
    // GPIO081
    DEBUG ((DEBUG_INFO, "GPIO081 Output Low IspCamera0Power Power Off\n"));
    GpioConfig (81, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  } else {
    // GPIO081
    DEBUG ((DEBUG_INFO, "GPIO081 Output High IspCamera0Power Power On\n"));
    GpioConfig (81, OUTPUT, INOUT_HIGH, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  }

  if (ConfigData->PlatConfig->IspCamera1Power == 0) {
    // GPIOXX2
    DEBUG ((DEBUG_INFO, "GPIOXX2 Output Low IspCamera1Power Power Off\n"));
  } else {
    // GPIOXX2
    DEBUG ((DEBUG_INFO, "GPIOXX2 Output High IspCamera1Power Power On\n"));
  }

  if (ConfigData->PlatConfig->IspCamera2Power == 0) {
    // GPIOXX3
    DEBUG ((DEBUG_INFO, "GPIOXX3 Output Low IspCamera2Power Power Off\n"));
  } else {
    // GPIOXX3
    DEBUG ((DEBUG_INFO, "GPIOXX3 Output High IspCamera2Power Power On\n"));
  }

  if (ConfigData->PlatConfig->IspCamera3Power == 0) {
    // GPIOXX4
    DEBUG ((DEBUG_INFO, "GPIOXX4 Output Low IspCamera3Power Power Off\n"));
  } else {
    // GPIOXX4
    DEBUG ((DEBUG_INFO, "GPIOXX4 Output High IspCamera3Power Power On\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
RtcWakupEnable (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  if (ConfigData->PlatConfig->RtcWakeup) {
    GpioConfig (11, INPUT, INOUT_VALUE_DEFAULT, INTERRUPT_ENABLE, LEVEL_LOW);
  } else {
    GpioConfig (11, INPUT, INOUT_VALUE_DEFAULT, INTERRUPT_DISABLE, LEVEL_LOW);
  }

  return EFI_SUCCESS;
}

#ifdef STMM_SUPPORT
EFI_STATUS
EFIAPI
FenceFchXspiHost (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS                     Status;
  MBOX_GASKET_FENCING_PARAMETER  Params;
  MBOX_GASKET_FENCING_RESPONSE   Respon;

  Params.Id = FCH_XSPI_RANGE_ID;
  Status    = MboxEnableGasketFencing (&Params, &Respon);
  DEBUG ((DEBUG_INFO, "[%a] enable FCH XSPI gasket fencing status %r, response error code %d\n", __FUNCTION__, Status, Respon.ErrCode));

  return Status;
}
#endif

STATIC PLATFORM_ENV_INIT_TABLE  mPlatformEnvInitTable[] = {
  { NULL,                        NULL,                 InitGpio                },
  { NULL,                        NULL,                 InitPinmux              },
  { NULL,                        NULL,                 UpdatePcdDmaDeviceLimit },
  { NULL,                        NULL,                 WakeupSourceInit        },
  { NULL,                        NULL,                 OnboardDevicePowerOff   },
  { NULL,                        NULL,                 RtcWakupEnable          },
  { &gEfiI2cMasterProtocolGuid,  InstallRtcProtocol,   NULL                    },
#ifdef STMM_SUPPORT
  { NULL,                        NULL,                 FenceFchXspiHost        },
 #endif
  // add platform initialization routines on ENV phase BEFORE this line, and they were invoked from top to down.
  { NULL,                        NULL,                 NULL                    }
};

STATIC UINT32  mPlatformEnvInitTableSize = ARRAY_SIZE (mPlatformEnvInitTable);

VOID
EFIAPI
PlatformEnvHook (
  IN OUT ENV_HOOK_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      Index  = 0;
  VOID        *Registration;

  if (ConfigData == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: platform ENV hook routine failed to get config data\n", __FUNCTION__));
    return;
  }

  while (Index < mPlatformEnvInitTableSize) {
    if (mPlatformEnvInitTable[Index].Guid == NULL) {
      if (mPlatformEnvInitTable[Index].Init != NULL) {
        Status = mPlatformEnvInitTable[Index].Init (ConfigData);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a: platform ENV hook routine failed at index %d, status %r\n", __FUNCTION__, Index, Status));
        }
      }
    } else {
      EfiCreateProtocolNotifyEvent (
        mPlatformEnvInitTable[Index].Guid,
        TPL_CALLBACK,
        mPlatformEnvInitTable[Index].Notify,
        ConfigData,
        &Registration
        );
    }

    Index++;
  }
}
