/** @file
*
*  Copyright 2024 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.
*  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __PLATFORM_CONFIG_HII_H__
#define __PLATFORM_CONFIG_HII_H__

#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/NetworkStackSetup.h>
#include <Guid/ConsolePrefFormSet.h>
#include <PlatformSetupVar.h>

#define CONSOLE_PREF_GRAPHICAL  0x0
#define CONSOLE_PREF_SERIAL     0x1

typedef struct {
  UINT8    Console;
  UINT8    Reserved[3];
} CONSOLE_PREF_VARSTORE_DATA;

typedef struct {
  BOOLEAN    DisableCPPC;
  BOOLEAN    DisableSmallCores;
  BOOLEAN    EnableLPI0;
} COMPLIANCE_VARSTORE_DATA;

#define COMPLIANCE_VAR  L"ComplianceVar"

#define PLATFORM_CONFIG_FORMSET_GUID  \
  { 0x7dbfcdc4, 0xfb50, 0x452a, { 0xae, 0xd6, 0xfe, 0x21, 0x63, 0x70, 0x24, 0x61 } }

#define PLATFORM_SETUP_VARIABLE_GUID \
  {0x5E5B2ABF, 0x599A, 0x4329, {0xBA, 0xA4, 0x6D, 0x6E, 0xAA, 0xAC, 0x8B, 0xC1}}
#define CIX_GLOBAL_VARIABLE_GUID \
  {0x31d1bbc6, 0xc793, 0x48af, {0x8d, 0xcf, 0xf9, 0x18, 0x38, 0xc7, 0x3a, 0x2e}}

extern EFI_GUID  gPlatformConfigFormSetGuid;

#define PLATFORM_CONFIG_FORM_ID     0x2001

// #define SYSTEM_SETUP_FORM_SET_ID    0x2001
#define SOC_FORM_SET_ID             0x2002
#define ADVANCED_FORM_SET_ID        0x2003
#define HARDWARE_INFO_FORM_SET_ID   0x2004
#define PLATFORM_FORM_SET_ID        0x2005
#define MEMORY_CONFIG_FORM_ID       0x2010
#define PCIE_CONFIG_FORM_ID         0x2011
#define USB_CONFIG_FORM_ID          0x2012
#define I2C_CONFIG_FORM_ID          0x2013
#define CPU_CONFIG_FORM_ID          0x2014
#define GMAC_CONFIG_FORM_ID         0x2015
#define COMPLIANCE_CONFIG_FORM_ID   0x2016

#define PCIE_ROOT_PORT0_CONFIG_FORM_ID  0x2100
#define PCIE_ROOT_PORT1_CONFIG_FORM_ID  0x2101
#define PCIE_ROOT_PORT2_CONFIG_FORM_ID  0x2102
#define PCIE_ROOT_PORT3_CONFIG_FORM_ID  0x2103
#define PCIE_ROOT_PORT4_CONFIG_FORM_ID  0x2104

#define I2C0_CONFIG_FORM_ID  0x2300
#define I2C1_CONFIG_FORM_ID  0x2301
#define I2C2_CONFIG_FORM_ID  0x2302
#define I2C3_CONFIG_FORM_ID  0x2303
#define I2C4_CONFIG_FORM_ID  0x2304
#define I2C5_CONFIG_FORM_ID  0x2305
#define I2C6_CONFIG_FORM_ID  0x2306
#define I2C7_CONFIG_FORM_ID  0x2307

#define QUESTION_STATE_AFTER_G3_ID  0x1800
#define KEY_ENABLE_NETWORK_STACK    0x1900
#define KEY_DISABLE_ACPI_CPPC       0x1901
#define KEY_DISABLE_SMALL_CORE      0x1902
#define KEY_ENABLE_ACPI_LPI0        0x1903

#endif
