## @file
#
#  Copyright 2024 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.
#  Copyright (c) 2018, Linaro Ltd. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  INF Platform/Radxa/Drivers/Logo/LogoDxe.inf
  INF Platform/Radxa/Platforms/CIX/Sky1/Drivers/SystemInfoDxe/SystemInfoDxe.inf

!if $(TOKEN_SETUP_SUPPORT) == FALSE
  INF Platform/CIX/Sky1/Drivers/PlatformSetupVariableInitDxe/PlatformSetupVariableInitDxe.inf
  INF Platform/Radxa/Platforms/CIX/Sky1/Drivers/PlatformConfigDxe/PlatformConfigDxe.inf
!endif

!if $(FW_CONFIG_UPDATE_SUPPORT) == TRUE
  INF Platform/Radxa/Platforms/CIX/Sky1/Drivers/RadxaSetupVariableInitDxe/RadxaSetupVariableInitDxe.inf
  INF Platform/Radxa/Platforms/CIX/Sky1/Drivers/FirmwareConfigUpdateDxe/FirmwareConfigUpdateDxe.inf
!endif
  INF Platform/Radxa/Platforms/CIX/Sky1/Drivers/SeConfigUpdateDxe/SeConfigUpdateDxe.inf
  INF EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf

!if $(VARIABLE_SUPPORT) == "SPI" && $(STMM_SUPPORT) == TRUE
  INF MdeModulePkg/Application/VariableInfo/VariableInfo.inf
!endif

!if $(TOKEN_RAM_DISK_SUPPORT) == FALSE
  INF MdeModulePkg/Universal/Disk/RamDiskDxe/RamDiskDxe.inf
!endif

!if $(ARCH) == AARCH64
  INF Emulator/X86EmulatorDxe/X86EmulatorDxe.inf
!endif

  #
  # AHCI Support
  #
  INF MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  INF MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf

  #
  # SATA Controller
  #
  INF MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf

  #
  # Firmware Performance Data Table (FPDT)
  #
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

!if $(CAPSULE_ENABLE)
  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf
  INF FILE_GUID = $(SYSTEM_FMP_ESRT_GUID) FmpDevicePkg/FmpDxe/FmpDxe.inf
!else
  INF MdeModulePkg/Universal/EsrtDxe/EsrtDxe.inf
  INF Platform/CIX/Sky1/Drivers/SystemFirmwareUpdate/SystemFirmwareReportDxe.inf
!endif
