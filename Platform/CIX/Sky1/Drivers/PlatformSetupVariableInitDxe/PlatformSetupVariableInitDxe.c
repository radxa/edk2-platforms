/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved

**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <PlatformSetupVar.h>
#include <PlatformSetupVarEntry.h>
#include <Library/ConfigParamsDataBlockLib.h>
#include <Library/PlatformConfigParamsDataBlockLib.h>
#include <Library/CpuInfoLib.h>
#include <Protocol/ConfigParamsManageProtocol.h>
#include <Protocol/PlatformConfigParamsManageProtocol.h>
#include <Protocol/PlatformSetupDataInfoProtocol.h>
#include <Guid/NetworkStackSetup.h>

PLATFORM_SETUP_DATA_INFO_TABLE  mPlatformSetupDataInfoTable = {
  PLATFORM_SETUP_VAR,
  sizeof (PLATFORM_SETUP_DATA),
  CIX_PLATFORM_SETUP_VARIABLE_GUID,
  PlatformSetupDataEntry,
  (sizeof (PlatformSetupDataEntry)/ sizeof (PLATFORM_SETUP_DATA_ENTRY))
};

EFI_STATUS
EFIAPI
CheckCpuShareInfo (
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  UINTN                VarSize;
  PLATFORM_SETUP_DATA  PlatformSetupVar;
  UINT32               CpuShareInfo;
  UINT8                CpuCoreNum;

  VarSize = sizeof (PLATFORM_SETUP_DATA);

  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PlatformSetupVar
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a EfiGetVariable failed: %r\n", __FUNCTION__, Status));
    return Status;
  }

  GetCpuShareInfo (&CpuShareInfo);
  if (CpuShareInfo != PlatformSetupVar.CpuShareInfo) {
    GetValidCpuCoreNum (&CpuCoreNum);
    PlatformSetupVar.CpuCoreNum        = CpuCoreNum;
    PlatformSetupVar.CpuShareInfo      = CpuShareInfo;
    PlatformSetupVar.CpuCoreEnable[0]  = 1;        // cpu core 0 is boot core and never be disabled.
    PlatformSetupVar.CpuCoreEnable[1]  = FixedPcdGetBool (PcdCpuCore1En);
    PlatformSetupVar.CpuCoreEnable[2]  = FixedPcdGetBool (PcdCpuCore2En);
    PlatformSetupVar.CpuCoreEnable[3]  = FixedPcdGetBool (PcdCpuCore3En);
    PlatformSetupVar.CpuCoreEnable[4]  = FixedPcdGetBool (PcdCpuCore4En);
    PlatformSetupVar.CpuCoreEnable[5]  = FixedPcdGetBool (PcdCpuCore5En);
    PlatformSetupVar.CpuCoreEnable[6]  = FixedPcdGetBool (PcdCpuCore6En);
    PlatformSetupVar.CpuCoreEnable[7]  = FixedPcdGetBool (PcdCpuCore7En);
    PlatformSetupVar.CpuCoreEnable[8]  = FixedPcdGetBool (PcdCpuCore8En);
    PlatformSetupVar.CpuCoreEnable[9]  = FixedPcdGetBool (PcdCpuCore9En);
    PlatformSetupVar.CpuCoreEnable[10] = FixedPcdGetBool (PcdCpuCore10En);
    PlatformSetupVar.CpuCoreEnable[11] = FixedPcdGetBool (PcdCpuCore11En);

    Status = gRT->SetVariable (
                    PLATFORM_SETUP_VAR,
                    &gPlatformSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (PLATFORM_SETUP_DATA),
                    &PlatformSetupVar
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

BOOLEAN
IsRtcPowerfailure (
  )
{
  EFI_STATUS                                  Status;
  PLATFORM_CONFIG_PARAMS_DATA_BLOCK           *ConfigData = NULL;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;

  Status = gBS->LocateProtocol (&gCixPlatformConfigParamsManageProtocolGuid, NULL, (VOID **)&ConfigManage);
  if (!EFI_ERROR (Status)) {
    ConfigData = ConfigManage->Data;
    if (ConfigData != NULL) {
      DEBUG ((DEBUG_INFO, "RTC voltage drop status : %a\n", ConfigData->RtcVoltDrop ? "TRUE" : "FALSE"));
      return ConfigData->RtcVoltDrop;
    }
  }

  return FALSE;
}

VOID
EFIAPI
UpdateConfigParams (
  CONFIG_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS           Status;
  UINTN                VarSize;
  PLATFORM_SETUP_DATA  PlatformSetupVar;

  VarSize = sizeof (PLATFORM_SETUP_DATA);

  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PlatformSetupVar
                  );
  if (!EFI_ERROR (Status) && !IsRtcPowerfailure ()) {
    for (UINT8 i = 0; i < MAX_PCIE_PORT_NUM; i++) {
      ConfigData->Pcie.PcieRpEnable[i]        = PlatformSetupVar.PcieRpEnable[i];
      ConfigData->Pcie.PcieWidth[i]           = PlatformSetupVar.PcieWidth[i];
      ConfigData->Pcie.PcieMaxSpeed[i]        = PlatformSetupVar.PcieMaxSpeed[i];
      ConfigData->Pcie.PcieTargetLinkSpeed[i] = PlatformSetupVar.PcieTargetLinkSpeed[i];
      ConfigData->Pcie.PcieAspmMaxSupport[i]  = PlatformSetupVar.PcieAspmMaxSupport[i];
      ConfigData->Pcie.PcieAspm[i]            = PlatformSetupVar.PcieAspm[i];
      ConfigData->Pcie.PcieMaxPayload[i]      = PlatformSetupVar.PcieMaxPayload[i];
      ConfigData->Pcie.PcieMaxReadRequest[i]  = PlatformSetupVar.PcieMaxReadRequest[i];
      ConfigData->Pcie.PcieL1Substates[i]     = PlatformSetupVar.PcieL1Substates[i];
      ConfigData->Pcie.PcieDtiEnable[i]       = PlatformSetupVar.PcieDtiEnable[i];
      ConfigData->Pcie.PcieLinkTimeout[i]     = PlatformSetupVar.PcieLinkTimeout[i];
    }

    for (UINT8 i = 0; i < MAX_I2C_CTRL_NUM; i++) {
      ConfigData->Fch.I2c[i].Enable  = PlatformSetupVar.I2cEnable[i];
      ConfigData->Fch.I2c[i].BusFreq = PlatformSetupVar.I2cBusFreq[i];
    }

    ConfigData->Usb20[0].Enable     = PlatformSetupVar.Usb2Control0Enable;
    ConfigData->Usb20[1].Enable     = PlatformSetupVar.Usb2Control1Enable;
    ConfigData->Usb20[2].Enable     = PlatformSetupVar.Usb2Control2Enable;
    ConfigData->Usb20[3].Enable     = PlatformSetupVar.Usb2Control3Enable;
    ConfigData->Usb32Drd[0].Enable  = PlatformSetupVar.Usb3Control0Enable;
    ConfigData->Usb32Drd[1].Enable  = PlatformSetupVar.Usb3Control1Enable;
    ConfigData->UsbCDrd[0].Enable   = PlatformSetupVar.UsbCDrdControl0Enable;
    ConfigData->UsbC[0].Enable      = PlatformSetupVar.UsbCControl0Enable;
    ConfigData->UsbC[1].Enable      = PlatformSetupVar.UsbCControl1Enable;
    ConfigData->UsbC[2].Enable      = PlatformSetupVar.UsbCControl2Enable;
    ConfigData->UsbCDrd[0].DataRole = PlatformSetupVar.UsbCDrdControl0DataRole;

    ConfigData->Gmac[0].Enable     =  PlatformSetupVar.GmacEnable[0];
    ConfigData->Gmac[1].Enable     =  PlatformSetupVar.GmacEnable[1];
    ConfigData->Gmac[0].MacAddress =  PlatformSetupVar.GmacMacAddr[0];
    ConfigData->Gmac[1].MacAddress =  PlatformSetupVar.GmacMacAddr[1];

    ConfigData->S5.SocWatchdogTimer = PlatformSetupVar.SocWatchdogTimer;

    ConfigData->Cpu.LpiState       = PlatformSetupVar.CpuLpiState;
    ConfigData->Cpu.CoreNum        = PlatformSetupVar.CpuCoreNum;
    ConfigData->Cpu.ShareInfo      = PlatformSetupVar.CpuShareInfo;
    ConfigData->Cpu.CoreEnable[0]  = PlatformSetupVar.CpuCoreEnable[0];
    ConfigData->Cpu.CoreEnable[1]  = PlatformSetupVar.CpuCoreEnable[1];
    ConfigData->Cpu.CoreEnable[2]  = PlatformSetupVar.CpuCoreEnable[2];
    ConfigData->Cpu.CoreEnable[3]  = PlatformSetupVar.CpuCoreEnable[3];
    ConfigData->Cpu.CoreEnable[4]  = PlatformSetupVar.CpuCoreEnable[4];
    ConfigData->Cpu.CoreEnable[5]  = PlatformSetupVar.CpuCoreEnable[5];
    ConfigData->Cpu.CoreEnable[6]  = PlatformSetupVar.CpuCoreEnable[6];
    ConfigData->Cpu.CoreEnable[7]  = PlatformSetupVar.CpuCoreEnable[7];
    ConfigData->Cpu.CoreEnable[8]  = PlatformSetupVar.CpuCoreEnable[8];
    ConfigData->Cpu.CoreEnable[9]  = PlatformSetupVar.CpuCoreEnable[9];
    ConfigData->Cpu.CoreEnable[10] = PlatformSetupVar.CpuCoreEnable[10];
    ConfigData->Cpu.CoreEnable[11] = PlatformSetupVar.CpuCoreEnable[11];

    ConfigData->Mem.MemFreq                = PlatformSetupVar.MemFreq;
    ConfigData->Mem.MemBreakPoint          = PlatformSetupVar.MemBreakPoint;
    ConfigData->Mem.MemEyeScan             = PlatformSetupVar.MemEyeScan;
    ConfigData->Mem.MemHarvesting          = PlatformSetupVar.MemHarvesting;
    ConfigData->Mem.WckAlwaysOn            = PlatformSetupVar.WckAlwaysOn;
    ConfigData->Mem.DataMask               = PlatformSetupVar.DataMask;
    ConfigData->Mem.RfmEn                  = PlatformSetupVar.RfmEn;
    ConfigData->Mem.AutoPrechargeEn        = PlatformSetupVar.AutoPrechargeEn;
    ConfigData->Mem.PbrEn                  = PlatformSetupVar.PbrEn;
    ConfigData->Mem.MbistEn                = PlatformSetupVar.MbistEn;
    ConfigData->Mem.MbistMode              = PlatformSetupVar.MbistMode;
    ConfigData->Mem.MemWrLEcc              = PlatformSetupVar.MemWrLEcc;
    ConfigData->Mem.MemRdLEcc              = PlatformSetupVar.MemRdLEcc;
    ConfigData->Mem.PortPriority           = PlatformSetupVar.PortPriority;
    ConfigData->Mem.BdwP0Override          = PlatformSetupVar.BdwP0Override;
    ConfigData->Mem.BdwP0                  = PlatformSetupVar.BdwP0;
    ConfigData->Mem.BdwP1Override          = PlatformSetupVar.BdwP1Override;
    ConfigData->Mem.BdwP1                  = PlatformSetupVar.BdwP1;
    ConfigData->Mem.MemRPriorityP0Override = PlatformSetupVar.MemRPriorityP0Override;
    ConfigData->Mem.MemRPriorityP0         = PlatformSetupVar.MemRPriorityP0;
    ConfigData->Mem.MemWPriorityP0Override = PlatformSetupVar.MemWPriorityP0Override;
    ConfigData->Mem.MemWPriorityP0         = PlatformSetupVar.MemWPriorityP0;
    ConfigData->Mem.MemBdwOvflowP0         = PlatformSetupVar.MemBdwOvflowP0;
    ConfigData->Mem.MemRPriorityP1Override = PlatformSetupVar.MemRPriorityP1Override;
    ConfigData->Mem.MemRPriorityP1         = PlatformSetupVar.MemRPriorityP1;
    ConfigData->Mem.MemWPriorityP1Override = PlatformSetupVar.MemWPriorityP1Override;
    ConfigData->Mem.MemWPriorityP1         = PlatformSetupVar.MemWPriorityP1;
    ConfigData->Mem.MemBdwOvflowP1         = PlatformSetupVar.MemBdwOvflowP1;
    ConfigData->Mem.MemIEcc                = PlatformSetupVar.MemIEcc;
    ConfigData->Mem.MemWrDbi               = PlatformSetupVar.MemWrDbi;
    ConfigData->Mem.MemRdDbi               = PlatformSetupVar.MemRdDbi;

    ConfigData->Pm.VddSocVoltage         = PlatformSetupVar.VddSocVoltage;
    ConfigData->Pm.VddGpuVoltage         = PlatformSetupVar.VddGpuVoltage;
    ConfigData->Pm.VddDpuVoltage         = PlatformSetupVar.VddDpuVoltage;
    ConfigData->Pm.VddCpuBigCore0Voltage = PlatformSetupVar.VddCpuBigCore0Voltage;
    ConfigData->Pm.VddCpuBigCore1Voltage = PlatformSetupVar.VddCpuBigCore1Voltage;
    ConfigData->Pm.VddCpuMidCore0Voltage = PlatformSetupVar.VddCpuMidCore0Voltage;
    ConfigData->Pm.VddCpuMidCore1Voltage = PlatformSetupVar.VddCpuMidCore1Voltage;
    ConfigData->Pm.VddCpuLitCoreVoltage  = PlatformSetupVar.VddCpuLitCoreVoltage;
    ConfigData->Pm.CpuCoreClkGating      = PlatformSetupVar.CpuCoreClkGating;
    ConfigData->Pm.DsuClkGating          = PlatformSetupVar.DsuClkGating;
    ConfigData->Pm.GicdClkGating         = PlatformSetupVar.GicdClkGating;
    ConfigData->Pm.Ci700ClkGating        = PlatformSetupVar.Ci700ClkGating;
    ConfigData->Pm.SysNi700ClkGating     = PlatformSetupVar.SysNi700ClkGating;
    ConfigData->Pm.MmNi700ClkGating      = PlatformSetupVar.MmNi700ClkGating;
    ConfigData->Pm.PcieNi700ClkGating    = PlatformSetupVar.PcieNi700ClkGating;
    ConfigData->Pm.SmnNi700ClkGating     = PlatformSetupVar.SmnNi700ClkGating;
    ConfigData->Pm.GpuClkGating          = PlatformSetupVar.GpuClkGating;
    ConfigData->Pm.Dpu0ClkGating         = PlatformSetupVar.Dpu0ClkGating;
    ConfigData->Pm.Dpu1ClkGating         = PlatformSetupVar.Dpu1ClkGating;
    ConfigData->Pm.Dpu2ClkGating         = PlatformSetupVar.Dpu2ClkGating;
    ConfigData->Pm.Dpu3ClkGating         = PlatformSetupVar.Dpu3ClkGating;
    ConfigData->Pm.Dpu4ClkGating         = PlatformSetupVar.Dpu4ClkGating;
    ConfigData->Pm.VpuClkGating          = PlatformSetupVar.VpuClkGating;
    ConfigData->Misc.CpuCppcType         = PlatformSetupVar.CpuCppcType;
    ConfigData->Spi.TPMDeviceSelect      = PlatformSetupVar.TPMDeviceSelect;
  }
}

VOID
EFIAPI
UpdatePlatformConfigParams (
  PLATFORM_CONFIG_PARAMS_DATA_BLOCK  *ConfigData
  )
{
  EFI_STATUS           Status;
  UINTN                VarSize;
  PLATFORM_SETUP_DATA  PlatformSetupVar;
  SYSTEM_TABLE         SystemTableVar;
  UINTN                SystemTableVarSize;

  VarSize = sizeof (PLATFORM_SETUP_DATA);

  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PlatformSetupVar
                  );
  if (!EFI_ERROR (Status) && !IsRtcPowerfailure ()) {
    ConfigData->DtbMenuEntry     = PlatformSetupVar.DtbMenuEntry;
    ConfigData->GfxPower         = PlatformSetupVar.GfxPower;
    ConfigData->TouchPanelPower  = PlatformSetupVar.TouchPanelPower;
    ConfigData->TpmPower         = PlatformSetupVar.TpmPower;
    ConfigData->WwanPower        = PlatformSetupVar.WwanPower;
    ConfigData->PcieX2SlotPower  = PlatformSetupVar.PcieX2SlotPower;
    ConfigData->FingerPrintPower = PlatformSetupVar.FingerPrintPower;
    ConfigData->WlanPower        = PlatformSetupVar.WlanPower;
    ConfigData->M2SsdPower       = PlatformSetupVar.M2SsdPower;
    ConfigData->OnBoardLanPower  = PlatformSetupVar.OnBoardLanPower;
    ConfigData->IspCamera0Power  = PlatformSetupVar.IspCamera0Power;
    ConfigData->IspCamera1Power  = PlatformSetupVar.IspCamera1Power;
    ConfigData->IspCamera2Power  = PlatformSetupVar.IspCamera2Power;
    ConfigData->IspCamera3Power  = PlatformSetupVar.IspCamera3Power;
    ConfigData->StateAfterG3     = PlatformSetupVar.StateAfterG3;
    ConfigData->RtcWakeup        = PlatformSetupVar.RtcWakeup;
    ConfigData->LightSensorCtrl  = PlatformSetupVar.LightSensorCtrl;
    ConfigData->SpcrEnable       = PlatformSetupVar.SpcrEnable;
    ConfigData->EcFanMode        = PlatformSetupVar.EcFanMode;
  }

  SystemTableVarSize = sizeof (SYSTEM_TABLE);

  Status = gRT->GetVariable (
                  SYSTEM_TABLE_VAR,
                  &gCixGlobalVariableGuid,
                  NULL,
                  &SystemTableVarSize,
                  &SystemTableVar
                  );
  if (!EFI_ERROR (Status) && !IsRtcPowerfailure ()) {
    ConfigData->SystemTableSelect = SystemTableVar.SystemTableSelect;
  }
}

VOID
EFIAPI
ConfigParamsUpdateCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                         Status      = EFI_SUCCESS;
  CONFIG_PARAMS_DATA_BLOCK           *ConfigData = NULL;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateProtocol (&gCixConfigParamsManageProtocolGuid, NULL, (VOID **)&ConfigManage);
  if (!EFI_ERROR (Status)) {
    ConfigData = ConfigManage->Data;
    if (ConfigData != NULL) {
      UpdateConfigParams (ConfigData);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a LocateProtocol failed: %r\n", __FUNCTION__, Status));
    return;
  }
}

VOID
EFIAPI
PlatformConfigParamsUpdateCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                                  Status      = EFI_SUCCESS;
  PLATFORM_CONFIG_PARAMS_DATA_BLOCK           *ConfigData = NULL;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateProtocol (&gCixPlatformConfigParamsManageProtocolGuid, NULL, (VOID **)&ConfigManage);
  if (!EFI_ERROR (Status)) {
    ConfigData = ConfigManage->Data;
    if (ConfigData != NULL) {
      UpdatePlatformConfigParams (ConfigData);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a LocateProtocol failed: %r\n", __FUNCTION__, Status));
    return;
  }
}

VOID
EFIAPI
ConstructSetupVariable (
  PLATFORM_SETUP_DATA  *PlatformSetupVar
  )
{
  UINT8   CpuCoreNum;
  UINT32  CpuShareInfo;

  GetValidCpuCoreNum (&CpuCoreNum);
  GetCpuShareInfo (&CpuShareInfo);

  PlatformSetupVar->PcieRpEnable[0]        = FixedPcdGetBool (PcdPcieRootPort0Enable);
  PlatformSetupVar->PcieWidth[0]           = FixedPcdGet8 (PcdPcieRootPort0LaneNum);
  PlatformSetupVar->PcieMaxSpeed[0]        = FixedPcdGet8 (PcdPcieRootPort0MaxSpeed);
  PlatformSetupVar->PcieTargetLinkSpeed[0] = FixedPcdGet8 (PcdPcieRootPort0SpeedGen);
  PlatformSetupVar->PcieAspmMaxSupport[0]  = FixedPcdGet8 (PcdPcieRootPort0MaxAspmSupport);
  PlatformSetupVar->PcieAspm[0]            = FixedPcdGet8 (PcdPcieRootPort0AspmSupport);
  PlatformSetupVar->PcieMaxPayload[0]      = FixedPcdGet8 (PcdPcieRootPort0MaxPayload);
  PlatformSetupVar->PcieMaxReadRequest[0]  = FixedPcdGet8 (PcdPcieRootPort0MaxReadRequest);
  PlatformSetupVar->PcieDtiEnable[0]       = FixedPcdGet8 (PcdPcieRootPort0DtiEnable);
  PlatformSetupVar->PcieLinkTimeout[0]     = FixedPcdGet16 (PcdPcieRootPort0LinkUpTimeout);

  PlatformSetupVar->PcieRpEnable[1]        = FixedPcdGetBool (PcdPcieRootPort1Enable);
  PlatformSetupVar->PcieWidth[1]           = FixedPcdGet8 (PcdPcieRootPort1LaneNum);
  PlatformSetupVar->PcieMaxSpeed[1]        = FixedPcdGet8 (PcdPcieRootPort1MaxSpeed);
  PlatformSetupVar->PcieTargetLinkSpeed[1] = FixedPcdGet8 (PcdPcieRootPort1SpeedGen);
  PlatformSetupVar->PcieAspmMaxSupport[1]  = FixedPcdGet8 (PcdPcieRootPort1MaxAspmSupport);
  PlatformSetupVar->PcieAspm[1]            = FixedPcdGet8 (PcdPcieRootPort1AspmSupport);
  PlatformSetupVar->PcieMaxPayload[1]      = FixedPcdGet8 (PcdPcieRootPort1MaxPayload);
  PlatformSetupVar->PcieMaxReadRequest[1]  = FixedPcdGet8 (PcdPcieRootPort1MaxReadRequest);
  PlatformSetupVar->PcieDtiEnable[1]       = FixedPcdGet8 (PcdPcieRootPort1DtiEnable);
  PlatformSetupVar->PcieLinkTimeout[1]     = FixedPcdGet16 (PcdPcieRootPort1LinkUpTimeout);

  PlatformSetupVar->PcieRpEnable[2]        = FixedPcdGetBool (PcdPcieRootPort2Enable);
  PlatformSetupVar->PcieWidth[2]           = FixedPcdGet8 (PcdPcieRootPort2LaneNum);
  PlatformSetupVar->PcieMaxSpeed[2]        = FixedPcdGet8 (PcdPcieRootPort2MaxSpeed);
  PlatformSetupVar->PcieTargetLinkSpeed[2] = FixedPcdGet8 (PcdPcieRootPort2SpeedGen);
  PlatformSetupVar->PcieAspmMaxSupport[2]  = FixedPcdGet8 (PcdPcieRootPort2MaxAspmSupport);
  PlatformSetupVar->PcieAspm[2]            = FixedPcdGet8 (PcdPcieRootPort2AspmSupport);
  PlatformSetupVar->PcieMaxPayload[2]      = FixedPcdGet8 (PcdPcieRootPort2MaxPayload);
  PlatformSetupVar->PcieMaxReadRequest[2]  = FixedPcdGet8 (PcdPcieRootPort2MaxReadRequest);
  PlatformSetupVar->PcieDtiEnable[2]       = FixedPcdGet8 (PcdPcieRootPort2DtiEnable);
  PlatformSetupVar->PcieLinkTimeout[2]     = FixedPcdGet16 (PcdPcieRootPort2LinkUpTimeout);

  PlatformSetupVar->PcieRpEnable[3]        = FixedPcdGetBool (PcdPcieRootPort3Enable);
  PlatformSetupVar->PcieWidth[3]           = FixedPcdGet8 (PcdPcieRootPort3LaneNum);
  PlatformSetupVar->PcieMaxSpeed[3]        = FixedPcdGet8 (PcdPcieRootPort3MaxSpeed);
  PlatformSetupVar->PcieTargetLinkSpeed[3] = FixedPcdGet8 (PcdPcieRootPort3SpeedGen);
  PlatformSetupVar->PcieAspmMaxSupport[3]  = FixedPcdGet8 (PcdPcieRootPort3MaxAspmSupport);
  PlatformSetupVar->PcieAspm[3]            = FixedPcdGet8 (PcdPcieRootPort3AspmSupport);
  PlatformSetupVar->PcieMaxPayload[3]      = FixedPcdGet8 (PcdPcieRootPort3MaxPayload);
  PlatformSetupVar->PcieMaxReadRequest[3]  = FixedPcdGet8 (PcdPcieRootPort3MaxReadRequest);
  PlatformSetupVar->PcieDtiEnable[3]       = FixedPcdGet8 (PcdPcieRootPort3DtiEnable);
  PlatformSetupVar->PcieLinkTimeout[3]     = FixedPcdGet16 (PcdPcieRootPort3LinkUpTimeout);

  PlatformSetupVar->PcieRpEnable[4]        = FixedPcdGetBool (PcdPcieRootPort4Enable);
  PlatformSetupVar->PcieWidth[4]           = FixedPcdGet8 (PcdPcieRootPort4LaneNum);
  PlatformSetupVar->PcieMaxSpeed[4]        = FixedPcdGet8 (PcdPcieRootPort4MaxSpeed);
  PlatformSetupVar->PcieTargetLinkSpeed[4] = FixedPcdGet8 (PcdPcieRootPort4SpeedGen);
  PlatformSetupVar->PcieAspmMaxSupport[4]  = FixedPcdGet8 (PcdPcieRootPort4MaxAspmSupport);
  PlatformSetupVar->PcieAspm[4]            = FixedPcdGet8 (PcdPcieRootPort4AspmSupport);
  PlatformSetupVar->PcieMaxPayload[4]      = FixedPcdGet8 (PcdPcieRootPort4MaxPayload);
  PlatformSetupVar->PcieMaxReadRequest[4]  = FixedPcdGet8 (PcdPcieRootPort4MaxReadRequest);
  PlatformSetupVar->PcieDtiEnable[4]       = FixedPcdGet8 (PcdPcieRootPort4DtiEnable);
  PlatformSetupVar->PcieLinkTimeout[4]     = FixedPcdGet16 (PcdPcieRootPort4LinkUpTimeout);

  PlatformSetupVar->I2cEnable[0]  = FixedPcdGetBool (PcdI2c0En);
  PlatformSetupVar->I2cEnable[1]  = FixedPcdGetBool (PcdI2c1En);
  PlatformSetupVar->I2cEnable[2]  = FixedPcdGetBool (PcdI2c2En);
  PlatformSetupVar->I2cEnable[3]  = FixedPcdGetBool (PcdI2c3En);
  PlatformSetupVar->I2cEnable[4]  = FixedPcdGetBool (PcdI2c4En);
  PlatformSetupVar->I2cEnable[5]  = FixedPcdGetBool (PcdI2c5En);
  PlatformSetupVar->I2cEnable[6]  = FixedPcdGetBool (PcdI2c6En);
  PlatformSetupVar->I2cEnable[7]  = FixedPcdGetBool (PcdI2c7En);
  PlatformSetupVar->I2cBusFreq[0] = FixedPcdGet32 (PcdI2c0BusFreq);
  PlatformSetupVar->I2cBusFreq[1] = FixedPcdGet32 (PcdI2c1BusFreq);
  PlatformSetupVar->I2cBusFreq[2] = FixedPcdGet32 (PcdI2c2BusFreq);
  PlatformSetupVar->I2cBusFreq[3] = FixedPcdGet32 (PcdI2c3BusFreq);
  PlatformSetupVar->I2cBusFreq[4] = FixedPcdGet32 (PcdI2c4BusFreq);
  PlatformSetupVar->I2cBusFreq[5] = FixedPcdGet32 (PcdI2c5BusFreq);
  PlatformSetupVar->I2cBusFreq[6] = FixedPcdGet32 (PcdI2c6BusFreq);
  PlatformSetupVar->I2cBusFreq[7] = FixedPcdGet32 (PcdI2c7BusFreq);

  PlatformSetupVar->GmacEnable[0]  = FixedPcdGetBool (PcdGmac0Enable);
  PlatformSetupVar->GmacEnable[1]  = FixedPcdGetBool (PcdGmac1Enable);
  PlatformSetupVar->GmacMacAddr[0] = FixedPcdGetBool (PcdGmac0MacAddress);
  PlatformSetupVar->GmacMacAddr[1] = FixedPcdGetBool (PcdGmac1MacAddress);

  PlatformSetupVar->MemBreakPoint   = FixedPcdGet8 (PcdMemBreakPoint);
  PlatformSetupVar->MemEyeScan      = FixedPcdGet8 (PcdMemEyeScan);
  PlatformSetupVar->MemFreq         = FixedPcdGet16 (PcdMemFreq);
  PlatformSetupVar->MemHarvesting   = FixedPcdGet8 (PcdMemHarvesting);
  PlatformSetupVar->WckAlwaysOn     = FixedPcdGet8 (PcdMemWckAlwaysOn);
  PlatformSetupVar->DataMask        = FixedPcdGet8 (PcdMemDataMask);
  PlatformSetupVar->RfmEn           = FixedPcdGet8 (PcdMemRfmEn);
  PlatformSetupVar->AutoPrechargeEn = FixedPcdGet8 (PcdMemAutoPrechargeEn);
  PlatformSetupVar->PbrEn           = FixedPcdGet8 (PcdMemPbrEn);
  PlatformSetupVar->MbistEn         = FixedPcdGet8 (PcdMemMbistEn);
  PlatformSetupVar->MbistMode       = FixedPcdGet8 (PcdMemMbistMode);
  PlatformSetupVar->MemWrLEcc       = FixedPcdGet8 (PcdMemWrLEcc);
  PlatformSetupVar->MemRdLEcc       = FixedPcdGet8 (PcdMemRdLEcc);
  PlatformSetupVar->PortPriority    = FixedPcdGet8 (PcdMemPortPriority);
  PlatformSetupVar->BdwP0Override   = 0;
  PlatformSetupVar->BdwP1Override   = 0;
  PlatformSetupVar->BdwP0           = FixedPcdGet8 (PcdMemBdwP0);
  PlatformSetupVar->BdwP1           = FixedPcdGet8 (PcdMemBdwP1);
  PlatformSetupVar->MemRPriorityP0  = FixedPcdGet8 (PcdMemRPriorityP0);
  PlatformSetupVar->MemWPriorityP0  = FixedPcdGet8 (PcdMemWPriorityP0);
  PlatformSetupVar->MemBdwOvflowP0  = FixedPcdGet8 (PcdMemBdwOvflowP0);
  PlatformSetupVar->MemRPriorityP1  = FixedPcdGet8 (PcdMemRPriorityP1);
  PlatformSetupVar->MemWPriorityP1  = FixedPcdGet8 (PcdMemWPriorityP1);
  PlatformSetupVar->MemBdwOvflowP1  = FixedPcdGet8 (PcdMemBdwOvflowP1);
  PlatformSetupVar->MemIEcc         = FixedPcdGet8 (PcdMemIEcc);
  PlatformSetupVar->MemWrDbi        = FixedPcdGet8 (PcdMemWrDbi);
  PlatformSetupVar->MemRdDbi        = FixedPcdGet8 (PcdMemRdDbi);

  PlatformSetupVar->StateAfterG3 = FixedPcdGet8 (PcdStateAfterG3);
  PlatformSetupVar->DtbMenuEntry = FixedPcdGet8 (PcdDtbMenuEntry);

  PlatformSetupVar->SocWatchdogTimer      = FixedPcdGet8 (PcdSocWatchdogTimer);
  PlatformSetupVar->VddSocVoltage         = FixedPcdGet16 (PcdVddSocVoltage);
  PlatformSetupVar->VddGpuVoltage         = FixedPcdGet16 (PcdVddGpuVoltage);
  PlatformSetupVar->VddDpuVoltage         = FixedPcdGet16 (PcdVddDpuVoltage);
  PlatformSetupVar->VddCpuBigCore0Voltage = FixedPcdGet16 (PcdVddCpuBigCore0Voltage);
  PlatformSetupVar->VddCpuBigCore1Voltage = FixedPcdGet16 (PcdVddCpuBigCore1Voltage);
  PlatformSetupVar->VddCpuMidCore0Voltage = FixedPcdGet16 (PcdVddCpuMidCore0Voltage);
  PlatformSetupVar->VddCpuMidCore1Voltage = FixedPcdGet16 (PcdVddCpuMidCore1Voltage);
  PlatformSetupVar->VddCpuLitCoreVoltage  = FixedPcdGet16 (PcdVddCpuLitCoreVoltage);
  PlatformSetupVar->GfxPower              = FixedPcdGet8 (PcdGfxPower);
  PlatformSetupVar->TouchPanelPower       = FixedPcdGet8 (PcdTouchPanelPower);
  PlatformSetupVar->TpmPower              = FixedPcdGet8 (PcdTpmPower);
  PlatformSetupVar->WwanPower             = FixedPcdGet8 (PcdWwanPower);
  PlatformSetupVar->PcieX2SlotPower       = FixedPcdGet8 (PcdPcieX2SlotPower);
  PlatformSetupVar->FingerPrintPower      = FixedPcdGet8 (PcdFingerPrintPower);
  PlatformSetupVar->WlanPower             = FixedPcdGet8 (PcdWlanPower);
  PlatformSetupVar->M2SsdPower            = FixedPcdGet8 (PcdM2SsdPower);
  PlatformSetupVar->OnBoardLanPower       = FixedPcdGet8 (PcdOnBoardLanPower);
  PlatformSetupVar->IspCamera0Power       = FixedPcdGet8 (PcdIspCamera0Power);
  PlatformSetupVar->IspCamera1Power       = FixedPcdGet8 (PcdIspCamera1Power);
  PlatformSetupVar->IspCamera2Power       = FixedPcdGet8 (PcdIspCamera2Power);
  PlatformSetupVar->IspCamera3Power       = FixedPcdGet8 (PcdIspCamera3Power);
  PlatformSetupVar->CpuCoreClkGating      = FixedPcdGet8 (PcdCpuCoreClkGating);
  PlatformSetupVar->DsuClkGating          = FixedPcdGet8 (PcdDsuClkGating);
  PlatformSetupVar->GicdClkGating         = FixedPcdGet8 (PcdGicdClkGating);
  PlatformSetupVar->Ci700ClkGating        = FixedPcdGet8 (PcdCi700ClkGating);
  PlatformSetupVar->SysNi700ClkGating     = FixedPcdGet8 (PcdSysNi700ClkGating);
  PlatformSetupVar->MmNi700ClkGating      = FixedPcdGet8 (PcdMmNi700ClkGating);
  PlatformSetupVar->PcieNi700ClkGating    = FixedPcdGet8 (PcdPcieNi700ClkGating);
  PlatformSetupVar->SmnNi700ClkGating     = FixedPcdGet8 (PcdSmnNi700ClkGating);
  PlatformSetupVar->GpuClkGating          = FixedPcdGet8 (PcdGpuClkGating);
  PlatformSetupVar->Dpu0ClkGating         = FixedPcdGet8 (PcdDpu0ClkGating);
  PlatformSetupVar->Dpu1ClkGating         = FixedPcdGet8 (PcdDpu1ClkGating);
  PlatformSetupVar->Dpu2ClkGating         = FixedPcdGet8 (PcdDpu2ClkGating);
  PlatformSetupVar->Dpu3ClkGating         = FixedPcdGet8 (PcdDpu3ClkGating);
  PlatformSetupVar->Dpu4ClkGating         = FixedPcdGet8 (PcdDpu4ClkGating);
  PlatformSetupVar->VpuClkGating          = FixedPcdGet8 (PcdVpuClkGating);
  PlatformSetupVar->RtcWakeup             = FixedPcdGet8 (PcdRtcWakeup);
  PlatformSetupVar->LightSensorCtrl       = FixedPcdGet8 (PcdLightSensorCtrl);
  PlatformSetupVar->SpcrEnable            = FixedPcdGetBool (PcdAcpiSpcrEnable);
  PlatformSetupVar->EcFanMode             = FixedPcdGetBool (PcdEcDefaultFanMode);

  PlatformSetupVar->Usb2Control0Enable      = FixedPcdGetBool (PcdUsb2Control0Enable);
  PlatformSetupVar->Usb2Control1Enable      = FixedPcdGetBool (PcdUsb2Control1Enable);
  PlatformSetupVar->Usb2Control2Enable      = FixedPcdGetBool (PcdUsb2Control2Enable);
  PlatformSetupVar->Usb2Control3Enable      = FixedPcdGetBool (PcdUsb2Control3Enable);
  PlatformSetupVar->Usb3Control0Enable      = FixedPcdGetBool (PcdUsb3Control0Enable);
  PlatformSetupVar->Usb3Control1Enable      = FixedPcdGetBool (PcdUsb3Control1Enable);
  PlatformSetupVar->UsbCDrdControl0Enable   = FixedPcdGetBool (PcdUsbCDrdControl0Enable);
  PlatformSetupVar->UsbCControl0Enable      = FixedPcdGetBool (PcdUsbCControl0Enable);
  PlatformSetupVar->UsbCControl1Enable      = FixedPcdGetBool (PcdUsbCControl1Enable);
  PlatformSetupVar->UsbCControl2Enable      = FixedPcdGetBool (PcdUsbCControl2Enable);
  PlatformSetupVar->UsbCDrdControl0DataRole = FixedPcdGetBool (PcdUsbCDrdControl0DataRole);
  PlatformSetupVar->CpuCoreEnable[0]        = 1; // cpu core 0 is boot core and never be disabled.
  PlatformSetupVar->CpuCoreEnable[1]        = FixedPcdGetBool (PcdCpuCore1En);
  PlatformSetupVar->CpuCoreEnable[2]        = FixedPcdGetBool (PcdCpuCore2En);
  PlatformSetupVar->CpuCoreEnable[3]        = FixedPcdGetBool (PcdCpuCore3En);
  PlatformSetupVar->CpuCoreEnable[4]        = FixedPcdGetBool (PcdCpuCore4En);
  PlatformSetupVar->CpuCoreEnable[5]        = FixedPcdGetBool (PcdCpuCore5En);
  PlatformSetupVar->CpuCoreEnable[6]        = FixedPcdGetBool (PcdCpuCore6En);
  PlatformSetupVar->CpuCoreEnable[7]        = FixedPcdGetBool (PcdCpuCore7En);
  PlatformSetupVar->CpuCoreEnable[8]        = FixedPcdGetBool (PcdCpuCore8En);
  PlatformSetupVar->CpuCoreEnable[9]        = FixedPcdGetBool (PcdCpuCore9En);
  PlatformSetupVar->CpuCoreEnable[10]       = FixedPcdGetBool (PcdCpuCore10En);
  PlatformSetupVar->CpuCoreEnable[11]       = FixedPcdGetBool (PcdCpuCore11En);
  PlatformSetupVar->CpuCoreNum              = CpuCoreNum;
  PlatformSetupVar->CpuShareInfo            = CpuShareInfo;
  PlatformSetupVar->CpuLpiState             = FixedPcdGet8 (PcdAcpiCpuLpiState);
  PlatformSetupVar->CpuCppcType             = FixedPcdGet8 (PcdAcpiCppcType);
  PlatformSetupVar->TPMDeviceSelect         = FixedPcdGet8 (PcdDefaultTpmDeviceSelect);
}

EFI_STATUS
PlatformSetupVariableInit (
  VOID
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  UINTN                VarSize;
  PLATFORM_SETUP_DATA  PlatformSetupVar;

  VarSize = sizeof (PLATFORM_SETUP_DATA);

  Status = gRT->GetVariable (
                  PLATFORM_SETUP_VAR,
                  &gPlatformSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &PlatformSetupVar
                  );
  if (EFI_ERROR (Status) || IsRtcPowerfailure ()) {
    //
    // Variable does not exist yet - create it
    //
    ZeroMem (&PlatformSetupVar, VarSize);
    ConstructSetupVariable (&PlatformSetupVar);

    Status = gRT->SetVariable (
                    PLATFORM_SETUP_VAR,
                    &gPlatformSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (PLATFORM_SETUP_DATA),
                    &PlatformSetupVar
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: EfiSetVariable failed - %r\n", __FUNCTION__, Status));
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: EfiGetVariable Success - %r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS
NetworkStackVariableInit (
  VOID
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;
  UINTN          VarSize;
  NETWORK_STACK  NetworkStack;

  VarSize = sizeof (NETWORK_STACK);

  Status = gRT->GetVariable (
                  NETWORK_STACK_VAR,
                  &gEfiNetworkStackSetupGuid,
                  NULL,
                  &VarSize,
                  &NetworkStack
                  );
  if (EFI_ERROR (Status) || IsRtcPowerfailure ()) {
    ZeroMem (&NetworkStack, VarSize);
    //
    // Variable does not exist yet - create it
    //
    NetworkStack.Enable   = FixedPcdGet8 (PcdNetworkStackSupport);
    NetworkStack.Ipv4Pxe  = FixedPcdGet8 (PcdIPv4PXESupport);
    NetworkStack.Ipv6Pxe  = FixedPcdGet8 (PcdIPv6PXESupport);
    NetworkStack.Ipv4Http = FixedPcdGet8 (PcdIPv4HttpSupport);
    NetworkStack.Ipv6Http = FixedPcdGet8 (PcdIPv6HttpSupport);
    Status                = gRT->SetVariable (
                                   NETWORK_STACK_VAR,
                                   &gEfiNetworkStackSetupGuid,
                                   EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                   sizeof (NETWORK_STACK),
                                   &NetworkStack
                                   );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: EfiSetVariable failed - %r\n", __FUNCTION__, Status));
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: EfiGetVariable Success - %r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS
SystemTableVariableInit (
  VOID
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         VarSize;
  SYSTEM_TABLE  SystemTableVar;

  VarSize = sizeof (SYSTEM_TABLE);

  Status = gRT->GetVariable (
                  SYSTEM_TABLE_VAR,
                  &gCixGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &SystemTableVar
                  );

  if (EFI_ERROR (Status) || IsRtcPowerfailure ()) {
    //
    // Variable does not exist yet - create it
    //
    SystemTableVar.SystemTableSelect = FixedPcdGet8 (PcdSystemTableSelect);
    Status                           = gRT->SetVariable (
                                              SYSTEM_TABLE_VAR,
                                              &gCixGlobalVariableGuid,
                                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                              VarSize,
                                              &SystemTableVar
                                              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: EfiSetVariable failed - %r\n", __FUNCTION__, Status));
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: EfiGetVariable Success - %r\n", __FUNCTION__, Status));
  }

  return Status;
}

EFI_STATUS
EFIAPI
PlatformSetupVariableInitDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                  Status      = EFI_SUCCESS;
  CONFIG_PARAMS_DATA_BLOCK                    *ConfigData = NULL;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL           *ConfigManage;
  PLATFORM_CONFIG_PARAMS_DATA_BLOCK           *PlatformConfigData = NULL;
  CIX_PLATFORM_CONFIG_PARAMS_MANAGE_PROTOCOL  *PlatformConfigManage;
  CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL       *PlatformSetupDataInfoProtocol;
  VOID                                        *Registration;
  EFI_HANDLE                                  Handle;

  Status = PlatformSetupVariableInit ();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: PlatformSetupVariableInit failed - %r\n", __FUNCTION__, Status));
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "%a: PlatformSetupVariableInit Success - %r\n", __FUNCTION__, Status));
  }

  Status = NetworkStackVariableInit ();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: NetworkStackVariableInit failed - %r\n", __FUNCTION__, Status));
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "%a: NetworkStackVariableInit Success - %r\n", __FUNCTION__, Status));
  }

  Status = SystemTableVariableInit ();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: SystemTableVariableInit failed - %r\n", __FUNCTION__, Status));
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "%a: SystemTableVariableInit Success - %r\n", __FUNCTION__, Status));
  }

  //
  Status = gBS->LocateProtocol (&gCixConfigParamsManageProtocolGuid, NULL, (VOID **)&ConfigManage);
  if (!EFI_ERROR (Status)) {
    ConfigData = ConfigManage->Data;
    if (ConfigData != NULL) {
      UpdateConfigParams (ConfigData);
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: locate gCixConfigParamsManageProtocolGuid failed - %r\n", __FUNCTION__, Status));

    EfiCreateProtocolNotifyEvent (
      &gCixConfigParamsManageProtocolGuid,
      TPL_CALLBACK,
      ConfigParamsUpdateCallBack,
      NULL,
      &Registration
      );
  }

  Status = gBS->LocateProtocol (&gCixPlatformConfigParamsManageProtocolGuid, NULL, (VOID **)&PlatformConfigManage);
  if (!EFI_ERROR (Status)) {
    PlatformConfigData = PlatformConfigManage->Data;
    if (PlatformConfigData != NULL) {
      UpdatePlatformConfigParams (PlatformConfigData);
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: locate gCixPlatformConfigParamsManageProtocolGuid failed - %r\n", __FUNCTION__, Status));

    EfiCreateProtocolNotifyEvent (
      &gCixPlatformConfigParamsManageProtocolGuid,
      TPL_CALLBACK,
      PlatformConfigParamsUpdateCallBack,
      NULL,
      &Registration
      );
  }

  Status = CheckCpuShareInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: CheckCpuShareInfo failed - %r\n", __FUNCTION__, Status));
  }

  PlatformSetupDataInfoProtocol          = AllocateZeroPool (sizeof (CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL));
  PlatformSetupDataInfoProtocol->Version = CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL_VERSION;
  CopyMem (&PlatformSetupDataInfoProtocol->Table, &mPlatformSetupDataInfoTable, sizeof (PLATFORM_SETUP_DATA_INFO_TABLE));

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gCixPlatformSetupDataInfoProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  PlatformSetupDataInfoProtocol
                  );

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gCixSetupVarInitCompleteProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );

  return Status;
}
