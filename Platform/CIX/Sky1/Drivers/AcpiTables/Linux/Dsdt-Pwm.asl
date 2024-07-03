/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device (PWM0) {
  Name (_HID, "CIXH2011")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      0x04110000,
      0x1000
      )
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_fch_pwm0", ResourceConsumer,)
  })
}

Device (PWM1) {
  Name (_HID, "CIXH2011")
  Name (_UID, 0x1)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (
      ReadWrite,
      0x04111000,
      0x1000
      )
    PinGroupFunction(Exclusive, 0x0, "\\_SB.MUX0", 0, "pinctrl_fch_pwm1", ResourceConsumer,)
  })
}
