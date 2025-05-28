/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/CixPostCodeLib.h>
#include <Library/GpioLib.h>
#include <Library/PinMuxLib.h>

#define  POWER_BUTTON_POLLING_INTERVAL  200 * 1000 * 10


UINT8  PreviousLevel = 0xFF;
UINT8  CurrentLevel  = 0xFF;

UINT8
ReadGpioLevel (
  UINT8  GpioNum
  )
{
  UINT8  PinStatus;

  // get pin status
  GpioConfig (GpioNum, INPUT, INOUT_VALUE_DEFAULT, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
  GpioGet (GpioNum, (IO_INOUT_VALUE_SEL *)&PinStatus);

  return PinStatus;
}

VOID
EFIAPI
GpioPowerButtonCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{

  CurrentLevel = ReadGpioLevel (PcdGet8(PcdPowerButtonGpioNum));
  //DebugPrint (DEBUG_INFO, "[PBTN] [%d].\n", CurrentLevel);
  if (CurrentLevel == 1) {
    if (PreviousLevel == 0) {
      //DebugPrint (DEBUG_INFO, "[PBTN] Pressed.\n");
      gBS->Stall (200*1000);
      LibResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    }
  }else {
    if( PreviousLevel == 0xFF) {
      PreviousLevel = CurrentLevel;
    }
  }


}

STATIC
EFI_STATUS
EFIAPI
CreatGpioPowerButtonCallBack (
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   PwrBtnEvent;

  Status = gBS->CreateEvent (
                             EVT_NOTIFY_SIGNAL | EVT_TIMER,
                             TPL_CALLBACK,
                             GpioPowerButtonCallback,
                             NULL,
                             &PwrBtnEvent
                             );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = gBS->SetTimer (PwrBtnEvent, TimerPeriodic, POWER_BUTTON_POLLING_INTERVAL);
Exit:
  DEBUG ((DEBUG_INFO, "[PBTN]Create Power Button callback %r\n", Status));
  return Status;
}

EFI_STATUS
EFIAPI
GpioPowerButtonDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = CreatGpioPowerButtonCallBack ();

  return Status;
}
