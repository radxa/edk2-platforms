#ifndef __DEFAULT_LINUX_ACPI_CONFIG_H__
#define __DEFAULT_LINUX_ACPI_CONFIG_H__

/*
 * USB Typec port configs
 *
 * %USBCx_PD_EN set 1 if USB work with a PD controller.
 * Typec usb device or typc display device will be recognized by PD
 * controller, as well as forward or backward insertion.
 * if USBCx_PD_EN set, USBCx_PD_DEVICE must be sepcified, and the
 * usb phy mode config USBCx_DEF_PMODE will be ignored.
 *
 * %USBCx_PD_DEVICE USB PD controller device.
 *
 * %USBCx_DEF_PMODE is the USBC phy default mode.
 *      USBC_PHY_MODE_USB   : phy mode for usb only
 *      USBC_PHY_MODE_DP    : phy mode for dp only
 *      USBC_PHY_MODE_COMBO : phy mode for usb and dp
 * ignored if USBCx_PD_EN set.
 *
 * %USBCx_DISABLE_USB3 force usb in high speed mode(USB2.0).
 *
 * %USBxx_OC_EN USB over current mode enable.
 * the "oc" pin must be set as "oc" function in pin settings.
 */
#define USBC_PHY_MODE_USB    0x1
#define USBC_PHY_MODE_DP     0x2
#define USBC_PHY_MODE_COMBO  0x3// USB and DP

#define USBC0_PD_EN         1
#define USBC0_PD_DEVICE     \_SB.I2C7.PD00
#define USBC0_DEF_PMODE     USBC_PHY_MODE_COMBO
#define USBC0_DISABLE_USB3  0

#define USBC1_PD_EN         1
#define USBC1_PD_DEVICE     \_SB.I2C7.PD01
#define USBC1_DEF_PMODE     USBC_PHY_MODE_COMBO
#define USBC1_DISABLE_USB3  0

#define USBC2_PD_EN         1
#define USBC2_PD_DEVICE     \_SB.I2C1.PD10
#define USBC2_DEF_PMODE     USBC_PHY_MODE_COMBO
#define USBC2_DISABLE_USB3  0

#define USBC3_PD_EN         1
#define USBC3_PD_DEVICE     \_SB.I2C1.PD11
#define USBC3_DEF_PMODE     USBC_PHY_MODE_COMBO
#define USBC3_DISABLE_USB3  0

#define USBC0_OC_EN  1
#define USBC1_OC_EN  1
#define USBC2_OC_EN  1
#define USBC3_OC_EN  1
#define USBA4_OC_EN  1
#define USBA5_OC_EN  1
#define USBH6_OC_EN  1
#define USBH7_OC_EN  1
#define USBH8_OC_EN  1
#define USBH9_OC_EN  1

// PCIe
#define PCIE_X2_PERST            0
#define PCIE_X2_PERST_GPIO_CTR   "\\_SB.GPI4"
#define PCIE_X2_PERST_GPIO       4
#define PERST_GPIO_ACTIVE_LEVEL  GPIO_ACTIVE_HIGH

#define PCIE_X2_EP_VBAT           0
#define PCIE_X2_EP_VBAT_GPIO_CTR  "\\_SB.GPI4"
#define PCIE_X2_EP_VBAT_GPIO      12
#define VBAT_GPIO_ACTIVE_LEVEL    GPIO_ACTIVE_HIGH

#endif
