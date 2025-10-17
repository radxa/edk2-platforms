/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ConfigParamsDataBlockInternal.h"

CONFIG_PARAMS_DATA_BLOCK  mConfigParamsDataBlock = {
  {
    // CONFIG_PARAMS_CPU
    1,
    {
      1, // cpu core 0 is boot core and never be disabled.
      FixedPcdGetBool (PcdCpuCore1En),
      FixedPcdGetBool (PcdCpuCore2En),
      FixedPcdGetBool (PcdCpuCore3En),
      FixedPcdGetBool (PcdCpuCore4En),
      FixedPcdGetBool (PcdCpuCore5En),
      FixedPcdGetBool (PcdCpuCore6En),
      FixedPcdGetBool (PcdCpuCore7En),
      FixedPcdGetBool (PcdCpuCore8En),
      FixedPcdGetBool (PcdCpuCore9En),
      FixedPcdGetBool (PcdCpuCore10En),
      FixedPcdGetBool (PcdCpuCore11En),
    },
    0,
    FixedPcdGet8 (PcdAcpiCpuLpiState),
  },
  {
    // CONFIG_PARAMS_GPU
  },
  {
    // CONFIG_PARAMS_DSU
  },
  {
    // CONFIG_PARAMS_GIC
  },
  {
    // CONFIG_PARAMS_DF
  },
  {
    // CONFIG_PARAMS_MEM
    FixedPcdGet16 (PcdMemFreq),
    FixedPcdGet8 (PcdMemBreakPoint),
    FixedPcdGet8 (PcdMemEyeScan),
    FixedPcdGet8 (PcdMemHarvesting),
    FixedPcdGet8 (PcdMemWckAlwaysOn),
    FixedPcdGet8 (PcdMemDataMask),
    FixedPcdGet8 (PcdMemRfmEn),
    FixedPcdGet8 (PcdMemAutoPrechargeEn),
    FixedPcdGet8 (PcdMemPbrEn),
    FixedPcdGet8 (PcdMemMbistEn),
    FixedPcdGet8 (PcdMemMbistMode),
    FixedPcdGet8 (PcdMemWrLEcc),
    FixedPcdGet8 (PcdMemRdLEcc),
    FixedPcdGet8 (PcdMemPortPriority),
    0,
    FixedPcdGet8 (PcdMemBdwP0),
    0,
    FixedPcdGet8 (PcdMemBdwP1),
    0,
    FixedPcdGet8 (PcdMemRPriorityP0),
    0,
    FixedPcdGet8 (PcdMemWPriorityP0),
    FixedPcdGet8 (PcdMemBdwOvflowP0),
    0,
    FixedPcdGet8 (PcdMemRPriorityP1),
    0,
    FixedPcdGet8 (PcdMemWPriorityP1),
    FixedPcdGet8 (PcdMemBdwOvflowP1),
    FixedPcdGet8 (PcdMemIEcc),
    FixedPcdGet8 (PcdMemWrDbi),
    FixedPcdGet8 (PcdMemRdDbi),
  },
  {
    // CONFIG_PARAMS_PCIE
    {
      FixedPcdGetBool (PcdPcieRootPort0Enable),
      FixedPcdGetBool (PcdPcieRootPort1Enable),
      FixedPcdGetBool (PcdPcieRootPort2Enable),
      FixedPcdGetBool (PcdPcieRootPort3Enable),
      FixedPcdGetBool (PcdPcieRootPort4Enable),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0LaneNum),
      FixedPcdGet8 (PcdPcieRootPort1LaneNum),
      FixedPcdGet8 (PcdPcieRootPort2LaneNum),
      FixedPcdGet8 (PcdPcieRootPort3LaneNum),
      FixedPcdGet8 (PcdPcieRootPort4LaneNum),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0MaxSpeed),
      FixedPcdGet8 (PcdPcieRootPort1MaxSpeed),
      FixedPcdGet8 (PcdPcieRootPort2MaxSpeed),
      FixedPcdGet8 (PcdPcieRootPort3MaxSpeed),
      FixedPcdGet8 (PcdPcieRootPort4MaxSpeed),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0SpeedGen),
      FixedPcdGet8 (PcdPcieRootPort1SpeedGen),
      FixedPcdGet8 (PcdPcieRootPort2SpeedGen),
      FixedPcdGet8 (PcdPcieRootPort3SpeedGen),
      FixedPcdGet8 (PcdPcieRootPort4SpeedGen),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0MaxAspmSupport),
      FixedPcdGet8 (PcdPcieRootPort1MaxAspmSupport),
      FixedPcdGet8 (PcdPcieRootPort2MaxAspmSupport),
      FixedPcdGet8 (PcdPcieRootPort3MaxAspmSupport),
      FixedPcdGet8 (PcdPcieRootPort4MaxAspmSupport),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0AspmSupport),
      FixedPcdGet8 (PcdPcieRootPort1AspmSupport),
      FixedPcdGet8 (PcdPcieRootPort2AspmSupport),
      FixedPcdGet8 (PcdPcieRootPort3AspmSupport),
      FixedPcdGet8 (PcdPcieRootPort4AspmSupport),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0MaxPayload),
      FixedPcdGet8 (PcdPcieRootPort1MaxPayload),
      FixedPcdGet8 (PcdPcieRootPort2MaxPayload),
      FixedPcdGet8 (PcdPcieRootPort3MaxPayload),
      FixedPcdGet8 (PcdPcieRootPort4MaxPayload),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0MaxReadRequest),
      FixedPcdGet8 (PcdPcieRootPort1MaxReadRequest),
      FixedPcdGet8 (PcdPcieRootPort2MaxReadRequest),
      FixedPcdGet8 (PcdPcieRootPort3MaxReadRequest),
      FixedPcdGet8 (PcdPcieRootPort4MaxReadRequest),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0L1SubStates),
      FixedPcdGet8 (PcdPcieRootPort1L1SubStates),
      FixedPcdGet8 (PcdPcieRootPort2L1SubStates),
      FixedPcdGet8 (PcdPcieRootPort3L1SubStates),
      FixedPcdGet8 (PcdPcieRootPort4L1SubStates),
    },
    {
      0,
      0,
      0,
      0,
      0,
    },
    {
      FixedPcdGetBool (PcdPcieRootPort0DevicePower),
      FixedPcdGetBool (PcdPcieRootPort1DevicePower),
      FixedPcdGetBool (PcdPcieRootPort2DevicePower),
      FixedPcdGetBool (PcdPcieRootPort3DevicePower),
      FixedPcdGetBool (PcdPcieRootPort4DevicePower),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0PeResetPin),
      FixedPcdGet8 (PcdPcieRootPort1PeResetPin),
      FixedPcdGet8 (PcdPcieRootPort2PeResetPin),
      FixedPcdGet8 (PcdPcieRootPort3PeResetPin),
      FixedPcdGet8 (PcdPcieRootPort4PeResetPin),
    },
    {
      FixedPcdGet8 (PcdPcieRootPort0DtiEnable),
      FixedPcdGet8 (PcdPcieRootPort1DtiEnable),
      FixedPcdGet8 (PcdPcieRootPort2DtiEnable),
      FixedPcdGet8 (PcdPcieRootPort3DtiEnable),
      FixedPcdGet8 (PcdPcieRootPort4DtiEnable),
    },
  },
  {
    // CONFIG_PARAMS_DPU
    {
      FixedPcdGetBool (PcdDP0Enable),
      FixedPcdGetBool (PcdDP1Enable),
      FixedPcdGetBool (PcdDP2Enable),
      FixedPcdGetBool (PcdDP3Enable),
      FixedPcdGetBool (PcdDP4Enable),
    },
    {
      FixedPcdGet8 (PcdDPPriority0),
      FixedPcdGet8 (PcdDPPriority1),
      FixedPcdGet8 (PcdDPPriority2),
      FixedPcdGet8 (PcdDPPriority3),
      FixedPcdGet8 (PcdDPPriority4),
    },
  },
  {
    // CONFIG_PARAMS_NPU
  },
  {
    // CONFIG_PARAMS_VPU
  },
  {
    // CONFIG_PARAMS_ISP
  },
  {
    // CONFIG_PARAMS_CSI
  },
  {
    // CONFIG_PARAMS_USB_20
    {
      FixedPcdGetBool (PcdUsb2Control0Enable),
      FixedPcdGet8 (PcdUsb2Control0MaxSpeed),
    },
    {
      FixedPcdGetBool (PcdUsb2Control1Enable),
      FixedPcdGet8 (PcdUsb2Control1MaxSpeed),
    },
    {
      FixedPcdGetBool (PcdUsb2Control2Enable),
      FixedPcdGet8 (PcdUsb2Control2MaxSpeed),
    },
    {
      FixedPcdGetBool (PcdUsb2Control3Enable),
      FixedPcdGet8 (PcdUsb2Control3MaxSpeed),
    },
  },
  {
    // CONFIG_PARAMS_USB_32_DRD
    {
      FixedPcdGetBool (PcdUsb3Control0Enable),
      FixedPcdGet8 (PcdUsb3Control0MaxSpeed),
      FixedPcdGetBool (PcdUsb3Control0DataRole),
      FixedPcdGetBool (PcdUsb3Control0VbusDetect),
    },
    {
      FixedPcdGetBool (PcdUsb3Control1Enable),
      FixedPcdGet8 (PcdUsb3Control1MaxSpeed),
      FixedPcdGetBool (PcdUsb3Control1DataRole),
      FixedPcdGetBool (PcdUsb3Control1VbusDetect),
    },
  },
  {
    // CONFIG_PARAMS_USB_TYPEC_DRD
    {
      FixedPcdGetBool (PcdUsbCDrdControl0Enable),
      FixedPcdGet8 (PcdUsbCDrdControl0MaxSpeed),
      FixedPcdGetBool (PcdUsbCDrdControl0DataRole),
      FixedPcdGetBool (PcdUsbCDrdControl0VbusDetect),
    },
  },
  {
    // CONFIG_PARAMS_USB_TYPE_C
    {
      FixedPcdGetBool (PcdUsbCControl0Enable),
      FixedPcdGet8 (PcdUsbCControl0MaxSpeed),
    },
    {
      FixedPcdGetBool (PcdUsbCControl1Enable),
      FixedPcdGet8 (PcdUsbCControl1MaxSpeed),
    },
    {
      FixedPcdGetBool (PcdUsbCControl2Enable),
      FixedPcdGet8 (PcdUsbCControl2MaxSpeed),
    },
  },
  {
    // CONFIG_PARAMS_GMAC
    {
      FixedPcdGetBool (PcdGmac0Enable),         // Enable
      FixedPcdGet32 (PcdGmacApbClockMHz),       // ApbClockMHz
      FixedPcdGet32 (PcdGmac0ClockMHz),         // ClockMHz
      FixedPcdGet8 (PcdGmac0QueueEnMap),        // QueueEnMap
      FixedPcdGet8 (PcdGmac0Queue0SegmentNum),  // Queue0SegmentNum
      FixedPcdGet8 (PcdGmac0Queue1SegmentNum),  // Queue1SegmentNum
      FixedPcdGet8 (PcdGmac0Queue2SegmentNum),  // Queue2SegmentNum
      FixedPcdGet8 (PcdGmac0Queue3SegmentNum),  // Queue3SegmentNum
      FixedPcdGet8 (PcdGmac0Queue4SegmentNum),  // Queue4SegmentNum
      FixedPcdGet8 (PcdGmac0Queue5SegmentNum),  // Queue5SegmentNum
      FixedPcdGet8 (PcdGmac0Queue6SegmentNum),  // Queue6SegmentNum
      FixedPcdGet8 (PcdGmac0Queue7SegmentNum),  // Queue7SegmentNum
      FixedPcdGetBool (PcdGmac0DuplexEn),       // DuplexEn
      FixedPcdGetBool (PcdGmac0RxPauseEn),      // RxPauseEn
      FixedPcdGetBool (PcdGmac0IntEn),          // IntEn
      FixedPcdGet64 (PcdGmac0MacAddress)        // MacAddress
    },
    {
      FixedPcdGetBool (PcdGmac1Enable),         // Enable
      FixedPcdGet32 (PcdGmacApbClockMHz),       // ApbClockMHz
      FixedPcdGet32 (PcdGmac1ClockMHz),         // ClockMHz
      FixedPcdGet8 (PcdGmac1QueueEnMap),        // QueueEnMap
      FixedPcdGet8 (PcdGmac1Queue0SegmentNum),  // Queue0SegmentNum
      FixedPcdGet8 (PcdGmac1Queue1SegmentNum),  // Queue1SegmentNum
      FixedPcdGet8 (PcdGmac1Queue2SegmentNum),  // Queue2SegmentNum
      FixedPcdGet8 (PcdGmac1Queue3SegmentNum),  // Queue3SegmentNum
      FixedPcdGet8 (PcdGmac1Queue4SegmentNum),  // Queue4SegmentNum
      FixedPcdGet8 (PcdGmac1Queue5SegmentNum),  // Queue5SegmentNum
      FixedPcdGet8 (PcdGmac1Queue6SegmentNum),  // Queue6SegmentNum
      FixedPcdGet8 (PcdGmac1Queue7SegmentNum),  // Queue7SegmentNum
      FixedPcdGetBool (PcdGmac1DuplexEn),       // DuplexEn
      FixedPcdGetBool (PcdGmac1RxPauseEn),      // RxPauseEn
      FixedPcdGetBool (PcdGmac1IntEn),          // IntEn
      FixedPcdGet64 (PcdGmac1MacAddress)        // MacAddress
    },
  },
  {
    // CONFIG_PARAMS_SENSOR_FUSION
  },
  {
    // CONFIG_PARAMS_AUDIO
    FixedPcdGetBool (PcdAudioHdaEn),        // HdaEn
  },
  {
    // CONFIG_PARAMS_LSIO_FCH
    {
      // CONFIG_PARAMS_LSIO_FCH_I2C
      {
        FixedPcdGetBool (PcdI2c0En),       // Enable
        FixedPcdGet32 (PcdI2c0BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c0MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c1En),       // Enable
        FixedPcdGet32 (PcdI2c1BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c1MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c2En),       // Enable
        FixedPcdGet32 (PcdI2c2BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c2MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c3En),       // Enable
        FixedPcdGet32 (PcdI2c3BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c3MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c4En),       // Enable
        FixedPcdGet32 (PcdI2c4BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c4MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c5En),       // Enable
        FixedPcdGet32 (PcdI2c5BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c5MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c6En),       // Enable
        FixedPcdGet32 (PcdI2c6BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c6MutexId)      // MutexId
      },
      {
        FixedPcdGetBool (PcdI2c7En),       // Enable
        FixedPcdGet32 (PcdI2c7BusFreq),    // BusFreq
        FixedPcdGet8 (PcdI2c7MutexId)      // MutexId
      },
    },
    {
      // CONFIG_PARAMS_LSIO_FCH_SPI
      {
        FixedPcdGetBool (PcdSpi0En),       // Enable
        FixedPcdGet32 (PcdSpi0BusFreqMHz), // BusFreq
        FixedPcdGet8 (PcdSpi0ChipSelect),  // ChipSelect
        FixedPcdGet8 (PcdSpi0Mode)         // Mode
      },
      {
        FixedPcdGetBool (PcdSpi1En),       // Enable
        FixedPcdGet32 (PcdSpi1BusFreqMHz), // BusFreq
        FixedPcdGet8 (PcdSpi1ChipSelect),  // ChipSelect
        FixedPcdGet8 (PcdSpi1Mode)         // Mode
      },
    }
  },
  {
    // CONFIG_PARAMS_S5_SS
    FixedPcdGet8 (PcdSocWatchdogTimer),
  },
  {
    // CONFIG_PARAMS_PM
    FixedPcdGet16 (PcdVddSocVoltage),
    FixedPcdGet16 (PcdVddGpuVoltage),
    FixedPcdGet16 (PcdVddDpuVoltage),
    FixedPcdGet16 (PcdVddCpuBigCore0Voltage),
    FixedPcdGet16 (PcdVddCpuBigCore1Voltage),
    FixedPcdGet16 (PcdVddCpuMidCore0Voltage),
    FixedPcdGet16 (PcdVddCpuMidCore1Voltage),
    FixedPcdGet16 (PcdVddCpuLitCoreVoltage),
    FixedPcdGet8 (PcdCpuCoreClkGating),
    FixedPcdGet8 (PcdDsuClkGating),
    FixedPcdGet8 (PcdGicdClkGating),
    FixedPcdGet8 (PcdCi700ClkGating),
    FixedPcdGet8 (PcdSysNi700ClkGating),
    FixedPcdGet8 (PcdMmNi700ClkGating),
    FixedPcdGet8 (PcdPcieNi700ClkGating),
    FixedPcdGet8 (PcdSmnNi700ClkGating),
    FixedPcdGet8 (PcdGpuClkGating),
    FixedPcdGet8 (PcdDpu0ClkGating),
    FixedPcdGet8 (PcdDpu1ClkGating),
    FixedPcdGet8 (PcdDpu2ClkGating),
    FixedPcdGet8 (PcdDpu3ClkGating),
    FixedPcdGet8 (PcdDpu4ClkGating),
    FixedPcdGet8 (PcdVpuClkGating),
  },
  {
    // CONFIG_PARAMS_MISC
    FixedPcdGet8 (PcdAcpiCppcType),
  },
  {
    //CONFIG_USB20_PHY
  },
  {
    //CONFIG_USB32_PHY
  },
  {
    //CONFIG_USBCOM_PHY
  },
  {
    // CONFIG_PARAMS_SPI
    FixedPcdGet8 (PcdDefaultTpmDeviceSelect)
  }
};

CONFIG_PARAMS_DATA_ENTRY  mConfigDataEntryTable[] = {
  { PARAMS_DATA_GMAC0_ENABLE_ID,               PARAMS_DATA_OFFSET_SIZE (Gmac[0].Enable),              PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC0 enable",                 L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC0_CLOCKMHZ_ID,             PARAMS_DATA_OFFSET_SIZE (Gmac[0].ClockMHz),            PARAMS_DATA_MULTI_OPTION_TYPE, L"GMAC0 clock",                  L"10:10MHz, 100:100MHz, 1000:1000MHz"                                                                                                                     },
  { PARAMS_DATA_GMAC0_DUPLEXEN_ID,             PARAMS_DATA_OFFSET_SIZE (Gmac[0].DuplexEn),            PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC0 duplex enable",          L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC0_RXPAUSEEN_ID,            PARAMS_DATA_OFFSET_SIZE (Gmac[0].RxPauseEn),           PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC0 RX pause enable",        L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC0_INTEN_ID,                PARAMS_DATA_OFFSET_SIZE (Gmac[0].IntEn),               PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC0 interrupt enable",       L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC0_MACADDRESS_ID,           PARAMS_DATA_OFFSET_SIZE (Gmac[0].MacAddress),          PARAMS_DATA_INTEGER_TYPE,      L"GMAC0 MAC address",            L"(hex)"                                                                                                                                                  },
  { PARAMS_DATA_GMAC1_ENABLE_ID,               PARAMS_DATA_OFFSET_SIZE (Gmac[1].Enable),              PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC1 enable",                 L"0:disable,1:enable"                                                                                                                                     },
  { PARAMS_DATA_GMAC1_CLOCKMHZ_ID,             PARAMS_DATA_OFFSET_SIZE (Gmac[1].ClockMHz),            PARAMS_DATA_MULTI_OPTION_TYPE, L"GMAC1 clock",                  L"10:10MHz, 100:100MHz, 1000:1000MHz"                                                                                                                     },
  { PARAMS_DATA_GMAC1_DUPLEXEN_ID,             PARAMS_DATA_OFFSET_SIZE (Gmac[1].DuplexEn),            PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC1 duplex enable",          L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC1_RXPAUSEEN_ID,            PARAMS_DATA_OFFSET_SIZE (Gmac[1].RxPauseEn),           PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC1 RX pause enable",        L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC1_INTEN_ID,                PARAMS_DATA_OFFSET_SIZE (Gmac[1].IntEn),               PARAMS_DATA_BOOLEAN_TYPE,      L"GMAC1 interrupt enable",       L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_GMAC1_MACADDRESS_ID,           PARAMS_DATA_OFFSET_SIZE (Gmac[1].MacAddress),          PARAMS_DATA_INTEGER_TYPE,      L"GMAC1 MAC address",            L"(hex)"                                                                                                                                                  },
  { PARAMS_DATA_AUDIO_HDAEN_ID,                PARAMS_DATA_OFFSET_SIZE (Audio.HdaEn),                 PARAMS_DATA_BOOLEAN_TYPE,      L"audio HDA enable",             L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C0_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[0].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C0 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C0_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[0].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C0 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C1_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[1].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C1 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C1_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[1].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C1 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C2_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[2].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C2 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C2_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[2].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C2 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C3_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[3].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C3 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C3_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[3].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C3 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C4_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[4].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C4 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C4_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[4].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C4 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C5_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[5].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C5 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C5_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[5].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C5 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C6_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[6].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C6 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C6_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[6].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C6 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_I2C7_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.I2c[7].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH I2C7 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_I2C7_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.I2c[7].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH I2C7 bus frequency",       L"100000:100KHz, 400000:400KHz, 1000000:1MHz"                                                                                                             },
  { PARAMS_DATA_LSIO_FCH_SPI0_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.Spi[0].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH SPI0 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_SPI0_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.Spi[0].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI0 bus frequency(MHz)",  L"5:5MHz, 10:10MHz, 20:20MHz, 25:25MHz"                                                                                                                   },
  { PARAMS_DATA_LSIO_FCH_SPI0_CS_ID,           PARAMS_DATA_OFFSET_SIZE (Fch.Spi[0].ChipSelect),       PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI0 chip select",         L"1:chip 0 selected, 2:chip 1 selected, 15:no_chip selected"                                                                                              },
  { PARAMS_DATA_LSIO_FCH_SPI0_MODE_ID,         PARAMS_DATA_OFFSET_SIZE (Fch.Spi[0].Mode),             PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI0 mode",                L"0:CPOL-0 CPHA-0, 1:CPOL-0 CPHA-1, 2:CPOL-1 CPHA-0, 3:CPOL-1 CPHA-1"                                                                                     },
  { PARAMS_DATA_LSIO_FCH_SPI1_ENABLE_ID,       PARAMS_DATA_OFFSET_SIZE (Fch.Spi[1].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"FCH SPI1 enable",              L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_LSIO_FCH_SPI1_BUSFREQ_ID,      PARAMS_DATA_OFFSET_SIZE (Fch.Spi[1].BusFreq),          PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI1 bus frequency(MHz)",  L"5:5MHz, 10:10MHz, 20:20MHz, 25:25MHz"                                                                                                                   },
  { PARAMS_DATA_LSIO_FCH_SPI1_CS_ID,           PARAMS_DATA_OFFSET_SIZE (Fch.Spi[1].ChipSelect),       PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI1 chip select",         L"1:chip 0 selected, 2:chip 1 selected, 15:no_chip selected"                                                                                              },
  { PARAMS_DATA_LSIO_FCH_SPI1_MODE_ID,         PARAMS_DATA_OFFSET_SIZE (Fch.Spi[1].Mode),             PARAMS_DATA_MULTI_OPTION_TYPE, L"FCH SPI1 mode",                L"0:CPOL-0 CPHA-0, 1:CPOL-0 CPHA-1, 2:CPOL-1 CPHA-0, 3:CPOL-1 CPHA-1"                                                                                     },
  { PARAMS_DATA_PCIE_RP0_ID,                   PARAMS_DATA_OFFSET_SIZE (Pcie.PcieRpEnable[0]),        PARAMS_DATA_BOOLEAN_TYPE,      L"PCI Express Root Port 1",      L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP1_ID,                   PARAMS_DATA_OFFSET_SIZE (Pcie.PcieRpEnable[1]),        PARAMS_DATA_BOOLEAN_TYPE,      L"PCI Express Root Port 2",      L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP2_ID,                   PARAMS_DATA_OFFSET_SIZE (Pcie.PcieRpEnable[2]),        PARAMS_DATA_BOOLEAN_TYPE,      L"PCI Express Root Port 3",      L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP3_ID,                   PARAMS_DATA_OFFSET_SIZE (Pcie.PcieRpEnable[3]),        PARAMS_DATA_BOOLEAN_TYPE,      L"PCI Express Root Port 4",      L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP4_ID,                   PARAMS_DATA_OFFSET_SIZE (Pcie.PcieRpEnable[4]),        PARAMS_DATA_BOOLEAN_TYPE,      L"PCI Express Root Port 5",      L"0:disable, 1:enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP0_WIDTH_ID,             PARAMS_DATA_OFFSET_SIZE (Pcie.PcieWidth[0]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Bandwidth",         L"0:x1, 1:x2, 2:x4, 3:x8"                                                                                                                                 },
  { PARAMS_DATA_PCIE_RP1_WIDTH_ID,             PARAMS_DATA_OFFSET_SIZE (Pcie.PcieWidth[1]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Bandwidth",         L"0:x1, 1:x2, 2:x4"                                                                                                                                       },
  { PARAMS_DATA_PCIE_RP2_WIDTH_ID,             PARAMS_DATA_OFFSET_SIZE (Pcie.PcieWidth[2]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Bandwidth",         L"0:x1, 1:x2"                                                                                                                                             },
  { PARAMS_DATA_PCIE_RP3_WIDTH_ID,             PARAMS_DATA_OFFSET_SIZE (Pcie.PcieWidth[3]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Bandwidth",         L"0:x1"                                                                                                                                                   },
  { PARAMS_DATA_PCIE_RP4_WIDTH_ID,             PARAMS_DATA_OFFSET_SIZE (Pcie.PcieWidth[4]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Bandwidth",         L"0:x1"                                                                                                                                                   },
  { PARAMS_DATA_PCIE_RP0_MAX_SPEED_ID,         PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxSpeed[0]),        PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Max Speed",         L"0:GEN1, 1:GEN2, 2:GEN3, 3:GEN4"                                                                                                                         },
  { PARAMS_DATA_PCIE_RP1_MAX_SPEED_ID,         PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxSpeed[1]),        PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Max Speed",         L"0:GEN1, 1:GEN2, 2:GEN3, 3:GEN4"                                                                                                                         },
  { PARAMS_DATA_PCIE_RP2_MAX_SPEED_ID,         PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxSpeed[2]),        PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Max Speed",         L"0:GEN1, 1:GEN2, 2:GEN3, 3:GEN4"                                                                                                                         },
  { PARAMS_DATA_PCIE_RP3_MAX_SPEED_ID,         PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxSpeed[3]),        PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Max Speed",         L"0:GEN1, 1:GEN2, 2:GEN3, 3:GEN4"                                                                                                                         },
  { PARAMS_DATA_PCIE_RP4_MAX_SPEED_ID,         PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxSpeed[4]),        PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Max Speed",         L"0:GEN1, 1:GEN2, 2:GEN3, 3:GEN4"                                                                                                                         },
  { PARAMS_DATA_PCIE_RP0_TARGET_LINK_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieTargetLinkSpeed[0]), PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Target Link Speed", L"1:2.5 GT/s, 2:5 GT/s, 3:8 GT/s, 4:16 GT/s"                                                                                                              },
  { PARAMS_DATA_PCIE_RP1_TARGET_LINK_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieTargetLinkSpeed[1]), PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Target Link Speed", L"1:2.5 GT/s, 2:5 GT/s, 3:8 GT/s, 4:16 GT/s"                                                                                                              },
  { PARAMS_DATA_PCIE_RP2_TARGET_LINK_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieTargetLinkSpeed[2]), PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Target Link Speed", L"1:2.5 GT/s, 2:5 GT/s, 3:8 GT/s, 4:16 GT/s"                                                                                                              },
  { PARAMS_DATA_PCIE_RP3_TARGET_LINK_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieTargetLinkSpeed[3]), PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Target Link Speed", L"1:2.5 GT/s, 2:5 GT/s, 3:8 GT/s, 4:16 GT/s"                                                                                                              },
  { PARAMS_DATA_PCIE_RP4_TARGET_LINK_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieTargetLinkSpeed[4]), PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Target Link Speed", L"1:2.5 GT/s, 2:5 GT/s, 3:8 GT/s, 4:16 GT/s"                                                                                                              },
  { PARAMS_DATA_PCIE_RP0_MAX_ASPM_ID,          PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspmMaxSupport[0]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 ASPM Max Support",  L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP1_MAX_ASPM_ID,          PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspmMaxSupport[1]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 ASPM Max Support",  L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP2_MAX_ASPM_ID,          PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspmMaxSupport[2]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 ASPM Max Support",  L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP3_MAX_ASPM_ID,          PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspmMaxSupport[3]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 ASPM Max Support",  L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP4_MAX_ASPM_ID,          PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspmMaxSupport[4]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 ASPM Max Support",  L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP0_ASPM_ID,              PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspm[0]),            PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 ASPM",              L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP1_ASPM_ID,              PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspm[1]),            PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 ASPM",              L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP2_ASPM_ID,              PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspm[2]),            PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 ASPM",              L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP3_ASPM_ID,              PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspm[3]),            PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 ASPM",              L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP4_ASPM_ID,              PARAMS_DATA_OFFSET_SIZE (Pcie.PcieAspm[4]),            PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 ASPM",              L"0:disable, 1:L0ss, 2:L1, 3:L0sL1"                                                                                                                       },
  { PARAMS_DATA_PCIE_RP0_MAX_PAYLOAD_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxPayload[0]),      PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Max Payload",       L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP1_MAX_PAYLOAD_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxPayload[1]),      PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Max Payload",       L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP2_MAX_PAYLOAD_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxPayload[2]),      PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Max Payload",       L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP3_MAX_PAYLOAD_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxPayload[3]),      PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Max Payload",       L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP4_MAX_PAYLOAD_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxPayload[4]),      PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Max Payload",       L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP0_MAX_READ_REQUEST_ID,  PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxReadRequest[0]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Max Read Request",  L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP1_MAX_READ_REQUEST_ID,  PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxReadRequest[1]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Max Read Request",  L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP2_MAX_READ_REQUEST_ID,  PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxReadRequest[2]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Max Read Request",  L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP3_MAX_READ_REQUEST_ID,  PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxReadRequest[3]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Max Read Request",  L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP4_MAX_READ_REQUEST_ID,  PARAMS_DATA_OFFSET_SIZE (Pcie.PcieMaxReadRequest[4]),  PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Max Read Request",  L"0:128 Bytes, 1:256 Bytes, 2:512 Bytes, 3:1024 Bytes, 4:2048 Bytes, 5:4096 Bytes"                                                                        },
  { PARAMS_DATA_PCIE_RP0_LINK_STATUS_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieLinkUpStatus[0]),    PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 Linkup Status",     L"0:FALSE, 1:TRUE"                                                                                                                                        },
  { PARAMS_DATA_PCIE_RP1_LINK_STATUS_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieLinkUpStatus[1]),    PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 Linkup Status",     L"0:FALSE, 1:TRUE"                                                                                                                                        },
  { PARAMS_DATA_PCIE_RP2_LINK_STATUS_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieLinkUpStatus[2]),    PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 Linkup Status",     L"0:FALSE, 1:TRUE"                                                                                                                                        },
  { PARAMS_DATA_PCIE_RP3_LINK_STATUS_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieLinkUpStatus[3]),    PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 Linkup Status",     L"0:FALSE, 1:TRUE"                                                                                                                                        },
  { PARAMS_DATA_PCIE_RP4_LINK_STATUS_ID,       PARAMS_DATA_OFFSET_SIZE (Pcie.PcieLinkUpStatus[4]),    PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 Linkup Status",     L"0:FALSE, 1:TRUE"                                                                                                                                        },
  { PARAMS_DATA_PCIE_RP0_DTI_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Pcie.PcieDtiEnable[0]),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port0 DTI Enable",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP1_DTI_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Pcie.PcieDtiEnable[1]),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port1 DTI Enable",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP2_DTI_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Pcie.PcieDtiEnable[2]),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port2 DTI Enable",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP3_DTI_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Pcie.PcieDtiEnable[3]),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port3 DTI Enable",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_RP4_DTI_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Pcie.PcieDtiEnable[4]),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE Port4 DTI Enable",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_USB_20_0_ENABLE_ID,            PARAMS_DATA_OFFSET_SIZE (Usb20[0].Enable),             PARAMS_DATA_BOOLEAN_TYPE,      L"USB2.0 Control0 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_20_1_ENABLE_ID,            PARAMS_DATA_OFFSET_SIZE (Usb20[1].Enable),             PARAMS_DATA_BOOLEAN_TYPE,      L"USB2.0 Control1 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_20_2_ENABLE_ID,            PARAMS_DATA_OFFSET_SIZE (Usb20[2].Enable),             PARAMS_DATA_BOOLEAN_TYPE,      L"USB2.0 Control2 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_20_3_ENABLE_ID,            PARAMS_DATA_OFFSET_SIZE (Usb20[3].Enable),             PARAMS_DATA_BOOLEAN_TYPE,      L"USB2.0 Control3 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_32_DRD_0_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Usb32Drd[0].Enable),          PARAMS_DATA_BOOLEAN_TYPE,      L"USB3.0 Control0 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_32_DRD_1_ENABLE_ID,        PARAMS_DATA_OFFSET_SIZE (Usb32Drd[1].Enable),          PARAMS_DATA_BOOLEAN_TYPE,      L"USB3.0 Control1 Enable",       L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_TYPE_C_DRD_0_ENABLE_ID,    PARAMS_DATA_OFFSET_SIZE (UsbCDrd[0].Enable),           PARAMS_DATA_BOOLEAN_TYPE,      L"USBC Drd Control Enable",      L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_TYPE_C_HOST_0_ENABLE_ID,   PARAMS_DATA_OFFSET_SIZE (UsbC[0].Enable),              PARAMS_DATA_BOOLEAN_TYPE,      L"USBC host Control0 Enable",    L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_TYPE_C_HOST_1_ENABLE_ID,   PARAMS_DATA_OFFSET_SIZE (UsbC[1].Enable),              PARAMS_DATA_BOOLEAN_TYPE,      L"USBC host Control1 Enable",    L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_TYPE_C_HOST_2_ENABLE_ID,   PARAMS_DATA_OFFSET_SIZE (UsbC[2].Enable),              PARAMS_DATA_BOOLEAN_TYPE,      L"USBC host Control2 Enable",    L"0:false, 1:true"                                                                                                                                        },
  { PARAMS_DATA_USB_20_0_MAXSPEED_ID,          PARAMS_DATA_OFFSET_SIZE (Usb20[0].MaxSpeed),           PARAMS_DATA_MULTI_OPTION_TYPE, L"USB2.0 Control0 Max Speed",    L"3:high, 2:full"                                                                                                                                         },
  { PARAMS_DATA_USB_20_1_MAXSPEED_ID,          PARAMS_DATA_OFFSET_SIZE (Usb20[1].MaxSpeed),           PARAMS_DATA_MULTI_OPTION_TYPE, L"USB2.0 Control1 Max Speed",    L"3:high, 2:full"                                                                                                                                         },
  { PARAMS_DATA_USB_20_2_MAXSPEED_ID,          PARAMS_DATA_OFFSET_SIZE (Usb20[2].MaxSpeed),           PARAMS_DATA_MULTI_OPTION_TYPE, L"USB2.0 Control2 Max Speed",    L"3:high, 2:full"                                                                                                                                         },
  { PARAMS_DATA_USB_20_3_MAXSPEED_ID,          PARAMS_DATA_OFFSET_SIZE (Usb20[3].MaxSpeed),           PARAMS_DATA_MULTI_OPTION_TYPE, L"USB2.0 Control3 Max Speed",    L"3:high, 2:full"                                                                                                                                         },
  { PARAMS_DATA_USB_32_DRD_0_MAXSPEED_ID,      PARAMS_DATA_OFFSET_SIZE (Usb32Drd[0].MaxSpeed),        PARAMS_DATA_MULTI_OPTION_TYPE, L"USB3.0 Control0 Max Speed",    L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_32_DRD_1_MAXSPEED_ID,      PARAMS_DATA_OFFSET_SIZE (Usb32Drd[1].MaxSpeed),        PARAMS_DATA_MULTI_OPTION_TYPE, L"USB3.0 Control1 Max Speed",    L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_TYPE_C_DRD_0_MAXSPEED_ID,  PARAMS_DATA_OFFSET_SIZE (UsbCDrd[0].MaxSpeed),         PARAMS_DATA_MULTI_OPTION_TYPE, L"USBC Drd Control Max Speed",   L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_TYPE_C_HOST_0_MAXSPEED_ID, PARAMS_DATA_OFFSET_SIZE (UsbC[0].MaxSpeed),            PARAMS_DATA_MULTI_OPTION_TYPE, L"USBC host Control0 Max Speed", L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_TYPE_C_HOST_1_MAXSPEED_ID, PARAMS_DATA_OFFSET_SIZE (UsbC[1].MaxSpeed),            PARAMS_DATA_MULTI_OPTION_TYPE, L"USBC host Control1 Max Speed", L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_TYPE_C_HOST_2_MAXSPEED_ID, PARAMS_DATA_OFFSET_SIZE (UsbC[2].MaxSpeed),            PARAMS_DATA_MULTI_OPTION_TYPE, L"USBC host Control2 Max Speed", L"3:high, 2:full, 4:super, 5:super plus"                                                                                                                  },
  { PARAMS_DATA_USB_32_DRD_0_DATAROLE_ID,      PARAMS_DATA_OFFSET_SIZE (Usb32Drd[0].DataRole),        PARAMS_DATA_BOOLEAN_TYPE,      L"USB3.0 Control0 data role",    L"0:host, 1:device"                                                                                                                                       },
  { PARAMS_DATA_USB_32_DRD_1_DATAROLE_ID,      PARAMS_DATA_OFFSET_SIZE (Usb32Drd[1].DataRole),        PARAMS_DATA_BOOLEAN_TYPE,      L"USB3.0 Control1 data role",    L"0:host, 1:device"                                                                                                                                       },
  { PARAMS_DATA_USB_TYPE_C_DRD_0_DATAROLE_ID,  PARAMS_DATA_OFFSET_SIZE (UsbCDrd[0].DataRole),         PARAMS_DATA_BOOLEAN_TYPE,      L"USBC Drd Control data role",   L"0:host, 1:device"                                                                                                                                       },
  { PARAMS_DATA_SOC_WATCH_DOG_TIMER_ID,        PARAMS_DATA_OFFSET_SIZE (S5.SocWatchdogTimer),         PARAMS_DATA_BOOLEAN_TYPE,      L"SOC Watchdog  Timer",          L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_LPI_STATE_ID,              PARAMS_DATA_OFFSET_SIZE (Cpu.LpiState),                PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU LPI State ",               L"0:Disable, 1:LPI0, 2:LPI1, 3:LPI2"                                                                                                                      },
  { PARAMS_DATA_CPU_CORE_1_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[1]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 1",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_2_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[2]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 2",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_3_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[3]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 3",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_4_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[4]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 4",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_5_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[5]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 5",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_6_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[6]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 6",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_7_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[7]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 7",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_8_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[8]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 8",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_9_ENABLE_ID,          PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[9]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 9",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_10_ENABLE_ID,         PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[10]),          PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 10",                  L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CORE_11_ENABLE_ID,         PARAMS_DATA_OFFSET_SIZE (Cpu.CoreEnable[11]),          PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core 11",                  L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU0_ENABLE_ID,                PARAMS_DATA_OFFSET_SIZE (Dpu.DpEnable[0]),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Dp0 Enable",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU1_ENABLE_ID,                PARAMS_DATA_OFFSET_SIZE (Dpu.DpEnable[1]),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Dp1 Enable",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU2_ENABLE_ID,                PARAMS_DATA_OFFSET_SIZE (Dpu.DpEnable[2]),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Dp2 Enable",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU3_ENABLE_ID,                PARAMS_DATA_OFFSET_SIZE (Dpu.DpEnable[3]),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Dp3 Enable",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU4_ENABLE_ID,                PARAMS_DATA_OFFSET_SIZE (Dpu.DpEnable[4]),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Dp4 Enable",                   L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU_PRIORITY_0_ID,             PARAMS_DATA_OFFSET_SIZE (Dpu.DpPriority[0]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Display Priority0",            L"0:Dp0, 1:Dp1, 2:Dp2, 3:Dp3, 4:Dp4"                                                                                                                      },
  { PARAMS_DATA_DPU_PRIORITY_1_ID,             PARAMS_DATA_OFFSET_SIZE (Dpu.DpPriority[1]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Display Priority1",            L"0:Dp0, 1:Dp1, 2:Dp2, 3:Dp3, 4:Dp4"                                                                                                                      },
  { PARAMS_DATA_DPU_PRIORITY_2_ID,             PARAMS_DATA_OFFSET_SIZE (Dpu.DpPriority[2]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Display Priority2",            L"0:Dp0, 1:Dp1, 2:Dp2, 3:Dp3, 4:Dp4"                                                                                                                      },
  { PARAMS_DATA_DPU_PRIORITY_3_ID,             PARAMS_DATA_OFFSET_SIZE (Dpu.DpPriority[3]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Display Priority3",            L"0:Dp0, 1:Dp1, 2:Dp2, 3:Dp3, 4:Dp4"                                                                                                                      },
  { PARAMS_DATA_DPU_PRIORITY_4_ID,             PARAMS_DATA_OFFSET_SIZE (Dpu.DpPriority[4]),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Display Priority4",            L"0:Dp0, 1:Dp1, 2:Dp2, 3:Dp3, 4:Dp4"                                                                                                                      },
  { PARAMS_DATA_MEM_FREQ_ID,                   PARAMS_DATA_OFFSET_SIZE (Mem.MemFreq),                 PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory Frequency",             L"800:1600MHz, 1067:2133MHz, 1375:2750MHz, 1600:3200MHz, 1867:3733MHz, 2133:4266MHz, 2400:4800MHz, 2750:5500MHz, 3000:6000MHz, 3200:6400MHz, 0xFFFF:Auto" },
  { PARAMS_DATA_MEM_BREAK_POINT_ID,            PARAMS_DATA_OFFSET_SIZE (Mem.MemBreakPoint),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory Break Point",           L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_EYE_SCAN_ID,               PARAMS_DATA_OFFSET_SIZE (Mem.MemEyeScan),              PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory Eye Scan",              L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_HARVESTING_ID,             PARAMS_DATA_OFFSET_SIZE (Mem.MemHarvesting),           PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory Harvesting",            L"0xC:Channel 2 + Channel 3, 0xF:Channel 0 + 1 + 2 + 3, 0xFF:Auto"                                                                                        },
  { PARAMS_DATA_WCK_ALWAYS_ON_ID,              PARAMS_DATA_OFFSET_SIZE (Mem.WckAlwaysOn),             PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory WckAlwaysOn",           L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_DATA_MASK_ID,                  PARAMS_DATA_OFFSET_SIZE (Mem.DataMask),                PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory DataMask",              L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_RFM_EN_ID,                     PARAMS_DATA_OFFSET_SIZE (Mem.RfmEn),                   PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory RFM",                   L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_AUTO_PRECHARGE_EN_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.AutoPrechargeEn),         PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory AutoPrechage",          L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_PBR_EN_ID,                     PARAMS_DATA_OFFSET_SIZE (Mem.PbrEn),                   PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory PBR",                   L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MBIST_EN_ID,                   PARAMS_DATA_OFFSET_SIZE (Mem.MbistEn),                 PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory MBIST",                 L"0:Disable, 1:DDRC BIST, 2:DDRPI BIST, 0xFF:Auto"                                                                                                        },
  { PARAMS_DATA_MBIST_MODE_ID,                 PARAMS_DATA_OFFSET_SIZE (Mem.MbistMode),               PARAMS_DATA_MULTI_OPTION_TYPE, L"Memory MBIST Mode",            L"0:MOVI1_3N, 1:LIMIT_MOVI(DDRC BIST) MARCH_C(DDRPI BIST), 2:GALPAT(DDRPI BIST), 3:PRBS(DDRPI BIST), 0xFF:Auto"                                           },
  { PARAMS_DATA_MEM_WR_L_ECC_ID,               PARAMS_DATA_OFFSET_SIZE (Mem.MemWrLEcc),               PARAMS_DATA_MULTI_OPTION_TYPE, L"WR LinkECC",                   L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_RD_L_ECC_ID,               PARAMS_DATA_OFFSET_SIZE (Mem.MemRdLEcc),               PARAMS_DATA_MULTI_OPTION_TYPE, L"RD LinkECC",                   L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_PORT_PRIORITY_ID,              PARAMS_DATA_OFFSET_SIZE (Mem.PortPriority),            PARAMS_DATA_MULTI_OPTION_TYPE, L"Port Priority",                L"0:Fixed, 1:QOS Based, 0xFF:Auto"                                                                                                                        },
  { PARAMS_DATA_BDW_P0_OVERRIDE_ID,            PARAMS_DATA_OFFSET_SIZE (Mem.BdwP0Override),           PARAMS_DATA_MULTI_OPTION_TYPE, L"BDW P0 Override",              L"0:Auto, 1:Manual"                                                                                                                                       },
  { PARAMS_DATA_BDW_P0_ID,                     PARAMS_DATA_OFFSET_SIZE (Mem.BdwP0),                   PARAMS_DATA_INTEGER_TYPE,      L"BDW P0 Value",                 L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_BDW_OVFLOW_P0_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemBdwOvflowP0),          PARAMS_DATA_MULTI_OPTION_TYPE, L"BDW Overflow P0",              L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_BDW_P1_OVERRIDE_ID,            PARAMS_DATA_OFFSET_SIZE (Mem.BdwP1Override),           PARAMS_DATA_MULTI_OPTION_TYPE, L"BDW P1 Override",              L"0:Auto, 1:Manual"                                                                                                                                       },
  { PARAMS_DATA_BDW_P1_ID,                     PARAMS_DATA_OFFSET_SIZE (Mem.BdwP1),                   PARAMS_DATA_INTEGER_TYPE,      L"BDW P1 Value",                 L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_BDW_OVFLOW_P1_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemBdwOvflowP1),          PARAMS_DATA_MULTI_OPTION_TYPE, L"BDW Overflow P1",              L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_R_PRIORITY_P0_OVERRIDE_ID, PARAMS_DATA_OFFSET_SIZE (Mem.MemRPriorityP0Override),  PARAMS_DATA_MULTI_OPTION_TYPE, L"R_Priority P0",                L"0:Auto, 1:Manual"                                                                                                                                       },
  { PARAMS_DATA_MEM_R_PRIORITY_P0_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemRPriorityP0),          PARAMS_DATA_INTEGER_TYPE,      L"R_Priority P0 Value",          L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_W_PRIORITY_P0_OVERRIDE_ID, PARAMS_DATA_OFFSET_SIZE (Mem.MemWPriorityP0Override),  PARAMS_DATA_MULTI_OPTION_TYPE, L"W_Priority P0",                L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_W_PRIORITY_P0_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemWPriorityP0),          PARAMS_DATA_INTEGER_TYPE,      L"W_Priority P0 Value",          L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_R_PRIORITY_P1_OVERRIDE_ID, PARAMS_DATA_OFFSET_SIZE (Mem.MemRPriorityP1Override),  PARAMS_DATA_MULTI_OPTION_TYPE, L"R_Priority P1",                L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_R_PRIORITY_P1_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemRPriorityP1),          PARAMS_DATA_INTEGER_TYPE,      L"R_Priority P1 Value",          L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_W_PRIORITY_P1_OVERRIDE_ID, PARAMS_DATA_OFFSET_SIZE (Mem.MemWPriorityP1Override),  PARAMS_DATA_MULTI_OPTION_TYPE, L"W_Priority P1",                L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_W_PRIORITY_P1_ID,          PARAMS_DATA_OFFSET_SIZE (Mem.MemWPriorityP1),          PARAMS_DATA_INTEGER_TYPE,      L"W_Priority P1 Value",          L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_MEM_I_ECC_ID,                  PARAMS_DATA_OFFSET_SIZE (Mem.MemIEcc),                 PARAMS_DATA_MULTI_OPTION_TYPE, L"Inline ECC",                   L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_WR_DBI_ID,                 PARAMS_DATA_OFFSET_SIZE (Mem.MemWrDbi),                PARAMS_DATA_MULTI_OPTION_TYPE, L"WRITE DBI",                    L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_MEM_RD_DBI_ID,                 PARAMS_DATA_OFFSET_SIZE (Mem.MemRdDbi),                PARAMS_DATA_MULTI_OPTION_TYPE, L"READ DBI",                     L"0:Disable, 1:Enable, 0xFF:Auto"                                                                                                                         },
  { PARAMS_DATA_VDD_SOC_VOLTAGE_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.VddSocVoltage),            PARAMS_DATA_INTEGER_TYPE,      L"VDD SOC voltage",              L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_GPU_VOLTAGE_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.VddGpuVoltage),            PARAMS_DATA_INTEGER_TYPE,      L"VDD GPU voltage",              L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_DPU_VOLTAGE_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.VddDpuVoltage),            PARAMS_DATA_INTEGER_TYPE,      L"VDD DPU voltage",              L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_CPU_BIG_CORE_0_VOLTAGE_ID, PARAMS_DATA_OFFSET_SIZE (Pm.VddCpuBigCore0Voltage),    PARAMS_DATA_INTEGER_TYPE,      L"VDD_CPU_B0 voltag",            L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_CPU_BIG_CORE_1_VOLTAGE_ID, PARAMS_DATA_OFFSET_SIZE (Pm.VddCpuBigCore1Voltage),    PARAMS_DATA_INTEGER_TYPE,      L"VDD_CPU_B1 voltage",           L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_CPU_MID_CORE_0_VOLTAGE_ID, PARAMS_DATA_OFFSET_SIZE (Pm.VddCpuMidCore0Voltage),    PARAMS_DATA_INTEGER_TYPE,      L"VDD_CPU_M0 voltage",           L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_CPU_MID_CORE_1_VOLTAGE_ID, PARAMS_DATA_OFFSET_SIZE (Pm.VddCpuMidCore1Voltage),    PARAMS_DATA_INTEGER_TYPE,      L"VDD_CPU_M1 voltage",           L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_VDD_CPU_LIT_CORE_VOLTAGE_ID,   PARAMS_DATA_OFFSET_SIZE (Pm.VddCpuLitCoreVoltage),     PARAMS_DATA_INTEGER_TYPE,      L"VDD_CPU_L voltage",            L"(dec)"                                                                                                                                                  },
  { PARAMS_DATA_CPU_CORE_CLK_GATING_ID,        PARAMS_DATA_OFFSET_SIZE (Pm.CpuCoreClkGating),         PARAMS_DATA_MULTI_OPTION_TYPE, L"CPU Core Clock Gating",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DSU_CLK_GATING_ID,             PARAMS_DATA_OFFSET_SIZE (Pm.DsuClkGating),             PARAMS_DATA_MULTI_OPTION_TYPE, L"DSU Clock Gating",             L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_GICD_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.GicdClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"GICD Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CI700_CLK_GATING_ID,           PARAMS_DATA_OFFSET_SIZE (Pm.Ci700ClkGating),           PARAMS_DATA_MULTI_OPTION_TYPE, L"CI700 Clock Gating",           L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_SYS_NI700_CLK_GATING_ID,       PARAMS_DATA_OFFSET_SIZE (Pm.SysNi700ClkGating),        PARAMS_DATA_MULTI_OPTION_TYPE, L"SYS-NI700 Clock Gating",       L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_MM_NI700_CLK_GATING_ID,        PARAMS_DATA_OFFSET_SIZE (Pm.MmNi700ClkGating),         PARAMS_DATA_MULTI_OPTION_TYPE, L"MM-NI700 Clock Gating",        L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_PCIE_NI700_CLK_GATING_ID,      PARAMS_DATA_OFFSET_SIZE (Pm.PcieNi700ClkGating),       PARAMS_DATA_MULTI_OPTION_TYPE, L"PCIE-NI700 Clock Gating",      L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_SMN_NI700_CLK_GATING_ID,       PARAMS_DATA_OFFSET_SIZE (Pm.SmnNi700ClkGating),        PARAMS_DATA_MULTI_OPTION_TYPE, L"SMN-NI700 Clock Gating",       L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_GPU_CLK_GATING_ID,             PARAMS_DATA_OFFSET_SIZE (Pm.GpuClkGating),             PARAMS_DATA_MULTI_OPTION_TYPE, L"GPU Clock Gating",             L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU0_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.Dpu0ClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"DPU0 Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU1_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.Dpu1ClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"DPU1 Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU2_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.Dpu2ClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"DPU2 Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU3_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.Dpu3ClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"DPU3 Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_DPU4_CLK_GATING_ID,            PARAMS_DATA_OFFSET_SIZE (Pm.Dpu4ClkGating),            PARAMS_DATA_MULTI_OPTION_TYPE, L"DPU4 Clock Gating",            L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_VPU_CLK_GATING_ID,             PARAMS_DATA_OFFSET_SIZE (Pm.VpuClkGating),             PARAMS_DATA_MULTI_OPTION_TYPE, L"VPU Clock Gating",             L"0:Disable, 1:Enable"                                                                                                                                    },
  { PARAMS_DATA_CPU_CPPC_TYPE_ID,              PARAMS_DATA_OFFSET_SIZE (Misc.CpuCppcType),            PARAMS_DATA_MULTI_OPTION_TYPE, L"CPPC Interface Type",          L"0:Disable, 1:Fast Channel, 2:Pcc"                                                                                                                       },
  { PARAMS_DATA_UART2_LIST_FIRST,              PARAMS_DATA_OFFSET_SIZE (Misc.Uart2ListFirst),         PARAMS_DATA_MULTI_OPTION_TYPE, L"List UART2 as ttyAMA0",        L"0:Disable, 1:Enable"                                                                                                                                    },
};

UINT32  mConfigDataEntryNum = sizeof (mConfigDataEntryTable) / sizeof (CONFIG_PARAMS_DATA_ENTRY);

EFI_STATUS
EFIAPI
ParseConfigDataOption (
  IN  CHAR16                      *String,
  OUT UINT32                      *Num,
  OUT CONFIG_PARAMS_DATA_OPTIONS  *Options
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR16      *Head, *Tail;
  CHAR16      *Str;
  UINT32      Index;
  UINTN       Location, Size;

  if ((String == NULL) || (Num == NULL) || (Options == NULL)) {
    Status = EFI_INVALID_PARAMETER;
  }

  Str = AllocateCopyPool (StrSize (String), String);
  if (Str == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Head = Str;
  Tail = Str;
  Size = StrSize (Str);

  Index    = 0;
  Location = 0;
  while ((Index < MAX_PARAMS_OPTION_NUM) && (Head != NULL) && (Location < Size)) {
    Head = StrStr (Tail, L":");
    if (Head == NULL) {
      FreePool (Str);
      return EFI_UNSUPPORTED;
    }

    *Head     = L'\0';
    Location += StrSize (Tail);
    if ((StrStr (Tail, L"0x") == NULL) && (StrStr (Tail, L"0X") == NULL)) {
      Options[Index].Value = StrDecimalToUint64 (Tail);
    } else {
      Options[Index].Value = StrHexToUint64 (Tail);
    }

    Tail = Head + 1;
    Head = StrStr (Tail, L",");
    if (Head == NULL) {
      Location += StrSize (Tail);
      if (Location != Size) {
        FreePool (Str);
        return EFI_UNSUPPORTED;
      }
    } else {
      *Head     = L'\0';
      Location += StrSize (Tail);
    }

    Status = StrCpyS (Options[Index].String, MAX_PARAMS_OPTION_STRING_SIZE, Tail);
    if (EFI_ERROR (Status)) {
      FreePool (Str);
      return Status;
    }

    if (Head != NULL) {
      Tail = Head + 1;
    }

    Index++;
  }

  *Num = Index;

  if ((Index == MAX_PARAMS_OPTION_NUM) && (Location < Size)) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}
