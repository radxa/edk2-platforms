/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define USB_REMOTE_PD_DSD(Match) \
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { Match, 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () { \
                Package () { "port@0", "PRT0" },\
            }

#define USB_REMOTE_PD(DevRef,UsbcCon,PortRole,EptRole) \
  Name (PRT0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP00" },\
            }\
  })\
  Name (EP00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package() { DevRef, UsbcCon, PortRole, EptRole } },\
            }\
  })

#define USB_PHY_REMOTE_PD_DSD(MatchOri, MatchMux) \
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { MatchOri, 0 }, \
                Package () { MatchMux, 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () { \
                Package () { "port@0", "PRT0" },\
            }

#define USB_PHY_REMOTE_PD(DevRef,UsbcCon,PortOrienSw,EptOrienSw,PortAltmodeMux,EptAltmodeMux) \
  Name (PRT0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP00" },\
                Package () { "endpoint@1", "EP01" },\
            }\
  })\
  Name (EP00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package() { DevRef, UsbcCon, PortOrienSw, EptOrienSw } },\
            }\
  }) \
  Name (EP01, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 1 }, \
                Package () { "remote-endpoint", Package() { DevRef, PortAltmodeMux, EptAltmodeMux } },\
            }\
  })

Device (SUB0)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x00)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09000310, 0x4)
    Memory32Fixed (ReadWrite, 0x09000400, 0x4)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x0 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB0)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x00)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x09010000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09014000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09018000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_U3_TYPEC_DRD_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_U3_TYPEC_DRD_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_U3_TYPEC_DRD_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEC_U3_TYPEC_DRD_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "otg" },
            Package () { "cdnsp,usb3-phy", \_SB.UCP0.USBP },
          },
      USB_REMOTE_PD_DSD("usb-role-switch")
    })
    USB_REMOTE_PD(\_SB.I2C7.PD00, "usbc_con0", "port@0", "endpoint@0")
  }
}

Device (U2P4) //USB 2.0 PHY4
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x04)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (UCP0) //USB 3.0 PHY0
{
  Name (_HID, "CIXH2033")     // _HID: Hardware ID
  Name (_UID, 0x00)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09030000, 0x40000)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x0 },
          Package () { "cix,usbphy_syscon", \_SB.CRU0 },
          Package () { "svid", 0xff01 },
          Package () { "default_conf", 0x3 },
        },
    USB_PHY_REMOTE_PD_DSD("orientation-switch", "mode-switch"),
  })
  USB_PHY_REMOTE_PD(\_SB.I2C7.PD00, "usbc_con0", "port@1", "endpoint@0", "port@2", "endpoint@0")

  Device(USBP)
  {
    Name (_ADR, 0x00)
  }
  Device(UDPP)
  {
    Name (_ADR, 0x01)
  }
}

Device (SUB1)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x01)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09070310, 0x4)
    Memory32Fixed (ReadWrite, 0x09070400, 0x4)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x1 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB1)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x01)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x09080000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09084000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09088000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST0_U3_TYPEC_HOST0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST0_U3_TYPEC_HOST0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST0_U3_TYPEC_HOST0_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEC_HOST0_U3_TYPEC_HOST0_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "otg" },
            Package () { "cdnsp,usb3-phy", \_SB.UCP1.USBP },
          },
      USB_REMOTE_PD_DSD("usb-role-switch")
    })
    USB_REMOTE_PD(\_SB.I2C7.PD01, "usbc_con0", "port@0", "endpoint@0")
  }
}

Device (U2P5) //USB 2.0 PHY5
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x05)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (UCP1) //USB 3.0 PHY0
{
  Name (_HID, "CIXH2033")     // _HID: Hardware ID
  Name (_UID, 0x01)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x090A0000, 0x40000)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x1 },
          Package () { "cix,usbphy_syscon", \_SB.CRU0 },
          Package () { "svid", 0xff01 },
          Package () { "default_conf", 0x3 },
        },
    USB_PHY_REMOTE_PD_DSD("orientation-switch", "mode-switch"),
  })
  USB_PHY_REMOTE_PD(\_SB.I2C7.PD01, "usbc_con0", "port@1", "endpoint@0", "port@2", "endpoint@0")

  Device(USBP)
  {
    Name (_ADR, 0x00)
  }
  Device(UDPP)
  {
    Name (_ADR, 0x01)
  }
}

Device (SUB2)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x02)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x090e0310, 0x4)
    Memory32Fixed (ReadWrite, 0x090e0400, 0x4)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x2 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB2)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x02)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x090f0000, 0x4000)
      Memory32Fixed (ReadWrite, 0x090f4000, 0x4000)
      Memory32Fixed (ReadWrite, 0x090f8000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST1_U3_TYPEC_HOST1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST1_U3_TYPEC_HOST1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST1_U3_TYPEC_HOST1_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEC_HOST1_U3_TYPEC_HOST1_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "otg" },
            Package () { "cdnsp,usb3-phy", \_SB.UCP2.USBP },
          },
      USB_REMOTE_PD_DSD("usb-role-switch")
    })
    USB_REMOTE_PD(\_SB.I2C1.PD10, "usbc_con0", "port@0", "endpoint@0")
  }
}

Device (U2P8) //USB 2.0 PHY8
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x08)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (UCP2) //USB 3.0 PHY0
{
  Name (_HID, "CIXH2033")     // _HID: Hardware ID
  Name (_UID, 0x02)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09110000, 0x40000)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x2 },
          Package () { "cix,usbphy_syscon", \_SB.CRU0 },
          Package () { "svid", 0xff01 },
          Package () { "default_conf", 0x3 },
        },
    USB_PHY_REMOTE_PD_DSD("orientation-switch", "mode-switch"),
  })
  USB_PHY_REMOTE_PD(\_SB.I2C1.PD10, "usbc_con0", "port@1", "endpoint@0", "port@2", "endpoint@0")

  Device(USBP)
  {
    Name (_ADR, 0x00)
  }
  Device(UDPP)
  {
    Name (_ADR, 0x01)
  }
}

Device (SUB3)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x03)           // _UID: Unique ID
  Name (_CCA, 0x0F)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09150310, 0x4)
    Memory32Fixed (ReadWrite, 0x09150400, 0x4)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x3 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB3)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x03)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x09160000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09164000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09168000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST2_U3_TYPEC_HOST2_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST2_U3_TYPEC_HOST2_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEC_HOST2_U3_TYPEC_HOST2_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEC_HOST2_U3_TYPEC_HOST2_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "otg" },
            Package () { "cdnsp,usb3-phy", \_SB.UCP3.USBP },
          },
      USB_REMOTE_PD_DSD("usb-role-switch")
    })
    USB_REMOTE_PD(\_SB.I2C1.PD11, "usbc_con0", "port@0", "endpoint@0")
  }
}

Device (U2P9) //USB 2.0 PHY9
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x09)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (UCP3) //USB 3.0 PHY0
{
  Name (_HID, "CIXH2033")     // _HID: Hardware ID
  Name (_UID, 0x03)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09180000, 0x40000)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x3 },
          Package () { "cix,usbphy_syscon", \_SB.CRU0 },
          Package () { "svid", 0xff01 },
          Package () { "default_conf", 0x3 },
        },
    USB_PHY_REMOTE_PD_DSD("orientation-switch", "mode-switch"),
  })
  USB_PHY_REMOTE_PD(\_SB.I2C1.PD11, "usbc_con0", "port@1", "endpoint@0", "port@2", "endpoint@0")

  Device(USBP)
  {
    Name (_ADR, 0x00)
  }
  Device(UDPP)
  {
    Name (_ADR, 0x01)
  }
}

Device (SUB4)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x04)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x091c0314, 0x4)
    Memory32Fixed (ReadWrite, 0x091c0400, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "pinctrl_usb4", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x4 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB4)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x04)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x091d0000, 0x4000)
      Memory32Fixed (ReadWrite, 0x091d4000, 0x4000)
      Memory32Fixed (ReadWrite, 0x091d8000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL0_U3_TYPEA_CTL0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL0_U3_TYPEA_CTL0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL0_U3_TYPEA_CTL0_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEA_CTL0_U3_TYPEA_CTL0_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "host" },
            Package () { "cdnsp,usb3-phy", \_SB.U3P4.USB0 },
          },
    })
  }
}

Device (SUB5)
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x05)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x091c0324, 0x4)
    Memory32Fixed (ReadWrite, 0x091c0410, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "pinctrl_usb5", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x5 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB5)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x05)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x091e0000, 0x4000)
      Memory32Fixed (ReadWrite, 0x091e4000, 0x4000)
      Memory32Fixed (ReadWrite, 0x091e8000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL1_U3_TYPEA_CTL1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL1_U3_TYPEA_CTL1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB3_TYPEA_CTL1_U3_TYPEA_CTL1_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB3_TYPEA_CTL1_U3_TYPEA_CTL1_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "super-speed-plus" },
            Package () { "dr_mode", "host" },
            Package () { "cdnsp,usb3-phy", \_SB.U3P4.USB1 },
          },
    })
  }
}

Device (U2P6) //USB 2.0 PHY6
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x06)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (U2P7) //USB 2.0 PHY7
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x07)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (U3P4) //USB 3.0 PHY0
{
  Name (_HID, "CIXH2034")     // _HID: Hardware ID
  Name (_UID, 0x04)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09210000, 0x40000)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "cix,usbphy_syscon", \_SB.CRU0 },
        },
  })

  Device(USB0)
  {
    Name (_ADR, 0x00)
  }
  Device(USB1)
  {
    Name (_ADR, 0x01)
  }
}

Device (HUB0) //High Speed USB 0, USB2.0
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x06)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09250310, 0x4)
    Memory32Fixed (ReadWrite, 0x09250400, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_usb6", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x6 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB0)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x00)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x09260000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09264000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09268000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST0_U2_HOST0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST0_U2_HOST0_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST0_U2_HOST0_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB2_HOST0_U2_HOST0_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "high-speed" },
            Package () { "dr_mode", "host" },
          },
    })
  }
}

Device (HUB1) //High Speed USB 1, USB2.0
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x07)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x09280310, 0x4)
    Memory32Fixed (ReadWrite, 0x09280400, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_usb7", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x7 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB1)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x01)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x09290000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09294000, 0x4000)
      Memory32Fixed (ReadWrite, 0x09298000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST1_U2_HOST1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST1_U2_HOST1_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST1_U2_HOST1_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB2_HOST1_U2_HOST1_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "high-speed" },
            Package () { "dr_mode", "host" },
          },
    })
  }
}

Device (U2P0) //USB 2.0 PHY0
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x00)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (U2P1) //USB 2.0 PHY1
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x01)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}

Device (HUB2) //High Speed USB 2, USB2.0
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x08)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x092b0310, 0x4)
    Memory32Fixed (ReadWrite, 0x092b0400, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_usb8", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x8 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB2)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x02)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x092c0000, 0x4000)
      Memory32Fixed (ReadWrite, 0x092c4000, 0x4000)
      Memory32Fixed (ReadWrite, 0x092c8000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST2_U2_HOST2_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST2_U2_HOST2_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST2_U2_HOST2_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB2_HOST2_U2_HOST2_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "high-speed" },
            Package () { "dr_mode", "host" },
          },
    })
  }
}

Device (U2P2) //USB 2.0 PHY2
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x02)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}


Device (HUB3) //High Speed USB 3, USB2.0
{
  Name (_HID, "CIXH2030")     // _HID: Hardware ID
  Name (_UID, 0x09)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Method (_STA)               // _STA: Device status
  {
      Return (0xF)
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x092e0310, 0x4)
    Memory32Fixed (ReadWrite, 0x092e0400, 0x4)
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_usb9", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
          Package () { "id", 0x9 },
          Package () { "axi_bmax_value", 0x7 },
          Package () { "cix,usb_syscon", \_SB.CRU0 },
          Package () { "sof_clk_freq", 8000000 },
          Package () { "lpm_clk_freq", 32000 },
        }
  })

  Device(CUB3)
  {
    Name (_HID, "CIXH2031")     // _HID: Hardware ID
    Name (_UID, 0x03)           // _UID: Unique ID
    Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
    Method (_STA)               // _STA: Device status
    {
        Return (0xF)
    }

    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0x092f0000, 0x4000)
      Memory32Fixed (ReadWrite, 0x092f4000, 0x4000)
      Memory32Fixed (ReadWrite, 0x092f8000, 0x8000)
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST3_U2_HOST3_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST3_U2_HOST3_IRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { USB2_HOST3_U2_HOST3_OTGIRQ_INTERRUPT_ID }
      Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake, 0, "\\_SB.PDC0") { USB2_HOST3_U2_HOST3_IRQ_INTERRUPT_ID }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "maximum-speed", "high-speed" },
            Package () { "dr_mode", "host" },
          },
    })
  }
}

Device (U2P3) //USB 2.0 PHY3
{
  Name (_HID, "CIXH2032")     // _HID: Hardware ID
  Name (_UID, 0x03)           // _UID: Unique ID
  Name (_CCA, 0x00)           // _CCA: Cache Coherency Attribute
  Name (_STA, 0x0F)           // _STA: Device status
}
