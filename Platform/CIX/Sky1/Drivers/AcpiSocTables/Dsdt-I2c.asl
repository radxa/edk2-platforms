/** @file

  Copyright 2023-2024 Cix Technology Group Co., Ltd. All Rights Reserved.

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
}

