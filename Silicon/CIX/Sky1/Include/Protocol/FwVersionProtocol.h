/** @file
  SocInitLib.h

  Copyright 2024 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __FW_VERSION_INFO_H__
#define __FW_VERSION_INFO_H__

#include <Uefi.h>

typedef enum {
  FwVerSE,
  FwVerPM,
  FwVerPBL,
  FwVerATF,
  FwVerTEE,
  FwVerEC,
  FwVerUEFI,
  FwVerBoardId
} FW_VERSION_TYPE;

typedef struct _CIX_FW_VERSION_PROTOCOL CIX_FW_VERSION_PROTOCOL;

typedef
  EFI_STATUS
(EFIAPI *GET_FW_VERSION)(
                         FW_VERSION_TYPE  FwVerType,
                         IN OUT CHAR16  **FwVerBuff,
                         IN OUT UINT32  *FwVerSize
                         );

struct _CIX_FW_VERSION_PROTOCOL {
  GET_FW_VERSION    GetFwVersion;
};

#endif
