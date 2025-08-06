/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (GPI0) {
  Name (_HID, "CIXH1003")
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio0IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO043",
        "GPIO044",
        "GPIO045",
        "GPIO046",
        "GPIO047",
        "GPIO048",
        "GPIO049",
        "GPIO050",
        "GPIO051",
        "GPIO052",
        "GPIO053",
        "GPIO054",
        "GPIO055",
        "GPIO056",
        "GPIO057",
        "GPIO058",
        "GPIO059",
        "GPIO060",
        "GPIO061",
        "GPIO062",
        "GPIO063",
        "GPIO064",
        "GPIO065",
        "GPIO066",
        "GPIO067",
        "GPIO068",
        "GPIO069",
        "GPIO070",
        "GPIO071",
        "GPIO072",
        "GPIO073",
        "GPIO074"
      }},
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio1IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO075",
        "GPIO076",
        "GPIO077",
        "GPIO078",
        "GPIO079",
        "GPIO080",
        "GPIO081",
        "GPIO082",
        "GPIO083",
        "GPIO084",
        "GPIO085",
        "GPIO086",
        "GPIO087",
        "GPIO088",
        "GPIO089",
        "GPIO090",
        "GPIO091",
        "GPIO092",
        "GPIO093",
        "GPIO094",
        "GPIO095",
        "GPIO096",
        "GPIO097",
        "GPIO098",
        "GPIO099",
        "GPIO100",
        "GPIO101",
        "GPIO102",
        "GPIO103",
        "GPIO104",
        "GPIO105",
        "GPIO106"
      }},
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio2IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO107",
        "GPIO108",
        "GPIO109",
        "GPIO110",
        "GPIO111",
        "GPIO112",
        "GPIO113",
        "GPIO114",
        "GPIO115",
        "GPIO116",
        "GPIO117",
        "GPIO118",
        "GPIO119",
        "GPIO120",
        "GPIO121",
        "GPIO122",
        "GPIO123",
        "GPIO124",
        "GPIO125",
        "GPIO126",
        "GPIO127",
        "GPIO128",
        "GPIO129",
        "GPIO130",
        "GPIO131",
        "GPIO132",
        "GPIO133",
        "GPIO134",
        "GPIO135",
        "GPIO136",
        "GPIO137",
        "GPIO138"
      }},
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio3IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO139",
        "GPIO140",
        "GPIO141",
        "GPIO142",
        "GPIO143",
        "GPIO144",
        "GPIO145",
        "GPIO146",
        "GPIO147",
        "GPIO148",
        "GPIO149",
        "GPIO150",
        "GPIO151",
        "GPIO152",
        "GPIO153",
        "DP2_DIGON",
        "DP2_BLON"
      }},
    }
  })

  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI3},
  })
  Name (RSTL, Package() {
    Package() {\_SB.RST1, SW_GPIO_RST_APB_N, \_SB.GPI3, "apb_reset"},
  })
}

// S5 GPIO0
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio4IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO001",
        "GPIO002",
        "GPIO003",
        "GPIO004",
        "GPIO005",
        "GPIO006",
        "GPIO007",
        "GPIO008",
        "GPIO009",
        "GPIO010",
        "GPIO011",
        "GPIO012",
        "GPIO013",
        "GPIO014",
        "GPIO025",
        "GPIO026",
        "GPIO027",
        "GPIO028",
        "GPIO029",
        "GPIO030",
        "GPIO031",
        "GPIO032",
        "GPIO033",
        "GPIO034",
        "GPIO035",
        "GPIO036",
        "GPIO037",
        "GPIO038",
        "GPIO039",
        "GPIO040",
        "GPIO041",
        "GPIO042"
      }},
    }
  })
  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI4},
  })
}

// S5 GPIO1
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
      Package () { "gpio-io-mask", FixedPcdGet32(PcdAcpiGpio5IoMask) },
      Package () { "gpio-line-names", Package () {
        "GPIO015",
        "GPIO016",
        "GPIO017",
        "GPIO018",
        "GPIO019",
        "GPIO020",
        "GPIO021",
        "GPIO022",
        "GPIO023",
        "GPIO024"
      }},
    }
  })
  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI5},
  })
}

// S5 GPIO2
Device (GPI6) {
  Name (_HID, "CIXH1003")
  Name (_UID, 0x6)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, GPIO2_S5_BASE, GPIO2_S5_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { S5_SS_U2_CDNSGPIO_GPIO_INT_INTERRUPT_ID }
  })
  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "ngpios", 10 },
      Package () { "id", 2 },
    }
  })
  Name (CLKT, Package() {
    Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI6},
  })
}
