/** @file
  PPTT Table Generator

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Reference(s):
  - ACPI 6.4 Specification, January 2021

  @par Glossary:
  - Cm or CM   - Configuration Manager
  - Obj or OBJ - Object
**/

#include <Library/AcpiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiTable.h>

// Module specific include files.
#include <AcpiTableGenerator.h>
#include <ConfigurationManagerObject.h>
#include <ConfigurationManagerHelper.h>
#include <AcpiNameSpaceObjects.h>
#include <Library/TableHelperLib.h>
#include <Protocol/ConfigurationManagerProtocol.h>

#include "PpttGenerator.h"

/**
  This macro expands to a function that retrieves the Cpu
  topology information
*/
GET_OBJECT_LIST (
  EObjNameSpaceCix,
  ECixObjCpuTopoInfo,
  CM_CIX_CPU_TOPO_INFO
  );

STATIC
UINT32
GetClusterValidCoreNum (
  CIX_CLUSTER_TOPO  *ClusterTopoInfo
  )
{
  UINT32  CoreIndex;
  UINT32  CoreCount = 0;

  for (CoreIndex = 0; CoreIndex < ClusterTopoInfo->CoreNumber; CoreIndex++) {
    if (ClusterTopoInfo->Core[CoreIndex].Enable) {
      CoreCount++;
    }
  }

  return CoreCount;
}

STATIC
UINT32
GetValidProcTopoNodeNum (
  CM_CIX_CPU_TOPO_INFO  *CpuTopoInfo
  )
{
  UINT32            ClusterIndex;
  UINT32            ValidNodeCount = 1;  // Socket node
  UINT32            CoreCount;
  CIX_CLUSTER_TOPO  *ClusterTopo;

  for (ClusterIndex = 0; ClusterIndex < CpuTopoInfo->ClusterNumber; ClusterIndex++) {
    ClusterTopo = &CpuTopoInfo->ClusterTopo[ClusterIndex];
    CoreCount   = GetClusterValidCoreNum (ClusterTopo);
    if (CoreCount > 0) {
      ValidNodeCount = ValidNodeCount + CoreCount + 1;      // include a valid cluster node
    }
  }

  return ValidNodeCount;
}

/**
  Construct the PPTT ACPI table.

  This function invokes the Configuration Manager protocol interface
  to get the required hardware information for generating the ACPI
  table.

  If this function allocates any resources then they must be freed
  in the FreeXXXXTableResources function.

  @param [in]  This                 Pointer to the table generator.
  @param [in]  AcpiTableInfo        Pointer to the ACPI table generator to be used.
  @param [in]  CfgMgrProtocol       Pointer to the Configuration Manager
                                    Protocol Interface.
  @param [out] Table                Pointer to the constructed ACPI Table.

  @retval EFI_SUCCESS               Table generated successfully.
  @retval EFI_INVALID_PARAMETER     A parameter is invalid.
  @retval EFI_NOT_FOUND             The required object was not found.
  @retval EFI_BAD_BUFFER_SIZE       The size returned by the Configuration
                                    Manager is less than the Object size for
                                    the requested object.
**/
STATIC
EFI_STATUS
EFIAPI
BuildPpttTable (
  IN  CONST ACPI_TABLE_GENERATOR                  *CONST  This,
  IN  CONST CM_STD_OBJ_ACPI_TABLE_INFO            *CONST  AcpiTableInfo,
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  CfgMgrProtocol,
  OUT       EFI_ACPI_DESCRIPTION_HEADER          **CONST  Table
  )
{
  EFI_STATUS            Status;
  UINT32                TableSize;
  CM_CIX_CPU_TOPO_INFO  *CpuTopoInfo;
  UINT32                CpuTopoInfoCount;
  UINT32                ProcTopologyStructCount;
  UINT32                ClusterIndex, CoreIndex;
  UINT32                SocketOffset, ClusterOffset;
  UINT32                ValidCoreCount;
  UINT8                 *New;
  CIX_CLUSTER_TOPO      *ClusterTopo;
  CIX_CPU_CORE          *CpuCore;

  EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_HEADER  *Pptt;

  ASSERT (
    (This != NULL) &&
    (AcpiTableInfo != NULL) &&
    (CfgMgrProtocol != NULL) &&
    (Table != NULL) &&
    (AcpiTableInfo->TableGeneratorId == This->GeneratorID) &&
    (AcpiTableInfo->AcpiTableSignature == This->AcpiTableSignature)
    );

  if ((AcpiTableInfo->AcpiTableRevision < This->MinAcpiTableRevision) ||
      (AcpiTableInfo->AcpiTableRevision > This->AcpiTableRevision))
  {
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: PPTT: Requested table revision = %d is not supported. "
       "Supported table revisions: Minimum = %d. Maximum = %d\n",
       AcpiTableInfo->AcpiTableRevision,
       This->MinAcpiTableRevision,
       This->AcpiTableRevision
      )
      );
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;

  Status = GetECixObjCpuTopoInfo (
             CfgMgrProtocol,
             CM_NULL_TOKEN,
             &CpuTopoInfo,
             &CpuTopoInfoCount
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: PPTT: Failed to get processor topology info. Status = %r\n", Status));
    goto error_handler;
  }

  ProcTopologyStructCount = GetValidProcTopoNodeNum (CpuTopoInfo);

  // Calculate the size of the PPTT table
  TableSize  = sizeof (EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_HEADER);
  TableSize += ProcTopologyStructCount*sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
  DEBUG (
    (
     DEBUG_INFO,
     "INFO: PPTT:\n" \
     " ProcTopologyStructCount = %d\n" \
     " TableSize = %d\n",
     ProcTopologyStructCount,
     TableSize
    )
    );

  // Allocate the Buffer for the PPTT table
  *Table = (EFI_ACPI_DESCRIPTION_HEADER *)AllocateZeroPool (TableSize);
  if (*Table == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: PPTT: Failed to allocate memory for PPTT Table. " \
       "Size = %d. Status = %r\n",
       TableSize,
       Status
      )
      );
    goto error_handler;
  }

  Pptt = (EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_HEADER *)*Table;
  New  = (UINT8 *)(UINTN)*Table;
  New += sizeof (EFI_ACPI_DESCRIPTION_HEADER);

  DEBUG (
    (
     DEBUG_INFO,
     "PPTT: Pptt = 0x%p. TableSize = 0x%x\n",
     Pptt,
     TableSize
    )
    );

  // Add ACPI header
  Status = AddAcpiHeader (
             CfgMgrProtocol,
             This,
             &Pptt->Header,
             AcpiTableInfo,
             TableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: PPTT: Failed to add ACPI header. Status = %r\n",
       Status
      )
      );
    goto error_handler;
  }

  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR_FLAGS  SocketFlags = {
    EFI_ACPI_6_3_PPTT_PACKAGE_PHYSICAL,
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_NOT_IDENTICAL
  };

  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR_FLAGS  ClusterFlags = {
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,
    EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF,
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL
  };

  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR_FLAGS  CoreFlags = {
    EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL,
    EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID,
    EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD,
    EFI_ACPI_6_3_PPTT_NODE_IS_LEAF,
    EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL
  };

  // Add the Socket PPTT structure
  EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Socket = ACPI_PROCESSOR_HIERARCHY_NODE_STRUCTURE_INIT (
                                                    SocketFlags,
                                                    0,
                                                    0,
                                                    0
                                                    );

  CopyMem (New, &Socket, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR));
  New          += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
  SocketOffset  = sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  ClusterOffset = SocketOffset;
  for (ClusterIndex = 0; ClusterIndex < CpuTopoInfo->ClusterNumber; ClusterIndex++) {
    ClusterTopo    = &CpuTopoInfo->ClusterTopo[ClusterIndex];
    ValidCoreCount = GetClusterValidCoreNum (ClusterTopo);
    if (ValidCoreCount == 0) {
      continue;
    }

    // Add the Cluster PPTT structure
    EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Cluster = ACPI_PROCESSOR_HIERARCHY_NODE_STRUCTURE_INIT (
                                                       ClusterFlags,
                                                       SocketOffset,
                                                       ClusterTopo->Uid,
                                                       0
                                                       );
    CopyMem (New, &Cluster, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR));
    New           += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
    ClusterOffset += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
    for (CoreIndex = 0; CoreIndex < ClusterTopo->CoreNumber; CoreIndex++) {
      CpuCore = &ClusterTopo->Core[CoreIndex];
      if (!CpuCore->Enable) {
        continue;
      }

      EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR  Core = ACPI_PROCESSOR_HIERARCHY_NODE_STRUCTURE_INIT (
                                                      CoreFlags,
                                                      ClusterOffset,
                                                      CpuCore->Uid,
                                                      0
                                                      );
      CopyMem (New, &Core, sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR));
      New += sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
    }

    ClusterOffset += ValidCoreCount*sizeof (EFI_ACPI_6_3_PPTT_STRUCTURE_PROCESSOR);
  }

  return Status;

error_handler:

  if (*Table != NULL) {
    FreePool (*Table);
    *Table = NULL;
  }

  return Status;
}

/**
  Free any resources allocated for constructing the PPTT

  @param [in]      This             Pointer to the table generator.
  @param [in]      AcpiTableInfo    Pointer to the ACPI Table Info.
  @param [in]      CfgMgrProtocol   Pointer to the Configuration Manager
                                    Protocol Interface.
  @param [in, out] Table            Pointer to the ACPI Table.

  @retval EFI_SUCCESS               The resources were freed successfully.
  @retval EFI_INVALID_PARAMETER     The table pointer is NULL or invalid.
**/
STATIC
EFI_STATUS
FreePpttTableResources (
  IN      CONST ACPI_TABLE_GENERATOR                  *CONST  This,
  IN      CONST CM_STD_OBJ_ACPI_TABLE_INFO            *CONST  AcpiTableInfo,
  IN      CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  CfgMgrProtocol,
  IN OUT        EFI_ACPI_DESCRIPTION_HEADER          **CONST  Table
  )
{
  ASSERT (
    (This != NULL) &&
    (AcpiTableInfo != NULL) &&
    (CfgMgrProtocol != NULL) &&
    (AcpiTableInfo->TableGeneratorId == This->GeneratorID) &&
    (AcpiTableInfo->AcpiTableSignature == This->AcpiTableSignature)
    );

  if ((Table == NULL) || (*Table == NULL)) {
    DEBUG ((DEBUG_ERROR, "ERROR: PPTT: Invalid Table Pointer\n"));
    ASSERT (
      (Table != NULL) &&
      (*Table != NULL)
      );
    return EFI_INVALID_PARAMETER;
  }

  FreePool (*Table);
  *Table = NULL;
  return EFI_SUCCESS;
}

/** The interface for the PPTT Table Generator.
*/
STATIC
ACPI_TABLE_GENERATOR  PpttGenerator = {
  // Generator ID
  CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdPptt),
  // Generator Description
  L"ACPI.STD.PPTT.GENERATOR",
  // ACPI Table Signature
  EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE,
  // ACPI Table Revision supported by this Generator
  EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION,
  // Minimum supported ACPI Table Revision
  EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION,
  // Creator ID
  EFI_ACPI_CREATOR_ID,
  // Creator Revision
  EFI_ACPI_CREATOR_REVISION,
  // Build Table function
  BuildPpttTable,
  // Free Resource function
  FreePpttTableResources,
  // Extended build function not needed
  NULL,
  // Extended build function not implemented by the generator.
  // Hence extended free resource function is not required.
  NULL
};

/**
  Register the Generator with the ACPI Table Factory.

  @param [in]  ImageHandle        The handle to the image.
  @param [in]  SystemTable        Pointer to the System Table.

  @retval EFI_SUCCESS             The Generator is registered.
  @retval EFI_INVALID_PARAMETER   A parameter is invalid.
  @retval EFI_ALREADY_STARTED     The Generator for the Table ID
                                  is already registered.
**/
EFI_STATUS
EFIAPI
AcpiPpttLibConstructor (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = RegisterAcpiTableGenerator (&PpttGenerator);
  DEBUG ((DEBUG_INFO, "PPTT: Register Generator. Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Deregister the Generator from the ACPI Table Factory.

  @param [in]  ImageHandle        The handle to the image.
  @param [in]  SystemTable        Pointer to the System Table.

  @retval EFI_SUCCESS             The Generator is deregistered.
  @retval EFI_INVALID_PARAMETER   A parameter is invalid.
  @retval EFI_NOT_FOUND           The Generator is not registered.
**/
EFI_STATUS
EFIAPI
AcpiPpttLibDestructor (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = DeregisterAcpiTableGenerator (&PpttGenerator);
  DEBUG ((DEBUG_INFO, "PPTT: Deregister Generator. Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  return Status;
}
