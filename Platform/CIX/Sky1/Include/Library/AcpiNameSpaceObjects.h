/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ACPI_NAME_SPACE_OBJECT_H_
#define _ACPI_NAME_SPACE_OBJECT_H_

#include <Uefi.h>
#include <AcpiPlatform.h>

#define EObjNameSpaceCix  3

#pragma pack(1)

/** The EARM_OBJECT_ID enum describes the Object IDs
    in the ARM Namespace
*/
typedef enum CixObjectID {
  ECixObjReserved,                     ///<  0 - Reserved
  ECixObjCpuUidtoCoreNumberMap,        ///<  1 - Mapping table of CPU UID to core number
  ECixObjCpuTopoInfo,                  ///<  2 - Cpu topology information
  ECixObjMax
} ECIX_OBJECT_ID;

// A structure that map each cpu core number to corresponding LPI Name
typedef CHAR8 CM_CIX_LPI_MAP_INFO[PLAT_CPU_COUNT][5];

typedef struct AmlPsdInfo {
  UINT16    NumEntries;
  UINT8     Revision;
  UINT32    Domain;
  UINT32    CoordType;
  UINT32    NumProcessors;
} AML_PSD_INFO;

typedef  UINTN CM_CIX_CPUUID_CORENUMBER_MAP;

typedef struct CixCpuCore {
  UINT32     Coreid;
  UINT32     Uid;
  BOOLEAN    Enable;
} CIX_CPU_CORE;

typedef struct CixClusterTopology {
  CIX_CPU_CORE    *Core;
  UINT32          Uid;
  UINT32          CoreNumber;
} CIX_CLUSTER_TOPO;

typedef struct CmCixCpuTopoInfo {
  CIX_CLUSTER_TOPO    *ClusterTopo;
  UINT32              ClusterNumber;
} CM_CIX_CPU_TOPO_INFO;

#pragma pack()
#endif
