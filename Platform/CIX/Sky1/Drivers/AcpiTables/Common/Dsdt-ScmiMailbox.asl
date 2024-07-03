/** @file
 *
 *  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Include/AcpiScmi.h>

#define MAILBOX_SCMI_BEGIN                  \
      Acquire(MBXM, 0xFFFF)                 \
      CERR = 0                              \
      If(CFRE==0)                           \
      {                                     \
        Local0 = 400                        \
        While( Local0 >0 ){                 \
          if(CFRE == 1) {                   \
            Break                           \
          }                                 \
          Sleep(1)                          \
          Local0--                          \
        }                                   \
        if(Local0 == 0){                    \
          Release(MBXM)                     \
          Return(Buffer(){ACPI_SCMI_BUSY})  \
        }                                   \
      }                                     \
      SIGN = 0x50434303                     \
      FLAG = 0x00                           \

#define MAILBOX_SCMI_PROCESS                \
    MSGP = BUFF                             \
    CFRE = 0                                \
    BEEL = 1                                \
    Local0 = 400                            \
    While( Local0 >0 ){                     \
        if(CFRE == 1) {                     \
            Break                           \
        }                                   \
        Sleep(1)                            \
        Local0--                            \
    }                                       \
    if(Local0 == 0){                        \
        Release(MBXM)                       \
        Return(Buffer(){ACPI_SCMI_TIMEOUT}) \
    }                                       \
    BUFF = MSGP                             \
    Release(MBXM)                           \

Mutex(MBXM,0)

Device(PMMX){
  Name(_HID,"CIXHA000")

  OperationRegion(MBXO,SystemMemory,0x06590000,0xA0)
  Field (MBXO, DWordAcc, NoLock, Preserve) {
    Offset (0x04),
    CFRE, 1,
    CERR, 1,
    Offset (0x0C),
    SIGN, 32,
    FLAG, 32,
    LENG, 32,
    MSID, 8,
    MSTP, 2,
    PRID, 8,
    TOKN, 10,
    ,4,
    Offset (0x1C),
    MSGP, 256,
    Offset (0x80),
    BEEL, 1,
  }

  Method(_STA)
  {
    Return(0x03)
  }

  Name(BUFF, Buffer(32) {})
  CreateDWordField(BUFF, 0x00, DAT0)
  CreateDWordField(BUFF, 0x04, DAT1)
  CreateDWordField(BUFF, 0x08, DAT2)
  CreateDWordField(BUFF, 0x0c, DAT3)

  //Set power state,Arg0=power domain id, Arg1=Flag
  Method(PRSS,2,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    DAT0 = 0x0
    DAT1 = Arg0
    DAT2 = Arg1
    //Length
    LENG = 0x10
    //Message Header
    MSID = SCMI_PD_POWER_STATE_SET
    PRID = SCMI_PROTOCOL_ID_POWER_DOMAIN
    //Process Request
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }

  //Get power domain state,Arg0=power domain id
  Method(PRSG,1,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    LENG = 0x08
    DAT0 = Arg0
    MSID = SCMI_PD_POWER_STATE_GET
    PRID = SCMI_PROTOCOL_ID_POWER_DOMAIN
    //Process Requess
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }

  //Get Clock Rate, Arg0=ClockId
  Method(CLKG,1,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    DAT0 = Arg0
    //Length
    LENG = 0x8
    //Message Header
    MSID = SCMI_CLOCK_RATE_GET
    PRID = SCMI_PROTOCOL_ID_CLOCK
    //Process Requess
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }

  //Set Clock Rate, Arg0=ClockId, Arg1=Clock Low Word, Arg2=Clock High Word.
  Method(CLKS,3,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    DAT0 = 0
    DAT1 = Arg0
    DAT2 = Arg1
    DAT3 = Arg2
    //Length
    LENG = 0x14
    //Message Header
    MSID = SCMI_CLOCK_RATE_SET
    PRID = SCMI_PROTOCOL_ID_CLOCK
    //Process Request
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }

  //Clock Describe Rates, Arg0=ClockId, Arg1=Rate Index
  Method(CLKD,2,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    DAT0 = Arg0
    DAT1 = Arg1
    //Length
    LENG = 0x0c
    //Message Header
    MSID = SCMI_CLOCK_DESCRIBE_RATES
    PRID = SCMI_PROTOCOL_ID_CLOCK
    //Process Request
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }


  //Clock Device configure, Arg0=ClockId, Arg1=attributes
  Method(CLKC,2,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    Store(Arg0, DAT0)
    Store(Arg1, DAT1)
    //Length
    Store(0x0C, LENG)
    //Message Header
    Store(SCMI_CLOCK_CONFIG_SET, MSID)
    Store(SCMI_PROTOCOL_ID_CLOCK, PRID)
    //Process Request
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }

  /* System Control and Management Interface (SCMI) Sensor Protocol*/

  /**
   * SENG - Read scalar sensor value
   * @Arg0: sensor ID
   * @Arg1: Asynchronous sensor read support
   *
   * Return: the buffer received from PM
   */
  Method(SENG,2,Serialized){
    MAILBOX_SCMI_BEGIN
    //Message Payload
    Store(Arg0, DAT0)
    Store(Arg1, DAT1)
    //Length
    Store(0x0C, LENG)
    //Message Header
    Store(SCMI_SENSOR_READING_GET, MSID)
    Store(SCMI_PROTOCOL_ID_SENSOR, PRID)
    //Process Request
    MAILBOX_SCMI_PROCESS
    //Process response
    Return(BUFF)
  }
}
