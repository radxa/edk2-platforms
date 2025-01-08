/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (NPU0) {
  Name (_HID, "CIXH4000")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CCA, 0)

  Method (_INI, 0, NotSerialized)
  {
    Local0 = GETV(ARV_NPU_SUPPORT_INFO_OFFSET)
    Store(Local0, Index (DerefOf (Index (DerefOf (Index(_DSD, 1)), 2)),1))
  }

  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, NPU_CONTROLLER_BASE, NPU_CONTROLLER_SIZE)
    Memory32Fixed (ReadWrite, FixedPcdGet32 (PcdNpuReservedMemoryBase) , FixedPcdGet32 (PcdNpuReservedMemorySize))
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { NPU_EXT_U_NPU_CORE_IRQO_INTERRUPT_ID }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package () {"cluster-partition", Package (){0, 0}},
        Package () {"gm-policy", 1},
        Package () {"core_mask", 3},
    }
  })

  PowerResource(PPRS, 0, 0)
  {
    OperationRegion(OPRT,SystemMemory,NPU_TOP_PD_REG, 0x04)
    Field (OPRT, DWordAcc, NoLock, Preserve) {
      TMSK, 32,
    }
    Method(_STA, 0, Serialized)
    {
      Local0 = TMSK
      Local1 = TMSK
      Local0 = Local0 & PD_PG_EN
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
      Local0 = TMSK
      Local0 = Local0 | PD_PG_EN | TIME_CYCLE_CNT
      TMSK = Local0
      \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_NPU, NPU_RCSU_BASE_REG, BIT0)
    }
    Method(_OFF, 0, Serialized)
    {
      Local0 = TMSK
      Local0 = Local0 & ~PD_PG_EN
      TMSK = Local0
    }
  }

  Name(_PR0, Package(1){PPRS})
  Name(_PR3, Package(1){PPRS})

  Device(CRE0)
  {
    Name(_ADR, 0x00)

    PowerResource(PRS0, 0, 0)
    {
      OperationRegion(OPR0,SystemMemory,NPU_CORE0_PD_REG, 0x04)
      Field (OPR0, DWordAcc, NoLock, Preserve) {
        MSK0, 32,
      }
      Method(_STA, 0, Serialized)
      {
        Local0 = MSK0
        Local1 = MSK0
        Local0 = Local0 & PD_PG_EN
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
        Local0 = Local0 | PD_PG_EN | TIME_CYCLE_CNT
        MSK0 = Local0
        \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_NPU, NPU_RCSU_BASE_REG, BIT1)
      }
      Method(_OFF, 0, Serialized)
      {
        Local0 = MSK0
        Local0 = Local0 & ~PD_PG_EN
        MSK0 = Local0
      }
    }

    Name(_PR0, Package(2){PRS0, \_SB_.NPU0.PPRS})
    Name(_PR3, Package(2){PRS0, \_SB_.NPU0.PPRS})
  }
  Device(CRE1)
  {
    Name(_ADR, 0x01)

    PowerResource(PRS1, 0, 0)
    {
      OperationRegion(OPR1,SystemMemory,NPU_CORE1_PD_REG, 0x04)
      Field (OPR1, DWordAcc, NoLock, Preserve) {
        MSK1, 32,
      }
      Method(_STA, 0, Serialized)
      {
        Local0 = MSK1
        Local1 = MSK1
        Local0 = Local0 & PD_PG_EN
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
        Local0 = MSK1
        Local0 = Local0 | PD_PG_EN | TIME_CYCLE_CNT
        MSK1 = Local0
        \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_NPU, NPU_RCSU_BASE_REG, BIT2)
      }
      Method(_OFF, 0, Serialized)
      {
        Local0 = MSK1
        Local0 = Local0 & ~PD_PG_EN
        MSK1 = Local0
      }
    }

    Name(_PR0, Package(2){PRS1, \_SB_.NPU0.PPRS})
    Name(_PR3, Package(2){PRS1, \_SB_.NPU0.PPRS})
  }
  Device(CRE2)
  {
    Name(_ADR, 0x02)

    PowerResource(PRS2, 0, 0)
    {
      OperationRegion(OPR2,SystemMemory,NPU_CORE2_PD_REG, 0x04)
      Field (OPR2, DWordAcc, NoLock, Preserve) {
        MSK2, 32,
      }
      Method(_STA, 0, Serialized)
      {
        Local0 = MSK2
        Local1 = MSK2
        Local0 = Local0 & PD_PG_EN
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
        Local0 = MSK2
        Local0 = Local0 | PD_PG_EN | TIME_CYCLE_CNT
        MSK2 = Local0
        \_SB.DMRP(MEMORY_ENABLE, MEMR_GROUP_ID_NPU, NPU_RCSU_BASE_REG, BIT3)
      }
      Method(_OFF, 0, Serialized)
      {
        Local0 = MSK2
        Local0 = Local0 & ~PD_PG_EN
        MSK2 = Local0
      }
    }

    Name(_PR0, Package(2){PRS2, \_SB_.NPU0.PPRS})
    Name(_PR3, Package(2){PRS2, \_SB_.NPU0.PPRS})
  }
}