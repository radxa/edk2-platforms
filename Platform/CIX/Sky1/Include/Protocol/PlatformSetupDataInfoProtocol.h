/**
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PlatformSetupVarEntry.h>

#ifndef _PLATFORM_SETUP_DATA_INFO_PROTOCOL_H_
#define _PLATFORM_SETUP_DATA_INFO_PROTOCOL_H_

#define MAX_VARIABLE_NAME_SIZE  128

#define CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL_VERSION           0x50534401    //   'P' 'S' 'D' '(version ID)'
#define CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL_VERSION_STR_MASK  0xFFFFFF00
#define CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL_VERSION_NUM_MASK  0x000000FF

#define CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL_GUID   \
    {0x97BBCAC1, 0x13CB, 0x4B4C, {0xB0, 0xF8, 0xA3, 0x08, 0x5D, 0xBF, 0x1C, 0xD6}}

typedef struct _CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL;

typedef struct {
  CHAR16                       VarName[MAX_VARIABLE_NAME_SIZE];
  UINTN                        VarSize;
  EFI_GUID                     Guid;
  PLATFORM_SETUP_DATA_ENTRY    *Entry;
  UINTN                        Count;
} PLATFORM_SETUP_DATA_INFO_TABLE;

struct _CIX_PLATFORM_SETUP_DATA_INFO_PROTOCOL {
  UINT32                            Version;
  PLATFORM_SETUP_DATA_INFO_TABLE    Table;
};

extern EFI_GUID  gCixPlatformConfigParamsManageProtocolGuid;

#endif
