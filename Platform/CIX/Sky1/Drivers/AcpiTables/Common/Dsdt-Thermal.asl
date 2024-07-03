/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Name(FNS0,0)
PowerResource(FN00, 0, 0)
{
  Method(_STA, 0, Serialized)
  {
      Return(FNS0)
  }
  Method(_ON, 0, Serialized)
  {
    printf ("CIX Debug: Fan on")
    Store(1,FNS0)
  }
  Method(_OFF, 0, Serialized)
  {
    printf ("CIX Debug: Fan off")
    Store(0,FNS0)
  }
}
Name(TEMP,3000)
Device(FAN0)
{
  Name(_HID, EISAID("PNP0C0B"))
  Name(_UID,0)
  Name(_PR0, Package(1){FN00})
}

Method(C2DK, 1, Serialized) {
  Multiply(Arg0,10,Local0)
  Add(Local0,2732,Local0)
  Return(Local0)
}

// TODO:Active cooling
ThermalZone(TZ00) {
  Method(_CRT) { Return(3880) } //Critical TemperaturM
  Method(_HOT) { Return(3860) } //Hot Temperature
  Method(_PSV) { Return(3500) } //Passive
  Method(_TC1) { Return(4) } //Thermal Constant1
  Method(_TC2) { Return(3) } //Thermal Constant2
  Method(_TSP) { Return(200) } //Thermal Sampling Period
  Method(_PSL) { Return( Package(){\_SB.CPU0}) } //Passive List
  /*Method(_AC0) {} //Active Cooling
  Method(_AL0) {} //Active List
  Method(_TZD) {} //Thermal Zone Devices  */
  Method(_TMP, 0, Serialized) {
    Store(\_SB.PMMX.SENG(0, 0),Local0)
    CreateDWordField(Local0, 0x00, STAT)
    CreateQWordField(Local0, 0x04, TEMP)
    TEMP = ToInteger(TEMP)
    Return( C2DK(TEMP) )
  }
  Method(_SCP, 1, Serialized) {} //Set Cooling Policy
  Method(_TZP) { Return(300) } //Thermal Zone Polling
}

ThermalZone(TZ01){

  Method(_CRT) { Return(3880) }//114.85
  Name(_TZP, 300)

  Method(_AC0) { Return (3250)} //51.85
  Name(_AL0, Package(){FAN0})

  Method(_PSV) { Return(3500) }//76.85
  Method(_TC1) { Return (4) }
  Method(_TC2) { Return (3) }
  Method(_TSP) { Return (100) }
//  Method(_PSL, 0, Serialized){ Return( Package(){\_SB.CPU0} )}
  Method(_SCP, 1, Serialized)
  {
    If(LEqual(Arg0,0)){
      printf ("CIX Debug: Active cooling")
    }ElseIf(LEqual(Arg0,1)){
      printf ("CIX Debug: Passive cooling")
    }
  }

  Method(_TMP, 0, Serialized)
  {
    Add(TEMP,20,TEMP)
    printf ("CIX Debug: Enter _TMP(%o)",TEMP)
    Return(TEMP)
  }
}
