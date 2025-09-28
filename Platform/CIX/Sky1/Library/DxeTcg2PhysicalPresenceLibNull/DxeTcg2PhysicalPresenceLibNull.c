/** @file
  Execute pending TPM2 requests from OS or BIOS.

  Caution: This module requires additional review when modified.
  This driver will have external input - variable.
  This external input must be validated carefully to avoid security issue.

  Tpm2ExecutePendingTpmRequest() will receive untrusted input and do validation.

Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Library/Tcg2PhysicalPresenceLib.h>
#include <Guid/Tcg2PhysicalPresenceData.h>

/**
  Get string by string id from HII Interface.

  @param[in] Id          String ID.

  @retval    CHAR16 *    String from ID.
  @retval    NULL        If error occurs.

**/
CHAR16 *
Tcg2PhysicalPresenceGetStringById (
  IN  EFI_STRING_ID  Id
  )
{
  return NULL;
}

/**
  Send ClearControl and Clear command to TPM.

  @param[in]  PlatformAuth      platform auth value. NULL means no platform auth change.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
Tpm2CommandClear (
  IN TPM2B_AUTH  *PlatformAuth  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Change EPS.

  @param[in]  PlatformAuth      platform auth value. NULL means no platform auth change.

  @retval EFI_SUCCESS Operation completed successfully.
**/
EFI_STATUS
Tpm2CommandChangeEps (
  IN TPM2B_AUTH  *PlatformAuth  OPTIONAL
  )
{
    return EFI_UNSUPPORTED;
}

/**
  Execute physical presence operation requested by the OS.

  @param[in]      PlatformAuth        platform auth value. NULL means no platform auth change.
  @param[in]      CommandCode         Physical presence operation value.
  @param[in]      CommandParameter    Physical presence operation parameter.
  @param[in, out] PpiFlags            The physical presence interface flags.

  @retval TCG_PP_OPERATION_RESPONSE_BIOS_FAILURE   Unknown physical presence operation.
  @retval TCG_PP_OPERATION_RESPONSE_BIOS_FAILURE   Error occurred during sending command to TPM or
                                                   receiving response from TPM.
  @retval Others                                   Return code from the TPM device after command execution.
**/
UINT32
Tcg2ExecutePhysicalPresence (
  IN      TPM2B_AUTH                        *PlatformAuth   OPTIONAL,
  IN      UINT32                            CommandCode,
  IN      UINT32                            CommandParameter,
  IN OUT  EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  *PpiFlags
  )
{
  return TCG_PP_OPERATION_RESPONSE_BIOS_FAILURE;
}

/**
  Read the specified key for user confirmation.

  @param[in]  CautionKey  If true,  F12 is used as confirm key;
                          If false, F10 is used as confirm key.

  @retval     TRUE        User confirmed the changes by input.
  @retval     FALSE       User discarded the changes.
**/
BOOLEAN
Tcg2ReadUserKey (
  IN     BOOLEAN  CautionKey
  )
{
  return FALSE;
}

/**
  Fill Buffer With BootHashAlg.

  @param[in] Buffer               Buffer to be filled.
  @param[in] BufferSize           Size of buffer.
  @param[in] BootHashAlg          BootHashAlg.

**/
VOID
Tcg2FillBufferWithBootHashAlg (
  IN UINT16  *Buffer,
  IN UINTN   BufferSize,
  IN UINT32  BootHashAlg
  )
{
}

/**
  Display the confirm text and get user confirmation.

  @param[in] TpmPpCommand             The requested TPM physical presence command.
  @param[in] TpmPpCommandParameter    The requested TPM physical presence command parameter.

  @retval    TRUE          The user has confirmed the changes.
  @retval    FALSE         The user doesn't confirm the changes.
**/
BOOLEAN
Tcg2UserConfirm (
  IN      UINT32  TpmPpCommand,
  IN      UINT32  TpmPpCommandParameter
  )
{
  return FALSE;
}

/**
  Check if there is a valid physical presence command request. Also updates parameter value
  to whether the requested physical presence command already confirmed by user

   @param[in]  TcgPpData                 EFI Tcg2 Physical Presence request data.
   @param[in]  Flags                     The physical presence interface flags.
   @param[out] RequestConfirmed          If the physical presence operation command required user confirm from UI.
                                           True, it indicates the command doesn't require user confirm, or already confirmed
                                                 in last boot cycle by user.
                                           False, it indicates the command need user confirm from UI.

   @retval  TRUE        Physical Presence operation command is valid.
   @retval  FALSE       Physical Presence operation command is invalid.

**/
BOOLEAN
Tcg2HaveValidTpmRequest  (
  IN      EFI_TCG2_PHYSICAL_PRESENCE        *TcgPpData,
  IN      EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  Flags,
  OUT     BOOLEAN                           *RequestConfirmed
  )
{
  return FALSE;
}

/**
  Check and execute the requested physical presence command.

  Caution: This function may receive untrusted input.
  TcgPpData variable is external input, so this function will validate
  its data structure to be valid value.

  @param[in]      PlatformAuth      platform auth value. NULL means no platform auth change.
  @param[in, out] TcgPpData         Pointer to the physical presence NV variable.
  @param[in, out] Flags             Pointer to the physical presence interface flags.
**/
VOID
Tcg2ExecutePendingTpmRequest (
  IN      TPM2B_AUTH                        *PlatformAuth   OPTIONAL,
  IN OUT  EFI_TCG2_PHYSICAL_PRESENCE        *TcgPpData,
  IN OUT  EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  *Flags
  )
{
  return;
}

/**
  Check and execute the pending TPM request.

  The TPM request may come from OS or BIOS. This API will display request information and wait
  for user confirmation if TPM request exists. The TPM request will be sent to TPM device after
  the TPM request is confirmed, and one or more reset may be required to make TPM request to
  take effect.

  This API should be invoked after console in and console out are all ready as they are required
  to display request information and get user input to confirm the request.

  @param[in]  PlatformAuth                   platform auth value. NULL means no platform auth change.
**/
VOID
EFIAPI
Tcg2PhysicalPresenceLibProcessRequest (
  IN      TPM2B_AUTH  *PlatformAuth  OPTIONAL
  )
{
  return;
}

/**
  Check if the pending TPM request needs user input to confirm.

  The TPM request may come from OS. This API will check if TPM request exists and need user
  input to confirmation.

  @retval    TRUE        TPM needs input to confirm user physical presence.
  @retval    FALSE       TPM doesn't need input to confirm user physical presence.

**/
BOOLEAN
EFIAPI
Tcg2PhysicalPresenceLibNeedUserConfirm (
  VOID
  )
{
  return FALSE;
}

/**
  The handler for TPM physical presence function:
  Return TPM Operation Response to OS Environment.

  @param[out]     MostRecentRequest Most recent operation request.
  @param[out]     Response          Response to the most recent operation request.

  @return Return Code for Return TPM Operation Response to OS Environment.
**/
UINT32
EFIAPI
Tcg2PhysicalPresenceLibReturnOperationResponseToOsFunction (
  OUT UINT32  *MostRecentRequest,
  OUT UINT32  *Response
  )
{
  return TCG_PP_RETURN_TPM_OPERATION_RESPONSE_FAILURE;
}

/**
  The handler for TPM physical presence function:
  Submit TPM Operation Request to Pre-OS Environment and
  Submit TPM Operation Request to Pre-OS Environment 2.

  Caution: This function may receive untrusted input.

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      RequestParameter TPM physical presence operation request parameter.

  @return Return Code for Submit TPM Operation Request to Pre-OS Environment and
          Submit TPM Operation Request to Pre-OS Environment 2.
**/
UINT32
EFIAPI
Tcg2PhysicalPresenceLibSubmitRequestToPreOSFunction (
  IN UINT32  OperationRequest,
  IN UINT32  RequestParameter
  )
{
  return TCG_PP_SUBMIT_REQUEST_TO_PREOS_GENERAL_FAILURE;
}

/**
  Return TPM2 ManagementFlags set by PP interface.

  @retval    ManagementFlags    TPM2 Management Flags.
**/
UINT32
EFIAPI
Tcg2PhysicalPresenceLibGetManagementFlags (
  VOID
  )
{
  return -1;
}
