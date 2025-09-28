/** @file
  SSDT Cpu Topology Table Generator.

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright (c) 2021, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Reference(s):
    - ACPI 6.3 Specification - January 2019 - s8.4 Declaring Processors
**/

#ifndef SSDT_CPU_TOPOLOGY_GENERATOR_H_
#define SSDT_CPU_TOPOLOGY_GENERATOR_H_

#include <AcpiPlatform.h>
#include <AcpiNameSpaceObjects.h>

#pragma pack(1)

#define SB_SCOPE              "\\_SB_"
#define SB_SCOPE_PREFIX       SB_SCOPE "."
#define SOCKET_SCOPE_PREFIX   SB_SCOPE_PREFIX  "C000."
#define CLUSTER_SCOPE_PREFIX  SOCKET_SCOPE_PREFIX  "Cxxx"
/// Size of the SB_SCOPE_PREFIX string.
#define SB_SCOPE_PREFIX_SIZE  sizeof (SB_SCOPE_PREFIX)
/// Size of the CLUSTER_SCOPE_PREFIX string.
#define SOCKET_SCOPE_PREFIX_SIZE  sizeof (SOCKET_SCOPE_PREFIX)
/// Size of the CLUSTER_SCOPE_PREFIX string.
#define CLUSTER_SCOPE_PREFIX_SIZE  sizeof (CLUSTER_SCOPE_PREFIX)
/// HID for a processor device.
#define ACPI_HID_PROCESSOR_DEVICE  "ACPI0007"

/// HID for a processor container device.
#define ACPI_HID_PROCESSOR_CONTAINER_DEVICE  "ACPI0010"

/** Node names of Cpus and Clusters are 'Cxxx', and 'Lxxx' for LPI states.
    The 'xxx' is an index on 12 bits is given to node name,
    thus the limitation in the number of nodes.
*/
#define MAX_NODE_COUNT  (1 << 12)

#pragma pack()

#endif // SSDT_CPU_TOPOLOGY_GENERATOR_H_
