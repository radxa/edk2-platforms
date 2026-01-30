# @file
#
#  Copyright 2024-2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.
#  Copyright 2022 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
#  Copyright (c) 2011 - 2020, ARM Limited. All rights reserved.
#  Copyright (c) 2017 - 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
#  Copyright (c) 2015 - 2021, Intel Corporation. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = O6
  PLATFORM_GUID                  = 53cfca21-0399-4802-a3c0-e86437a42183
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x0001001A
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/Radxa/Orion/$(PLATFORM_NAME)/$(PLATFORM_NAME).fdf
  PCD_DYNAMIC_AS_DYNAMICEX       = TRUE
  ACPI_IOMUX_INPUT               = Platform/Radxa/Orion/$(PLATFORM_NAME)/Drivers/AcpiPlatfomTables/RadxaO6Iomux.asl
  ACPI_IOMUX_OUTPUT              = $(OUTPUT_DIRECTORY)/Iomux.asl
  PREBUILD                       = python Platform/CIX/Sky1/Drivers/AcpiSocTables/tool/python3/ParseIomuxTemplate.py $(ACPI_IOMUX_INPUT) $(ACPI_IOMUX_OUTPUT)

!include  Platform/CIX/Sky1/Sky1Define.dsc.inc
!include  Platform/Radxa/RadxaDefine.dsc.inc
!include  Platform/Radxa/Platforms/CIX/Sky1/Sky1Define.dsc.inc

################################################################################
#
# Define Macro section - list of all macro defined by this Platform.
# Override some marco define for platform requirement
#
################################################################################

##################################################
# Define override here for evb
##################################################
  DEFINE DTB_UPDATE_ENABLE          = FALSE
  DEFINE SMBIOS_ENABLE              = FALSE
  DEFINE ACPI_ENABLE                = FALSE
  DEFINE TOKEN_CONSOLE_PREF_SUPPORT = TRUE
  DEFINE FW_VERSION_ENABLE          = TRUE
  DEFINE SOC_PWR_CLK_RST_ENABLE     = TRUE
  DEFINE WATCH_DOG_ENABLE           = FALSE
  DEFINE NO_GIC_NO_TIMER            = FALSE
  DEFINE SOC_I2C_ENABLE             = TRUE
  DEFINE I2C_EC_ENABLE              = TRUE
  DEFINE I2C_HID_ENABLE             = TRUE
  DEFINE FW_UPDATE_ENABLE           = TRUE
  DEFINE PCIE_HOST_ENABLE           = TRUE
  DEFINE SOC_CDNSP_HOST_ENABLE      = TRUE
  DEFINE PLATFORM_PD_ENABLE         = TRUE
  DEFINE SOC_GMAC_ENABLE            = FALSE
  DEFINE TOKEN_SETUP_SUPPORT        = FALSE
  DEFINE NTFS_DRIVER_SUPPORT        = FALSE
  DEFINE EXT4_DRIVER_SUPPORT        = FALSE
  DEFINE AMD_GOP_DRIVER_SUPPORT     = TRUE
  DEFINE TOKEN_RAM_DISK_SUPPORT     = FALSE
  DEFINE VARIABLE_SUPPORT           = $(COMPILE_VARIABLE_TYPE)
  DEFINE STMM_SUPPORT               = $(COMPILE_STMM_SUPPORT)
  DEFINE REALTEK_LAN_DRIVER_SUPPORT = FALSE
  DEFINE PM_CONFIG_UPDATE_SUPPORT   = FALSE
  DEFINE MEM_CONFIG_UPDATE_SUPPORT  = FALSE
  DEFINE DYNAMIC_ACPI_CPU_ENABLE    = TRUE
  DEFINE SOC_SPI_ENABLE             = TRUE
  # DEFINE SOC_GPIO_INTR_ENABLE       = TRUE
  DEFINE FV_SIMPLE_FILE_SUPPORT     = FALSE
  DEFINE SOC_GOP_ENABLE             = TRUE
  DEFINE DYNAMIC_GET_MEM_SIZE       = TRUE
  DEFINE SECURE_BOOT_ENABLE         = TRUE
  DEFINE DEFAULT_KEYS               = TRUE
  DEFINE SOC_XSPI_ENABLE            = TRUE
  DEFINE FW_CONFIG_UPDATE_SUPPORT   = TRUE
  DEFINE UEFI_FW_STAGE              = Beta2
  DEFINE BOOT_LOGO_ENABLE           = FALSE
  DEFINE GLOBAL_WATCHDOG_ENABLE     = TRUE
  DEFINE FUNC_BOOT_PERF_ENABLE      = TRUE
  DEFINE CAPSULE_ENABLE             = FALSE
  DEFINE POWER_BUTTON_ENABLE        = TRUE

!if $(COMPILE_FASTBOOT_LOAD) == nvme
  DEFINE PCIE_HOST_ENABLE           = TRUE
  DEFINE FW_UPDATE_ENABLE           = TRUE
  DEFINE SOC_USB_DEVICE_ENABLE      = TRUE
  DEFINE SOC_CDNSP_ENABLE           = TRUE
!elseif $(COMPILE_FASTBOOT_LOAD) == ddr
  DEFINE PCIE_HOST_ENABLE           = FALSE
  DEFINE FW_UPDATE_ENABLE           = TRUE
  DEFINE SOC_USB_DEVICE_ENABLE      = TRUE
  DEFINE SOC_CDNSP_ENABLE           = TRUE
!elseif $(COMPILE_FASTBOOT_LOAD) == usb
  DEFINE SOC_CDNSP_HOST_ENABLE      = TRUE
  DEFINE FW_UPDATE_ENABLE           = TRUE
  DEFINE SOC_USB_DEVICE_ENABLE      = TRUE
  DEFINE SOC_CDNSP_ENABLE           = TRUE
!endif

!if $(COMPILE_SYSTEM_LOADER) == android
  DEFINE CAPSULE_ENABLE             = TRUE
  DEFINE OPTEE_AVB_ENABLE           = FALSE
!endif

#
# Network definition
#
  DEFINE NETWORK_ENABLE                 = TRUE
!if $(NETWORK_ENABLE) == TRUE
  DEFINE NETWORK_IP4_ENABLE             = TRUE
  DEFINE NETWORK_SNP_ENABLE             = TRUE
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_IP6_ENABLE             = TRUE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = TRUE
  DEFINE NETWORK_HTTP_ENABLE            = TRUE
  DEFINE NETWORK_ALLOW_HTTP_CONNECTIONS = TRUE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
!endif

#ACPI Boot
  DEFINE ACPI_ENABLE                = TRUE
  DEFINE SMBIOS_ENABLE              = TRUE


  DEFINE SPI_VARIABLE_BASE          = 0x00380000
  DEFINE SPI_VARIABLE_SIZE          = 0x28000

  DEFINE LINUX_ACPI_CONFIG_OVERRIDE = TRUE

!include Platform/CIX/Sky1/Sky1Common.dsc.inc
!include Platform/Radxa/RadxaCommon.dsc.inc
!include Platform/Radxa/Platforms/CIX/Sky1/Sky1Common.dsc.inc
!include NetworkPkg/NetworkDefines.dsc.inc

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
# Override some library for platform requirement
#
################################################################################

[LibraryClasses.common]
  PlatformConfigParamsHookLib|Platform/Radxa/Orion/O6/Library/PlatformConfigParamsHookLib/PlatformConfigParamsHookLib.inf
  PlatformEnvHookLib|Platform/Radxa/Orion/O6/Library/PlatformEnvHookLib/PlatformEnvHookLib.inf
  RealTimeClockLib|Platform/Radxa/Library/Hym8563RealTimeClockLib/Hym8563RealTimeClockLib.inf

  PlatformBootHookLib|Platform/CIX/Sky1/Merak/Library/PlatformBootHookLib/PlatformBootHookLib.inf

  TrngLib|Silicon/CIX/Sky1/Library/TrngLib/TrngLib.inf
  RngLib|Silicon/CIX/Sky1/Library/RngLib/RngLib.inf
  DtbUpdateLibSi|Platform/CIX/Sky1/Library/DtbUpdateLibSi/DtbUpdateLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  EfiResetSystemLib|Platform/Radxa/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  EcLib|Platform/CIX/Sky1/Library/Ite5570EcLib/Ite5570EcRuntimeLib.inf

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
# Add some INF components for platform requirement
#
################################################################################
[Components.common]
# Network stack
  !include NetworkPkg/Network.dsc.inc
# This modification is to fix a PXE bug.
# If the Code Base is upgraded, this modification will cause a compilation error and should be deleted.
  NetworkPkg/UefiPxeBcDxe/UefiPxeBcDxe.inf {
    <PcdsFixedAtBuild>
      gEfiNetworkPkgTokenSpaceGuid.PcdIPv4PXESupport|TRUE
      gEfiNetworkPkgTokenSpaceGuid.PcdIPv6PXESupport|TRUE
  }

  Platform/CIX/Sky1/PrePi/PeiUniCore.inf
!if $(SHELL_EMBEDDED_ENABLE) == TRUE
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellAcpiViewCommandLib/UefiShellAcpiViewCommandLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
      gEfiShellPkgTokenSpaceGuid.PcdShellFileOperationSize|0x200000
  }
!endif
  Platform/CIX/Sky1/Drivers/DtbUpdateDxeSi/DtbUpdateDxe.inf
!if $(ACPI_ENABLE) == TRUE
  Platform/Radxa/Orion/O6/Drivers/AcpiPlatfomTables/AcpiPlatfomTables.inf {
    <BuildOptions>
      *_*_*_ASLCC_FLAGS = -I$(WORKSPACE)/$(OUTPUT_DIRECTORY)
  }
  Platform/Radxa/Drivers/AcpiPlatformDxe/AcpiPlatformDxe.inf
!endif
!if $(SMBIOS_ENABLE) == TRUE
  Platform/Radxa/Orion/O6/Drivers/PlatformSmbios/PlatformSmbios.inf
!endif
  Platform/Radxa/Orion/O6/DeviceTree/DeviceTree.inf

###################################################################################################
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process.
# add some BuildOptions for platform requirement
###################################################################################################
[BuildOptions]
  GCC:DEBUG_*_*_CC_FLAGS          = -DDEBUG_MODE
  GCC:RELEASE_*_*_CC_FLAGS        = -DMDEPKG_NDEBUG -DNDEBUG

!if $(COMPILE_FASTBOOT_LOAD) == nvme
  GCC:*_*_*_CC_FLAGS          = -DFASTBOOT_NVME
!if $(OPTEE_AVB_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS          = -DOPTEE_AVB_ENABLE
!endif
!elseif $(COMPILE_FASTBOOT_LOAD) == ddr
  GCC:*_*_*_CC_FLAGS          = -DFASTBOOT_DDR
!elseif $(COMPILE_FASTBOOT_LOAD) == usb
  GCC:*_*_*_CC_FLAGS          = -DFASTBOOT_USB
!endif

!if $(COMPILE_SMP_ENABLE) == 1
  GCC:*_*_*_CC_FLAGS              = -DMULTICORE_ENABLE
  GCC:*_*_*_ASLCC_FLAGS           = -DMULTICORE_ENABLE
  GCC:*_*_*_ASLPP_FLAGS           = -DMULTICORE_ENABLE
!endif

!if $(COMPILE_SYSTEM_LOADER) == android
  GCC:*_*_*_CC_FLAGS              = -DANDROID_BOOT
!endif

!if $(DYNAMIC_GET_MEM_SIZE) == TRUE
  GCC:*_*_*_PP_FLAGS              = -DUPDATE_MEMORY_TOP
  GCC:*_*_*_CC_FLAGS              = -DUPDATE_MEMORY_TOP
!endif

!if $(RAM_LOG_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DCONFIG_RLOG_ENABLE
!endif

!if $(REBOOT_REASON_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DREBOOT_REASON_ENABLE
!endif

!if $(NO_GIC_NO_TIMER) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DNO_GIC_NO_TIMER
!endif

  GCC:*_*_*_CC_FLAGS              = -DPHYSICAL_PHY

!if $(FV_MAIN_COMPRESSED_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DFV_MAIN_COMPRESSED
!endif

!if $(DYNAMIC_ACPI_CPU_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DDYNAMIC_ACPI_CPU_ENABLE
  GCC:*_*_*_ASLPP_FLAGS           = -DDYNAMIC_ACPI_CPU_ENABLE
  GCC:*_*_*_ASLPP_FLAGS           = -DDYNAMIC_ACPI_CPU_ENABLE
!endif

!if $(SOC_GPIO_INTR_ENABLE) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DSOC_GPIO_INTR_ENABLE
!endif

!if $(LINUX_ACPI_CONFIG_OVERRIDE) == TRUE
  GCC:*_*_*_ASLPP_FLAGS           = -DLINUX_ACPI_CONFIG_OVERRIDE
  GCC:*_*_*_ASLPP_FLAGS           = -I$(WORKSPACE)/../edk2-platforms/Platform/Radxa/Orion/O6/Drivers
!endif

!if $(STMM_SUPPORT) == TRUE
  GCC:*_*_*_CC_FLAGS              = -DSTMM_SUPPORT
!endif

!if $(FW_CONFIG_UPDATE_SUPPORT) == TRUE
  GCC:*_*_*_VFRPP_FLAGS       = -DFW_CONFIG_UPDATE_SUPPORT=1
!endif

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
# Override some PCD define for platform requirement
#
################################################################################
[PcdsFixedAtBuild.common]
  gCixPlatformTokenSpaceGuid.PcdSiliconDtbUpdateFileName|L"sky1-orion-o6.dtb"
  gCixPlatformTokenSpaceGuid.PcdSiliconDtbUpdateEnable|TRUE

  gCixTokenSpaceGuid.PcdPcieRootPort0Enable|TRUE
  gCixTokenSpaceGuid.PcdPcieRootPort1Enable|TRUE
  gCixTokenSpaceGuid.PcdPcieRootPort2Enable|TRUE
  gCixTokenSpaceGuid.PcdPcieRootPort3Enable|TRUE
  gCixTokenSpaceGuid.PcdPcieRootPort4Enable|TRUE
  gCixTokenSpaceGuid.PcdPcieRootPort0MaxSpeed|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort1MaxSpeed|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort2MaxSpeed|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort3MaxSpeed|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort4MaxSpeed|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort0LaneNum|0x03
  gCixTokenSpaceGuid.PcdPcieRootPort1LaneNum|0x02
  gCixTokenSpaceGuid.PcdPcieRootPort2LaneNum|0x01
  gCixTokenSpaceGuid.PcdPcieRootPort3LaneNum|0x00
  gCixTokenSpaceGuid.PcdPcieRootPort4LaneNum|0x00
  gCixTokenSpaceGuid.PcdPcieRootPort0PeResetPin|2
  gCixTokenSpaceGuid.PcdPcieRootPort1PeResetPin|4
  gCixTokenSpaceGuid.PcdPcieRootPort2PeResetPin|5
  gCixTokenSpaceGuid.PcdPcieRootPort3PeResetPin|3
  gCixTokenSpaceGuid.PcdPcieRootPort4PeResetPin|6

  gCixTokenSpaceGuid.PcdI2c2En|FALSE
  gCixTokenSpaceGuid.PcdI2c2BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c3En|TRUE
  gCixTokenSpaceGuid.PcdI2c3Runtime|TRUE          # For RTC runtime service
  gCixTokenSpaceGuid.PcdI2c3BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c4En|TRUE
  gCixTokenSpaceGuid.PcdI2c4BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c5En|TRUE
  gCixTokenSpaceGuid.PcdI2c5BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c6En|TRUE
  gCixTokenSpaceGuid.PcdI2c6BusFreq|50000

  # PD
  gCixTokenSpaceGuid.PcdI2c0En|TRUE
  gCixTokenSpaceGuid.PcdI2c0BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c1En|TRUE
  gCixTokenSpaceGuid.PcdI2c1BusFreq|100000
  gCixTokenSpaceGuid.PcdI2c7En|TRUE
  gCixTokenSpaceGuid.PcdI2c7BusFreq|100000

  # RTC I2C canot be controlled in setup
  gCixTokenSpaceGuid.PcdI2cCtrlEn|0xF7

  gCixPlatformTokenSpaceGuid.PcdPdDevI2cBuses|{ 0x1, 0xFF, 0x1, 0xFF }
  gCixPlatformTokenSpaceGuid.PcdPdDevI2cSlaveAddresses|{ 0x30, 0xFF, 0x31, 0xFF }
  gCixPlatformTokenSpaceGuid.PcdPdDevAlertPins|{ 9, 0xFF, 9, 0xFF }
  gCixPlatformTokenSpaceGuid.PcdTypecPortDefaultModes|{ 1, 4, 1, 4}

  # USB3_A
  gCixTokenSpaceGuid.PcdUsb3Control0Enable|TRUE
  gCixTokenSpaceGuid.PcdUsb3Control1Enable|TRUE

  # USBC0
  gCixTokenSpaceGuid.PcdUsbCDrdControl0Enable|TRUE
  gCixTokenSpaceGuid.PcdUsbCDrdControl0DataRole|TRUE
  # USBC1
  gCixTokenSpaceGuid.PcdUsbCControl0Enable|TRUE
  # USBC2
  gCixTokenSpaceGuid.PcdUsbCControl1Enable|TRUE
  # USBC3
  gCixTokenSpaceGuid.PcdUsbCControl2Enable|TRUE

  gCixTokenSpaceGuid.PcdUsb2Control0Enable|TRUE
  gCixTokenSpaceGuid.PcdUsb2Control1Enable|TRUE
  gCixTokenSpaceGuid.PcdUsb2Control2Enable|TRUE
  gCixTokenSpaceGuid.PcdUsb2Control3Enable|TRUE

  gCixTokenSpaceGuid.PcdAcpiI2s5Enable|TRUE
  gCixTokenSpaceGuid.PcdAcpiI2s6Enable|TRUE
  gCixTokenSpaceGuid.PcdAcpiI2s7Enable|TRUE
  gCixTokenSpaceGuid.PcdAcpiI2s8Enable|TRUE
  gCixTokenSpaceGuid.PcdAcpiI2s9Enable|TRUE

  gArmTokenSpaceGuid.PcdSystemMemorySize|0x400000000
  gEfiNetworkPkgTokenSpaceGuid.PcdNetworkStackSupport|FALSE
  gEfiNetworkPkgTokenSpaceGuid.PcdIPv4PXESupport|TRUE
  gEfiNetworkPkgTokenSpaceGuid.PcdIPv6PXESupport|TRUE
  gEfiNetworkPkgTokenSpaceGuid.PcdIPv4HttpSupport|TRUE
  gEfiNetworkPkgTokenSpaceGuid.PcdIPv6HttpSupport|TRUE

  gCixTokenSpaceGuid.PcdSpi0En|TRUE
# ISP Camera Power
  gCixTokenSpaceGuid.PcdIspCamera0Power|0x00
  gCixTokenSpaceGuid.PcdIspCamera1Power|0x00
  gCixTokenSpaceGuid.PcdIspCamera2Power|0x00
  gCixTokenSpaceGuid.PcdIspCamera3Power|0x00

  gCixPlatformTokenSpaceGuid.PcdEcAcpiI2cEn|TRUE
  gCixPlatformTokenSpaceGuid.PcdAcpiGpio0IoMask|0x20000000 # vbus for usb port6-7
  gCixPlatformTokenSpaceGuid.PcdAcpiGpio3IoMask|0x00018000 # pwm/edp en pin output

# Platform specific defaults
  # Set SMBIOS product name
  gArmTokenSpaceGuid.PcdSystemProductName|L"Radxa Orion O6"

  # RTC (taken from Phecda PcdI2c3BusFreq)
  gHym8563RealTimeClockLibTokenSpaceGuid.PcdI2cBusFrequency|100000

  # Fill in dpu index to config display priority
  # Index | Output Name
  # ------|------------
  #     0 | USB-C 0
  #     1 | HDMI
  #     2 | eDP
  #     3 | USB-C 1
  #     4 | DP
  gCixTokenSpaceGuid.PcdDPPriority0|1 # highest priority
  gCixTokenSpaceGuid.PcdDPPriority1|4 #
  gCixTokenSpaceGuid.PcdDPPriority2|0 #
  gCixTokenSpaceGuid.PcdDPPriority3|3 #
  gCixTokenSpaceGuid.PcdDPPriority4|2 # lowest priority

  gCixPlatformTokenSpaceGuid.PcdAcpiPrefPmProf|0x01  # Desktop
  gCixTokenSpaceGuid.PcdAcpiCsiDmaEnable|FALSE

[PcdsDynamicDefault.common]

  gEmbeddedTokenSpaceGuid.PcdDmaDeviceLimit|0x47fffffff

  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1920
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600

  gCixPlatformTokenSpaceGuid.PcdDynamicUint64Test|0x11111111

!if $(COMPILE_SYSTEM_LOADER) == android
  gCixPlatformTokenSpaceGuid.AndroidFastboot|TRUE
!endif
[PcdsDynamicHii.common.DEFAULT]
