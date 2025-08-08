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

#define CACHE_NODE_NUMBER 7

#define CIX_SKY1_HUNTER_ACPI_PPTT_L1_D_CACHE_STRUCT  {                         \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                       /* SizePropertyValid */                         \
      1,                       /* NumberOfSetsValid */                         \
      1,                       /* AssociativityValid */                        \
      1,                       /* AllocationTypeValid */                       \
      1,                       /* CacheTypeValid */                            \
      1,                       /* WritePolicyValid */                          \
      1,                       /* LineSizeValid */                             \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    65536,                     /* Size */                                      \
    256,                         /* NumberOfSets */                              \
    4,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                     \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_DATA,                           \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK,                   \
    },                                                                         \
    64                         /* LineSize */                                  \
  }

#define CIX_SKY1_HUNTER_ACPI_PPTT_L1_I_CACHE_STRUCT  {                         \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                       /* SizePropertyValid */                         \
      1,                       /* NumberOfSetsValid */                         \
      1,                       /* AssociativityValid */                        \
      1,                       /* AllocationTypeValid */                       \
      1,                       /* CacheTypeValid */                            \
      1,                       /* WritePolicyValid */                          \
      1,                       /* LineSizeValid */                             \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    65536,                     /* Size */                                      \
    256,                         /* NumberOfSets */                              \
    4,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ,                           \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_INSTRUCTION,                    \
      0,                                                                       \
    },                                                                         \
    64                         /* LineSize */                                  \
  }

#define CIX_SKY1_HUNTER_ACPI_PPTT_L2_CACHE_STRUCT  {                           \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                     /* SizePropertyValid */                           \
      1,                     /* NumberOfSetsValid */                           \
      1,                     /* AssociativityValid */                          \
      1,                     /* AllocationTypeValid */                         \
      1,                     /* CacheTypeValid */                              \
      1,                     /* WritePolicyValid */                            \
      1,                     /* LineSizeValid */                               \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    524288,                    /* Size */                                      \
    1024,                         /* NumberOfSets */                              \
    8,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                     \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_UNIFIED,                        \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK,                   \
    },                                                                         \
    64                       /* LineSize */                                    \
  }

#define CIX_SKY1_HAYES_ACPI_PPTT_L1_D_CACHE_STRUCT  {                          \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                       /* SizePropertyValid */                         \
      1,                       /* NumberOfSetsValid */                         \
      1,                       /* AssociativityValid */                        \
      1,                       /* AllocationTypeValid */                       \
      1,                       /* CacheTypeValid */                            \
      1,                       /* WritePolicyValid */                          \
      1,                       /* LineSizeValid */                             \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    32768,                     /* Size */                                      \
    128,                         /* NumberOfSets */                              \
    4,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                     \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_DATA,                           \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK,                   \
    },                                                                         \
    64                         /* LineSize */                                  \
  }

#define CIX_SKY1_HAYES_ACPI_PPTT_L1_I_CACHE_STRUCT  {                          \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                       /* SizePropertyValid */                         \
      1,                       /* NumberOfSetsValid */                         \
      1,                       /* AssociativityValid */                        \
      1,                       /* AllocationTypeValid */                       \
      1,                       /* CacheTypeValid */                            \
      1,                       /* WritePolicyValid */                          \
      1,                       /* LineSizeValid */                             \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    32768,                     /* Size */                                      \
    128,                         /* NumberOfSets */                              \
    4,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ,                           \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_INSTRUCTION,                    \
      0,                                                                       \
    },                                                                         \
    64                         /* LineSize */                                  \
  }

#define CIX_SKY1_HAYES_ACPI_PPTT_L2_CACHE_STRUCT  {                           \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                     /* SizePropertyValid */                           \
      1,                     /* NumberOfSetsValid */                           \
      1,                     /* AssociativityValid */                          \
      1,                     /* AllocationTypeValid */                         \
      1,                     /* CacheTypeValid */                              \
      1,                     /* WritePolicyValid */                            \
      1,                     /* LineSizeValid */                               \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    0,                         /* Size */                                      \
    0,                         /* NumberOfSets */                              \
    0,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                     \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_UNIFIED,                        \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK,                   \
    },                                                                         \
    64                       /* LineSize */                                    \
  }

#define CIX_SKY1_ACPI_PPTT_L3_CACHE_STRUCT  {                                  \
    EFI_ACPI_6_3_PPTT_TYPE_CACHE,                                              \
    sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_CACHE),                                \
    { EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE },                        \
    {                                                                          \
      1,                     /* SizePropertyValid */                           \
      1,                     /* NumberOfSetsValid */                           \
      1,                     /* AssociativityValid */                          \
      1,                     /* AllocationTypeValid */                         \
      1,                     /* CacheTypeValid */                              \
      1,                     /* WritePolicyValid */                            \
      1,                     /* LineSizeValid */                               \
    },                                                                         \
    0,                         /* NextLevelOfCache */                          \
    12582912,                  /* Size */                                      \
    16384,                         /* NumberOfSets */                              \
    12,                         /* Associativity */                             \
    {                                                                          \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE,                     \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_UNIFIED,                        \
      EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK,                   \
    },                                                                         \
    64                       /* LineSize */                                    \
  }

#endif // PPTT_GENERATOR_H_
