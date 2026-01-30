/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CSU_PM_IPC_LIB
#define CSU_PM_IPC_LIB

#include <Base.h>
#include <Uefi.h>

#define MAX_PMIC_NUM  3

#pragma pack(1)

typedef struct _PMIC_VERSION_INFO {
  UINT32    Count;
  UINT32    Version[MAX_PMIC_NUM];
} PMIC_VERSION_INFO;

#pragma pack()

EFI_STATUS
CsuPmMsgClockCountDecrase (
  IN UINT32  ClockId
  );

EFI_STATUS
CsuPmMsgGetFirmwareVersion (
  );

EFI_STATUS
CsuPmMsgRegisterS3ScritTable (
  IN UINT32  AddressL32,
  IN UINT32  AddressH32,
  IN UINT32  CrcL32,
  IN UINT32  CrcH32
  );

EFI_STATUS
CsuPmMsgGetPmicVersion (
  IN OUT PMIC_VERSION_INFO  *Info
  );

EFI_STATUS
CsuPmIpcCommandGetPayload (
  OUT UINT32  **Payload
  );

EFI_STATUS
CsuPmIpcCommandExecute (
  IN     UINT32  MsgId,
  IN OUT UINT32  *PayloadLength,
  OUT    UINT32  **ReturnValues OPTIONAL
  );

#endif // CSU_PM_IPC_LIB
