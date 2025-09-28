/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (GPI0) {
  Name (_HID, "CIXH1002")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO0_BASE, GPIO0_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_GPIO0_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 32 },
      Package () { "id", 3 },
    }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI0},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_GPIO_RST_APB_N, \_SB.GPI0, "apb_reset"},
  })
}

Device (GPI1) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x1)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO1_BASE, GPIO1_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_GPIO1_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 32 },
      Package () { "id", 4 },
    }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI1},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_GPIO_RST_APB_N, \_SB.GPI1, "apb_reset"},
  })
}

Device (GPI2) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x2)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO2_BASE, GPIO2_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_GPIO2_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 32 },
      Package () { "id", 5 },
    }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI2},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_GPIO_RST_APB_N, \_SB.GPI2, "apb_reset"},
  })
}

Device (GPI3) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x3)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO3_BASE, GPIO3_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_GPIO3_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 17 },
      Package () { "id", 6 },
    }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI3},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_GPIO_RST_APB_N, \_SB.GPI3, "apb_reset"},
  })
}

Device (GPI4) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x4)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO0_S5_BASE, GPIO0_S5_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { S5_SS_U0_CDNSGPIO_GPIO_INT_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 32 },
      Package () { "id", 0 },
    }
  })
  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI4},
  })
}

Device (GPI5) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x5)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO1_S5_BASE, GPIO1_S5_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { S5_SS_U1_CDNSGPIO_GPIO_INT_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 10 },
      Package () { "id", 1 },
    }
  })
  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI5},
  })
}

// S5 GPIO2 not in used
// Device (GPI6) {
//   Name (_HID, "CIXH1003")
//   Name (_UID, 0x6)
//   Name (_STA, 0xF)
//   Name (_CRS, ResourceTemplate () {
//     Memory32Fixed (ReadWrite, GPIO2_S5_BASE, GPIO2_S5_SIZE)
//     Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { S5_SS_U2_CDNSGPIO_GPIO_INT_INTERRUPT_ID }
//   })
//   Name (_DSD, Package () {
//     ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
//     Package () {
//       Package () { "ngpios", 10 },
//       Package () { "id", 2 },
//     }
//   })
// }
