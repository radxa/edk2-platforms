/** @file
 *
 *  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
 **/


 #include <Uefi.h>
 #include <Uefi/UefiBaseType.h>
 #include <Library/UefiLib.h>
 #include <Library/UefiBootServicesTableLib.h>
 #include <Library/BaseLib.h>
 #include <Library/DebugLib.h>
 #include <Library/BaseMemoryLib.h>
#include <libfdt.h>
#include <fdt.h>

#define ADD_OF(a, b)       (MAX_UINT32 - b > a) ? (a + b) : 0
#define CHECK_ADD64(a, b)  ((MAX_UINT64 - b < a) ? TRUE : FALSE)

INT32
fdt_alloc_phandle (
  IN VOID  *Fdt
  )
{
  INT32  Offset;
  INT32  Phandle;

  Phandle = 0;

  for (Offset = fdt_next_node (Fdt, -1, NULL); Offset >= 0;
       Offset = fdt_next_node (Fdt, Offset, NULL))
  {
    Phandle = MAX (Phandle, fdt_get_phandle (Fdt, Offset));
  }

  return Phandle + 1;
}


INT32
fdt_set_phandle (
  IN VOID    *Fdt,
  IN UINT32  NodeOffset,
  IN UINT32  Phandle
  )
{
  INT32  Ret;

  Ret = fdt_setprop_cell (Fdt, NodeOffset, "phandle", Phandle);
  if (Ret < 0) {
    return Ret;
  }

  Ret = fdt_setprop_cell (Fdt, NodeOffset, "linux,phandle", Phandle);

  return Ret;
}

INT32
fdt_create_phandle (
  IN VOID    *Fdt,
  IN UINT32  NodeOffset
  )
{
  INT32  Phandle, Ret;

  Phandle = fdt_get_phandle (Fdt, NodeOffset);
  if (Phandle == 0) {
    Phandle = fdt_alloc_phandle (Fdt);
    Ret     = fdt_set_phandle (Fdt, NodeOffset, Phandle);

    if (Ret < 0) {
      DEBUG (
             (
              DEBUG_INFO,
              "%a: Can't set phandle %d: %a\n",
              __FUNCTION__,
              Phandle,
              fdt_strerror (Ret)
             )
             );
      return 0;
    }
  }

  return Phandle;
}

UINT32
fdt_check_header_ext (
  VOID  *fdt
  )
{
  UINT64  fdt_start, fdt_end;
  UINT32  sum;

  fdt_start = (UINT64)fdt;

  if (fdt_start + fdt_totalsize (fdt) <= fdt_start) {
    return FDT_ERR_BADOFFSET;
  }

  fdt_end = fdt_start + fdt_totalsize (fdt);

  if (!(sum = ADD_OF (fdt_off_dt_struct (fdt), fdt_size_dt_struct (fdt)))) {
    return FDT_ERR_BADOFFSET;
  } else {
    if (CHECK_ADD64 (fdt_start, sum)) {
      return FDT_ERR_BADOFFSET;
    } else if (fdt_start + sum > fdt_end) {
      return FDT_ERR_BADOFFSET;
    }
  }

  if (!(sum = ADD_OF (fdt_off_dt_strings (fdt), fdt_size_dt_strings (fdt)))) {
    return FDT_ERR_BADOFFSET;
  } else {
    if (CHECK_ADD64 (fdt_start, sum)) {
      return FDT_ERR_BADOFFSET;
    } else if (fdt_start + sum > fdt_end) {
      return FDT_ERR_BADOFFSET;
    }
  }

  if (fdt_start + fdt_off_mem_rsvmap (fdt) > fdt_end) {
    return FDT_ERR_BADOFFSET;
  }

  return 0;
}

VOID
EnableDtbNode (
  IN  VOID         *fdt,
  IN  CONST CHAR8  *NodePath
  )
{
  INT32  Node;
  INT32  Rc;

  Node = fdt_path_offset (fdt, NodePath);
  if (Node < 0) {
    DEBUG (
           (DEBUG_ERROR, "%a: failed to locate DT path '%a': %a\n",
            __FUNCTION__, NodePath, fdt_strerror (Node))
           );
    return;
  }

  Rc = fdt_setprop_string (fdt, Node, "status", "okay");
  if (Rc < 0) {
    DEBUG ((DEBUG_ERROR, "%a: failed to set status to 'disabled' on '%a': %a\n", __FUNCTION__, NodePath, fdt_strerror (Rc)));
  }
}


VOID
DisableDtbNode (
  IN  VOID         *fdt,
  IN  CONST CHAR8  *NodePath
  )
{
  INT32  Node;
  INT32  Rc;

  Node = fdt_path_offset (fdt, NodePath);
  if (Node < 0) {
    DEBUG (
           (DEBUG_ERROR, "%a: failed to locate DT path '%a': %a\n",
            __FUNCTION__, NodePath, fdt_strerror (Node))
           );
    return;
  }

  Rc = fdt_setprop_string (fdt, Node, "status", "disabled");
  if (Rc < 0) {
    DEBUG ((DEBUG_ERROR, "%a: failed to set status to 'disabled' on '%a': %a\n", __FUNCTION__, NodePath, fdt_strerror (Rc)));
  }
}



VOID
DbtNodePropRm (
  IN  VOID         *Fdt,
  IN  CONST CHAR8  *DeviceNodeName0,
  IN  CONST CHAR8  *DeviceNodeName1,
  IN  CONST CHAR8  *PropName
  )
{
  INT32  Node, NodeIdle;
  INT32  PhandleVal, Phandle[2] = { 0, 0 }, Error;

  NodeIdle = fdt_path_offset (Fdt, DeviceNodeName0);
  if (NodeIdle <= 0) {
    DEBUG (
           (
            DEBUG_INFO,
            "%a: Failed to find path %a: %a\n",
            __FUNCTION__,
            DeviceNodeName0,
            fdt_strerror (NodeIdle)
           )
           );
    return;
  }

  PhandleVal = fdt_get_phandle (Fdt, NodeIdle);
  if (PhandleVal <= 0) {
    PhandleVal = fdt_create_phandle (Fdt, NodeIdle);
    if (!PhandleVal) {
      DEBUG (
             (
              DEBUG_INFO,
              "%a: Failed to create Phandle %s: %a\n",
              __FUNCTION__,
              DeviceNodeName0,
              fdt_strerror (PhandleVal)
             )
             );
      return;
    }
  }

  DEBUG ((DEBUG_INFO, "%a: Phandle : %d\n", __FUNCTION__, PhandleVal));

  Node = fdt_path_offset (Fdt, DeviceNodeName1);
  if (Node <= 0) {
    DEBUG (
           (
            DEBUG_INFO,
            "%a: Failed to find path %s: %a\n",
            __FUNCTION__,
            DeviceNodeName1,
            fdt_strerror (Node)
           )
           );
    return;
  }

 #if 1
  Phandle[0] = cpu_to_fdt32 (PhandleVal);
  Error      = fdt_setprop (Fdt, Node, PropName, Phandle, sizeof (Phandle));
  if (Error != 0) {
    DEBUG (
           (
            DEBUG_INFO,
            "%a: Failed to delete property %a: %a\n",
            __FUNCTION__,
            PropName,
            fdt_strerror (Error)
           )
           );
    return;
  }

 #endif
}


