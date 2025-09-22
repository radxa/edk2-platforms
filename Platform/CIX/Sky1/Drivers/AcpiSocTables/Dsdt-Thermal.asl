/** @file

  Copyright 2024-2025 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Include/AcpiScmi.h>

// External CPU device declarations
External (\_SB.CPB0, PkgObj)          //(CPU9, CPU10)
External (\_SB.CPB1, PkgObj)          //(CPU0, CPU11)
External (\_SB.CPM0, PkgObj)          //(CPU5, CPU6)
External (\_SB.CPM1, PkgObj)          //(CPU7, CPU8)

// Temperature conversion method (Celsius to Kelvin)
// Celsius to Kelvin conversion formula: K = 10 * C + 2732
Method(C2DK, 1, Serialized) {
  Multiply(Arg0, 10, Local0)
  Add(Local0, 2732, Local0)
  Return(Local0)
}

// Thermal Zone for CPU-B0
ThermalZone(TZB0) {
  Method(_PSV) { Return(3582) }       // Passive trip point: 85°C
  Method(_CRT) { Return(3712) }       // Critical trip point: 98°C
  Method(_TC1) { Return(4) }          // Thermal Constant1
  Method(_TC2) { Return(3) }          // Thermal Constant2
  Method(_TSP) { Return(3) }          // Sampling Period: in 100ms
  Method(_PSL) { Return(\_SB.CPB0)}   // Passive cooling list
  // Method(_TZD) {}                     // Thermal Zone Devices
  Method(_TMP, 0, Serialized) {       // Temperature reading
    Store(\_SB.PMMX.SENG(11, 0), Local0)
    CreateDWordField(Local0, 0x00, STAT)
    If (STAT == SCMI_SUCCESS) {
      CreateQWordField(Local0, 0x04, TEMP)
      TEMP = ToInteger(TEMP)
      Return(C2DK(TEMP))
    } Else {
      Return (0xFFFFFFFFFFFFFFFF)
    }
  }
  Method(_SCP, 1, Serialized) {}      // Set Cooling Policy
  Method(_TZP) { Return(50) }         // Polling Interval: in 100ms
  Name (_STR, Unicode ("CPU-B0"))
}

// Thermal Zone for CPU-B1
ThermalZone(TZB1) {
  Method(_PSV) { Return(3582) }       // Passive trip point: 85°C
  Method(_CRT) { Return(3712) }       // Critical trip point: 98°C
  Method(_TC1) { Return(4) }          // Thermal Constant1
  Method(_TC2) { Return(3) }          // Thermal Constant2
  Method(_TSP) { Return(3) }          // Sampling Period: in 100ms
  Method(_PSL) { Return(\_SB.CPB1) }  // Passive cooling list
  // Method(_TZD) {}                     // Thermal Zone Devices
  Method(_TMP, 0, Serialized) {       // Temperature reading
    Store(\_SB.PMMX.SENG(9, 0), Local0)
    CreateDWordField(Local0, 0x00, STAT)
    If (STAT == SCMI_SUCCESS) {
      CreateQWordField(Local0, 0x04, TEMP)
      TEMP = ToInteger(TEMP)
      Return(C2DK(TEMP))
    } Else {
      Return (0xFFFFFFFFFFFFFFFF)
    }
  }
  Method(_SCP, 1, Serialized) {}      // Set Cooling Policy
  Method(_TZP) { Return(50) }         // Polling Interval: in 100ms
  Name (_STR, Unicode ("CPU-B1"))
}

// Thermal Zone for CPU-M0
ThermalZone(TZM0) {
  Method(_PSV) { Return(3582) }       // Passive trip point: 85°C
  Method(_CRT) { Return(3712) }       // Critical trip point: 98°C
  Method(_TC1) { Return(4) }          // Thermal Constant1
  Method(_TC2) { Return(3) }          // Thermal Constant2
  Method(_TSP) { Return(3) }          // Sampling Period: in 100ms
  Method(_PSL) { Return(\_SB.CPM0) }  // Passive cooling list
  // Method(_TZD) {}                     // Thermal Zone Devices
  Method(_TMP, 0, Serialized) {       // Temperature reading
    Store(\_SB.PMMX.SENG(10, 0), Local0)
    CreateDWordField(Local0, 0x00, STAT)
    If (STAT == SCMI_SUCCESS) {
      CreateQWordField(Local0, 0x04, TEMP)
      TEMP = ToInteger(TEMP)
      Return(C2DK(TEMP))
    } Else {
      Return (0xFFFFFFFFFFFFFFFF)
    }
  }
  Method(_SCP, 1, Serialized) {}      // Set Cooling Policy
  Method(_TZP) { Return(50) }         // Polling Interval: in 100ms
  Name (_STR, Unicode ("CPU-M0"))
}

// Thermal Zone for CPU-M1
ThermalZone(TZM1) {
  Method(_PSV) { Return(3582) }       // Passive trip point: 85°C
  Method(_CRT) { Return(3712) }       // Critical trip point: 98°C
  Method(_TC1) { Return(4) }          // Thermal Constant1
  Method(_TC2) { Return(3) }          // Thermal Constant2
  Method(_TSP) { Return(3) }          // Sampling Period: in 100ms
  Method(_PSL) { Return(\_SB.CPM1) }  // Passive cooling list
  // Method(_TZD) {}                     // Thermal Zone Devices
  Method(_TMP, 0, Serialized) {       // Temperature reading
    Store(\_SB.PMMX.SENG(8, 0), Local0)
    CreateDWordField(Local0, 0x00, STAT)
    If (STAT == SCMI_SUCCESS) {
      CreateQWordField(Local0, 0x04, TEMP)
      TEMP = ToInteger(TEMP)
      Return(C2DK(TEMP))
    } Else {
      Return (0xFFFFFFFFFFFFFFFF)
    }
  }
  Method(_SCP, 1, Serialized) {}      // Set Cooling Policy
  Method(_TZP) { Return(50) }         // Polling Interval: in 100ms
  Name (_STR, Unicode ("CPU-M1"))
}

//
// Name: SPFA [Set PM Fan to Auto Mode]
// Description: Function to set PM fan to auto mode
// Input: None
// Output: None
//
Method(SPFA, 0, Serialized){
  \_SB.PMMX.SFMD(1)
}

//
// Name: SPFM [Set PM Fan to Mute Mode]
// Description: Function to set PM fan to mute mode
// Input: None
// Output: None
//
Method(SPFM, 0, Serialized){
  \_SB.PMMX.SFMD(0)
}

//
// Name: SPFP [Set PM Fan to Performance Mode]
// Description: Function to set PM fan to performance mode
// Input: None
// Output: None
//
Method(SPFP, 0, Serialized){
  \_SB.PMMX.SFMD(2)
}