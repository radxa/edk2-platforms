/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Gpio.h"

Device (VTP0) {
  Name (_HID, "PRP0001")
  Name (_UID, 0x22)
  Name (_STA, 0x0B)

  Name (_CRS, ResourceTemplate () {
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "tpm_pwr_en", ResourceConsumer,)
    GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
    "\\_SB.GPI5", 0, ResourceConsumer) { 8 }  // GPIO023
  })

  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "compatible", "regulator-fixed" },
      Package () { "regulator-name", "vdd_tpm_pwr_en" },
      Package () { "regulator-min-microvolt", 3300000 },
      Package () { "regulator-max-microvolt", 3300000 },
      Package () { "gpio", Package () { ^VTP0, 0, 0, GPIO_ACTIVE_LOW } },
      Package () { "regulator-always-on", 1 },
      Package () { "regulator-pull-down", 1 },
      Package () { "off-on-delay-us", 15000 },
    },
  })
}
