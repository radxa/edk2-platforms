/**
#
#  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#
#**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/TimeBaseLib.h>
#include <Protocol/I2cMaster.h>
#include <Library/BaseMemoryLib.h>

#define IS_VALID_BCD(Value) \
  (((Value) < 0xA0) && (((Value) & 0x0F) < 0x0A))

#define SLAVE_ADDRESS  (FixedPcdGet8 (PcdI2cSlaveAddress))

#define HYM8563_CONTROL1_REG_OFFSET         0x0
#define HYM8563_CONTROL2_REG_OFFSET         0x1
#define HYM8563_SECOND_REG_OFFSET           0x2
#define HYM8563_MINUTE_REG_OFFSET           0x3
#define HYM8563_HOUR_REG_OFFSET             0x4
#define HYM8563_DAY_REG_OFFSET              0x5
#define HYM8563_WEEKDAY_REG_OFFSET          0x6
#define HYM8563_MONTH_REG_OFFSET            0x7
#define HYM8563_YEAR_REG_OFFSET             0x8
#define HYM8563_ALARM_MINUTE_REG_OFFSET     0x9
#define HYM8563_ALARM_HOUR_REG_OFFSET       0xa
#define HYM8563_ALARM_DAY_REG_OFFSET        0xb
#define HYM8563_ALARM_WEEKDAY_REG_OFFSET    0xc
#define HYM8563_CLKOUT_REG_OFFSET           0xd
#define HYM8563_TIMER_CONTROL_REG_OFFSET    0xe
#define HYM8563_TIMER_COUNTDOWN_REG_OFFSET  0xf

#define HYM8563_CLOCK_INVALID  0x80
#define HYM8563_SECONDS_MASK   0x7f
#define HYM8563_MINUTES_MASK   0x7f
#define HYM8563_HOURS_MASK     0x3f
#define HYM8563_DAYS_MASK      0x3f
#define HYM8563_WEEKDAYS_MASK  0x07
#define HYM8563_MONTHS_MASK    0x1f
#define HYM8563_CENTURY_MASK   0x80

/* Each alarm check can be disabled by setting this bit in the register */
#define HYM8563_ALM_BIT_DISABLE  BIT7
#define HYM8563_CTL2_AIE         BIT1
#define HYM8563_CTL2_AF		       BIT3

//
// The HYM8563 has a 'century' flag, which means it could theoretically span
// 200 years. However, it treats all years divisible by 4 as leap years,
// including the years 1900 and 2100 (which are not leap years), so the only
// sane base year is 2000 (which was a leap year).
//
#define EPOCH_BASE  2000

STATIC EFI_HANDLE               mI2cMasterHandle;
STATIC VOID                     *mI2cMasterEventRegistration;
STATIC EFI_I2C_MASTER_PROTOCOL  *mI2cMaster;
STATIC EFI_EVENT                mRtcVirtualAddrChangeEvent;

#pragma pack(1)
typedef struct {
  UINT8    VL_seconds;
  UINT8    Minutes;
  UINT8    Hours;
  UINT8    Days;
  UINT8    Weekdays;
  UINT8    Century_months;
  UINT8    Years;
} RTC_DATETIME;

typedef struct {
  UINT8           Reg;
  RTC_DATETIME    DateTime;
} RTC_SET_DATETIME_PACKET;
#pragma pack()

typedef struct {
  UINTN                OperationCount;
  EFI_I2C_OPERATION    SetAddressOp;
  EFI_I2C_OPERATION    GetDateTimeOp;
} RTC_GET_I2C_REQUEST;

typedef EFI_I2C_REQUEST_PACKET RTC_SET_I2C_REQUEST;

/**
  Returns the current time and date information, and the time-keeping
  capabilities of the hardware platform.

  @param  Time                  A pointer to storage to receive a snapshot of
                                the current time.
  @param  Capabilities          An optional pointer to a buffer to receive the
                                real time clock device's capabilities.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The time could not be retrieved due to hardware
                                error.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME               *Time,
  OUT EFI_TIME_CAPABILITIES  *Capabilities
  )
{
  RTC_GET_I2C_REQUEST        Op;
  RTC_DATETIME               DateTime;
  EFI_STATUS                 Status;
  UINT8                      Reg;
  INT16                      TimeZone;
  UINT8                      Daylight;

  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TimeZone = Time->TimeZone;
  Daylight = Time->Daylight;

  // Clear Time
  // In other word, Init Time
  if (IsZeroBuffer (ZeroMem (Time, sizeof (EFI_TIME)), sizeof (EFI_TIME)) == FALSE) {
    DEBUG ((DEBUG_INFO, "Time can't reset\n"));
  }
  Time->TimeZone = TimeZone;
  Time->Daylight = Daylight;

  if (mI2cMaster == NULL) {
    return EFI_DEVICE_ERROR;
  }

  Reg = HYM8563_SECOND_REG_OFFSET;

  Op.OperationCount = 2;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Reg;

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = sizeof (RTC_DATETIME);
  Op.GetDateTimeOp.Buffer        = (VOID *)&DateTime;

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Time->Nanosecond = 0;

  if ((DateTime.VL_seconds & HYM8563_CLOCK_INVALID) != 0) {
    Time->Second = 0;
    Time->Minute = 0;
    Time->Hour   = 0;
    Time->Day    = 1;
    Time->Month  = 1;
    Time->Year   = EPOCH_BASE;
  } else {
    Time->Second = BcdToDecimal8 (DateTime.VL_seconds & HYM8563_SECONDS_MASK);
    Time->Minute = BcdToDecimal8 (DateTime.Minutes & HYM8563_MINUTES_MASK);
    Time->Hour   = BcdToDecimal8 (DateTime.Hours & HYM8563_HOURS_MASK);
    Time->Day    = BcdToDecimal8 (DateTime.Days & HYM8563_DAYS_MASK);
    Time->Month  = BcdToDecimal8 (DateTime.Century_months & HYM8563_MONTHS_MASK);
    Time->Year   = BcdToDecimal8 (DateTime.Years) + EPOCH_BASE;

    if (DateTime.Century_months & HYM8563_CENTURY_MASK) {
      Time->Year += 100;
    }
  }

  if (Capabilities != NULL) {
    Capabilities->Resolution = 1;
    Capabilities->Accuracy   = 0;
    Capabilities->SetsToZero = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Sets the current local time and date information.

  @param  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware
                                error.

**/
EFI_STATUS
EFIAPI
LibSetTime (
  IN EFI_TIME  *Time
  )
{
  RTC_SET_I2C_REQUEST      Op;
  RTC_SET_DATETIME_PACKET  Packet;
  EFI_STATUS               Status;

  if(Time == NULL || !IsTimeValid(Time))
    return EFI_INVALID_PARAMETER;

  if (mI2cMaster == NULL) {
    return EFI_DEVICE_ERROR;
  }

  Packet.DateTime.VL_seconds     = DecimalToBcd8 (Time->Second);
  Packet.DateTime.Minutes        = DecimalToBcd8 (Time->Minute);
  Packet.DateTime.Hours          = DecimalToBcd8 (Time->Hour);
  Packet.DateTime.Days           = DecimalToBcd8 (Time->Day);
  Packet.DateTime.Weekdays       = 0;
  Packet.DateTime.Century_months = DecimalToBcd8 (Time->Month);
  Packet.DateTime.Years          = DecimalToBcd8 (Time->Year % 100);
  if (Time->Year >= EPOCH_BASE + 100) {
    if (Time->Year >= EPOCH_BASE + 200) {
      return EFI_DEVICE_ERROR;
    }

    Packet.DateTime.Century_months |= HYM8563_CENTURY_MASK;
  }

  Packet.Reg = HYM8563_SECOND_REG_OFFSET;

  Op.OperationCount             = 1;
  Op.Operation[0].Flags         = 0;
  Op.Operation[0].LengthInBytes = sizeof (RTC_SET_DATETIME_PACKET);
  Op.Operation[0].Buffer        = (VOID *)&Packet;

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Time->Nanosecond = 0;

  return EFI_SUCCESS;
}

/**
  Returns the current wakeup alarm clock setting.

  @param  Enabled               Indicates if the alarm is currently enabled or
                                disabled.
  @param  Pending               Indicates if the alarm signal is pending and
                                requires acknowledgement.
  @param  Time                  The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a
                                hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this
                                platform.

**/
EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN   *Enabled,
  OUT BOOLEAN   *Pending,
  OUT EFI_TIME  *Time
  )
{
  EFI_STATUS           Status;
  RTC_GET_I2C_REQUEST  Op;
  UINT8                Buffer[4];
  UINT8                Reg;

  if ((Time == NULL) || (Pending == NULL) || (Enabled == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // if Enabled == TRUE Or Pending == TRUE when AIE Or AF is not setting,
  // Getwakuptime will return Enabled == TRUE Or Pending == TRUE.
  // It not correctly.
  *Enabled = 0;
  *Pending = 0;

  if (IsZeroBuffer (ZeroMem (Time, sizeof (EFI_TIME)), sizeof (EFI_TIME)) == FALSE) {
    return EFI_UNSUPPORTED;
  }
  LibGetTime(Time, NULL);

  Reg = HYM8563_ALARM_MINUTE_REG_OFFSET;

  Op.OperationCount = 2;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Reg;

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = sizeof (Buffer);
  Op.GetDateTimeOp.Buffer        = (VOID *)Buffer;

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IS_VALID_BCD(Buffer[0] & HYM8563_MINUTES_MASK) && IS_VALID_BCD(Buffer[1] & HYM8563_HOURS_MASK) 
        && IS_VALID_BCD(Buffer[2] & HYM8563_DAYS_MASK)){
    /* The alarm only has a minute accuracy */
    Time->Second = 0;
    Time->Minute = (Buffer[0] & HYM8563_ALM_BIT_DISABLE) ? -1 : BcdToDecimal8 (Buffer[0] & HYM8563_MINUTES_MASK);
    Time->Hour   = (Buffer[1] & HYM8563_ALM_BIT_DISABLE) ? -1 : BcdToDecimal8 (Buffer[1] & HYM8563_HOURS_MASK);
    Time->Day    = (Buffer[2] & HYM8563_ALM_BIT_DISABLE) ? -1 : BcdToDecimal8 (Buffer[2] & HYM8563_DAYS_MASK);
  }else{
    Time->Second = 0;
    Time->Minute = 0;
    Time->Hour   = 0;
    Time->Day    = 1;
  }

  // Patch
  if(!IsTimeValid(Time)){
    Time->Second = 0;
    Time->Minute = 0;
    Time->Hour   = 0;
    Time->Day    = 1;
  }

  Op.OperationCount = 2;
  Reg               = HYM8563_CONTROL2_REG_OFFSET;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Reg;

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = 1;
  Op.GetDateTimeOp.Buffer        = (VOID *)Buffer;

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Buffer[0] & HYM8563_CTL2_AIE) {
    *Enabled = 1;
  }

  // Pending is setting when AF is setting.
  if (Buffer[0] & HYM8563_CTL2_AF) {
    *Pending = 1;
  }

  return EFI_SUCCESS;
}

/**
  Sets the system wakeup alarm clock time.

  @param  Enabled               Enable or disable the wakeup alarm.
  @param  Time                  If Enable is TRUE, the time to set the wakeup
                                alarm for.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was
                                enabled. If Enable is FALSE, then the wakeup
                                alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a
                                hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this
                                platform.

**/
EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN BOOLEAN    Enabled,
  OUT EFI_TIME  *Time
  )
{
  EFI_STATUS           Status;
  RTC_GET_I2C_REQUEST  Op;
  RTC_SET_I2C_REQUEST  SetOp;
  UINT8                Buffer[4];
  UINT8                Day;
  UINT8                Hour;
  UINT8                Minute;

  if(Enabled == FALSE)
    goto SetRTCAlarmFLAG;

  if(Time == NULL || !IsTimeValid(Time))
    return EFI_INVALID_PARAMETER;

  Day    = Time->Day;
  Hour   = Time->Hour;
  Minute = Time->Minute;
  if (IsZeroBuffer (ZeroMem (Time, sizeof (EFI_TIME)), sizeof (EFI_TIME)) == FALSE) {
    return EFI_UNSUPPORTED;
  }
  LibGetTime(Time, NULL);
  Time->Day    = Day;
  Time->Hour   = Hour;
  Time->Minute = Minute;
  Time->Second = 0;

  Op.OperationCount = 2;
  Buffer[0]         = HYM8563_CONTROL2_REG_OFFSET;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Buffer[0];

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = 1;
  Op.GetDateTimeOp.Buffer        = (VOID *)&Buffer[1];

  Status = mI2cMaster->StartRequest (
                                    mI2cMaster,
                                    SLAVE_ADDRESS,
                                    (VOID *)&Op,
                                    NULL,
                                    NULL
                                    );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Buffer[1] &= ~HYM8563_CTL2_AIE;

  SetOp.OperationCount             = 1;
  SetOp.Operation[0].Flags         = 0;
  SetOp.Operation[0].LengthInBytes = 2;
  SetOp.Operation[0].Buffer        = (VOID *)&Buffer[0];

  Status = mI2cMaster->StartRequest (
                                    mI2cMaster,
                                    SLAVE_ADDRESS,
                                    (VOID *)&SetOp,
                                    NULL,
                                    NULL
                                    );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Enabled != FALSE) {
    if ((Time == NULL) || !IsTimeValid (Time)) {
      return EFI_INVALID_PARAMETER;
    }

    Buffer[0] = HYM8563_ALARM_MINUTE_REG_OFFSET;
    Buffer[1] = DecimalToBcd8 (Time->Minute);
    Buffer[2] = DecimalToBcd8 (Time->Hour);
    Buffer[3] = DecimalToBcd8 (Time->Day);

    DEBUG ((DEBUG_INFO, "%a: alarm at day %x %x:%x\n", __FUNCTION__, Buffer[3], Buffer[2], Buffer[1]));
    SetOp.OperationCount             = 1;
    SetOp.Operation[0].Flags         = 0;
    SetOp.Operation[0].LengthInBytes = 4;
    SetOp.Operation[0].Buffer        = (VOID *)&Buffer[0];

    Status = mI2cMaster->StartRequest (
                                       mI2cMaster,
                                       SLAVE_ADDRESS,
                                       (VOID *)&SetOp,
                                       NULL,
                                       NULL
                                       );
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }

SetRTCAlarmFLAG:
  Op.OperationCount = 2;
  Buffer[0]         = HYM8563_CONTROL2_REG_OFFSET;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Buffer[0];

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = 1;
  Op.GetDateTimeOp.Buffer        = (VOID *)&Buffer[1];

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (!EFI_ERROR (Status)) {
    if (Enabled) {
      Buffer[1] |= HYM8563_CTL2_AIE;
    } else {
      Buffer[1] &= ~HYM8563_CTL2_AIE;
    }

    SetOp.OperationCount             = 1;
    SetOp.Operation[0].Flags         = 0;
    SetOp.Operation[0].LengthInBytes = 2;
    SetOp.Operation[0].Buffer        = (VOID *)&Buffer[0];

    Status = mI2cMaster->StartRequest (
                                       mI2cMaster,
                                       SLAVE_ADDRESS,
                                       (VOID *)&SetOp,
                                       NULL,
                                       NULL
                                       );
  }

  return Status;
}

STATIC
VOID
I2cMasterRegistrationEvent (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_HANDLE               Handle;
  UINTN                    BufferSize;
  EFI_STATUS               Status;
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster;
  UINTN                    BusFrequency;

  //
  // Try to connect the newly registered driver to our handle.
  //
  do {
    BufferSize = sizeof (EFI_HANDLE);
    Status     = gBS->LocateHandle (
                                    ByRegisterNotify,
                                    &gEfiI2cMasterProtocolGuid,
                                    mI2cMasterEventRegistration,
                                    &BufferSize,
                                    &Handle
                                    );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG (
               (DEBUG_WARN, "%a: gBS->LocateHandle () returned %r\n",
                __FUNCTION__, Status)
               );
      }

      break;
    }

    if (Handle != mI2cMasterHandle) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: found I2C master!\n", __FUNCTION__));

    gBS->CloseEvent (Event);

    Status = gBS->OpenProtocol (
                                mI2cMasterHandle,
                                &gEfiI2cMasterProtocolGuid,
                                (VOID **)&I2cMaster,
                                gImageHandle,
                                NULL,
                                EFI_OPEN_PROTOCOL_EXCLUSIVE
                                );
    ASSERT_EFI_ERROR (Status);

    Status = I2cMaster->Reset (I2cMaster);
    if (EFI_ERROR (Status)) {
      DEBUG (
             (DEBUG_ERROR, "%a: I2CMaster->Reset () failed - %r\n",
              __FUNCTION__, Status)
             );
      break;
    }

    BusFrequency = FixedPcdGet16 (PcdI2cBusFrequency);
    Status       = I2cMaster->SetBusFrequency (I2cMaster, &BusFrequency);
    if (EFI_ERROR (Status)) {
      DEBUG (
             (DEBUG_ERROR, "%a: I2CMaster->SetBusFrequency () failed - %r\n",
              __FUNCTION__, Status)
             );
      break;
    }

    mI2cMaster = I2cMaster;
    break;
  } while (TRUE);
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
VOID
EFIAPI
LibRtcVirtualNotifyEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EfiConvertPointer (0x0, (VOID **)&mI2cMaster);
}

STATIC
EFI_STATUS
RtcDeviceInitialize (
  IN  VOID
  )
{
  EFI_STATUS           Status;
  RTC_GET_I2C_REQUEST  Op;
  RTC_SET_I2C_REQUEST  SetOp;
  UINT8                Buffer[4];

  Op.OperationCount = 2;
  Buffer[0]         = HYM8563_CONTROL2_REG_OFFSET;

  Op.SetAddressOp.Flags         = 0;
  Op.SetAddressOp.LengthInBytes = 1;
  Op.SetAddressOp.Buffer        = &Buffer[0];

  Op.GetDateTimeOp.Flags         = I2C_FLAG_READ;
  Op.GetDateTimeOp.LengthInBytes = 1;
  Op.GetDateTimeOp.Buffer        = (VOID *)&Buffer[1];

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&Op,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Buffer[1] &= ~HYM8563_CTL2_AIE;
  Buffer[1] &= ~HYM8563_CTL2_AF;

  SetOp.OperationCount             = 1;
  SetOp.Operation[0].Flags         = 0;
  SetOp.Operation[0].LengthInBytes = 2;
  SetOp.Operation[0].Buffer        = (VOID *)&Buffer[0];

  Status = mI2cMaster->StartRequest (
                                     mI2cMaster,
                                     SLAVE_ADDRESS,
                                     (VOID *)&SetOp,
                                     NULL,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

/**
  Library entry point

  @param  ImageHandle           Handle that identifies the loaded image.
  @param  SystemTable           System Table for this image.

  @retval EFI_SUCCESS           The operation completed successfully.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;
  EFI_TIME    InitTime = {2011, 1, 1 , 0, 0, 0, 0, 0, 0, 0, 0};
  EFI_TIME    GetTime;

  //
  // Find the handle that marks the controller
  // that will provide the I2C master protocol.
  //
  BufferSize = sizeof (EFI_HANDLE);
  Status     = gBS->LocateHandle (
                                  ByProtocol,
                                  &gHym8563RealTimeClockLibI2cMasterProtocolGuid,
                                  NULL,
                                  &BufferSize,
                                  &mI2cMasterHandle
                                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register a protocol registration notification callback on the I2C master
  // protocol. This will notify us even if the protocol instance we are looking
  // for has already been installed.
  //
  EfiCreateProtocolNotifyEvent (
                                &gEfiI2cMasterProtocolGuid,
                                TPL_CALLBACK,
                                I2cMasterRegistrationEvent,
                                NULL,
                                &mI2cMasterEventRegistration
                                );

  Status = RtcDeviceInitialize ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC init fail, %r\n", Status));
  }

  LibGetTime(&GetTime,NULL);
  if(!IsTimeValid(&GetTime))
    LibSetTime(&InitTime);

  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (
                               EVT_NOTIFY_SIGNAL,
                               TPL_NOTIFY,
                               LibRtcVirtualNotifyEvent,
                               NULL,
                               &gEfiEventVirtualAddressChangeGuid,
                               &mRtcVirtualAddrChangeEvent
                               );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
