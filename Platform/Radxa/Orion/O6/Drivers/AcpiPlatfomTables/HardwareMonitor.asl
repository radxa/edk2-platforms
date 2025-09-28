/** @file

  Copyright 2025 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Device(HWMN){
    Name(_UID, 0)
    Name(_HID, "CIXHA024")

    Method(_STA)
    {
      Return(0x03)
    }

    //
    // Name: SFAT [Set Fan to Auto Mode]
    // Description: Function to set fan to auto mode
    // Input: None
    // Output: None
    //
    Method(SFAT, 0, Serialized){
        // Set the fan auto mode through EC
        \_SB.EC0.SFAT()
    }

    //
    // Name: SFMT [Set Fan to Mute Mode]
    // Description: Function to set fan to mute mode
    // Input: None
    // Output: None
    //
    Method(SFMT, 0, Serialized){
        // Set the fan mute mode through EC
        \_SB.EC0.SFMT()
    }

    //
    // Name: SFPF [Set Fan to Performance Mode]
    // Description: Function to set fan to performance mode
    // Input: None
    // Output: None
    //
    Method(SFPF, 0, Serialized){
        // Set the fan performance mode through EC
        \_SB.EC0.SFPF()
    }
}
