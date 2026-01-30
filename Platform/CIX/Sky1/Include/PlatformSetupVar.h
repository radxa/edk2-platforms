/**
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PLATFORM_SETUP_VAR_H_
#define _PLATFORM_SETUP_VAR_H_

#define PLATFORM_SETUP_VAR  L"PlatformSetupVar"
#define SYSTEM_TABLE_VAR    L"SystemTableVar"
#define MAX_CPU_CORE_NUM    12
#define MAX_PCIE_PORT_NUM   5
#define MAX_I2C_CTRL_NUM    8
#define MAX_USB_PORT_NUM    10
#define MAX_GMAC_PORT_NUM   2
#define MAX_DPU_PORT_NUM    5

#define CIX_PLATFORM_SETUP_VARIABLE_GUID   \
    {0x5E5B2ABF, 0x599A, 0x4329, {0xBA, 0xA4, 0x6D, 0x6E, 0xAA, 0xAC, 0x8B, 0xC1}}

#pragma pack(1)
typedef struct {
  UINT8     PcieRpEnable[MAX_PCIE_PORT_NUM];
  UINT8     PcieWidth[MAX_PCIE_PORT_NUM];
  UINT8     PcieMaxSpeed[MAX_PCIE_PORT_NUM];
  UINT8     PcieTargetLinkSpeed[MAX_PCIE_PORT_NUM];
  UINT8     PcieAspmMaxSupport[MAX_PCIE_PORT_NUM];
  UINT8     PcieAspm[MAX_PCIE_PORT_NUM];
  UINT8     PcieMaxPayload[MAX_PCIE_PORT_NUM];
  UINT8     PcieMaxReadRequest[MAX_PCIE_PORT_NUM];
  UINT8     PcieL1Substates[MAX_PCIE_PORT_NUM];
  UINT8     PcieDtiEnable[MAX_PCIE_PORT_NUM];
  UINT16    PcieLinkTimeout[MAX_PCIE_PORT_NUM];
  UINT8     I2cEnable[MAX_I2C_CTRL_NUM];
  UINT32    I2cBusFreq[MAX_I2C_CTRL_NUM];
  UINT8     GmacEnable[MAX_GMAC_PORT_NUM];
  UINT64    GmacMacAddr[MAX_GMAC_PORT_NUM];
  UINT8     Usb2Control0Enable;
  UINT8     Usb2Control1Enable;
  UINT8     Usb2Control2Enable;
  UINT8     Usb2Control3Enable;
  UINT8     Usb3Control0Enable;
  UINT8     Usb3Control1Enable;
  UINT8     UsbCDrdControl0Enable;
  UINT8     UsbCControl0Enable;
  UINT8     UsbCControl1Enable;
  UINT8     UsbCControl2Enable;
  UINT8     UsbCDrdControl0DataRole;
  UINT16    MemFreq;
  UINT8     MemBreakPoint;
  UINT8     MemEyeScan;
  UINT8     MemHarvesting;
  UINT8     WckAlwaysOn;
  UINT8     DataMask;
  UINT8     RfmEn;
  UINT8     AutoPrechargeEn;
  UINT8     PbrEn;
  UINT8     MbistEn;
  UINT8     MbistMode;
  UINT8     MemWrLEcc;
  UINT8     MemRdLEcc;
  UINT8     PortPriority;
  UINT8     BdwP0Override;
  UINT8     BdwP0;
  UINT8     BdwP1Override;
  UINT8     BdwP1;
  UINT8     MemRPriorityP0Override;
  UINT8     MemRPriorityP0;
  UINT8     MemWPriorityP0Override;
  UINT8     MemWPriorityP0;
  UINT8     MemBdwOvflowP0;
  UINT8     MemRPriorityP1Override;
  UINT8     MemRPriorityP1;
  UINT8     MemWPriorityP1Override;
  UINT8     MemWPriorityP1;
  UINT8     MemBdwOvflowP1;
  UINT8     MemIEcc;
  UINT8     MemWrDbi;
  UINT8     MemRdDbi;
  UINT8     StateAfterG3;
  UINT8     DtbMenuEntry;
  UINT8     SocWatchdogTimer;
  UINT16    VddSocVoltage;
  UINT16    VddGpuVoltage;
  UINT16    VddDpuVoltage;
  UINT16    VddCpuBigCore0Voltage;
  UINT16    VddCpuBigCore1Voltage;
  UINT16    VddCpuMidCore0Voltage;
  UINT16    VddCpuMidCore1Voltage;
  UINT16    VddCpuLitCoreVoltage;
  UINT8     GfxPower;
  UINT8     TouchPanelPower;
  UINT8     TpmPower;
  UINT8     WwanPower;
  UINT8     PcieX2SlotPower;
  UINT8     FingerPrintPower;
  UINT8     WlanPower;
  UINT8     M2SsdPower;
  UINT8     OnBoardLanPower;
  UINT8     IspCamera0Power;
  UINT8     IspCamera1Power;
  UINT8     IspCamera2Power;
  UINT8     IspCamera3Power;
  UINT8     CpuCoreClkGating;
  UINT8     DsuClkGating;
  UINT8     GicdClkGating;
  UINT8     Ci700ClkGating;
  UINT8     SysNi700ClkGating;
  UINT8     MmNi700ClkGating;
  UINT8     PcieNi700ClkGating;
  UINT8     SmnNi700ClkGating;
  UINT8     GpuClkGating;
  UINT8     Dpu0ClkGating;
  UINT8     Dpu1ClkGating;
  UINT8     Dpu2ClkGating;
  UINT8     Dpu3ClkGating;
  UINT8     Dpu4ClkGating;
  UINT8     VpuClkGating;
  UINT8     CpuCoreNum;
  UINT8     CpuCoreEnable[MAX_CPU_CORE_NUM];
  UINT32    CpuShareInfo;
  UINT8     RtcWakeup;
  UINT8     LightSensorCtrl;
  UINT8     CpuLpiState;
  UINT8     SpcrEnable;
  UINT8     CpuFMax;
  UINT8     EcFanMode;
  UINT8     CpuCppcType;
  UINT8     DebugMode;
  UINT8     TPMDeviceSelect;
} PLATFORM_SETUP_DATA;

typedef struct {
  UINT8    SystemTableSelect;
} SYSTEM_TABLE;

#pragma pack()

#endif
