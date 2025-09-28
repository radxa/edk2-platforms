/** @file
  Header file for the dynamic PPTT generator

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright (c) 2019, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Reference(s):
  - ACPI 6.3 Specification, January 2019
  - ARM Architecture Reference Manual ARMv8 (D.a)

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef PPTT_GENERATOR_H_
#define PPTT_GENERATOR_H_

#pragma pack(1)

#define ACPI_PROCESSOR_HIERARCHY_NODE_STRUCTURE_INIT(Flags, Parent, ACPIProcessorID, NumberOfPrivateResources)                      \
  {                                                                                                                                 \
    EFI_ACPI_6_3_PPTT_TYPE_PROCESSOR,                                                            /* Type */                         \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR) + NumberOfPrivateResources * sizeof (UINT32), /* Length */                       \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                                          /* Reserved */                     \
    Flags,                                                                                       /* Flags */                        \
    Parent,                                                                                      /* Parent */                       \
    ACPIProcessorID,                                                                             /* ACPI Processor ID */            \
    NumberOfPrivateResources                                                                     /* Number of private resources */  \
  }

#pragma pack()

#endif // PPTT_GENERATOR_H_
