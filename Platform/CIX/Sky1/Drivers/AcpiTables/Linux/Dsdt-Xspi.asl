/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (XSPI) {
  Name (_HID, "CIXH2002")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, XSPI_BASE, XSPI_SIZE)
    Memory32Fixed (ReadWrite, XSPI_MMIO_BASE, XSPI_MMIO_SIZE)
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { FCH_INTR_XSPI_INTERRUPT_ID }
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX1", 0, "pinctrl_fch_xspi", ResourceConsumer,)
  })
}
