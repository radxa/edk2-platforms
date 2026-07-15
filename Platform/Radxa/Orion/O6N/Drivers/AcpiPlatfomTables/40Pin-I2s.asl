/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External (\_SB.I2S2, DeviceObj)

Name (M98S, 0)  // max98357a STA, change to 1 to enable the codec

// Pin 12 I2S4_SCLK     -> BCLK
// Pin 35 I2S4_TWS      -> LRC
// Pin 40 I2S4_DATA_OUT -> DIN

Device (MAXM) {
  Name (_HID, "MX98357A")
  Name (_DDN, "Maxim Integrated 98357A Amplifier")
  Name (_UID, 0)
  Name (_CCA, Zero)
  Method (_STA, 0x0, Serialized) {
    if (M98S == 0x1) {
      Return (0xB)
    } else {
      Return (0x0)
    }
  }
}
