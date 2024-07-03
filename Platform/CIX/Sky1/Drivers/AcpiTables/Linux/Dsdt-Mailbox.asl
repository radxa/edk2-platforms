/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/* mbox_ap2se */
Device (MBX0) {
  Name (_HID, "CIXHA001")
  Name (_CID, "CIXHA001")
  Name (_UID, 0x0)
  Name (_STA, 0x0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      0x05060000,
      0x10000
      )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
      CSU_SE_INTR_MAILB0_CALLER_INTERRUPT_ID
    }
  })
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "cix,mbox_dir", 0x0 },
      Package () { "#mbox-cells", 0x1 },
    }
  })
}

/* mbox_se2ap */
Device (MBX1) {
    Name (_HID, "CIXHA001")
    Name (_CID, "CIXHA001")
    Name (_UID, 0x1)
    Name (_STA, 0x0)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (
        ReadWrite,
        0x05070000,
        0x10000
        )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        CSU_SE_INTR_MAILB1_CALLEE_INTERRUPT_ID
      }
    })
    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "cix,mbox_dir", 0x1 },
          Package () { "#mbox-cells", 0x1 },
        }
    })
}

/* mbox_ap2sfh */
Device (MBX2) {
    Name (_HID, "CIXHA001")
    Name (_CID, "CIXHA001")
    Name (_UID, 0x2)
    Name (_STA, 0x0)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (
        ReadWrite,
        0x080a0000,
        0x10000
        )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        SENSOR_FUSION_INTR_MAILB1_CALLER_INTERRUPT_ID
      }
    })
    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "cix,mbox_dir", 0x0 },
          Package () { "#mbox-cells", 0x1 },
        }
    })
}

/* mbox_sfh2ap */
Device (MBX3) {
    Name (_HID, "CIXHA001")
    Name (_CID, "CIXHA001")
    Name (_UID, 0x3)
    Name (_STA, 0x0)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (
        ReadWrite,
        0x08090000,
        0x10000
        )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        SENSOR_FUSION_INTR_MAILB0_CALLEE_INTERRUPT_ID
      }
    })
    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "cix,mbox_dir", 0x1 },
          Package () { "#mbox-cells", 0x1 },
        }
    })
}

/* mbox_dsp2ap */
Device (MBX4) {
    Name (_HID, "CIXHA001")
    Name (_CID, "CIXHA001")
    Name (_UID, 0x4)
    Name (_STA, 0xf)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (
        ReadWrite,
        0x07100000,
        0x10000
        )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        AUDIO_IRQ_O_MAILBOX1_CALLEE_INTERRUPT_ID
      }
    })
    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "cix,mbox_dir", 0x1 },
          Package () { "#mbox-cells", 0x1 },
        }
    })
}

/* mbox_ap2dsp */
Device (MBX5) {
    Name (_HID, "CIXHA001")
    Name (_CID, "CIXHA001")
    Name (_UID, 0x5)
    Name (_STA, 0xf)
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (
        ReadWrite,
        0x070f0000,
        0x10000
        )
      Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
        AUDIO_IRQ_O_MAILBOX0_CALLER_INTERRUPT_ID
      }
    })
    Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () { "cix,mbox_dir", 0x0 },
          Package () { "#mbox-cells", 0x1 },
        }
    })
}
