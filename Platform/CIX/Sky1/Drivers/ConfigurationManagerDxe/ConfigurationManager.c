#/** @file
#
#  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
#
#  @par Glossary:
#    - Cm or CM   - Configuration Manager
#    - Obj or OBJ - Object
#**/

#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/ConfigurationManagerProtocol.h>

/** The platform specific headers
*/
#include "ConfigurationManager.h"
#include "MemoryMap.h"
#include <Protocol/ArmScmiPerformanceProtocol.h>
#include <Library/CpuInfoLib.h>
#include <Library/ConfigParamsDataBlockLib.h>
#include <Protocol/ConfigParamsManageProtocol.h>

/** The platform configuration repository information.
*/
STATIC
SKY1_PLATFORM_REPOSITORY_INFO  Sky1PlatformRepositoryInfo = {
  /// Configuration Manager information
  { CONFIGURATION_MANAGER_REVISION, CFG_MGR_OEM_ID },

  // ACPI Table List
  {
    // MADT Table
    {
      EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdMadt),
      NULL,
      EFI_ACPI_OEM_TABLE_ID,
      EFI_ACPI_OEM_REVISION
    },
    // SSDT Table
    {
      EFI_ACPI_6_3_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
      0,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdSsdtCpuTopology),
      NULL,
      EFI_ACPI_OEM_TABLE_ID,
      EFI_ACPI_OEM_REVISION
    },
    // PPTT Table
    {
      EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE,
      EFI_ACPI_6_3_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdPptt),
      NULL,
      EFI_ACPI_OEM_TABLE_ID,
      EFI_ACPI_OEM_REVISION
    }
  },

  // Pointer to GIC CPU Interface information
  NULL,

  // GIC Distributor Info
  PLAT_GIC_DISTRIBUTOR_INFO,

  // GIC Redistributor
  PLAT_GIC_REDISTRIBUTOR_INFO,

  // GIC ITS
  PLAT_GIC_ITS_INFO,

  // CPU _CPC information
  PLAT_CPC_INFO
};

STATIC BOOLEAN               CppcEnable = TRUE;
STATIC CM_CIX_CPU_TOPO_INFO  mCpuTopoInfo;

VOID
EFIAPI
SetCpuMaxFreqOnExitBootService (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  UINT32                     CpuCoreMask;
  UINT32                     MaxCpuCoreNum;
  UINT32                     DomainId;
  UINT32                     NumLevels;
  SCMI_PERFORMANCE_LEVEL     *LevelArra;
  UINT32                     MaxLevel;
  UINT32                     i;
  SCMI_PERFORMANCE_PROTOCOL  *ScmiPerfProtocol = NULL;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gArmScmiPerformanceProtocolGuid,
                  NULL,
                  (VOID **)&ScmiPerfProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a gArmScmiPerformanceProtocolGuid NOT found.\n", __FUNCTION__));
    return;
  }

  GetCpuCoreMask (&CpuCoreMask, &MaxCpuCoreNum);
  for (i = 0; i < MaxCpuCoreNum; i++) {
    if (((CpuCoreMask>>i)&BIT0) == 1) {
      continue;
    }

    GetCpuCorePerfDomainId (i, &DomainId);
    Status = GetPerfDomainLevelArra (DomainId, &LevelArra, &NumLevels);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Performance domain [%d] get performance level data failed.\n", DomainId));
      continue;
    }

    MaxLevel = LevelArra[NumLevels-1].Level;
    Status   = ScmiPerfProtocol->LevelSet (ScmiPerfProtocol, DomainId, MaxLevel);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set Performance domain [%d] to performance level [%d] failed.\n", DomainId, MaxLevel));
    }
  }
}

/** Get the Cpu performace capability granularity

  @param [in]      CpuID              the id of cpu
  @param [in,out]  CpcGranularity     Cpu performace capability granularity, unit of Hz/perf_level

  @retval EFI_SUCCESS   Success
**/
STATIC
EFI_STATUS
GetCpcGranularity (
  IN     UINTN   CpuID,
  IN OUT UINT32  *CpcGranularity
  )
{
  EFI_STATUS                          Status;
  SCMI_PERFORMANCE_PROTOCOL           *ScmiPerfProtocol = NULL;
  UINT32                              DomainId;
  AML_PSD_INFO                        PsdInfo[PLAT_CPU_COUNT] = PLAT_PSD_INFO;
  SCMI_PERFORMANCE_DOMAIN_ATTRIBUTES  DomainAttribute;

  if (CpuID >= PLAT_CPU_COUNT) {
    DEBUG ((DEBUG_ERROR, "Cpuid is over the max range, max cpuid = %d, current cpu id = %d\n", PLAT_CPU_COUNT - 1, CpuID));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gArmScmiPerformanceProtocolGuid,
                  NULL,
                  (VOID **)&ScmiPerfProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gArmScmiPerformanceProtocolGuid NOT found.\n"));
    return Status;
  }

  DomainId = PsdInfo[CpuID].Domain;

  Status = ScmiPerfProtocol->GetDomainAttributes (ScmiPerfProtocol, DomainId, &DomainAttribute);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Perfomance [%d] get domain attributes failed.\n", DomainId));
    return Status;
  }

  *CpcGranularity = (DomainAttribute.SustainedFreq*1000) / DomainAttribute.SustainedPerfLevel;
  DEBUG ((DEBUG_INFO, "CpcGranularity = %d\n", *CpcGranularity));
  return Status;
}

STATIC
EFI_STATUS
GetCpuPerfData (
  IN     UINTN   CpuID,
  IN OUT UINT32  *HighestPerf,
  IN OUT UINT32  *NominalPerf,
  IN OUT UINT32  *LowestNonlinearPerf,
  IN OUT UINT32  *LowestPerf
  )
{
  EFI_STATUS              Status;
  UINT32                  DomainId;
  UINT32                  NumLevels;
  SCMI_PERFORMANCE_LEVEL  *LevelArra;
  AML_PSD_INFO            PsdInfo[PLAT_CPU_COUNT] = PLAT_PSD_INFO;

  if (CpuID >= PLAT_CPU_COUNT) {
    DEBUG ((DEBUG_ERROR, "Cpuid is over the max range, max cpuid = %d, current cpu id = %d\n", PLAT_CPU_COUNT - 1, CpuID));
    return EFI_INVALID_PARAMETER;
  }

  DomainId = PsdInfo[CpuID].Domain;
  Status   = GetPerfDomainLevelArra (DomainId, &LevelArra, &NumLevels);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Performance domain [%d] get performance level data failed.\n", DomainId));
  }

  for (UINT8 i = 0; i < NumLevels; i++) {
    DEBUG (
      (
       DEBUG_INFO,
       "[CPU%d] Performace_Level = [%d].   Level = [%d]    ,  PowerCost = [%d], Latency = [%d]\n", \
       CpuID,
       i,
       LevelArra[i].Level,
       LevelArra[i].PowerCost,
       LevelArra[i].Latency
      )
      );
  }

  *HighestPerf         = LevelArra[NumLevels-1].Level;
  *NominalPerf         = LevelArra[NumLevels-1].Level;
  *LowestNonlinearPerf = LevelArra[0].Level;
  *LowestPerf          = LevelArra[0].Level;

  DEBUG ((DEBUG_INFO, "HighestPerf = %x\n", *HighestPerf));
  DEBUG ((DEBUG_INFO, "NominalPerf = %x\n", *NominalPerf));
  DEBUG ((DEBUG_INFO, "LowestNonlinearPerf = %x\n", *LowestNonlinearPerf));
  DEBUG ((DEBUG_INFO, "LowestPerf = %x\n", *LowestPerf));

  return EFI_SUCCESS;
}

/** Initialize the CPU CPC interface information in platform configuration repository.

  @param [in]  This     Pointer to the Configuration Manager Protocol.

  @retval EFI_SUCCESS   Success
**/
STATIC
EFI_STATUS
EFIAPI
InitializeCmArmCpcInfo (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This
  )
{
  EFI_STATUS       Status;
  CM_ARM_CPC_INFO  *CpuCpcInfo;
  UINT32           HighestPerf;
  UINT32           NominalPerf;
  UINT32           LowestNonlinearPerf;
  UINT32           LowestPerf;
  UINT32           i;
  UINT32           CpuCoreMask, MaxCpuCoreNum;
  UINT32           CpcGranularity;

  GetCpuCoreMask (&CpuCoreMask, &MaxCpuCoreNum);

  CpuCpcInfo = This->PlatRepoInfo->CpuCpcInfo;
  for (i = 0; i < PLAT_CPU_COUNT; i++) {
    // If a cluster of cpu is masked, then get core performace of this cpu cluster will fail
    if (((CpuCoreMask>>i)&BIT0) == 1) {
      continue;
    }

    // Get CPU performace parameter
    Status = GetCpuPerfData (i, &HighestPerf, &NominalPerf, &LowestNonlinearPerf, &LowestPerf);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get CPU%d performance data fail!\n"));
      continue;
    }

    // Get CPU performace granularity
    Status = GetCpcGranularity (i, &CpcGranularity);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Get CPU%d performance granularity fail!\n"));
      continue;
    }

    CpuCpcInfo[i].HighestPerformanceInteger         = HighestPerf;
    CpuCpcInfo[i].NominalPerformanceInteger         = NominalPerf;
    CpuCpcInfo[i].LowestNonlinearPerformanceInteger = LowestNonlinearPerf;
    CpuCpcInfo[i].LowestPerformanceInteger          = LowestPerf;
    CpuCpcInfo[i].LowestFrequencyInteger            = ROUND_DIVISION (LowestPerf * CpcGranularity, 1000000);
    CpuCpcInfo[i].NominalFrequencyInteger           = ROUND_DIVISION (NominalPerf * CpcGranularity, 1000000);
    DEBUG ((DEBUG_INFO, "LowestFrequencyInteger = %d\n", CpuCpcInfo[i].LowestFrequencyInteger));
    DEBUG ((DEBUG_INFO, "NominalFrequencyInteger = %d\n", CpuCpcInfo[i].NominalFrequencyInteger));
  }

  return EFI_SUCCESS;
}

/** Initialize the CPU topology information in platform configuration repository.

  @param [in]  This           Pointer to the Configuration Manager Protocol.
  @param [in]  ConfigData     Pointer to the Configuration Parameter data Block.

  @retval EFI_SUCCESS           Success
  @retval EFI_OUT_OF_RESOURCES  Not enough memory.
**/
STATIC
EFI_STATUS
EFIAPI
InitializeCmCixCpuTopoInfo (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN CONFIG_PARAMS_DATA_BLOCK                             *ConfigData
  )
{
  EFI_STATUS                     Status;
  UINT8                          ClusterIndex, CoreIndex, CpuCoreNum, CpuCoreEnableIndex;
  UINT32                         ClusterUid = 1;
  SKY1_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  CpuTopology                    *CpuTopo = NULL;
  CIX_CLUSTER_TOPO               *CixClusterTopo;
  CIX_CPU_CORE                   *CixCoreTopo;

  CpuCoreEnableIndex = 0;
  GetValidCpuCoreNum (&CpuCoreNum);

  GetCpuTopology (&CpuTopo);
  PlatformRepo              = This->PlatRepoInfo;
  PlatformRepo->CpuTopoInfo = &mCpuTopoInfo;

  mCpuTopoInfo.ClusterNumber = CpuTopo->SocketTopo[0].ClusterNumber;
  Status                     = gBS->AllocatePool (EfiBootServicesData, mCpuTopoInfo.ClusterNumber*sizeof (CIX_CLUSTER_TOPO), (VOID **)&mCpuTopoInfo.ClusterTopo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Out of memory resource\n"));
    return Status;
  }

  for (ClusterIndex = 0; ClusterIndex < mCpuTopoInfo.ClusterNumber; ClusterIndex++) {
    CixClusterTopo             = &mCpuTopoInfo.ClusterTopo[ClusterIndex];
    CixClusterTopo->CoreNumber = CpuTopo->SocketTopo[0].ClusterTopo[ClusterIndex].CoreNumber;
    CixClusterTopo->Uid        = ClusterUid;
    Status                     = gBS->AllocatePool (EfiBootServicesData, CixClusterTopo->CoreNumber*sizeof (CIX_CPU_CORE), (VOID **)&CixClusterTopo->Core);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Out of memory resource\n"));
      return Status;
    }

    for (CoreIndex = 0; CoreIndex < mCpuTopoInfo.ClusterTopo[ClusterIndex].CoreNumber; CoreIndex++) {
      CixCoreTopo         = &CixClusterTopo->Core[CoreIndex];
      CixCoreTopo->Coreid = CpuTopo->SocketTopo[0].ClusterTopo[ClusterIndex].Core[CoreIndex].Coreid;
      CixCoreTopo->Uid    = CpuTopo->SocketTopo[0].ClusterTopo[ClusterIndex].Core[CoreIndex].Uid;
      CixCoreTopo->Enable = ConfigData->Cpu.CoreEnable[CpuCoreEnableIndex];
      CpuCoreEnableIndex++;
    }

    ClusterUid++;
  }

  return EFI_SUCCESS;
}

/** Initialize the GIC CPU interface information in platform configuration repository.

  @param [in]  This            Pointer to the Configuration Manager Protocol.

  @retval EFI_SUCCESS   Success
**/
STATIC
EFI_STATUS
EFIAPI
InitializeCmArmGiccInfo (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This
  )
{
  EFI_STATUS                     Status;
  SKY1_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  UINT8                          ClusterIndex, CoreIndex, GicCIndex, CpuCoreNum;
  CIX_CLUSTER_TOPO               *ClusterTopo;
  CIX_CPU_CORE                   *CpuCore;
  CM_ARM_GICC_INFO               DefalutGicCInfo[PLAT_CPU_COUNT] = PLAT_GIC_CPU_INTERFACE;

  PlatformRepo = This->PlatRepoInfo;

  GetValidCpuCoreNum (&CpuCoreNum);
  Status = gBS->AllocatePool (EfiBootServicesData, CpuCoreNum*sizeof (CM_ARM_GICC_INFO), (VOID **)&(PlatformRepo->GicCInfo));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Out of memory resource\n"));
    return Status;
  }

  ZeroMem (PlatformRepo->GicCInfo, CpuCoreNum*sizeof (CM_ARM_GICC_INFO));

  GicCIndex = 0;

  for (ClusterIndex = 0; ClusterIndex < PlatformRepo->CpuTopoInfo->ClusterNumber; ClusterIndex++) {
    ClusterTopo = &PlatformRepo->CpuTopoInfo->ClusterTopo[ClusterIndex];
    for (CoreIndex = 0; CoreIndex < ClusterTopo->CoreNumber; CoreIndex++) {
      CpuCore = &ClusterTopo->Core[CoreIndex];

      PlatformRepo->GicCInfo[GicCIndex]                    = DefalutGicCInfo[CpuCore->Coreid];
      PlatformRepo->GicCInfo[GicCIndex].CPUInterfaceNumber = 0;
      PlatformRepo->GicCInfo[GicCIndex].AcpiProcessorUid   = CpuCore->Uid;
      if (!CpuCore->Enable) {
        PlatformRepo->GicCInfo[GicCIndex].Flags &= ~EFI_ACPI_6_2_GIC_ENABLED;
      }

      PlatformRepo->CpuUidtoCoreNumberMap[GicCIndex] = CpuCore->Coreid;
      GicCIndex++;
    }
  }

  return EFI_SUCCESS;
}

/** Initialize the platform configuration repository.

  @param [in]  This     Pointer to the Configuration Manager Protocol.

  @retval EFI_SUCCESS   Success
**/
STATIC
EFI_STATUS
EFIAPI
InitializePlatformRepository (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This
  )
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  EFI_EVENT                          Event  = NULL;
  CIX_CONFIG_PARAMS_MANAGE_PROTOCOL  *ConfigManage;
  CONFIG_PARAMS_DATA_BLOCK           *ConfigData                   = NULL;
  CM_ARM_CPC_INFO                    CpuCpcInfoPcc[PLAT_CPU_COUNT] = PLAT_CPC_INFO_PCC;

  CpuTopology  *CpuTopo = NULL;

  Status = gBS->LocateProtocol (
                  &gCixConfigParamsManageProtocolGuid,
                  NULL,
                  (VOID **)&ConfigManage
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: config parameters invalid %r\n", __FUNCTION__, Status));
    return Status;
  }

  ConfigData = ConfigManage->Data;

  Status = GetCpuTopology (&CpuTopo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Get cpu topology fail, %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = InitializeCmCixCpuTopoInfo (This, ConfigData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to initialize cix cpu topology info, %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = InitializeCmArmGiccInfo (This);
  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: Failed to initialize GIC CPU interface information." \
       " Status = %r\n",
       Status
      )
      );
  }

  if (ConfigData->Misc.CpuCppcType == CPPC_PCC) {
    CopyMem ((VOID *)This->PlatRepoInfo->CpuCpcInfo, (VOID *)CpuCpcInfoPcc, sizeof (CpuCpcInfoPcc));
  } else if (ConfigData->Misc.CpuCppcType == CPPC_DISABLE) {
    CppcEnable = FALSE;
    Status     = gBS->CreateEventEx (
                        EVT_NOTIFY_SIGNAL,
                        TPL_CALLBACK,
                        SetCpuMaxFreqOnExitBootService,
                        NULL,
                        &gEfiEventExitBootServicesGuid,
                        &Event
                        );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Register Set Cpu Exit Boot Service Failed %x \n", Status));
    }
  } else {
    // The default CPPC Interface Type is provided by Fast Channel.
    Status = InitializeCmArmCpcInfo (This);
    if (EFI_ERROR (Status)) {
      DEBUG (
        (
         DEBUG_ERROR,
         "ERROR: Failed to initialize CPU CPC interface information." \
         " Status = %r\n",
         Status
        )
        );
    }
  }

  return Status;
}

/** A helper function for returning the Configuration Manager Objects.

  @param [in]       CmObjectId     The Configuration Manager Object ID.
  @param [in]       Object         Pointer to the Object(s).
  @param [in]       ObjectSize     Total size of the Object(s).
  @param [in]       ObjectCount    Number of Objects.
  @param [in, out]  CmObjectDesc   Pointer to the Configuration Manager Object
                                   descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
**/
STATIC
EFI_STATUS
EFIAPI
HandleCmObject (
  IN  CONST CM_OBJECT_ID                CmObjectId,
  IN        VOID                        *Object,
  IN  CONST UINTN                       ObjectSize,
  IN  CONST UINTN                       ObjectCount,
  IN  OUT   CM_OBJ_DESCRIPTOR   *CONST  CmObjectDesc
  )
{
  CmObjectDesc->ObjectId = CmObjectId;
  CmObjectDesc->Size     = ObjectSize;
  CmObjectDesc->Data     = (VOID *)Object;
  CmObjectDesc->Count    = ObjectCount;
  DEBUG (
    (
     DEBUG_INFO,
     "INFO: CmObjectId = %x, Ptr = 0x%p, Size = %d, Count = %d\n",
     CmObjectId,
     CmObjectDesc->Data,
     CmObjectDesc->Size,
     CmObjectDesc->Count
    )
    );
  return EFI_SUCCESS;
}

/** Return a standard namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetStandardNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  EFI_STATUS                     Status;
  SKY1_PLATFORM_REPOSITORY_INFO  *PlatformRepo;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    case EStdObjCfgMgrInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->CmInfo,
                 sizeof (PlatformRepo->CmInfo),
                 1,
                 CmObject
                 );
      break;
    case EStdObjAcpiTableList:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->CmAcpiTableList,
                 sizeof (PlatformRepo->CmAcpiTableList),
                 ARRAY_SIZE (PlatformRepo->CmAcpiTableList),
                 CmObject
                 );
      break;
    default:
    {
      Status = EFI_NOT_FOUND;
      DEBUG (
        (
         DEBUG_ERROR,
         "WARNING: Object 0x%x. Status = %r\n",
         CmObjectId,
         Status
        )
        );
      break;
    }
  }

  return Status;
}

/** Return an CIX namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetCixNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  EFI_STATUS                     Status;
  SKY1_PLATFORM_REPOSITORY_INFO  *PlatformRepo;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    case ECixObjCpuUidtoCoreNumberMap:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->CpuUidtoCoreNumberMap,
                 PLAT_CPU_COUNT*sizeof (UINTN),
                 PLAT_CPU_COUNT,
                 CmObject
                 );
      break;

    case ECixObjCpuTopoInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->CpuTopoInfo,
                 sizeof (CM_CIX_CPU_TOPO_INFO),
                 1,
                 CmObject
                 );
      break;
    default:
    {
      Status = EFI_NOT_FOUND;
      DEBUG (
        (
         DEBUG_ERROR,
         "WARNING: Object 0x%x. Status = %r\n",
         CmObjectId,
         Status
        )
        );
      break;
    }
  }// switch

  return Status;
}

/** Return an ARM namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetArmNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  EFI_STATUS                     Status;
  SKY1_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  UINT8                          CpuCoreNum;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    case EArmObjGicCInfo:
      GetValidCpuCoreNum (&CpuCoreNum);
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->GicCInfo,
                 CpuCoreNum*sizeof (CM_ARM_GICC_INFO),
                 CpuCoreNum,
                 CmObject
                 );
      break;

    case EArmObjGicDInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->GicDInfo,
                 sizeof (PlatformRepo->GicDInfo),
                 1,
                 CmObject
                 );
      break;

    case EArmObjGicRedistributorInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->GicRedistInfo,
                 sizeof (PlatformRepo->GicRedistInfo),
                 PLAT_GIC_REDISTRIBUTOR_COUNT,
                 CmObject
                 );
      break;

    case EArmObjGicItsInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->GicItsInfo,
                 sizeof (PlatformRepo->GicItsInfo),
                 PLAT_GIC_ITS_COUNT,
                 CmObject
                 );
      break;

    case EArmObjCpcInfo:
      if (!CppcEnable) {
        return EFI_NOT_FOUND;
      }

      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->CpuCpcInfo,
                 PLAT_CPU_COUNT*sizeof (CM_ARM_CPC_INFO),
                 PLAT_CPU_COUNT,
                 CmObject
                 );
      break;

    default:
    {
      Status = EFI_NOT_FOUND;
      DEBUG (
        (
         DEBUG_ERROR,
         "WARNING: Object 0x%x. Status = %r\n",
         CmObjectId,
         Status
        )
        );
      break;
    }
  }// switch

  return Status;
}

/** Return an OEM namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetOemNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    default:
    {
      Status = EFI_NOT_FOUND;
      DEBUG (
        (
         DEBUG_ERROR,
         "WARNING: Object 0x%x. Status = %r\n",
         CmObjectId,
         Status
        )
        );
      break;
    }
  }

  return Status;
}

/** The GetObject function defines the interface implemented by the
    Configuration Manager Protocol for returning the Configuration
    Manager Objects.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
Sky1PlatformGetObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  EFI_STATUS  Status;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  switch (GET_CM_NAMESPACE_ID (CmObjectId)) {
    case EObjNameSpaceStandard:
      Status = GetStandardNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    case EObjNameSpaceArm:
      Status = GetArmNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    case EObjNameSpaceCix:
      Status = GetCixNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    case EObjNameSpaceOem:
      Status = GetOemNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    default:
    {
      Status = EFI_INVALID_PARAMETER;
      DEBUG (
        (
         DEBUG_ERROR,
         "ERROR: Unknown Namespace Object = 0x%x. Status = %r\n",
         CmObjectId,
         Status
        )
        );
      break;
    }
  }

  return Status;
}

/** The SetObject function defines the interface implemented by the
    Configuration Manager Protocol for updating the Configuration
    Manager Objects.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in]      CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the Object.

  @retval EFI_UNSUPPORTED  This operation is not supported.
**/
EFI_STATUS
EFIAPI
Sky1PlatformSetObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST  This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN        CM_OBJ_DESCRIPTOR                     *CONST  CmObject
  )
{
  return EFI_UNSUPPORTED;
}

/** A structure describing the configuration manager protocol interface.
*/
STATIC
CONST
EDKII_CONFIGURATION_MANAGER_PROTOCOL  Sky1ConfigManagerProtocol = {
  CREATE_REVISION (1,    0),
  Sky1PlatformGetObject,
  Sky1PlatformSetObject,
  &Sky1PlatformRepositoryInfo
};

/**
  Entrypoint of Configuration Manager Dxe.

  @param  ImageHandle
  @param  SystemTable

  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
ConfigurationManagerDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEdkiiConfigurationManagerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID *)&Sky1ConfigManagerProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: Failed to install Configuration Manager Protocol." \
       " Status = %r\n",
       Status
      )
      );
    goto error_handler;
  }

  Status = InitializePlatformRepository (
             &Sky1ConfigManagerProtocol
             );
  if (EFI_ERROR (Status)) {
    DEBUG (
      (
       DEBUG_ERROR,
       "ERROR: Failed to initialize the Platform Configuration Repository." \
       " Status = %r\n",
       Status
      )
      );
  }

error_handler:
  return Status;
}
