/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  Device (PVC5) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x08)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "3v3_bkey", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI0", 0, ResourceConsumer) { 30 }  // GPIO073
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "vdd_3v3_bkey" },
        Package () { "regulator-min-microvolt", 3300000 },
        Package () { "regulator-max-microvolt", 3300000 },
        Package () { "gpio", Package () { ^PVC5, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      },
    })
  }

  Device (PVC6) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x09)
    Name (_STA, 0x0B)

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "bkey_reset", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI0", 0, ResourceConsumer) { 31 }  // GPIO074
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "vdd_bkey_reset" },
        Package () { "regulator-min-microvolt", 1800000 },
        Package () { "regulator-max-microvolt", 1800000 },
        Package () { "gpio", Package () { ^PVC6, 0, 0, GPIO_ACTIVE_HIGH } },
        Package () { "regulator-pull-down", 1 },
        Package () { "enable-active-high", 1 },
        Package () { "off-on-delay-us", 15000 },
      },
    })
  }

  Device (PVC7) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x0A)
    Method (_STA, 0x0, Serialized) {
      If (LEqual (FixedPcdGet8 (PcdUFSPowerMode), 0xff)) {
        Return (0x0B)
      }
      Return (0)
    }

    Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "ufs_5v_en", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI4", 0, ResourceConsumer) { 13 } // GPIO014
    })

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", "regulator-fixed" },
        Package () { "regulator-name", "vdd_5v0_ufs" },
        Package () { "regulator-min-microvolt", 5000000 },
        Package () { "regulator-max-microvolt", 5000000 },
        Package () { "gpio", Package () { ^PVC7, 0, 0, GPIO_ACTIVE_LOW } },
        Package () { "regulator-pull-down", 1 },
        Package () { "off-on-delay-us", 15000 },
      }
    })
  }
