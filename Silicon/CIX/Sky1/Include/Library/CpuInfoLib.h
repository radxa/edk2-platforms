/*
  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef CPU_INFO_LIB
#define CPU_INFO_LIB

#include <Base.h>
#include <Uefi.h>
#include <Protocol/ArmScmiPerformanceProtocol.h>

typedef struct {
  UINT32    Coreid;
  UINT32    Uid;
} CpuCore;

typedef struct {
  CpuCore    *Core;
  UINT32     CoreNumber;
} ClusterTopology;

typedef struct {
  ClusterTopology    *ClusterTopo;
  UINT32             ClusterNumber;
} SocketTopology;

typedef struct {
  SocketTopology    *SocketTopo;
  UINT32            SocketNumber;
} CpuTopology;

/** Get the boot core id of cpu
  @param[in,out]  CpuBootCoreId     The boot core id of cpu.
  @retval VOID
**/
VOID
EFIAPI
GetCpuBootCoreId (
  IN OUT UINT8  *CpuBootCoreId
  );

/** Get the core mask of cpu
 *
 * ex. bit 0 in CpuCoreMask coresponding to cpu core 0, etc.
 *
  @param[in,out]  CpuCoreMask     The core mask of cpu, bit index in CpuCoreMask
                                  coresponding to cpu core index.
  @param[out]     MaxCpuCoreNum      The max core number of cpu.
  @retval VOID
**/
VOID
EFIAPI
GetCpuCoreMask (
  IN OUT UINT32  *CpuCoreMask,
  OUT    UINT32  *MaxCpuCoreNum
  );

/** Get the valid cpu core number
  @param[in,out]  ValidCpuCoreNum     The valid cpu core number.
  @retval VOID
**/
VOID
EFIAPI
GetValidCpuCoreNum (
  IN OUT UINT8  *ValidCpuCoreNum
  );

/** Get Cpu share information
  @param[in,out]  CpuShareInfo     Cpu share information.
  @retval VOID
**/
VOID
EFIAPI
GetCpuShareInfo (
  IN OUT UINT32  *CpuShareInfo
  );

/** Get the performace domain id of specific cpu core

  @param [in]      CpuCoreId                The id of cpu core
  @param [in,out]  DomainId                 The id of performance domain

  @retval EFI_SUCCESS   Success
**/
EFI_STATUS
EFIAPI
GetCpuCorePerfDomainId (
  IN     UINT32  CpuCoreId,
  IN OUT UINT32  *DomainId
  );

/** Get the performace level array of specific performance domain

  @param [in]      DomainId                  The id of performance domain
  @param [in,out]  LevelArra                 Array of the performance levels.
  @param [in,out]  NumLevels                 Total number of levels a domain can support.

  @retval EFI_SUCCESS   Success
**/
EFI_STATUS
EFIAPI
GetPerfDomainLevelArra (
  IN     UINT32                  DomainId,
  IN OUT SCMI_PERFORMANCE_LEVEL  **LevelArra,
  IN OUT UINT32                  *NumLevels
  );

/** Gets the max CPU frequency.
  @return               CPU frequency in Hz
**/
UINTN
EFIAPI
GetMaxCpuFreq (
  VOID
  );

/** Gets the current CPU frequency.
  @return               CPU frequency in Hz
**/
UINTN
EFIAPI
GetCurrentCpuFreq (
  VOID
  );

/** Gets the CPU topology.
  @param[in,out]  *CpuTopo     pointer to a pointer to cpu topology
  @retval EFI_SUCCESS     Success
          EFI_NOT_FOUND   Can not find cpu topology.
**/
EFI_STATUS
EFIAPI
GetCpuTopology (
  CpuTopology  **CpuTopo
  );

#define ROUND_DIVISION(a, b)  (((a) / (b)) + ((((a) % (b)) * 2 >= (b)) ? 1 : 0))

#endif // CPU_INFO_LIB
