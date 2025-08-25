/** @file
  Support ResetSystem Runtime call using PSCI calls
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved

  Note: A similar library is implemented in
  ArmVirtPkg/Library/ArmVirtualizationPsciResetSystemLib
  So similar issues might exist in this implementation too.

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013-2015, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2014, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/EcLib.h>
#include <Library/GpioLib.h>

#include <IndustryStandard/ArmStdSmc.h>

/**
  Resets the entire platform.

  @param  ResetType             The type of reset to perform.
  @param  ResetStatus           The status code for the reset.
  @param  DataSize              The size, in bytes, of WatchdogData.
  @param  ResetData             For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                Unicode string, optionally followed by additional binary data.

**/
EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN CHAR16          *ResetData OPTIONAL
  )
{
  ARM_SMC_ARGS              ArmSmcArgs;
  EC_PARAMS_FORCE_EC_RESET  Params;
  CHAR16 *                  PlatformDtbName;

  PlatformDtbName = (CHAR16 *)FixedPcdGetPtr (PcdSiliconDtbUpdateFileName);

  switch (ResetType) {
    case EfiResetPlatformSpecific:
      // Map the platform specific reset as reboot
      if (!StrCmp (L"sky1-orion-o6.dtb", PlatformDtbName)) {
        Params.Reserved = 0;
        ForceEcReset (&Params);

        DEBUG ((DEBUG_INFO, "%a: force EC reset\n", __FUNCTION__));
        CpuDeadLoop ();
      }
    case EfiResetWarm:
    // Map a warm reset into a cold reset
    case EfiResetCold:
      // Send a PSCI 0.2 SYSTEM_RESET command
      ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_RESET;
      break;
    case EfiResetShutdown:
      GpioConfig (FixedPcdGet8 (PcdPcieRootPort0PeResetPin), OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
      GpioConfig (FixedPcdGet8 (PcdPcieRootPort1PeResetPin), OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
      GpioConfig (FixedPcdGet8 (PcdPcieRootPort2PeResetPin), OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
      GpioConfig (FixedPcdGet8 (PcdPcieRootPort3PeResetPin), OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);
      GpioConfig (FixedPcdGet8 (PcdPcieRootPort4PeResetPin), OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);

      DEBUG ((DEBUG_INFO, "%a: disable additional GPIOs for %a\n", __FUNCTION__, PlatformDtbName));
      if (!StrCmp (L"sky1-orion-o6.dtb", PlatformDtbName)) {
        GpioConfig (12, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of M2_SSD_PWREN
        GpioConfig (17, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of VGFX_PWREN
      } else if (!StrCmp (L"sky1-orion-o6t.dtb", PlatformDtbName)) {
        GpioConfig (13, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of WLAN_PWREN
        GpioConfig (14, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of UFS_5V_EN
        GpioConfig (23, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of TPM_PWR_EN
        GpioConfig (70, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of OCULINK_PWREN
        GpioConfig (73, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of BKEY_PWR_EN
        GpioConfig (81, OUTPUT, INOUT_LOW, INTERRUPT_DISABLE, INTERRUPT_TYPE_DEFAULT);  //  output low of CAM_PWREN
      }

      // Send a PSCI 0.2 SYSTEM_OFF command
      ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_SYSTEM_OFF;
      break;
    default:
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
  }

  ArmCallSmc (&ArmSmcArgs);

  // We should never be here
  DEBUG ((DEBUG_ERROR, "%a: PSCI Reset failed\n", __FUNCTION__));
  CpuDeadLoop ();
  return EFI_UNSUPPORTED;
}

/**
  Initialize any infrastructure required for LibResetSystem () to function.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
LibInitializeResetSystem (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
