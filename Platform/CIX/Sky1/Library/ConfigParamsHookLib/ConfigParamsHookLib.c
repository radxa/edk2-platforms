/**

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/ConfigParamsHookLib.h>

EFI_STATUS
EFIAPI
ConfigParamsHookLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  return Status;
}

STATIC
EFI_STATUS
usb_phy_config_init (
  IN OUT CONFIG_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  UINT32  phy_index = 0;

  for (phy_index = 0; phy_index < MAX_USB_PHY_CONFIG_PARAMS_NUM; phy_index++) {
    ConfigData->USB20PHY.usb20_phy[phy_index].UsbPhyType = 0;
    ConfigData->USB20PHY.usb20_phy[phy_index].Address    = 0;
    ConfigData->USB20PHY.usb20_phy[phy_index].Value      = 0;
  }

  for (phy_index = 0; phy_index < MAX_USB_PHY_CONFIG_PARAMS_NUM; phy_index++) {
    ConfigData->USB32PHY.usb32_phy[phy_index].UsbPhyType = 0;
    ConfigData->USB32PHY.usb32_phy[phy_index].Address    = 0;
    ConfigData->USB32PHY.usb32_phy[phy_index].Value      = 0;
  }

  for (phy_index = 0; phy_index < MAX_USB_PHY_CONFIG_PARAMS_NUM; phy_index++) {
    ConfigData->USBCOMPHY.usbcom_phy[phy_index].UsbPhyType = 0;
    ConfigData->USBCOMPHY.usbcom_phy[phy_index].Address    = 0;
    ConfigData->USBCOMPHY.usbcom_phy[phy_index].Value      = 0;
  }

  for (phy_index = 0; phy_index < MAX_USB2_PHY_NUM; phy_index++) {
    ConfigData->USB20PHY.usb20_phy_reset[phy_index] = 0;
  }

  for (phy_index = 0; phy_index < MAX_USB32_PHY_NUM; phy_index++) {
    ConfigData->USB32PHY.usb32_phy_reset[phy_index] = 0;
  }

  for (phy_index = 0; phy_index < MAX_USBCOM_PHY_NUM; phy_index++) {
    ConfigData->USBCOMPHY.usbcom_phy_reset[phy_index] = 0;
  }

  return EFI_SUCCESS;
}

STATIC CONFIG_PARAMS_INIT_FUNC  mConfigParamsInitTable[] = {
  // add initializatrion routines for configuration parameters BEFORE this line, and they were invoked from top to down.
  usb_phy_config_init,
  NULL,
};

STATIC UINT32  mConfigParamsInitTableSize = ARRAY_SIZE (mConfigParamsInitTable);

VOID
EFIAPI
ConfigParamsHook (
  IN OUT CONFIG_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      Index  = 0;

  if (ConfigData == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: config params hook routine failed to get config data\n", __FUNCTION__));
    return;
  }

  while (Index < mConfigParamsInitTableSize) {
    if (mConfigParamsInitTable[Index] != NULL) {
      Status = mConfigParamsInitTable[Index](ConfigData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: config params hook routine failed at index %d, status %r\n", __FUNCTION__, Index, Status));
      }
    }

    Index++;
  }
}
