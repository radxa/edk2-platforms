
/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Gpio.h"

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

External (\_SB.I2C5, DeviceObj)
External (\_SB.SUB0.CUB0, DeviceObj)
External (\_SB.UCP0, DeviceObj)

Device (PDR0) {
  Name (_HID, "PRP0001")
  Name (_UID, 0x30)
  Name (_STA, 0x0B)

  Name (_CRS, ResourceTemplate () {
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "pd_reset", ResourceConsumer,)
    GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
    "\\_SB.GPI4", 0, ResourceConsumer) { 6 }  // GPIO007
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "compatible", "regulator-fixed" },
      Package () { "regulator-name", "vdd_pd_reset" },
      Package () { "regulator-min-microvolt", 3300000 },
      Package () { "regulator-max-microvolt", 3300000 },
      Package () { "gpio", Package () { ^PDR0, 0, 0, GPIO_ACTIVE_LOW } },
      Package () { "regulator-always-on", 1 },
      Package () { "regulator-pull-down", 1 },
      Package () { "off-on-delay-us", 15000 },
    },
  })
}

Scope (\_SB.I2C5)
{
  Device (PD10) {
    Name (_HID, "CIXH200D")
    Name (_UID, 0x0)
    Name (_STA, 0xF)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x51,
                      ControllerInitiated,
                      100000,
                      AddressingMode7Bit,
                      "\\_SB.I2C5",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
      GpioInt(Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI4") { 8 }    // GPIO009
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
}
