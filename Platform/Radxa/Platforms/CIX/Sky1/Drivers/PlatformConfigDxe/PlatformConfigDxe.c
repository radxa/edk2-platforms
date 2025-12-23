/** @file
*
*  Copyright 2024 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.
*  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "PlatformConfigDxe.h"

extern  UINT8  PlatformConfigHiiBin[];
extern  UINT8  PlatformConfigDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH  mPlatformConfigDxeVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    PLATFORM_CONFIG_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Request         A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress        On return, points to a character in the Request string.
                         Points to the string's null terminator if request was successful.
                         Points to the most recent '&' before the first failing name/value
                         pair (or the beginning of the string if the failure is in the
                         first name/value pair) if the request was not successful.
  @param Results         A null-terminated Unicode string in <ConfigAltResp> format which
                         has all values filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval  EFI_SUCCESS            The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
PlatformConfigExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT EFI_STRING                            *Progress,
  OUT EFI_STRING                            *Results
  )
{
  if ((Progress == NULL) || (Results == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a: Request = %s.\n", __FUNCTION__, Request));

  *Progress = Request;
  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Configuration   A null-terminated Unicode string in <ConfigResp> format.
  @param Progress        A pointer to a string filled in with the offset of the most
                         recent '&' before the first failing name/value pair (or the
                         beginning of the string if the failure is in the first
                         name/value pair) or the terminating NULL if all was successful.

  @retval  EFI_SUCCESS            The Results is processed successfully.
  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
PlatformConfigRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                            *Progress
  )
{
  if (Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  if (Configuration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HiiIsConfigHdrMatch (Configuration, &gPlatformConfigFormSetGuid, COMPLIANCE_VAR)) {
    DEBUG ((DEBUG_INFO, "%a: COMPLIANCE_VAR won't be saved.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a: Configuration = %s.\n", __FUNCTION__, Configuration));

  return EFI_NOT_FOUND;
}

/**
  This function is invoked if user selected a interactive opcode from Device Manager's
  Formset.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval  EFI_SUCCESS           The callback successfully handled the action.
  @retval  EFI_INVALID_PARAMETER The setup browser call this function with invalid parameters.

**/
EFI_STATUS
EFIAPI
PlatformConfigCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  IN  UINT8                                 Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  )
{
  NETWORK_STACK             NetworkStackVar;
  PLATFORM_SETUP_DATA       PlatformSetupVar;
  COMPLIANCE_VARSTORE_DATA  ComplianceVar;

  switch (QuestionId) {
    case KEY_ENABLE_NETWORK_STACK:
      DEBUG ((DEBUG_INFO, "%a:  QuestionId = KEY_ENABLE_NETWORK_STACK\n", __FUNCTION__));
      switch (Action) {
        case EFI_BROWSER_ACTION_CHANGED:
          if (Value->b) {
            DEBUG ((DEBUG_INFO, "  Enable Network Stack\n"));
            HiiGetBrowserData (&gEfiNetworkStackSetupGuid, NETWORK_STACK_VAR, sizeof (NetworkStackVar), (UINT8 *)&NetworkStackVar);
            NetworkStackVar.Ipv4Pxe = 1;
            NetworkStackVar.Ipv6Pxe = 1;
            NetworkStackVar.Ipv4Http = 1;
            NetworkStackVar.Ipv6Http = 1;
            HiiSetBrowserData (&gEfiNetworkStackSetupGuid, NETWORK_STACK_VAR, sizeof (NetworkStackVar), (UINT8 *)&NetworkStackVar, NULL);
          }
          break;
        default:
          DEBUG ((DEBUG_INFO, "%a:  Unkown Action\n", __FUNCTION__));
          DEBUG ((DEBUG_INFO, "    Action = %u.\n", Action));
          return EFI_UNSUPPORTED;
          break;
      }
      break;
    case KEY_DISABLE_ACPI_CPPC:
      DEBUG ((DEBUG_INFO, "%a:  QuestionId = KEY_DISABLE_ACPI_CPPC\n", __FUNCTION__));
      HiiGetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar);
      switch (Action) {
        case EFI_BROWSER_ACTION_RETRIEVE:
          Value->b = (PlatformSetupVar.CpuCppcType == 0);
          HiiGetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar);
          ComplianceVar.DisableCPPC = Value->b;
          HiiSetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar, NULL);
          break;
        case EFI_BROWSER_ACTION_CHANGED:
          DEBUG ((DEBUG_INFO, "  Adjust CpuCppcType\n"));
          PlatformSetupVar.CpuCppcType = (UINT8)(Value->b ? 0 : 1);
          HiiSetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar, NULL);
          break;
        default:
          DEBUG ((DEBUG_INFO, "%a:  Unkown Action\n", __FUNCTION__));
          DEBUG ((DEBUG_INFO, "    Action = %u.\n", Action));
          return EFI_UNSUPPORTED;
          break;
      }
      break;
    case KEY_DISABLE_SMALL_CORE:
      DEBUG ((DEBUG_INFO, "%a:  QuestionId = KEY_DISABLE_SMALL_CORE\n", __FUNCTION__));
      HiiGetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar);
      switch (Action) {
        case EFI_BROWSER_ACTION_RETRIEVE:
          Value->b = (PlatformSetupVar.CpuCoreEnable[2] == 0) &&
                     (PlatformSetupVar.CpuCoreEnable[3] == 0) &&
                     (PlatformSetupVar.CpuCoreEnable[4] == 0) &&
                     (PlatformSetupVar.CpuCoreEnable[5] == 0);
          HiiGetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar);
          ComplianceVar.DisableSmallCores = Value->b;
          HiiSetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar, NULL);
          break;
        case EFI_BROWSER_ACTION_CHANGED:
          DEBUG ((DEBUG_INFO, "  Adjust CpuCoreEnable\n"));
          PlatformSetupVar.CpuCoreEnable[2] = (Value->b ? 0 : 1);
          PlatformSetupVar.CpuCoreEnable[3] = (Value->b ? 0 : 1);
          PlatformSetupVar.CpuCoreEnable[4] = (Value->b ? 0 : 1);
          PlatformSetupVar.CpuCoreEnable[5] = (Value->b ? 0 : 1);
          HiiSetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar, NULL);
          break;
        default:
          DEBUG ((DEBUG_INFO, "%a:  Unkown Action\n", __FUNCTION__));
          DEBUG ((DEBUG_INFO, "    Action = %u.\n", Action));
          return EFI_UNSUPPORTED;
          break;
      }
      break;
    case KEY_ENABLE_ACPI_LPI0:
      DEBUG ((DEBUG_INFO, "%a:  QuestionId = KEY_ENABLE_ACPI_LPI0\n", __FUNCTION__));
      HiiGetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar);
      switch (Action) {
        case EFI_BROWSER_ACTION_RETRIEVE:
          Value->b = (PlatformSetupVar.CpuLpiState == 1);
          HiiGetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar);
          ComplianceVar.EnableLPI0 = Value->b;
          HiiSetBrowserData (&gPlatformConfigFormSetGuid, COMPLIANCE_VAR, sizeof (ComplianceVar), (UINT8 *)&ComplianceVar, NULL);
          break;
        case EFI_BROWSER_ACTION_CHANGED:
          DEBUG ((DEBUG_INFO, "  Adjust CpuLpiState\n"));
          PlatformSetupVar.CpuLpiState = (UINT8)(Value->b ? 1 : FixedPcdGet8 (PcdAcpiCpuLpiState));
          HiiSetBrowserData (&gPlatformSetupVariableGuid, PLATFORM_SETUP_VAR, sizeof (PlatformSetupVar), (UINT8 *)&PlatformSetupVar, NULL);
          break;
        default:
          DEBUG ((DEBUG_INFO, "%a:  Unkown Action\n", __FUNCTION__));
          DEBUG ((DEBUG_INFO, "    Action = %u.\n", Action));
          return EFI_UNSUPPORTED;
          break;
      }
      break;
    default:
      DEBUG ((DEBUG_INFO, "%a:  Unkown Action\n", __FUNCTION__));
      DEBUG ((DEBUG_INFO, "    Action = %u.\n", Action));
      DEBUG ((DEBUG_INFO, "    QuestionId = 0x%x.\n", QuestionId));
      DEBUG ((DEBUG_INFO, "    Type = %u.\n", Type));
      DEBUG ((DEBUG_INFO, "    Value = 0x%x.\n", *Value));

      return EFI_UNSUPPORTED;
      break;
  }

  return EFI_SUCCESS;
}

STATIC EFI_HII_CONFIG_ACCESS_PROTOCOL mPlatformConfigDxeConfigAccessProtocol = {
  PlatformConfigExtractConfig,
  PlatformConfigRouteConfig,
  PlatformConfigCallback
};

STATIC
EFI_STATUS
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS      Status;
  EFI_HII_HANDLE  HiiHandle;
  EFI_HANDLE      DriverHandle;

  DriverHandle = NULL;
  Status       = gBS->InstallMultipleProtocolInterfaces (
                        &DriverHandle,
                        &gEfiDevicePathProtocolGuid,
                        &mPlatformConfigDxeVendorDevicePath,
                        &gEfiHiiConfigAccessProtocolGuid,
                        &mPlatformConfigDxeConfigAccessProtocol,
                        NULL
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (
                &gPlatformConfigFormSetGuid,
                DriverHandle,
                PlatformConfigDxeStrings,
                PlatformConfigHiiBin,
                NULL
                );

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mPlatformConfigDxeVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mPlatformConfigDxeConfigAccessProtocol,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  The entry point for PlatformConfigDxe driver.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_ALREADY_STARTED     The driver already exists in system.
  @retval EFI_OUT_OF_RESOURCES    Fail to execute entry point due to lack of
                                  resources.
  @retval EFI_SUCCESS             All the related protocols are installed on
                                  the driver.

**/
EFI_STATUS
EFIAPI
PlatformConfigDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return InstallHiiPages();
}
