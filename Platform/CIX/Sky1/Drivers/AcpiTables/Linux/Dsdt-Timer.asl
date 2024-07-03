/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (TMR0) {
  Name (_HID, "CIXH1007")
  Name (_CID, "CIXH1007")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      0x04116000,
      0x2000
      )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 502 }
  })
}
