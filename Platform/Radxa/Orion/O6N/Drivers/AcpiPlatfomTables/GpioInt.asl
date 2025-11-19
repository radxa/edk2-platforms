/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Gpio.h"

External (\_SB.GPI1, DeviceObj)
External (\_SB.GPI4, DeviceObj)

/*
Scope (\_SB.GPI1)
{
  Name (_AEI, ResourceTemplate() {
    GpioInt (Level, ActiveLow, Exclusive, PullUp, , "\\_SB.GPI1") { 9 } // GPIO084
  })
}
*/

Scope (\_SB.GPI4)
{
  Name (_AEI, ResourceTemplate() {
    GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI4") { 10 } // GPIO011
/*
    GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI4") { 19 } // GPIO030
    GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI4") { 20 } // GPIO031
    GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI4") { 21 } // GPIO032
    GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI4") { 22 } // GPIO033
*/
  })

  Method (_L0A) {
    \_SB.ERTC.CAFG()
  }
}
