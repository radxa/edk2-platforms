/*
  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef __CS32G051ECLIB_H__
#define __CS32G051ECLIB_H__

#include <Library/PdEcLib.h>

typedef enum {
  CMD_GET_IC_STATUS = 0x3A,
} CS32G051_CMD;

typedef enum {
  CMD_GET_IC_STATUS_LEN = 0x03,
} CS32G051_WR_DATA_LEN;

typedef enum {
  CMD_GET_IC_STATUS_SUB_CMD = 0x00,
} CS32G051_SUB_CMD;

typedef enum {
  Wait      = 0x00,
  Complete  = 0x01,
  Busy      = 0x02,
  Error     = 0x03,
} CS32G051_PING_STATUS_CMD_STS;

#pragma pack(1)

typedef union {
  struct {
    UINT8 DATA_LEN : 6;
    UINT8 CMD_STS  : 2;
  } Bits;
  UINT8    Uint8;
} CS32G051_PING_STATUS;

typedef struct {
  UINT8 CMD;
  UINT8 WR_DATA_LEN;
  UINT8 SUB_CMD;
} CS32G051_REQUEST_HEADER;

typedef struct {
  CS32G051_REQUEST_HEADER Header;
  UINT8                   SUB_CMD2;
  UINT8                   SUB_CMD3;
} CS32G051_REQUEST_CMD_GET_IC_STATUS;

typedef struct {
  UINT8   ByteCount;
  UINT8   ChipId;
  UINT8   RunBank;
  UINT8   BootVersionPatch;
  UINT8   BootVersionMinor;
  UINT16  BootVersionMajor;
  UINT8   AppVersionPatch;
  UINT8   AppVersionMinor;
  UINT16  AppVersionMajor;
} CS32G051_RESULT_CMD_GET_IC_STATUS;

#pragma pack()

#define WR_DATA_LEN(r) (r ## _LEN)
#define SUB_CMD(r) (r ## _SUB_CMD)

#define CS32G051_PACKET_HEADER(r) {.CMD = (r), .WR_DATA_LEN = WR_DATA_LEN(r), .SUB_CMD = SUB_CMD(r)}

#define CS32G051_CMD_GET_IC_STATUS_PACKET {.Header = CS32G051_PACKET_HEADER(CMD_GET_IC_STATUS), .SUB_CMD2 = 0x00, .SUB_CMD3 = 0x00}

#endif
