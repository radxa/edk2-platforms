/*
  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef __PDECLIB_H__
#define __PDECLIB_H__

EFI_STATUS
EFIAPI
PdGetIcStatus (
  IN      UINT8 PdDevIdx,
  IN OUT  VOID  *IcStatus
  );

#endif
