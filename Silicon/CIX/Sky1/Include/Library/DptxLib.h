/** @file LcdHwLib.h

  This file contains interface functions for LcdHwLib of ArmPlatformPkg

  Copyright (c) 2017, ARM Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DPTX_LIB_H__
#define __DPTX_LIB_H__

#include <Protocol/ConnectorProtocol.h>
#include <Library/DisplayLib.h>
#include <Library/Edid.h>
#include <Library/drm_dp.h>
#include <Library/drm_dp_helper.h>
#include <Protocol/ResetId.h>
#include <Protocol/ClockId.h>

/* DPTX_SOC_GENERAL_CTL */
#define AUDIO_MODE_SPDIF_MODE      (0x1 << 8)
#define AUDIO_MODE_MASTER_MODE     (0x0 << 8)
#define MASTER_VIDEO_INTERLACE_EN  (0x1 << 4)
#define VIDEO_MASTER_CLK_SEL       (0x1 << 2)
#define VIDEO_MASTER_MODE_EN       (0x1 << 1)
#define VIDEO_MODE_MASK            (0x1 << 0)
#define VIDEO_MODE_SLAVE_MODE      (0x1 << 0)
#define VIDEO_MODE_MASTER_MODE     (0x0 << 0)

#define DP_TIMEOUT_LOOP_COUNT  200
#define MAX_CR_LOOP            5
#define MAX_EQ_LOOP            5

/* I2C EDID Chip ID, Slave Address */
#define I2C_EDID_DEVICE_ADDR    0x50
#define I2C_E_EDID_DEVICE_ADDR  0x30

#define EDID_BLOCK_LENGTH    0x80
#define EDID_HEADER_PATTERN  0x00
#define EDID_EXTENSION_FLAG  0x7e
#define EDID_CHECKSUM        0x7f

/* DP_MAX_LANE_COUNT */
#define DPCD_ENHANCED_FRAME_CAP(x)  (((x) >> 7) & 0x1)
#define DPCD_MAX_LANE_COUNT(x)      ((x) & 0x1f)

/* DP_LANE_COUNT_SET */
#define DPCD_LANE_COUNT_SET(x)  ((x) & 0x1f)

/* DP_TRAINING_LANE0_SET */
#define DPCD_PRE_EMPHASIS_SET(x)   (((x) & 0x3) << 3)
#define DPCD_PRE_EMPHASIS_GET(x)   (((x) >> 3) & 0x3)
#define DPCD_VOLTAGE_SWING_SET(x)  (((x) & 0x3) << 0)
#define DPCD_VOLTAGE_SWING_GET(x)  (((x) >> 0) & 0x3)

/* HDPTXPHy_GRF*/
#define HDPTXPHY0_GRF_BASE    0xFD5E0000
#define HDPTXPHY_GRF_CON0     0x00
#define HDPTXPHY_GRF_CON1     0x04
#define HDPTXPHY_GRF_STATUS0  0x80
#define PCS_BIASES            0x27c
#define O_PLL_LOCK            0x280
#define O_PLL_LOCK_done       (0x1<<3)
#define O_PHY_RDY             ((0x1<<1)&(0x1<<3))
/* ENABLE_BIASES */
#define ENABLE_BIASES_LANE6  (0x1<<6)
#define ENABLE_BIASES_LANE7  (0x1<<7)

/* INITIAL POR RESET*/
#define INITIAL_POR_RESET_LANE2  (0X1<<2)
#define INITIAL_POR_RESET_LANE3  (0X1<<3)

enum LinkLaneCountType {
  LANE_COUNT1 = 1,
  LANE_COUNT2 = 2,
  LANE_COUNT4 = 4
};

enum LinkTrainingState {
  START,
  CLOCK_RECOVERY,
  EQUALIZER_TRAINING,
  FINISHED,
  FAILED
};

enum VoltageSwingLevel {
  VOLTAGE_LEVEL_0,
  VOLTAGE_LEVEL_1,
  VOLTAGE_LEVEL_2,
  VOLTAGE_LEVEL_3,
};

enum PreEmphasisLevel {
  PRE_EMPHASIS_LEVEL_0,
  PRE_EMPHASIS_LEVEL_1,
  PRE_EMPHASIS_LEVEL_2,
  PRE_EMPHASIS_LEVEL_3,
};

enum PatternSet {
  TRAINING_OFF,
  TRAINING_PTN1,
  TRAINING_PTN2,
  TRAINING_PTN3,
  TRAINING_PTN4,
  DP_NONE
};

enum ColorSpace {
  COLOR_RGB,
  COLOR_YCBCR422,
  COLOR_YCBCR444
};

enum ColorDepth {
  COLOR_6,
  COLOR_8,
  COLOR_10,
  COLOR_12
};

enum ColorCoefficient {
  COLOR_YCBCR601,
  COLOR_YCBCR709
};

enum DynamicRange {
  VESA,
  CEA
};

enum PllStatus {
  PLL_UNLOCKED,
  PLL_LOCKED
};

enum ClockRecoveryMValueType {
  CALCULATED_M,
  REGISTER_M
};

enum VideoTimingRecognitionType {
  VIDEO_TIMING_FROM_CAPTURE,
  VIDEO_TIMING_FROM_REGISTER
};

enum DpIrqType {
  DP_IRQ_TYPE_HP_CABLE_IN  = BIT0,
  DP_IRQ_TYPE_HP_CABLE_OUT = BIT1,
  DP_IRQ_TYPE_HP_CHANGE    = BIT2,
  DP_IRQ_TYPE_UNKNOWN      = BIT3,
};

struct VideoInfo {
  char                      *Name;

  BOOLEAN                   H_Sync_Polarity;
  BOOLEAN                   V_Sync_Polarity;
  BOOLEAN                   Interlaced;

  enum ColorSpace           ColorSpace;
  enum DynamicRange         DynamicRange;
  enum ColorCoefficient     Ycbcr_Coeff;
  enum ColorDepth           ColorDepth;

  UINTN                     MaxLinkRate;
  enum LinkLaneCountType    MaxLaneCount;

  UINTN                     pclock;
  UINT8                     misc0;
  UINT8                     misc1;
  UINT8                     bpp;
};

struct LinkTrain {
  UINTN                     Eq_Loop;
  UINTN                     Cr_Loop[4];

  UINT8                     Link_Rate;
  UINT8                     LaneCount;
  UINT8                     TrainingLane[4];
  BOOLEAN                   SSC;

  enum LinkTrainingState    LtState;
};

struct Dptx_Plat_Data {
  // enum Dptx_Devtype        Dev_Type;
  // enum Dptx_Sub_Devtype    SubDev_Type;
  BOOLEAN    SSC;
};

struct DptxDevice {
  INT32                      ID;
  struct Udevice             *dev;
  VOID                       *Reg_Base;
  BOOLEAN                    Force_Hpd;
  struct VideoInfo           *VideoInfo;
  struct LinkTrain           LinkTrain;

  struct DRM_DSIPALY_MODE    *MODE;
  struct VideoMode           *Timing;
  struct Dptx_Plat_Data      Plat_Data;
  UINT8                      Enhanced;
  UINT8                      MaxLinkRate;
  UINT8                      SinkLaneCount;
  struct drm_dp_aux          aux;
  INT32                      State;
  CIX_RESET_PROTOCOL         *mCixResetProtocol;
  CIX_CLOCK_PROTOCOL         *mCixClockProtocol;
};

/* Dptx_reg.c */

UINT32
DptxRegRead (
  IN  UINT32  Offset
  );

VOID
EnablePwm (
  VOID
  );

VOID
DptxEnableVideoMute (
  BOOLEAN  Enable
  );

VOID
DptxLaneSwap (
  BOOLEAN  Enable
  );

VOID
DptxStopVideo (
  VOID
  );

VOID
DptxReset (
  VOID
  );

VOID
DptxSwreset (
  VOID
  );

VOID
DptxInitAnalogParam (
  VOID
  );

VOID
DptxInitInterrupt (
  VOID
  );

VOID
DptxEnableSwFunction (
  VOID
  );

VOID
DptxConfigInterrupt (
  VOID
  );

VOID
DptxSetPllPowerDown (
  BOOLEAN  Enable
  );

VOID
DptxInitHpd (
  VOID
  );

VOID
DptxClearHotplugInterrupts (
  VOID
  );

VOID
DptxInitAux (
  VOID
  );

VOID
DptxResetAux (
  VOID
  );

VOID
DptxInitAnalogFunc (
  VOID
  );

VOID
DptxSetAnalogPowerDown (
  VOID
  );

UINTN
DptxReadByteFromDpcd (
  OUT struct DptxDevice  *Dp,
  IN UINTN               Reg_Addr,
  IN UINT8               *Data
  );

UINTN
DptxStartAuxTransaction (
  VOID
  );

UINTN
DptxReadBytesFromDpcd (
  OUT struct DptxDevice  *Dp,
  IN  UINTN              Reg_Addr,
  IN  UINTN              Count,
  IN  UINT8              Data[]
  );

UINTN
DptxWriteBytesToDpcd (
  OUT struct DptxDevice  *Dp,
  IN  UINTN              Reg_Addr,
  IN  UINTN              Count,
  IN  UINT8              Data[]
  );

VOID
DptxSetTrainingPattern (
  OUT struct DptxDevice  *Dp,
  OUT enum PatternSet    Pattern
  );

UINT32
DptxGetLaneLinkTraining (
  OUT struct DptxDevice  *Dp,
  IN UINT8               Lane
  );

VOID
DptxGetLinkBandwidth (
  OUT struct DptxDevice  *Dp,
  UINT32                 *bwtype
  );

VOID
DptxGetLaneCount (
  OUT struct DptxDevice  *Dp,
  UINT32                 *Count
  );

VOID
DptxEnableVideoMaster (
  OUT struct DptxDevice  *Dp,
  BOOLEAN                Enable
  );

VOID
DptxStartVideo (
  OUT struct DptxDevice  *Dp
  );

INTN
DptxIsVideoStreamOn (
  OUT struct DptxDevice  *Dp
  );

VOID
DptxSetVideoFormat (
  OUT struct DptxDevice  *Dp
  );

VOID
RockchipHdptxPhySetVoltage (
  VOID
  );

VOID
Initialize (
  VOID
  );

VOID
PrinteDPReg (
  VOID
  );

VOID
PrintCruReg (
  VOID
  );

EFI_STATUS
DptxConnectorPreInit (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

EFI_STATUS
DptxConnectorInit (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

EFI_STATUS
DptxConnectorGetEdid (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

EFI_STATUS
DptxConnectorEnable (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

EFI_STATUS
DptxConnectorDisable (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

EFI_STATUS
DptxConnectorDetect (
  IN CIX_CONNECTOR_PROTOCOL  *This,
  OUT DISPLAY_STATE          *DisplayState
  );

#endif
