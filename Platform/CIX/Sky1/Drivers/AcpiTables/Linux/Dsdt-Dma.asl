/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (DMA0) {
  Name (_HID, "CIXHA014")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0x0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DMA_BASE, DMA_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_DMA_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "dma-channels", 8 },
      Package () { "dma-requests", 8 },
    }
  })
}

Device (DMA1) {
  Name (_HID, "CIXH1006")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0x0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, AUDIO_DMAC_BASE, AUDIO_DMAC_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { AUDIO_IRQ_O_AP_DMA_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "dma-channels", 8 },
      Package () { "dma-requests", 20 },
      Package () { "arm,reg-map", Package () { 0x07010000, 0x20000000 } },
      Package () { "arm,ram-map", Package () { 0xc0000000, 0x30000000 } },
      Package () { "arm,remote-ctrl", \_SB.ACRU },
    }
  })
}
