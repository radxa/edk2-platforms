/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Gpio.h"

External (\_SB.PRC0, DeviceObj)
External (\_SB.PRC1, DeviceObj)
External (\_SB.PRC2, DeviceObj)
External (\_SB.PRC3, DeviceObj)
External (\_SB.PRC4, DeviceObj)

#if PCIE_X8_VCC_SUPPLY
  Device (PVC0) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x03)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "gpio001_mkey_en", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI4", 0, ResourceConsumer) { 0 }  // GPIO001
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "vgfx_power" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC0, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-always-on", 1 },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
#endif

#if PCIE_X2_VCC_SUPPLY
  Device (PVC2) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x05)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "wlan_pwren", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI4", 0, ResourceConsumer) { 12 } // GPIO013
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "vdd_3v3_pcie" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC2, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-always-on", 1 },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
#endif

#if PCIE_X1_0_PERST
  Device (PVC3) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x06)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "phy2_pwr_en", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI5", 0, ResourceConsumer) { 7 }  // GPIO022
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "gbe2_power_3v3" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC3, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-always-on", 1 },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
#endif

#if PCIE_X1_1_PERST
  Device (PVC4) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x07)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "lom_pwren", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI4", 0, ResourceConsumer) { 9 }  // GPIO010
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "gbe1_power_3v3" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC4, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-always-on", 1 },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
#endif
