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
#include <Library/EcLib.h>
#include <Library/CixPostCodeLib.h>

#define  POWER_BUTTON_POLLING_INTERVAL  500 * 1000 * 10
#define  POWER_BUTTON_VALID             BIT1

VOID
EFIAPI
PowerButtonCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS                  Status;
  EC_RESPONSE_ACPI_INT_EVENT  AcpiIntEvent;

  DEBUG ((DEBUG_VERBOSE, "\n[PBTN]callback.\n"));
  Status = GetAcpiIntEvent (&AcpiIntEvent);
  if (!EFI_ERROR (Status)) {
    if ( AcpiIntEvent.Event & POWER_BUTTON_VALID) {
      DEBUG ((DEBUG_VERBOSE, "\n[PBTN]pressed.\n"));
      LibResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    }
  }
}

STATIC
EFI_STATUS
EFIAPI
CreatPowerButtonCallBack (
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   PwrBtnEvent;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL | EVT_TIMER,
                  TPL_CALLBACK,
                  PowerButtonCallback,
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
PowerButtonDxeDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                  Status;
  EC_RESPONSE_ACPI_INT_EVENT  AcpiIntEvent;

  GetAcpiIntEvent (&AcpiIntEvent); // clear
  Status = CreatPowerButtonCallBack ();

  return Status;
}
