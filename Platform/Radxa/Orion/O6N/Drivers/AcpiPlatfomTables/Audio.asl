/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Protocol/ClockId.h>

External (\_SB.ACRU, DeviceObj)

Device (SNDC) {
  Name (_HID, "CIXH6070")
  Name (_UID, 0x0)
  Name (_STA, 0xF)

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "cru-ctrl", \_SB.ACRU },
      }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.SNDC},
    Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.SNDC},
  })
}
