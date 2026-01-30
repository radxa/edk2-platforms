/** @file
 *
 *  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
 **/

#ifndef _REDRIVER_LIB_H_
#define _REDRIVER_LIB_H_

#include <Base.h>
#include <Protocol/TypecPhy.h>

EFI_STATUS
EFIAPI
RedriverSetConf (
  IN      UINT8             RdDevIdx,
  IN      TYPEC_MODE        Mode,
  IN      TYPEC_ORIENTATION Orientation
  );

#endif