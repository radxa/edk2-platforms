/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#define EMPTY_STR ""

#define VDP_PORT_INIT(RemoteDeviceReference,RemotePipeline,RemotePort,RemoteEndPoint) \
 Name (_DSD, Package () { \
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () { \
                Package () { "port@0", "PRT0" },\
            }\
        })\
  Name (PRT0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP00" },\
            }\
  })\
  Name (EP00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package() { RemoteDeviceReference, RemotePipeline, RemotePort, RemoteEndPoint } },\
            }\
  })

#define DP_PORT_INIT(RemoteDeviceReference, RemotePipeline, RemotePort, RemoteEndPoint, DpLaneNumber, DpMaxRate, AuxClockDivider, DpPhyRef, EdpPanelRef) \
 Name (_DSD, Package () { \
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () { \
            /*  Package () { "cix,dp-lane-number", DpLaneNumber }, */ \
            /*  Package () { "cix,dp-max-rate", DpMaxRate }, */ \
            /*  Package () { "cix,aux-clock-divider", AuxClockDivider }, */ \
                Package () { "cix,dp-phy", DpPhyRef }, \
                Package () { "edp-panel", EdpPanelRef }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () { \
                Package () { "port@0", "PRT0" },\
            }\
        })\
  Name (PRT0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP00" },\
            }\
  })\
  Name (EP00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package() { RemoteDeviceReference, RemotePipeline, RemotePort, RemoteEndPoint } },\
            }\
  })

#define DPU_PORT_INIT(RemoteDeviceReference1,RemotePort1,RemoteEndPoint1,RemoteDeviceReference2,RemotePort2,RemoteEndPoint2) \
     Name (_DSD, Package () { \
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "pipeline@0", "PIP0" },\
                Package () { "pipeline@1", "PIP1" },\
            }\
        })\
        Name (PIP0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "lpu_raxi_aoutstdcapb", 32 }, \
                Package () { "lpu_raxi_boutstdcapb", 32 }, \
                Package () { "lpu_raxi_ben", 0 }, \
                Package () { "lpu_raxi_burstlen", 16 }, \
                Package () { "lpu_raxi_arqos", 15 }, \
                Package () { "lpu_raxi_ord", 0 }, \
                Package () { "lpu_waxi_outstdcapb", 16 }, \
                Package () { "lpu_waxi_burstlen", 16 }, \
                Package () { "lpu_waxi_awqos", 15 }, \
                Package () { "lpu_waxi_ord", 0 }, \
                Package () { "lpu_l0_arcache", 3 }, \
                Package () { "lpu_l1_arcache", 3 }, \
                Package () { "lpu_l2_arcache", 3 }, \
                Package () { "lpu_l3_arcache", 3 }, \
                Package () { "lpu_lw_arcache", 3 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "port@0", "PRT0" },\
            }\
        })\
        Name (PRT0, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@0", "EP00" },\
            }\
        })\
        Name (EP00, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package () { RemoteDeviceReference1, RemotePort1, RemoteEndPoint1 } },\
            }\
        })\
        Name (PIP1, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 1 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "port@1", "PRT1" },\
            }\
        })\
         Name (PRT1, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
            },\
            ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),\
            Package () {\
                Package () { "endpoint@1", "EP10" },\
            }\
        })\
        Name (EP10, Package() {\
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),\
            Package () {\
                Package () { "reg", 0 }, \
                Package () { "remote-endpoint", Package () {RemoteDeviceReference2, RemotePort2, RemoteEndPoint2 } },\
            }\
        })

Device (VDP0) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x0)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU0,"pipepline0", "port@0", "endpoint@0")
}

Device (VDP1) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x1)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU0,"pipepline1", "port@1", "endpoint@1")
}

Device (VDP2) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x2)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU1,"pipepline0", "port@0", "endpoint@0")
}

Device (VDP3) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x3)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU1,"pipepline1", "port@1", "endpoint@1")
}

Device (VDP4) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x4)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU2,"pipeline@0", "port@0", "endpoint@0")
}

Device (VDP5) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x5)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU2,"pipeline@1", "port@1", "endpoint@1")
}

Device (VDP6) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x6)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU3,"pipepline0", "port@0", "endpoint@0")
}

Device (VDP7) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x7)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU3,"pipepline1", "port@1", "endpoint@1")
}

Device (VDP8) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x8)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU4,"pipepline0", "port@0", "endpoint@0")
}

Device (VDP9) {
  Name (_HID, "CIXH503F")
  Name (_UID, 0x9)
  Name (_STA, 0x0)
  VDP_PORT_INIT(\_SB.DPU4,"pipepline1", "port@1", "endpoint@1")
}

Device (DP00) {
  Name (_HID, "CIXH502F")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DP0_DP_CONTROLLER_BASE, DP0_CONTROLLER_SIZE, DP)
    Memory32Fixed (ReadWrite, DP0_DSC_CONTROLLER_BASE, DP0_CONTROLLER_SIZE, DSC)
    Memory32Fixed (ReadWrite, DP0_DP_PHY_CONTROLLER_BASE, DP0_PHY_CONTROLLER_SIZE, DPHY)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DP_DP0_APB_INT_INTERRUPT_ID }
  })

  DP_PORT_INIT(\_SB.DPU0, "pipeline@0", "port@0", "endpoint@0", 1, 162000, 200, \_SB.UCP0.UDPP, EMPTY_STR)
}

Device (DP01) {
  Name (_HID, "CIXH502F")
  Name (_UID, 0x1)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DP1_DP_CONTROLLER_BASE, DP1_CONTROLLER_SIZE, DP)
    Memory32Fixed (ReadWrite, DP1_DSC_CONTROLLER_BASE, DP1_CONTROLLER_SIZE, DSC)
    Memory32Fixed (ReadWrite, DP1_DP_PHY_CONTROLLER_BASE, DP1_PHY_CONTROLLER_SIZE, DPHY)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DP_DP1_APB_INT_INTERRUPT_ID }
  })

  DP_PORT_INIT(\_SB.DPU1, "pipeline@0", "port@0", "endpoint@0", 1, 162000, 200, \_SB.UCP1.UDPP, EMPTY_STR)
}

Device (DP02) {
  Name (_HID, "CIXH502F")
  Name (_UID, 0x2)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DP2_DP_CONTROLLER_BASE, DP2_CONTROLLER_SIZE, DP)
    Memory32Fixed (ReadWrite, DP2_DSC_CONTROLLER_BASE, DP2_CONTROLLER_SIZE, DSC)
    Memory32Fixed (ReadWrite, DP2_DP_PHY_CONTROLLER_BASE, DP2_CONTROLLER_SIZE, DPHY)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DP_DP2_APB_INT_INTERRUPT_ID }
  })

  DP_PORT_INIT(\_SB.DPU2, "pipeline@0", "port@0", "endpoint@0", 1, 540000, 200, EMPTY_STR, \_SB.EDP0)
}

Device (DP03) {
  Name (_HID, "CIXH502F")
  Name (_UID, 0x3)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DP3_DP_CONTROLLER_BASE, DP3_CONTROLLER_SIZE, DP)
    Memory32Fixed (ReadWrite, DP3_DSC_CONTROLLER_BASE, DP3_CONTROLLER_SIZE, DSC)
    Memory32Fixed (ReadWrite, DP3_DP_PHY_CONTROLLER_BASE, DP3_PHY_CONTROLLER_SIZE, DPHY)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DP_DP3_APB_INT_INTERRUPT_ID }
  })

  DP_PORT_INIT(\_SB.DPU3, "pipeline@0", "port@0", "endpoint@0", 1, 162000, 200, \_SB.UCP2.UDPP, EMPTY_STR)
}

Device (DP04) {
  Name (_HID, "CIXH502F")
  Name (_UID, 0x4)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DP4_DP_CONTROLLER_BASE, DP4_CONTROLLER_SIZE, DP)
    Memory32Fixed (ReadWrite, DP4_DSC_CONTROLLER_BASE, DP4_CONTROLLER_SIZE, DSC)
    Memory32Fixed (ReadWrite, DP4_DP_PHY_CONTROLLER_BASE, DP4_PHY_CONTROLLER_SIZE, DPHY)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DP_DP4_APB_INT_INTERRUPT_ID }
  })

  DP_PORT_INIT(\_SB.DPU4, "pipeline@0", "port@0", "endpoint@0", 1, 162000, 200, \_SB.UCP3.UDPP, EMPTY_STR)
}

Device (DPU0) {
  Name (_HID, "CIXH5010")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU0_CONTROLLER_BASE, DPU0_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU0_IRQ0_INTERRUPT_ID }
  })

  //DPU_PORT_INIT(\_SB.VDP0,"port@0","endpoint@0",\_SB.VDP1, "port@0", "endpoint@0")
  // DPU_PORT_INIT(\_SB.DP00,"port@0","endpoint@0",\_SB.VDP1, "port@0", "endpoint@0")
  DPU_PORT_INIT(\_SB.DP00,"port@0","endpoint@0", EMPTY_STR, EMPTY_STR, EMPTY_STR)

  PowerResource(PRS0, 0, 0)
  {
    OperationRegion(OPR0,SystemMemory,DPU0_RCSU_PD_REG, 0x04)
    Field (OPR0, DWordAcc, NoLock, Preserve) {
      MSK0, 32,
    }
    Method(_STA, 0, Serialized)
    {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN1
        If (Local0 > 0)
        {
          Return (1)
        }
        Else
        {
          Return (0)
        }
    }
    Method(_ON, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 | PD_PG_EN1 | TIME_CYCLE_CNT
      MSK0 = Local0
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_DPU, DPU_RCSU_BASE_REG, BIT0)
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 & ~PD_PG_EN1
      MSK0 = Local0
    }
  }

  Name(_PR0, Package(1){PRS0})
  Name(_PR3, Package(1){PRS0})
}

Device (DPU1) {
  Name (_HID, "CIXH5010")
  Name (_UID, 0x1)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU1_CONTROLLER_BASE, DPU1_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU1_IRQ0_INTERRUPT_ID }
  })

  //DPU_PORT_INIT(\_SB.VDP2,"port@0","endpoint@0",\_SB.VDP3, "port@0", "endpoint@0")
  //DPU_PORT_INIT(\_SB.DP01,"port@0","endpoint@0",\_SB.VDP3, "port@0", "endpoint@0")
  DPU_PORT_INIT(\_SB.DP01,"port@0","endpoint@0", EMPTY_STR, EMPTY_STR, EMPTY_STR)

  PowerResource(PRS1, 0, 0)
  {
    OperationRegion(OPR1,SystemMemory,DPU1_RCSU_PD_REG, 0x04)
    Field (OPR1, DWordAcc, NoLock, Preserve) {
      MSK0, 32,
    }
    Method(_STA, 0, Serialized)
    {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN1
        If (Local0 > 0)
        {
          Return (1)
        }
        Else
        {
          Return (0)
        }
    }
    Method(_ON, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 | PD_PG_EN1 | TIME_CYCLE_CNT
      MSK0 = Local0
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_DPU, DPU_RCSU_BASE_REG, BIT1)
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 & ~PD_PG_EN1
      MSK0 = Local0
    }
  }

  Name(_PR0, Package(1){PRS1})
  Name(_PR3, Package(1){PRS1})
}

Device (DPU2) {
  Name (_HID, "CIXH5010")
  Name (_UID, 0x2)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU2_CONTROLLER_BASE, DPU2_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU2_IRQ0_INTERRUPT_ID }
  })

  // DPU_PORT_INIT(\_SB.VDP4,"port@0","endpoint@0",\_SB.VDP5, "port@0", "endpoint@0")
  //DPU_PORT_INIT(\_SB.DP02,"port@0","endpoint@0",\_SB.VDP5, "port@0", "endpoint@0")
  DPU_PORT_INIT(\_SB.DP02,"port@0","endpoint@0", EMPTY_STR, EMPTY_STR, EMPTY_STR)

  PowerResource(PRS2, 0, 0)
  {
    OperationRegion(OPR2,SystemMemory,DPU2_RCSU_PD_REG, 0x04)
    Field (OPR2, DWordAcc, NoLock, Preserve) {
      MSK0, 32,
    }
    Method(_STA, 0, Serialized)
    {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN1
        printf ("CIX Debug: DPU2 get current state=%o:%o\n", Local0, Local1)
        If (Local0 > 0)
        {
          Return (1)
        }
        Else
        {
          Return (0)
        }
    }
    Method(_ON, 0, Serialized)
    {
      Local0 = MSK0
      printf ("CIX Debug: DPU2 power on, mask1=%o\n", Local0)
      Local0 = Local0 | PD_PG_EN1 | TIME_CYCLE_CNT
      MSK0 = Local0
      printf ("CIX Debug: DPU2 power on, mask2=%o\n", MSK0)
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_DPU, DPU_RCSU_BASE_REG, BIT2)
      printf ("CIX Debug: Call do_mem_repair end.\n")
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = MSK0
      printf ("CIX Debug: DPU2 power off, mask1=%o\n", Local0)
      Local0 = Local0 & ~PD_PG_EN1
      MSK0 = Local0
      printf ("CIX Debug: DPU2 power off, mask2=%o\n", MSK0)
    }
  }

  Name(_PR0, Package(1){PRS2})
  Name(_PR3, Package(1){PRS2})
}

Device (DPU3) {
  Name (_HID, "CIXH5010")
  Name (_UID, 0x3)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU3_CONTROLLER_BASE, DPU3_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU3_IRQ0_INTERRUPT_ID }
  })

  //DPU_PORT_INIT(\_SB.VDP6,"port@0","endpoint@0",\_SB.VDP7, "port@0", "endpoint@0")
  //DPU_PORT_INIT(\_SB.DP03,"port@0","endpoint@0",\_SB.VDP7, "port@0", "endpoint@0")
  DPU_PORT_INIT(\_SB.DP03,"port@0","endpoint@0", EMPTY_STR, EMPTY_STR, EMPTY_STR)

  PowerResource(PRS3, 0, 0)
  {
    OperationRegion(OPR3,SystemMemory,DPU3_RCSU_PD_REG, 0x04)
    Field (OPR3, DWordAcc, NoLock, Preserve) {
      MSK0, 32,
    }
    Method(_STA, 0, Serialized)
    {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN1
        If (Local0 > 0)
        {
          Return (1)
        }
        Else
        {
          Return (0)
        }
    }
    Method(_ON, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 | PD_PG_EN1 | TIME_CYCLE_CNT
      MSK0 = Local0
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_DPU, DPU_RCSU_BASE_REG, BIT3)
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 & ~PD_PG_EN1
      MSK0 = Local0
    }
  }

  Name(_PR0, Package(1){PRS3})
  Name(_PR3, Package(1){PRS3})
}

Device (DPU4) {
  Name (_HID, "CIXH5010")
  Name (_UID, 0x4)
  Name (_STA, 0xF)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU4_CONTROLLER_BASE, DPU4_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU4_IRQ0_INTERRUPT_ID }
  })

  //DPU_PORT_INIT(\_SB.VDP8,"port@0","endpoint@0",\_SB.VDP9, "port@0", "endpoint@0")
  //DPU_PORT_INIT(\_SB.DP04,"port@0","endpoint@0",\_SB.VDP9, "port@0", "endpoint@0")
  DPU_PORT_INIT(\_SB.DP04,"port@0","endpoint@0", EMPTY_STR, EMPTY_STR, EMPTY_STR)

  PowerResource(PRS4, 0, 0)
  {
    OperationRegion(OPR4,SystemMemory,DPU4_RCSU_PD_REG, 0x04)
    Field (OPR4, DWordAcc, NoLock, Preserve) {
      MSK0, 32,
    }
    Method(_STA, 0, Serialized)
    {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN1
        If (Local0 > 0)
        {
          Return (1)
        }
        Else
        {
          Return (0)
        }
    }
    Method(_ON, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 | PD_PG_EN1 | TIME_CYCLE_CNT
      MSK0 = Local0
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_DPU, DPU_RCSU_BASE_REG, BIT4)
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = MSK0
      Local0 = Local0 & ~PD_PG_EN1
      MSK0 = Local0
    }
  }

  Name(_PR0, Package(1){PRS4})
  Name(_PR3, Package(1){PRS4})
}

Device (AEU0) {
  Name (_HID, "CIXH5011")
  Name (_UID, 0x00)
  Name (_STA, 0x00)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU_AEU0_CONTROLLER_BASE, DPU_AEU0_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU0_IRQ1_INTERRUPT_ID }
  })
}

Device (AEU1) {
  Name (_HID, "CIXH5011")
  Name (_UID, 0x01)
  Name (_STA, 0x00)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU_AEU1_CONTROLLER_BASE, DPU_AEU1_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU1_IRQ1_INTERRUPT_ID }
  })
}

Device (AEU2) {
  Name (_HID, "CIXH5011")
  Name (_UID, 0x02)
  Name (_STA, 0x0)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU_AEU2_CONTROLLER_BASE, DPU_AEU2_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU2_IRQ1_INTERRUPT_ID }
  })
}

Device (AEU3) {
  Name (_HID, "CIXH5011")
  Name (_UID, 0x03)
  Name (_STA, 0x00)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU_AEU3_CONTROLLER_BASE, DPU_AEU3_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU3_IRQ1_INTERRUPT_ID }
  })
}

Device (AEU4) {
  Name (_HID, "CIXH5011")
  Name (_UID, 0x04)
  Name (_STA, 0x00)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, DPU_AEU4_CONTROLLER_BASE, DPU_AEU4_CONTROLLER_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { DPU_EXT_U_DPU4_IRQ1_INTERRUPT_ID }
  })
}

//DP2 backlight
Device (DPBL) {
  Name (_HID, "CIXH5041")
  Name (_UID, 0x00)
  Name (_STA, 0x0F)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
            "\\_SB.GPI3", 0, ResourceConsumer) { 15 }
  })

  Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "enable-gpios",  Package () {^DPBL, 0, 0, 0} },
        Package () { "pwms", Package () {\_SB.PWM0, 0, 100000 } },
        Package () { "default-brightness-level", 200 },
        Package () { "brightness-levels",  Package () {
			0  , 1  , 2  , 3  , 4  , 5  , 6  , 7 ,
			8  , 9  ,10  ,11  ,12  ,13  ,14  ,15 ,
			16  ,17  ,18  ,19  ,20  ,21  ,22  ,23 ,
			24  ,25  ,26  ,27  ,28  ,29  ,30  ,31 ,
			32  ,33  ,34  ,35  ,36  ,37  ,38  ,39 ,
			40  ,41  ,42  ,43  ,44  ,45  ,46  ,47 ,
			48  ,49  ,50  ,51  ,52  ,53  ,54  ,55 ,
			56  ,57  ,58  ,59  ,60  ,61  ,62  ,63 ,
			64  ,65  ,66  ,67  ,68  ,69  ,70  ,71 ,
			72  ,73  ,74  ,75  ,76  ,77  ,78  ,79 ,
			80  ,81  ,82  ,83  ,84  ,85  ,86  ,87 ,
			88  ,89  ,90  ,91  ,92  ,93  ,94  ,95 ,
			96  ,97  ,98  ,99 ,100 ,101 ,102 ,103 ,
			104 ,105 ,106 ,107 ,108 ,109 ,110 ,111 ,
			112 ,113 ,114 ,115 ,116 ,117 ,118 ,119 ,
			120 ,121 ,122 ,123 ,124 ,125 ,126 ,127 ,
			128 ,129 ,130 ,131 ,132 ,133 ,134 ,135 ,
			136 ,137 ,138 ,139 ,140 ,141 ,142 ,143 ,
			144 ,145 ,146 ,147 ,148 ,149 ,150 ,151 ,
			152 ,153 ,154 ,155 ,156 ,157 ,158 ,159 ,
			160 ,161 ,162 ,163 ,164 ,165 ,166 ,167 ,
			168 ,169 ,170 ,171 ,172 ,173 ,174 ,175 ,
			176 ,177 ,178 ,179 ,180 ,181 ,182 ,183 ,
			184 ,185 ,186 ,187 ,188 ,189 ,190 ,191 ,
			192 ,193 ,194 ,195 ,196 ,197 ,198 ,199 ,
			200 ,201 ,202 ,203 ,204 ,205 ,206 ,207 ,
			208 ,209 ,210 ,211 ,212 ,213 ,214 ,215 ,
			216 ,217 ,218 ,219 ,220 ,221 ,222 ,223 ,
			224 ,225 ,226 ,227 ,228 ,229 ,230 ,231 ,
			232 ,233 ,234 ,235 ,236 ,237 ,238 ,239 ,
			240 ,241 ,242 ,243 ,244 ,245 ,246 ,247 ,
			248 ,249 ,250 ,251 ,252 ,253 ,254
		    }
		  },
    }
  })
}

Device (EDP0) {
  Name (_HID, "CIXH5040")
  Name (_UID, 0x00)
  Name (_STA, 0x0F)
  Name (_CCA, 0)
  Name (_CRS, ResourceTemplate () {
      PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_edp0", ResourceConsumer,)
      GpioIo (Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly,
      "\\_SB.GPI3", 0, ResourceConsumer) { 16 }
  })

  Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "prepare-delay-ms", 120 },
        Package () { "enable-delay-ms", 120 },
        Package () { "unprepare-delay-ms", 500 },
        Package () { "disable-delay-ms", 120 },
        Package () { "width-mm", 129 },
        Package () { "height-mm", 171 },
        Package () { "enable-gpios",  Package () {^EDP0, 0, 0, 0} },
        Package () { "backlight", \_SB.DPBL },
      }
    })
}
