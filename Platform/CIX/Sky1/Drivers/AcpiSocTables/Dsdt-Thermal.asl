/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
Name(TEMP,3000)

Method(C2DK, 1, Serialized) {
  Multiply(Arg0,10,Local0)
  Add(Local0,2732,Local0)
  Return(Local0)
}

/*
 * This Thermal Zone apply for all cpu devices.
 * Passive trip point is 85C.
 * K = 85 * 10 + 2732 = 3582.
 * PVT sensor number is 11.
 */
ThermalZone(TZ00) {
  Method(_PSV) { Return(3582) } //Passive 85C
  Method(_TC1) { Return(4) } //Thermal Constant1
  Method(_TC2) { Return(3) } //Thermal Constant2
  Method(_TSP) { Return(200) } //Thermal Sampling Period
  Method(_PSL) { Return( Package(){\_SB.CPU0}) } //Passive List
  Method(_TZD) {} //Thermal Zone Devices  */
  Method(_TMP, 0, Serialized) {
    Store(\_SB.PMMX.SENG(11, 0),Local0)
    CreateDWordField(Local0, 0x00, STAT)
    CreateQWordField(Local0, 0x04, TEMP)
    TEMP = ToInteger(TEMP)
    Return( C2DK(TEMP) )
  }
  Method(_SCP, 1, Serialized) {} //Set Cooling Policy
  Method(_TZP) { Return(300) } //Thermal Zone Polling
}
