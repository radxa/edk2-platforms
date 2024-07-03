/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//ISP V4L2 config
Device (V4L2) {
  Name (_HID, "CIXH3020")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { ISP_EXT_U_ISP_CORE_O_ISP_INT_INTERRUPT_ID }
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { ISP_EXT_U_ISP_CORE_O_ISP_INT_ABNORMAL_INTERRUPT_ID }
  })
}

//ISP i7_isp
Device (ISP0) {
  Name (_HID, "CIXH3021")
  Name (_UID, 0x00)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, ISP_CONTROLLER0_BASE, ISP_CONTROLLER0_SIZE)
      Memory32Fixed (ReadWrite, ISP_CONTROLLER1_BASE, ISP_CONTROLLER1_SIZE)
  })
}

//ISP i7_isp_fake1
Device (ISP1) {
  Name (_HID, "CIXH3022")
  Name (_UID, 0x00)
  Name (_STA, 0x00)
}
//ISP i7_isp_fake2
Device (ISP2) {
  Name (_HID, "CIXH3022")
  Name (_UID, 0x01)
  Name (_STA, 0x00)
}
//ISP i7_isp_fake3
Device (ISP3) {
  Name (_HID, "CIXH3022")
  Name (_UID, 0x02)
  Name (_STA, 0x00)
}

//ISP ispmem
Device (ISPM) {
  Name (_HID, "CIXH3025")
  Name (_UID, 0x00)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ahb-pmctrl-res-base", ISPMEM_AHB_PMCTRL_RES_BASE},
      Package () { "ahb-pmctrl-res-size", ISPMEM_AHB_PMCTRL_RES_SIZE},
      Package () { "ahb-rcsuisp0-res-base", ISPMEM_AHB_RCSUISP0_RES_BASE},
      Package () { "ahb-rcsuisp0-res-size", ISPMEM_AHB_RCSUISP0_RES_SIZE},
      Package () { "ahb-rcsuisp1-res-base", ISPMEM_AHB_RCSUISP1_RES_BASE},
      Package () { "ahb-rcsuisp1-res-size", ISPMEM_AHB_RCSUISP1_RES_SIZE},
    }
  })
}

//ISP vi hw
Device (VIHW) {
  Name (_HID, "CIXH3026")
  Name (_UID, 0x00)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ahb-dphy0-base", 0x142A0000},
      Package () { "ahb-dphy0-size", 0x10000},
      Package () { "ahb-dphy1-base", 0x14300000},
      Package () { "ahb-dphy1-size", 0x10000},
      Package () { "ahb-csi0-base", 0x14280000},
      Package () { "ahb-csi0-size", 0x10000},
      Package () { "ahb-csi1-base", 0x14290000},
      Package () { "ahb-csi1-size", 0x10000},
      Package () { "ahb-csi2-base", 0x142E0000},
      Package () { "ahb-csi2-size", 0x10000},
      Package () { "ahb-csi3-base", 0x142F0000},
      Package () { "ahb-csi3-size", 0x10000},
      Package () { "ahb-csidma0-base", 0x142B0000},
      Package () { "ahb-csidma0-size", 0x10000},
      Package () { "ahb-csidma1-base", 0x142C0000},
      Package () { "ahb-csidma1-size", 0x10000},
      Package () { "ahb-csidma2-base", 0x14310000},
      Package () { "ahb-csidma2-size", 0x10000},
      Package () { "ahb-csidma3-base", 0x14320000},
      Package () { "ahb-csidma3-size", 0x10000},
      Package () { "ahb-csircsu0-base", 0x14270000},
      Package () { "ahb-csircsu0-size", 0x10000},
      Package () { "ahb-csircsu1-base", 0x142D0000},
      Package () { "ahb-csircsu1-size", 0x10000},
    }
  })
}