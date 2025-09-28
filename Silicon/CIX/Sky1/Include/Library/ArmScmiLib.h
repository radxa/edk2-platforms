/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SCMI_LIB_H_
#define _SCMI_LIB_H_

#include <Uefi.h>
#include <Protocol/ArmScmiBaseProtocol.h>
#include <Protocol/ArmScmiClockProtocol.h>
#include <Protocol/ArmScmiClock2Protocol.h>
#include <Protocol/ArmScmiPerformanceProtocol.h>
#include <Protocol/ClockId.h>

EFI_STATUS
BaseGetVersion (
  OUT UINT32  *Version
  );

EFI_STATUS
BaseGetTotalProtocols (
  OUT UINT32  *TotalProtocols
  );

/*clock protocol */
EFI_STATUS
ClockGetVersion (
  OUT UINT32  *Version
  );

EFI_STATUS
ClockGetTotalClocks (
  OUT UINT32  *TotalClocks
  );

EFI_STATUS
ClockGetClockAttributes (
  IN  UINT32   ClockId,
  OUT BOOLEAN  *Enabled,
  OUT CHAR8    *ClockAsciiName
  );

EFI_STATUS
ClockDescribeRates (
  IN     UINT32                  ClockId,
  OUT    SCMI_CLOCK_RATE_FORMAT  *Format,
  OUT    UINT32                  *TotalRates,
  IN OUT UINT32                  *RateArraySize,
  OUT    SCMI_CLOCK_RATE         *RateArray
  );

EFI_STATUS
ClockRateGet (
  IN  UINT32  ClockId,
  OUT UINT64  *Rate
  );

EFI_STATUS
ClockRateSet (
  IN UINT32  ClockId,
  IN UINT64  Rate
  );

EFI_STATUS
ClockEnable (
  IN UINT32   ClockId,
  IN BOOLEAN  Enable
  );

/*performance protocol */
EFI_STATUS
PerformanceGetVersion (
  OUT UINT32  *Version
  );

EFI_STATUS
PerformanceGetAttributes (
  OUT SCMI_PERFORMANCE_PROTOCOL_ATTRIBUTES  *Attributes
  );

EFI_STATUS
PerformanceDomainAttributes (
  IN  UINT32                              DomainId,
  OUT SCMI_PERFORMANCE_DOMAIN_ATTRIBUTES  *DomainAttributes
  );

EFI_STATUS
PerformanceDescribeLevels (
  IN     UINT32                  DomainId,
  OUT    UINT32                  *NumLevels,
  IN OUT UINT32                  *LevelArraySize,
  OUT    SCMI_PERFORMANCE_LEVEL  *LevelArray
  );

EFI_STATUS
PerformanceLimitsSet (
  IN UINT32                   DomainId,
  IN SCMI_PERFORMANCE_LIMITS  *Limits
  );

EFI_STATUS
PerformanceLimitsGet (
  UINT32                   DomainId,
  SCMI_PERFORMANCE_LIMITS  *Limits
  );

EFI_STATUS
PerformanceLevelSet (
  IN UINT32  DomainId,
  IN UINT32  Level
  );

EFI_STATUS
PerformanceLevelGet (
  IN  UINT32  DomainId,
  OUT UINT32  *Level
  );

#endif
