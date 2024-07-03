/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (HDA) {
  Name (_HID, "CIXH6020")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      0x070c0000,
      0x10000
      )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { AUDIO_IRQ_O_HDA_INTERRUPT_ID }
    GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
                "\\_SB.GPI3", 0, ResourceConsumer) {
        5,
    }
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_hda", ResourceConsumer,)
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "pdb-gpios", Package () { ^HDA, 0, 0, 0 } },
      Package () { "cru-ctrl", \_SB.ACRU },
    }
  })
}

Device (HDAC) {
  Name (_HID, "CIXH6030")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
}
