[LibraryClasses.common]

  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  PdEcLib|Platform/Radxa/Library/PdEcLibNull/PdEcLibNull.inf

!if $(CAPSULE_ENABLE) == FALSE
  EdkiiSystemCapsuleLib|SignedCapsulePkg/Library/EdkiiSystemCapsuleLib/EdkiiSystemCapsuleLib.inf
  FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
!endif

[PcdsFixedAtBuild.common]

!if $(DEBUG_ON_UART3) == FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040D0000 #0x091e1000 #0x0a051000
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040E0000
!endif

  # Default State After G3 is Power On
  gCixTokenSpaceGuid.PcdStateAfterG3|0x01

  # SPCR use UART2
  gArmPlatformTokenSpaceGuid.PcdSerialDbgRegisterBase|0x040D0000
  gArmPlatformTokenSpaceGuid.PcdSerialDbgInterrupt|330
  gCixPlatformTokenSpaceGuid.PcdAcpiSpcrEnable|TRUE

  # Default hardware description should be ACPI
  gEmbeddedTokenSpaceGuid.PcdDefaultDtPref|FALSE
  gCixPlatformTokenSpaceGuid.PcdAcpiSpcrEnable|TRUE

  # Default hardware description should be ACPI
  gEmbeddedTokenSpaceGuid.PcdDefaultDtPref|FALSE

  # Enable performance measurment
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
  gEfiMdeModulePkgTokenSpaceGuid.PcdExtFpdtBootRecordPadSize|0x300000

!if $(CAPSULE_ENABLE)
  # Must match SYSTEM_FMP_ESRT_GUID
  # gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid is defined in Platform/CIX/Sky1/Sky1Common.dsc.inc
  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageTypeIdGuid|{GUID("351c9808-4b53-4363-9314-d339386e5685")}
!endif

[Components.common]

!if $(DEBUG_ON_UART3) == FALSE
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
!else
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf {
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040D0000
  }
!endif

  Platform/Radxa/Drivers/Logo/LogoDxe.inf

  Platform/Radxa/Platforms/CIX/Sky1/Drivers/SystemInfoDxe/SystemInfoDxe.inf

!if $(TOKEN_SETUP_SUPPORT) == FALSE
  Platform/CIX/Sky1/Drivers/PlatformSetupVariableInitDxe/PlatformSetupVariableInitDxe.inf
  Platform/Radxa/Platforms/CIX/Sky1/Drivers/PlatformConfigDxe/PlatformConfigDxe.inf
!endif

!if $(FW_CONFIG_UPDATE_SUPPORT) == TRUE
  Platform/Radxa/Platforms/CIX/Sky1/Drivers/RadxaSetupVariableInitDxe/RadxaSetupVariableInitDxe.inf
  Platform/Radxa/Platforms/CIX/Sky1/Drivers/FirmwareConfigUpdateDxe/FirmwareConfigUpdateDxe.inf
!endif
  Platform/Radxa/Platforms/CIX/Sky1/Drivers/SeConfigUpdateDxe/SeConfigUpdateDxe.inf
  EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf {
    <LibraryClasses>
      DtPlatformDtbLoaderLib|EmbeddedPkg/Library/DxeDtPlatformDtbLoaderLibDefault/DxeDtPlatformDtbLoaderLibDefault.inf
  }

!if $(VARIABLE_SUPPORT) == "SPI" && $(STMM_SUPPORT) == TRUE
  MdeModulePkg/Application/VariableInfo/VariableInfo.inf
!endif

!if $(TOKEN_RAM_DISK_SUPPORT) == FALSE
  MdeModulePkg/Universal/Disk/RamDiskDxe/RamDiskDxe.inf
!endif

  #
  # AHCI Support
  #
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf

  #
  # SATA Controller
  #
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf

  #
  # Firmware Performance Data Table (FPDT)
  #
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf {
    <LibraryClasses>
      LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf
  }

!if $(CAPSULE_ENABLE)
  MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf
  FmpDevicePkg/FmpDxe/FmpDxe.inf {
    <Defines>
      #
      # FILE_GUID is used as ESRT GUID
      #
      FILE_GUID = $(SYSTEM_FMP_ESRT_GUID)
    <PcdsFixedAtBuild>
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"Radxa System Firmware Device"
      !include BaseTools/Source/Python/Pkcs7Sign/TestRoot.cer.gFmpDevicePkgTokenSpaceGuid.PcdFmpDevicePkcs7CertBufferXdr.inc
    <LibraryClasses>
      #
      # Generic libraries that are used "as is" by all FMP modules
      #
      FmpPayloadHeaderLib|FmpDevicePkg/Library/FmpPayloadHeaderLibV1/FmpPayloadHeaderLibV1.inf
      FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
      FmpDependencyLib|FmpDevicePkg/Library/FmpDependencyLib/FmpDependencyLib.inf
      FmpDependencyCheckLib|FmpDevicePkg/Library/FmpDependencyCheckLibNull/FmpDependencyCheckLibNull.inf
      FmpDependencyDeviceLib|FmpDevicePkg/Library/FmpDependencyDeviceLibNull/FmpDependencyDeviceLibNull.inf
      #
      # Platform specific capsule policy library
      #
      CapsuleUpdatePolicyLib|FmpDevicePkg/Library/CapsuleUpdatePolicyLibNull/CapsuleUpdatePolicyLibNull.inf
      #
      # Device specific library that processes a capsule and updates the FW storage device
      #
      FmpDeviceLib|FmpDevicePkg/Library/FmpDeviceLibNull/FmpDeviceLibNull.inf
  }
!else
  MdeModulePkg/Universal/EsrtDxe/EsrtDxe.inf
  Platform/CIX/Sky1/Drivers/SystemFirmwareUpdate/SystemFirmwareReportDxe.inf
!endif
