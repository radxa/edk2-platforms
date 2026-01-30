/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External (\_SB.PRC4, DeviceObj)

  Device (PVC4) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x40)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "wifi_vbat_gpio", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI4", 0, ResourceConsumer) { 12 }
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "VDD_3V3_PCIE" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC4, 0, 0, 0 } },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active_high", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
