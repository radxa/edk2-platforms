/** @file
 *
 *  Copyright 2022 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
 *  Copyright (c) 2014-2016, Linaro Limited. All rights reserved.
 *  Copyright (c) 2014, Red Hat, Inc.
 *  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
 *
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/IoLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Pi/PiBootMode.h>

#include <Ppi/ArmMpCoreInfo.h>
#include <Library/SocInitLib.h>
#include <Library/PinMuxLib.h>
#include <Library/BaseMemoryLib.h>

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

  @return   Return the current Boot Mode of the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

EFI_STATUS
SetPmuCounter (
  VOID
  )
{
  UINT8   BootCorePmuCounterNum;
  UINT32  Mdcr;

  BootCorePmuCounterNum = FixedPcdGet8 (PcdBootCorePmuCounterNum);
  Mdcr                  = ArmReadMdcr ();
  Mdcr                 &= ~(0x1F);
  Mdcr                 |= BootCorePmuCounterNum;
  ArmWriteMdcr (Mdcr);
  return RETURN_SUCCESS;
}

/**
  This function is called by PrePeiCore, in the SEC phase.
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN  MpId
  )
{
  SetPmuCounter ();
  SetSocFchIpClockAndReset ();
 #ifdef CONFIG_RLOG_ENABLE
  CopyMem ((VOID *)FixedPcdGet64 (PcdRamLogLastBootSaveAddress), (VOID *)FixedPcdGet64 (PcdRamLogBaseAddress), FixedPcdGet32 (PcdRamLogSize)/2);
  rlog_init_printf ((char *)FixedPcdGet64 (PcdRamLogBaseAddress), FixedPcdGet32 (PcdRamLogSize));
 #endif
  return RETURN_SUCCESS;
}

VOID
ArmPlatformInitializeSystemMemory (
  VOID
  )
{
}

STATIC ARM_CORE_INFO  mCixInfoTable[] = {
  { 0x0, 0x0, },             // Cluster 0, Core 0
  { 0x0, 0x1, },             // Cluster 0, Core 1
  { 0x0, 0x2, },             // Cluster 0, Core 2
  { 0x0, 0x3, },             // Cluster 0, Core 3
};

STATIC
EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN          *CoreCount,
  OUT ARM_CORE_INFO  **ArmCoreTable
  )
{
  // Only support one cluster
  *CoreCount    = sizeof (mCixInfoTable) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = mCixInfoTable;

  return EFI_SUCCESS;
}

STATIC ARM_MP_CORE_INFO_PPI    mMpCoreInfoPpi = {
  PrePeiCoreGetMpCoreInfo
};
STATIC EFI_PEI_PPI_DESCRIPTOR  mPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (mPlatformPpiTable);
  *PpiList     = mPlatformPpiTable;
}
