/** @file

  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

External (\_SB.GPI5, DeviceObj)

// Based on Microsoft documentation
// https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/acpi-button-device

Device(BTNS)
{
    Name(_HID, "ACPI0011")
    Name(_CRS, ResourceTemplate() {
      GpioInt (Level, ActiveLow, ExclusiveAndWake, PullUp, , "\\_SB.GPI5") { 1 } // GPIO016
    })
    Name(_DSD, Package(2) {
        //UUID for HID Button Descriptors:
        ToUUID("FA6BD625-9CE8-470D-A2C7-B3CA36C4282E"),
        //Data structure for this UUID:
        Package(2) {
            Package(5) {
                0,    // This is a Collection
                1,    // Unique ID for this Collection
                0,    // This is a Top-Level Collection
                0x01, // Usage Page ("Generic Desktop Page")
                0x0D  // Usage ("Portable Device Control")
            },
            Package(5) {
                1,    // This is a Control
                0,    // Interrupt index in _CRS for Power Button
                1,    // Unique ID of Parent Collection
                0x01, // Usage Page ("Generic Desktop Page")
                0x81  // Usage ("System Power Down")
            }
        }
    })
}
