/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (UART) {
  Name (_HID, "ARMH0011")
  Name (_CID, "ARMH0011")
  Name (_UID, 0x0)
  Name (_STA, 0xF)

  OperationRegion(DBGR,SystemMemory,0x05040100,0x20)
  Field (DBGR, DWordAcc, NoLock, Preserve) {
    Offset (0x14),
    RTLC, 32,
  }

  Name (RES0, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      FixedPcdGet64(PcdSerialRegisterBase),
      0x1000
      )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, , , INTR) { FixedPcdGet64(PL011UartInterrupt) }
  })

  Method(_CRS,0,Serialized)
  {
    CreateDWordField(RES0, ^INTR._INT, LIRQ)
    If (LLess(RTLC, 21248)) {
      Store(313, LIRQ)
    }
    Return (RES0)
  }
}
