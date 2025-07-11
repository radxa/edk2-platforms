/** @file
 *
 *  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
 **/

#ifndef __DTB_COMMON_LIB_H__
#define __DTB_COMMON_LIB_H__

#include <Uefi/UefiBaseType.h>


INT32
fdt_alloc_phandle (
  IN VOID  *Fdt
  );

INT32
fdt_set_phandle (
  IN VOID    *Fdt,
  IN UINT32  NodeOffset,
  IN UINT32  Phandle
  );

INT32
fdt_create_phandle (
  IN VOID    *Fdt,
  IN UINT32  NodeOffset
  );

UINT32
fdt_check_header_ext (
  VOID  *fdt
  );

VOID
DisableDtbNode (
  IN  VOID         *fdt,
  IN  CONST CHAR8  *NodePath
  );

VOID
EnableDtbNode (
  IN  VOID         *fdt,
  IN  CONST CHAR8  *NodePath
  );

VOID
DbtNodePropRm (
  IN  VOID         *Fdt,
  IN  CONST CHAR8  *DeviceNodeName0,
  IN  CONST CHAR8  *DeviceNodeName1,
  IN  CONST CHAR8  *PropName
  );

#endif // __DTB_COMMON_LIB_H__
