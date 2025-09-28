/** @file

  Copyright 2023-2024 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <sky1-iomux.h>

#define MIPI_LT7911UXC_PORT_INIT(RemoteDeviceReference,RemotePort,RemoteEndPoint) \
     Name (_DSD, Package () { \
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), \
            Package () { \
                Package () { "pwdn-gpios",  Package () { ^UXC0, 0, 0, 0 } },   \
                Package () { "power-gpios",  Package () { ^UXC0, 1, 0, 0 } },  \
                Package () { "power1-gpios",  Package () { ^UXC0, 1, 1, 0 } },   \
                Package () { "reset-gpios",  Package () { ^UXC0, 1, 2, 0 } },    \
                Package () { "pinctrl-names", Package () {"default", "gpio"}},   \
            }, \
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () { \
                Package () { "port@0", "PRT0" }, \
            }\
        })\
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
                Package () { "remote-endpoint", Package () { RemoteDeviceReference, RemotePort, RemoteEndPoint } },\
                Package () { "data-lanes", Package () { 4 } },\
                Package () { "clock-lanes", Package () { 0 } },\
            }\
        })

#define DP_USBC_CON_DSD(Name) \
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
        Package () { \
            Package () { Name, "UC00" },\
        }

#define DP_USBC_CON_NODES(DevRefUsbRole,DevRefUsbOriSwitch,DevRefDpAltMux) \
  Name (UC00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "data-role", "host" }, \
                Package () { "power-role", "source" }, \
                Package () { "try-power-role", "source" }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "port@0", "PRT0" },\
                Package () { "port@1", "PRT1" },\
                Package () { "port@2", "PRT2" },\
            }\
  })\
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
                Package () { "remote-endpoint", Package() { DevRefUsbRole, "port@0", "endpoint@0" } },\
            }\
  }) \
  Name (PRT1, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 1 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP01" },\
            }\
  })\
  Name (EP01, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package() { DevRefUsbOriSwitch, "port@0", "endpoint@0" } },\
            }\
  }) \
  Name (PRT2, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 2 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@1", "EP02" },\
            }\
  })\
  Name (EP02, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 1 }, \
                Package () { "remote-endpoint", Package() { DevRefDpAltMux, "port@0", "endpoint@1" } },\
            }\
  })

Device (I2C0) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x0)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_0_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C0_BASE, I2C0_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C0_INTERRUPT_ID }
    // Pinctrl
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_fch_i2c0", ResourceConsumer,)
  })

  //
  // For clock get/set in device driver
  // Device specific method
  // Arg0 = CIX CLOCK GUID
  // Arg1 = Revision
  // Arg2 = Function
  // Arg3 = Parameters
  /*
  Method (_DSM, 4, Serialized) {
    //CLOCK
    If (LEqual(Arg0,ToUUID("854BCF86-4DBF-4f3d-A19A-FBEF3C7CE730"))){
      Switch (ToInteger(Arg2)) {
        Case(GET_CLOCK_RATE)
        {
          Return(\_SB.MBX0.CLKG(CLK_TREE_FCH_I2C0_APB))
        }
        Case(SET_CLOCK_RATE) {
          Return(\_SB.MBX0.CLKS(CLK_TREE_FCH_I2C0_APB,DeRefOf(Index(Arg3, 0)),DeRefOf(Index(Arg3, 1))))
          }
      }
    }
    Return (Buffer() {ACPI_SCMI_INVALID_PARAMETERS})
  }

  Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package(2) {"ClockRange", Package(){100000,200000,400000}},
        }
  }) // _DSD()
  */

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c0_apb"},
        Package () { "clock-frequency", 400000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C0_APB, "", \_SB.I2C0},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_I2C0_RST_APB_N, \_SB.I2C0, "i2c_reset"},
  })

//ISP imgsensor00
  Device (IIS0) {
    Name (_HID, "CIXH3024")
    Name (_UID, 0x00)
    Name (_STA, 0x0F)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x34,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_cam0_hw", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI1", 0, ResourceConsumer) { 18, 16, 12, 15 }
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
          Package () {"actuator-src", \_SB.I2C0.MTR0},
          Package () {"isp-src", \_SB.ISP0},
          Package () {"cix,camera-module-index", 0},
          Package () { "pwren-gpios",  Package () { ^IIS0, 0, 0, 0 } },
          Package () { "pwren0-gpios",  Package () { ^IIS0, 0, 1, 0 } },
          Package () { "reset-gpios",  Package () { ^IIS0, 0, 2, 0 } },
          Package () { "pwdn-gpios",  Package () { ^IIS0, 0, 3, 0 } },
      }
    })
    Name (CLKT, Package() {
      Package() {CLK_TREE_CAMERA_MCLK0, "mclk", \_SB.I2C0.IIS0},
    })
  }
  //ISP motor
  Device (MTR0) {
    Name (_HID, "CIXH3023")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x40,
                      ControllerInitiated,
                      124999999,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
          Package () {"pi-max-frequency", 124999999},
      }
    })
  }

//MIPI lt7911uxc
  Device (UXC0) {
    Name (_HID, "CIXH302C")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x43,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_lt7911_hw", ResourceConsumer, , RawDataBuffer () { 0x0 })
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_lt7911_hw", ResourceConsumer, , RawDataBuffer () { 0x1 })
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI0", 0, ResourceConsumer) { 15 }
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI1", 0, ResourceConsumer) { 16, 18, 12 }
    })
    MIPI_LT7911UXC_PORT_INIT(\_SB_.DPR1, "port@0", "endpoint@1")
  }


//MIPI Virtual0 lt7911uxc
  Device (UXC1) {
    Name (_HID, "CIXH302C")
    Name (_UID, 0x01)
    Name (_STA, 0x0)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x0d,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    MIPI_LT7911UXC_PORT_INIT(\_SB_.DPR2, "port@0", "endpoint@1")
  }
//MIPI Virtual1 lt7911uxc
  Device (UXC2) {
    Name (_HID, "CIXH302C")
    Name (_UID, 0x02)
    Name (_STA, 0x0)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x1d,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    MIPI_LT7911UXC_PORT_INIT(\_SB_.DPR4, "port@0", "endpoint@1")
  }
//MIPI Virtual2 lt7911uxc
  Device (UXC3) {
    Name (_HID, "CIXH302C")
    Name (_UID, 0x03)
    Name (_STA, 0x0)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x2d,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C0",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    MIPI_LT7911UXC_PORT_INIT(\_SB_.DPR5, "port@0", "endpoint@1")
  }
}

Device (I2C1) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x1)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_1_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C1_BASE, I2C1_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C1_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c1_apb"},
        Package () { "clock-frequency", 100000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C1_APB, "", \_SB.I2C1},
  })

  Device (PD10) {
    Name (_HID, "CIXH200D")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x30,
                      ControllerInitiated,
                      100000,
                      AddressingMode7Bit,
                      "\\_SB.I2C1",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 8 }
    })
    Name (_DSD, Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
              Package () {"id", 2},
          },
          DP_USBC_CON_DSD("usbc_con2")
    })
    DP_USBC_CON_NODES(\_SB.SUB2.CUB2, \_SB.UCP2, \_SB.UCP2)
  }

  Device (PD11) {
    Name (_HID, "CIXH200D")
    Name (_UID, 0x1)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x31,
                      ControllerInitiated,
                      100000,
                      AddressingMode7Bit,
                      "\\_SB.I2C1",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 8 }
    })
    Name (_DSD, Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
              Package () {"id", 3},
          },
          DP_USBC_CON_DSD("usbc_con3")
    })
    DP_USBC_CON_NODES(\_SB.SUB3.CUB3, \_SB.UCP3, \_SB.UCP3)
  }

//ISP imgsensor02
  Device (IIS2) {
    Name (_HID, "CIXH3024")
    Name (_UID, 0x02)
    Name (_STA, 0x00)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x38,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C1",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_cam2_hw", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI1", 0, ResourceConsumer) { 11, 8, 10, 7 }
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
          Package () {"cix,camera-module-index", 2},
          Package () { "pwren-gpios",  Package () { ^IIS2, 0, 0, 0 } },
          Package () { "pwren0-gpios",  Package () { ^IIS2, 0, 1, 0 } },
          Package () { "reset-gpios",  Package () { ^IIS2, 0, 2, 0 } },
          Package () { "pwdn-gpios",  Package () { ^IIS2, 0, 3, 0 } },
      }
    })
    Name (CLKT, Package() {
      Package() {CLK_TREE_CAMERA_MCLK2, "mclk", \_SB.I2C1.IIS2},
    })
  }
}

Device (I2C2) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x2)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_2_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C2_BASE, I2C2_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C2_INTERRUPT_ID }
    GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
                "\\_SB.GPI0", 0, ResourceConsumer) { 12, 13 }
    // Pinctrl
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_fch_i2c2", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c2_apb"},
        Package () { "clock-frequency", 400000 },
        Package () { "scl-gpios", Package () { ^I2C2, 0, 0, 0 } },
        Package () { "sda-gpios", Package () { ^I2C2, 0, 1, 0 } },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C2_APB, "", \_SB.I2C2},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_I2C2_RST_APB_N, \_SB.I2C2, "i2c_reset"},
  })

    /* mmc5633 */
  Device (MMC0) {
    Name (_HID, "CIXHA011")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x30,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C2",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
  }

    /* alc5682 */
  Device (RTL5) {
    Name (_HID, "RTL5682")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      GpioInt(Edge, ActiveBoth, SharedAndWake, PullNone, 0,"\\_SB.GPI3",) { 10 }
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_alc5682_irq", ResourceConsumer,)

      I2cSerialBusV2 (0x1a,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C2",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
            Package () { "realtek,dmic1-data-pin", 1 }, /* using GPIO2 pin as dmic1 data pin */
            Package () { "realtek,dmic-clk-pin", 1 }, /* using GPIO3 pin as dmic1 clock pin */
            Package () { "realtek,dmic-clk-rate-hz", 2048000 },
            Package () { "realtek,jd-src", 1 },
          }
    })
  }
}

Device (I2C3) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x3)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_3_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C3_BASE, I2C3_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C3_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c3_apb"},
        Package () { "clock-frequency", 400000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C3_APB, "", \_SB.I2C3},
  })

// RTC RX8900
  Device (RTC0) {
    Name (_HID, "RX8900")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x32,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C3",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0,
                       "pinctrl_ra8900ce_irq", ResourceConsumer,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 10 }
    })
  }

//ISP imgsensor01
  Device (IIS1) {
    Name (_HID, "CIXH3024")
    Name (_UID, 0x1)
    Name (_STA, 0xF)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x36,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C3",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_cam1_hw", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI1", 0, ResourceConsumer) { 18, 16, 17, 19 }
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
          Package () {"cix,camera-module-index", 1 },
          Package () { "pwren-gpios",  Package () { ^IIS1, 0, 0, 0 } },
          Package () { "pwren0-gpios",  Package () { ^IIS1, 0, 1, 0 } },
          Package () { "reset-gpios",  Package () { ^IIS1, 0, 2, 0 } },
          Package () { "pwdn-gpios",  Package () { ^IIS1, 0, 3, 0 } },
      }
    })
    Name (CLKT, Package() {
      Package() {CLK_TREE_CAMERA_MCLK1, "mclk", \_SB.I2C3.IIS1},
    })
  }
}

Device (I2C4) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x4)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_4_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C4_BASE, I2C4_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C4_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c4_apb"},
        Package () { "clock-frequency", 400000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C4_APB, "", \_SB.I2C4},
  })

//ISP imgsensor03
  Device (IIS3) {
    Name (_HID, "CIXH3024")
    Name (_UID, 0x3)
    Name (_STA, 0xF)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x3a,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C4",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_cam3_hw", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI1", 0, ResourceConsumer) { 11, 8, 13, 14 }
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
          Package () {"cix,camera-module-index", 3},
          Package () { "pwren-gpios",  Package () { ^IIS3, 0, 0, 0 } },
          Package () { "pwren0-gpios",  Package () { ^IIS3, 0, 1, 0 } },
          Package () { "reset-gpios",  Package () { ^IIS3, 0, 2, 0 } },
          Package () { "pwdn-gpios",  Package () { ^IIS3, 0, 3, 0 } },
      }
    })
    Name (CLKT, Package() {
      Package() {CLK_TREE_CAMERA_MCLK3, "mclk", \_SB.I2C4.IIS3},
    })
  }
}

Device (I2C5) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x5)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_5_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C5_BASE, I2C5_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C5_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c5_apb"},
        Package () { "clock-frequency", 400000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C5_APB, "", \_SB.I2C5},
  })
}

Device (I2C7) {
  Name (_HID, "CIXH200B")
  Name (_UID, 0x7)
  Method (_STA, 0x0, Serialized) {
    If(\_SB.GETV(ARV_FCH_I2C_7_ENABLE_OFFSET)){
        Return (0xF)
    } else {
        Return (0x0)
    }
  }
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, I2C7_BASE, I2C7_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_I2C7_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"ClockName", "fch_i2c7_apb"},
        Package () { "clock-frequency", 100000 },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_I2C7_APB, "", \_SB.I2C7},
  })

  Device (PD00) {
    Name (_HID, "CIXH200D")
    Name (_UID, 0x1)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x30,
                      ControllerInitiated,
                      100000,
                      AddressingMode7Bit,
                      "\\_SB.I2C7",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 7 }
    })
    Name (_DSD, Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
              Package () {"id", 0},
          },
          DP_USBC_CON_DSD("usbc_con0")
    })
    DP_USBC_CON_NODES(\_SB.SUB0.CUB0, \_SB.UCP0, \_SB.UCP0)
  }

  Device (PD01) {
    Name (_HID, "CIXH200D")
    Name (_UID, 0x2)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x31,
                      ControllerInitiated,
                      100000,
                      AddressingMode7Bit,
                      "\\_SB.I2C7",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 7 }
    })
    Name (_DSD, Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
              Package () {"id", 1},
          },
          DP_USBC_CON_DSD("usbc_con1")
    })
    DP_USBC_CON_NODES(\_SB.SUB1.CUB1, \_SB.UCP1, \_SB.UCP1)
  }
}

