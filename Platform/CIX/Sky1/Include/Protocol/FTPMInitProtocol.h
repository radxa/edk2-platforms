#/** @file
#
#  Copyright 2022 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
#
#**/


#ifndef _FTPM_INIT_PROTOCOL_H_
#define _FTPM_INIT_PROTOCOL_H_

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/OpteeClientApiLib.h>
#include <Library/tee_client_api.h>
#include <Library/Tcg2PhysicalPresenceLib.h>

#include <Protocol/TrEEProtocol.h>

#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/Tpm2Acpi.h>

#define CIX_FTPM_INIT_PROTOCOL_GUID \
        {0xc999ecfb, 0x43f8, 0xeb6e, { 0x4b, 0x55, 0x10, 0x1a, 0x06, 0x01, 0xa3, 0x55 }}

typedef struct _FTPM_INIT_PROTOCOL FTPM_INIT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI* GET_FTPM_SESSION) (
  IN FTPM_INIT_PROTOCOL *this,
  OUT TEEC_Context* Context,
  OUT TEEC_Session* Session
  );

typedef
EFI_STATUS
(EFIAPI* FTPM_OPEN_SESSION) (
  IN FTPM_INIT_PROTOCOL *this
  );

typedef
BOOLEAN
(EFIAPI* FTPM_GET_STATUS) (
  IN FTPM_INIT_PROTOCOL *this
  );

struct _FTPM_INIT_PROTOCOL {
    GET_FTPM_SESSION    GetFTPMSession;
    FTPM_OPEN_SESSION   FTPMOpenSession;
    FTPM_GET_STATUS     GetFTPMStatus;
};

extern EFI_GUID  gCixFtpmInitProtocolGuid;
#endif
