/** @file

  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External (\_SB.I2C7, DeviceObj)

Scope (\_SB.I2C7)
{
  // Onboard EEPROM
  Device (EEP0) {
    Name (_HID, "PRP0001")
    Name (_UID, 0x21)
    Name (_STA, 0x0B)
    Name (_CCA, 0)
    Name (_CRS, ResourceTemplate () {
      I2cSerialBusV2 (0x50,
                      ControllerInitiated,
                      400000,
                      AddressingMode7Bit,
                      "\\_SB.I2C7",
                      0x0,
                      ResourceConsumer,
                      ,
                      Exclusive
                      ,)
    })
    Name (_DSD, Package () {
      ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () { "compatible", Package() { "atmel,24c16" } },
        Package () { "reg", 0x50 },
        Package () { "pagesize", 16 },
      }
    })
  }
}
