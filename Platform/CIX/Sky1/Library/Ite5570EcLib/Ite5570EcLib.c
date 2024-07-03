/**

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/EcLib.h>

STATIC I2C_HOST_DESCRIPTOR  mHost;

STATIC
UINT32
I2cGetBusClk (
  IN UINT8  Bus
  )
{
  switch (Bus) {
    case 0:
      return FixedPcdGet32 (PcdI2c0BusFreq);
    case 1:
      return FixedPcdGet32 (PcdI2c1BusFreq);
    case 2:
      return FixedPcdGet32 (PcdI2c2BusFreq);
    case 3:
      return FixedPcdGet32 (PcdI2c3BusFreq);
    case 4:
      return FixedPcdGet32 (PcdI2c4BusFreq);
    case 5:
      return FixedPcdGet32 (PcdI2c5BusFreq);
    case 6:
      return FixedPcdGet32 (PcdI2c6BusFreq);
    case 7:
      return FixedPcdGet32 (PcdI2c7BusFreq);
    default:
      DebugPrint (DEBUG_ERROR, "I2C[%d] bus frequency is not configured\n", Bus);
      break;
  }

  return -1;
}

RETURN_STATUS
EFIAPI
EcLibConstructor (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  mHost.Bus     = FixedPcdGet8 (PcdEcI2cBus);
  mHost.MemBase = I2cGetMemBase (mHost.Bus);
  mHost.BusClk  = I2cGetBusClk (mHost.Bus);

  if (PcdGetBool (PcdEcI2cHostInitialized) == FALSE) {
    I2cEnvInit (mHost.Bus);
    Status = I2cInitialize (&mHost);
    if (Status == EFI_SUCCESS) {
      PcdSetBoolS (PcdEcI2cHostInitialized, TRUE);
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetEcInfo (
  IN      UINT16  Command,
  IN      VOID    *RequestBuffer,
  IN      UINTN   RequestSize,
  OUT     VOID    *ResponseBuffer,
  IN OUT  UINTN   *ResponseSize
  )
{
  EFI_STATUS              Status            = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET  *RequestPacket    = NULL;
  EC_HOST_REQUEST_I2C     *EcRequestBuffer  = NULL;
  EC_HOST_RESPONSE_I2C    *EcResponseBuffer = NULL;

  if ((ResponseBuffer == NULL) || (ResponseSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  RequestPacket    = (EFI_I2C_REQUEST_PACKET *)AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET)+sizeof (EFI_I2C_OPERATION));
  EcRequestBuffer  = (EC_HOST_REQUEST_I2C *)AllocateZeroPool (sizeof (EC_HOST_REQUEST_I2C) + RequestSize);
  EcResponseBuffer = (EC_HOST_RESPONSE_I2C *)AllocateZeroPool (sizeof (EC_HOST_RESPONSE_I2C) + *ResponseSize);

  if ((RequestPacket == NULL) || (EcRequestBuffer == NULL) || (EcResponseBuffer == NULL)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    goto EndReturn;
  }

  EcRequestBuffer->CommandProtocol         = EC_COMMAND_PROTOCOL_3;
  EcRequestBuffer->EcRequest.StructVersion = EC_HOST_REQUEST_VERSION;
  EcRequestBuffer->EcRequest.Command       = Command;
  EcRequestBuffer->EcRequest.DataLen       = SwapBytes16 (RequestSize);
  CopyMem ((VOID *)(EcRequestBuffer + 1), (VOID *)RequestBuffer, RequestSize);
  EcRequestBuffer->EcRequest.Checksum = CalculateCheckSum8 ((UINT8 *)(&EcRequestBuffer->EcRequest), sizeof (EC_HOST_REQUEST) + RequestSize);

  // CopyMem ((VOID *)(EcRequestBuffer + 1), (VOID *)RequestBuffer, RequestSize);

  RequestPacket->OperationCount = I2C_WR_THEN_RD_OP_COUNT;

  RequestPacket->Operation[0].Flags         = I2C_FLAG_WRITE;
  RequestPacket->Operation[0].LengthInBytes = sizeof (EC_HOST_REQUEST_I2C) + RequestSize;
  RequestPacket->Operation[0].Buffer        = (UINT8 *)EcRequestBuffer;

  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = sizeof (EC_HOST_RESPONSE_I2C) + *ResponseSize;
  RequestPacket->Operation[1].Buffer        = (UINT8 *)EcResponseBuffer;

  Status = I2cMasterXfer (&mHost, FixedPcdGet8 (PcdEcI2cSlaveAddress), RequestPacket);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: fail to get EC info, status %r\n", __FUNCTION__, Status));
    goto EndReturn;
  }

  MicroSecondDelay (1000);  // here to delay 1 ms for EC responce to next call

  // skip checking Result filed of EcResponseBuffer
  // if (EcResponseBuffer->Result != 0) {
  //   DEBUG ((DEBUG_ERROR, "%a: response result %d\n", __FUNCTION__, EcResponseBuffer->Result));
  //   goto EndReturn;
  // }

  if (CalculateCheckSum8 ((UINT8 *)(&EcResponseBuffer->EcResponse), sizeof (EC_HOST_RESPONSE) + SwapBytes16 (EcResponseBuffer->EcResponse.DataLen)) != 0) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "%a: response checksum(%d) not correct\n", __FUNCTION__, EcResponseBuffer->EcResponse.Checksum));
    goto EndReturn;
  }

  if (EcResponseBuffer->EcResponse.Result != EC_RES_SUCCESS) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "%a: EC response result %d\n", __FUNCTION__, EcResponseBuffer->EcResponse.Result));
    goto EndReturn;
  }

  if (SwapBytes16 (EcResponseBuffer->EcResponse.DataLen) > *ResponseSize) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "%a: response length(%d) exceed budget(%d)\n", __FUNCTION__, SwapBytes16 (EcResponseBuffer->EcResponse.DataLen), *ResponseSize));
    goto EndReturn;
  }

  *ResponseSize = SwapBytes16 (EcResponseBuffer->EcResponse.DataLen);
  CopyMem ((VOID *)ResponseBuffer, (VOID *)(EcResponseBuffer + 1), SwapBytes16 (EcResponseBuffer->EcResponse.DataLen));

EndReturn:

  if (RequestPacket) {
    FreePool ((VOID *)RequestPacket);
  }

  if (EcRequestBuffer) {
    FreePool ((VOID *)EcRequestBuffer);
  }

  if (EcResponseBuffer) {
    FreePool ((VOID *)EcResponseBuffer);
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetEcVersion (
  IN OUT EC_RESPONSE_EC_VERSION_INFO  *Info
  )
{
  EFI_STATUS                   Status          = EFI_SUCCESS;
  EC_RESPONSE_EC_VERSION_INFO  *ResponseBuffer = NULL;
  UINTN                        ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_EC_VERSION_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_EC_VERSION), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    CopyMem (Info->Version, ResponseBuffer->Version, ResponseSize);
    Info->Version[EC_VER_TEXT_SIZE - 1] = '\0';

    DEBUG ((DEBUG_INFO, "EC response version info:\n"));
    DEBUG ((DEBUG_INFO, "\tVersion : %a\n", Info->Version));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetBoardId (
  IN OUT EC_RESPONSE_BOARD_ID_INFO  *Info
  )
{
  EFI_STATUS                 Status          = EFI_SUCCESS;
  EC_RESPONSE_BOARD_ID_INFO  *ResponseBuffer = NULL;
  UINTN                      ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_BOARD_ID_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_BOARD_ID), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->Value = SwapBytes16 (ResponseBuffer->Value);

    DEBUG ((DEBUG_INFO, "EC response board ID info:\n"));
    DEBUG ((DEBUG_INFO, "\tSKU ID       : 0x%x\n", Info->Id.Sku + (Info->Id.SkuExt << 3)));
    DEBUG ((DEBUG_INFO, "\tMemory ID    : 0x%x\n", Info->Id.Memory + (Info->Id.MemExt << 3)));
    // DEBUG ((DEBUG_INFO, "\tPMIC ID      : 0x%x\n", Info->Id.Pmic));
    DEBUG ((DEBUG_INFO, "\tReversion ID : 0x%x\n", Info->Id.Rev));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetBatteryStaticInfo (
  IN OUT EC_RESPONSE_BATTERY_STATIC_INFO  *Info
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  EC_PARAMS_BATTERY_STATIC_INFO    *RequestBuffer = NULL;
  EC_RESPONSE_BATTERY_STATIC_INFO  *ResponseBuffer = NULL;
  UINTN                            RequestSize, ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RequestSize   = sizeof (EC_PARAMS_BATTERY_STATIC_INFO);
  RequestBuffer = AllocateZeroPool (RequestSize);

  ResponseSize   = sizeof (EC_RESPONSE_BATTERY_STATIC_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if ((RequestBuffer == NULL) || (ResponseBuffer == NULL)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    goto EndReturn;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_BATTERY_GET_STATIC), (VOID *)RequestBuffer, RequestSize, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->DesignCapability = SwapBytes16 (ResponseBuffer->DesignCapability);
    Info->DesignVoltage    = SwapBytes16 (ResponseBuffer->DesignVoltage);
    Info->CycleCount       = SwapBytes32 (ResponseBuffer->CycleCount);
    CopyMem (Info->Manufacturer, ResponseBuffer->Manufacturer, EC_COMM_TEXT_MAX);
    CopyMem (Info->Model, ResponseBuffer->Model, EC_COMM_TEXT_MAX);
    CopyMem (Info->Serial, ResponseBuffer->Serial, EC_COMM_TEXT_MAX);
    CopyMem (Info->Type, ResponseBuffer->Type, EC_COMM_TEXT_MAX);
    Info->Manufacturer[EC_COMM_TEXT_MAX-1] = '\0';
    Info->Model[EC_COMM_TEXT_MAX-1]        = '\0';
    Info->Serial[EC_COMM_TEXT_MAX-1]       = '\0';
    Info->Type[EC_COMM_TEXT_MAX-1]         = '\0';

    DEBUG ((DEBUG_INFO, "EC response battery static info:\n"));
    DEBUG ((DEBUG_INFO, "\tDesign Capability : %d mAh\n", Info->DesignCapability));
    DEBUG ((DEBUG_INFO, "\tDesign Voltage    : %d mV\n", Info->DesignVoltage));
    DEBUG ((DEBUG_INFO, "\tManufacturer      : %a\n", Info->Manufacturer));
    DEBUG ((DEBUG_INFO, "\tModel             : %a\n", Info->Model));
    DEBUG ((DEBUG_INFO, "\tSerial            : %a\n", Info->Serial));
    DEBUG ((DEBUG_INFO, "\tType              : %a\n", Info->Type));
    DEBUG ((DEBUG_INFO, "\tCycle Count       : %d\n", Info->CycleCount));
  }

EndReturn:

  if (RequestBuffer) {
    FreePool ((VOID *)RequestBuffer);
  }

  if (ResponseBuffer) {
    FreePool ((VOID *)ResponseBuffer);
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetBatteryDynamicInfo (
  IN OUT EC_RESPONSE_BATTERY_DYNAMIC_INFO  *Info
  )
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  EC_PARAMS_BATTERY_DYNAMIC_INFO    *RequestBuffer = NULL;
  EC_RESPONSE_BATTERY_DYNAMIC_INFO  *ResponseBuffer = NULL;
  UINTN                             RequestSize, ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RequestSize   = sizeof (EC_PARAMS_BATTERY_DYNAMIC_INFO);
  RequestBuffer = AllocateZeroPool (RequestSize);

  ResponseSize   = sizeof (EC_RESPONSE_BATTERY_DYNAMIC_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if ((RequestBuffer == NULL) || (ResponseBuffer == NULL)) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    goto EndReturn;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_BATTERY_GET_DYNAMIC), (VOID *)RequestBuffer, RequestSize, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->ActualVoltage     = SwapBytes16 (ResponseBuffer->ActualVoltage);
    Info->ActualCurrent     = SwapBytes16 (ResponseBuffer->ActualCurrent);
    Info->RemainingCapacity = SwapBytes16 (ResponseBuffer->RemainingCapacity);
    Info->FullCapacity      = SwapBytes16 (ResponseBuffer->FullCapacity);
    Info->Flags             = SwapBytes16 (ResponseBuffer->Flags);
    Info->DesiredVoltage    = SwapBytes16 (ResponseBuffer->DesiredVoltage);
    Info->DesiredCurrent    = SwapBytes16 (ResponseBuffer->DesiredCurrent);

    DEBUG ((DEBUG_INFO, "EC response battery dynamic info:\n"));
    DEBUG ((DEBUG_INFO, "\tActual Voltage     : %d mV\n", Info->ActualVoltage));
    DEBUG ((DEBUG_INFO, "\tActual Current     : %d mA\n", Info->ActualCurrent));
    DEBUG ((DEBUG_INFO, "\tRemaining Capacity : %d mAh\n", Info->RemainingCapacity));
    DEBUG ((DEBUG_INFO, "\tFull Capacity      : %d mAh\n", Info->FullCapacity));
    DEBUG ((DEBUG_INFO, "\tFlags              : 0x%x\n", Info->Flags));
    DEBUG ((DEBUG_INFO, "\tDesired Voltage    : %d mV\n", Info->DesiredVoltage));
    DEBUG ((DEBUG_INFO, "\tDesired Current    : %d mA\n", Info->DesiredCurrent));
  }

EndReturn:
  if (RequestBuffer) {
    FreePool ((VOID *)RequestBuffer);
  }

  if (ResponseBuffer) {
    FreePool ((VOID *)ResponseBuffer);
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetFarmId (
  IN OUT EC_RESPONSE_FRAM_ID_INFO  *Info
  )
{
  EFI_STATUS                Status          = EFI_SUCCESS;
  EC_RESPONSE_FRAM_ID_INFO  *ResponseBuffer = NULL;
  UINTN                     ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_FRAM_ID_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_FRAM_ID), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->FRAM_ID = ResponseBuffer->FRAM_ID;

    DEBUG ((DEBUG_INFO, "EC response farm id info:\n"));
    DEBUG ((DEBUG_INFO, "\tFRAM_ID       : 0x%x\n", Info->FRAM_ID));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetGpio (
  IN EC_W_GPIO_INFO  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINT8       OutBuf[2];
  UINTN       InSize;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutBuf[0] = Info->GPIO_NUM;
  OutBuf[1] = Info->GPIO_VAL;
  OutSize   = sizeof (EC_W_GPIO_INFO);
  InSize    = 0;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_INT_WRITE_GPIO), (VOID *)OutBuf, OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request set gpio info:\n"));
    DEBUG ((DEBUG_INFO, "\tLen       : 0x%x\n", OutSize));
    DEBUG ((DEBUG_INFO, "\tNum       : 0x%x\n", Info->GPIO_NUM));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetGpio (
  IN OUT EC_R_GPIO_INFO  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize = 1;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_INT_READ_GPIO), &(Info->GPIO_NUM), 1, (VOID *)(&(Info->GPIO_VAL)), &ResponseSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request get gpio info:\n"));
    DEBUG ((DEBUG_INFO, "\tNum       : 0x%x\n", Info->GPIO_NUM));
    DEBUG ((DEBUG_INFO, "\tVal       : 0x%x\n", Info->GPIO_VAL));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetPmicVersion (
  IN OUT EC_RESPONSE_PMIC_VER_INFO  *Info
  )
{
  EFI_STATUS                 Status          = EFI_SUCCESS;
  EC_RESPONSE_PMIC_VER_INFO  *ResponseBuffer = NULL;
  UINTN                      ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_PMIC_VER_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_PMIC_VERSION), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->Pmic1Ver = ResponseBuffer->Pmic1Ver;
    Info->Pmic2Ver = ResponseBuffer->Pmic2Ver;
    Info->Pmic3Ver = ResponseBuffer->Pmic3Ver;

    DEBUG ((DEBUG_INFO, "EC response pmic version info:\n"));
    DEBUG ((DEBUG_INFO, "\tPMIC_VER       : 0x%x 0x%x 0x%x\n", Info->Pmic1Ver, Info->Pmic2Ver, Info->Pmic3Ver));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetPdVersion (
  IN OUT EC_RESPONSE_PD_VER_INFO  *Info
  )
{
  EFI_STATUS               Status          = EFI_SUCCESS;
  EC_RESPONSE_PD_VER_INFO  *ResponseBuffer = NULL;
  UINTN                    ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_PD_VER_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_PD_VERSION), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->Pd1Ver = ResponseBuffer->Pd1Ver;
    Info->Pd2Ver = ResponseBuffer->Pd2Ver;

    DEBUG ((DEBUG_INFO, "EC response pd version info:\n"));
    DEBUG ((DEBUG_INFO, "\tPD1_VER       : 0x%x\n", Info->Pd1Ver));
    DEBUG ((DEBUG_INFO, "\tPD2_VER       : 0x%x\n", Info->Pd2Ver));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetGreenPakVersion (
  IN OUT EC_RESPONSE_GREENPAK_VER_INFO  *Info
  )
{
  EFI_STATUS                     Status          = EFI_SUCCESS;
  EC_RESPONSE_GREENPAK_VER_INFO  *ResponseBuffer = NULL;
  UINTN                          ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_GREENPAK_VER_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_GREENPAK_VERSION), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->GreenpakVer = ResponseBuffer->GreenpakVer;

    DEBUG ((DEBUG_INFO, "EC response GreenPak version info:\n"));
    DEBUG ((DEBUG_INFO, "\tGREENPAK_VER       : 0x%x\n", Info->GreenpakVer));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetChagerInfo (
  IN OUT EC_RESPONSE_CHARGER_INFO  *Info
  )
{
  EFI_STATUS                Status          = EFI_SUCCESS;
  EC_RESPONSE_CHARGER_INFO  *ResponseBuffer = NULL;
  UINTN                     ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_CHARGER_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_CHARGER_INFO), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    CopyMem (Info->name, ResponseBuffer->name, 7);
    CopyMem (Info->type, ResponseBuffer->type, 3);
    Info->online = ResponseBuffer->online;

    DEBUG ((DEBUG_INFO, "EC response charger info:\n"));
    DEBUG ((DEBUG_INFO, "\tname       : %s\n", Info->name));
    DEBUG ((DEBUG_INFO, "\ttype       : %s\n", Info->type));
    DEBUG ((DEBUG_INFO, "\tonline     : 0x%x\n", Info->online));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetFanRpm (
  IN EC_FAN_SET_RPM  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINT8       OutBuf[sizeof (EC_FAN_SET_RPM)];
  UINTN       InSize = 0;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutSize   = sizeof (EC_FAN_SET_RPM);
  OutBuf[3] = (UINT8)(Info->rpm);
  OutBuf[2] = (UINT8)(Info->rpm >> 8);
  OutBuf[1] = (UINT8)(Info->rpm >> 16);
  OutBuf[0] = (UINT8)(Info->rpm >> 24);
  OutBuf[4] = Info->fan_idx;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_PWM_SET_FAN_TARGET_RPM), (VOID *)OutBuf, OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request set rpm info:\n"));
    DEBUG ((DEBUG_INFO, "\tLen       : 0x%x\n", OutSize));
    DEBUG ((DEBUG_INFO, "\trpm       : 0x%x\n", Info->rpm));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetFanRpm (
  IN OUT EC_RESPONSE_FAN_GET_RPM  *Info
  )
{
  EFI_STATUS               Status          = EFI_SUCCESS;
  EC_RESPONSE_FAN_GET_RPM  *ResponseBuffer = NULL;
  UINTN                    ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_FAN_GET_RPM);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_PWM_GET_FAN_TARGET_RPM), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->fan_rpm.rpm = SwapBytes32 (ResponseBuffer->fan_rpm.rpm);

    DEBUG ((DEBUG_INFO, "EC response rpm info:\n"));
    DEBUG ((DEBUG_INFO, "\tFAN_RPM       : 0x%x\n", Info->fan_rpm.rpm));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetPwmDuty (
  IN OUT EC_PWM_SET_DUTY  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       OutBuf[sizeof (EC_PWM_SET_DUTY)];
  UINTN       RequestSize;
  UINTN       InSize = 0;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RequestSize = sizeof (EC_PWM_SET_DUTY);
  OutBuf[1]   = (UINT8)(Info->duty);
  OutBuf[0]   = (UINT8)(Info->duty >> 8);
  OutBuf[2]   = (UINT8)(Info->pwm_type);
  OutBuf[3]   = (UINT8)(Info->index);
  Status      = GetEcInfo (SwapBytes16 (EC_CMD_PWM_SET_DUTY), (VOID *)OutBuf, RequestSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC fan set duty info:\n"));
    DEBUG ((DEBUG_INFO, "\tduty      : %x\n", Info->duty));
    DEBUG ((DEBUG_INFO, "\tpwm_type  : %x\n", Info->pwm_type));
    DEBUG ((DEBUG_INFO, "\tindex     : 0x%x\n", Info->index));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetPwmDuty (
  IN  EC_PWM_GET_DUTY           *Param,
  OUT EC_RESPONSE_PWM_GET_DUTY  *Info
  )
{
  EFI_STATUS                Status          = EFI_SUCCESS;
  EC_RESPONSE_PWM_GET_DUTY  *ResponseBuffer = NULL;
  UINTN                     ResponseSize;

  if ((Info == NULL) || (Param == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_PWM_GET_DUTY);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_PWM_GET_DUTY), &(Param->pwm_type), sizeof (EC_PWM_GET_DUTY), (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->duty = SwapBytes16 (ResponseBuffer->duty);

    DEBUG ((DEBUG_INFO, "EC response fan get duty info:\n"));
    DEBUG ((DEBUG_INFO, "\tpwm_type   : 0x%x\n", Param->pwm_type));
    DEBUG ((DEBUG_INFO, "\tindex      : 0x%x\n", Param->index));
    DEBUG ((DEBUG_INFO, "\tduty       : 0x%x\n", Info->duty));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetThermalFanAutoCtl (
  IN EC_THERMAL_AUTO_FAN_CTL_INFO  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINTN       InSize = 0;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutSize = sizeof (EC_THERMAL_AUTO_FAN_CTL_INFO);

  Status = GetEcInfo (SwapBytes16 (EC_CMD_THERMAL_AUTO_FAN_CTRL), &(Info->FAN_AUTO_FLG), OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request fan auto control info:\n"));
    DEBUG ((DEBUG_INFO, "\tLen       : 0x%x\n", OutSize));
    DEBUG ((DEBUG_INFO, "\tFAN_AUTO_FLG       : 0x%x\n", Info->FAN_AUTO_FLG));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetPvtTemp (
  IN OUT EC_RESPONSE_PVT_TEMP_INFO  *Info
  )
{
  EFI_STATUS                 Status          = EFI_SUCCESS;
  EC_RESPONSE_PVT_TEMP_INFO  *ResponseBuffer = NULL;
  UINTN                      ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_PVT_TEMP_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_PVT_TEMP), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->PVT_TEMPH = ResponseBuffer->PVT_TEMPH;
    Info->PVT_TEMPL = ResponseBuffer->PVT_TEMPL;

    DEBUG ((DEBUG_INFO, "EC response pvt info:\n"));
    DEBUG ((DEBUG_INFO, "\tPVT_TEMPH       : 0x%x\n", Info->PVT_TEMPH));
    DEBUG ((DEBUG_INFO, "\tPVT_TEMPL       : 0x%x\n", Info->PVT_TEMPL));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
GetPoweroffRsn (
  IN OUT EC_RESPONSE_POWER_OFF_RSN_INFO  *Info
  )
{
  EFI_STATUS                      Status          = EFI_SUCCESS;
  EC_RESPONSE_POWER_OFF_RSN_INFO  *ResponseBuffer = NULL;
  UINTN                           ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_POWER_OFF_RSN_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_PWROFFRSN), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->POWER_OFF_RSN = ResponseBuffer->POWER_OFF_RSN;

    DEBUG ((DEBUG_INFO, "EC response power off reason info:\n"));
    DEBUG ((DEBUG_INFO, "\tPoweroffRsn       : 0x%x\n", Info->POWER_OFF_RSN));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetECAutoPowerOn (
  IN EC_RESPONSE_EC_AUTO_POWER_ON_INFO  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINTN       InSize = 0;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutSize = sizeof (EC_RESPONSE_EC_AUTO_POWER_ON_INFO);

  Status = GetEcInfo (SwapBytes16 (EC_CMD_EC_AUTO_POWER_ON), &(Info->EC_AUTO_POWER_ON_FLG), OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request ec auto power on info:\n"));
    DEBUG ((DEBUG_INFO, "\tLen       : 0x%x\n", OutSize));
    DEBUG ((DEBUG_INFO, "\tEC_AUTO_POWER_ON_FLG       : 0x%x\n", Info->EC_AUTO_POWER_ON_FLG));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetAcpiIntEvent (
  IN OUT EC_RESPONSE_ACPI_INT_EVENT_INFO  *Info
  )
{
  EFI_STATUS                       Status          = EFI_SUCCESS;
  EC_RESPONSE_ACPI_INT_EVENT_INFO  *ResponseBuffer = NULL;
  UINTN                            ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_ACPI_INT_EVENT_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_INT_GET_INFO), NULL, 0, (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->TYPE           = ResponseBuffer->TYPE;
    Info->ACPI_INT_EVENT = SwapBytes32 (ResponseBuffer->ACPI_INT_EVENT);

    DEBUG ((DEBUG_INFO, "EC response acpi interrupt info:\n"));
    DEBUG ((DEBUG_INFO, "\tTYPE       : 0x%x\n", Info->TYPE));
    DEBUG ((DEBUG_INFO, "\tACPI_INT_EVENT       : 0x%x\n", Info->ACPI_INT_EVENT));
  }

  FreePool (ResponseBuffer);

  return Status;
}

EFI_STATUS
EFIAPI
SetPdFwUpdateInfo (
  IN EC_PARAMS_PD_FW_UPDATE_INFO  *Info
  )
{
  EFI_STATUS  Status  = EFI_SUCCESS;
  UINTN       OutSize = sizeof (EC_PARAMS_PD_FW_UPDATE_INFO);
  UINT8       OutBuf[sizeof (EC_PARAMS_PD_FW_UPDATE_INFO)];
  UINTN       InSize = 0;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutBuf[3] = (UINT8)(Info->PdFwFlashAddr);
  OutBuf[2] = (UINT8)(Info->PdFwFlashAddr >> 8);
  OutBuf[1] = (UINT8)(Info->PdFwFlashAddr >> 16);
  OutBuf[0] = (UINT8)(Info->PdFwFlashAddr >> 24);

  OutBuf[7] = (UINT8)(Info->PdFwSize);
  OutBuf[6] = (UINT8)(Info->PdFwSize >> 8);
  OutBuf[5] = (UINT8)(Info->PdFwSize >> 16);
  OutBuf[4] = (UINT8)(Info->PdFwSize >> 24);

  OutBuf[8] = Info->Index;
  OutBuf[9] = Info->TriggerFlag;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_SET_PD_UPDATE_INFO), OutBuf, OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC set pd fw update info:\n"));
    DEBUG ((DEBUG_INFO, "\tPdFwFlashAddr    : 0x%x\n", Info->PdFwFlashAddr));
    DEBUG ((DEBUG_INFO, "\tPdFwSize         : 0x%x\n", Info->PdFwSize));
    DEBUG ((DEBUG_INFO, "\tIndex            : 0x%x\n", Info->Index));
    DEBUG ((DEBUG_INFO, "\tTriggerFlag      : 0x%x\n", Info->TriggerFlag));
  }

  return Status;
}

EFI_STATUS
EFIAPI
GetPdFwUpdateState (
  IN  EC_PARAMS_PD_FW_UPDATE_STATE_INFO    *Param,
  OUT EC_RESPONSE_PD_FW_UPDATE_STATE_INFO  *Info
  )
{
  EFI_STATUS                           Status          = EFI_SUCCESS;
  EC_RESPONSE_PD_FW_UPDATE_STATE_INFO  *ResponseBuffer = NULL;
  UINTN                                ResponseSize;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseSize   = sizeof (EC_RESPONSE_PD_FW_UPDATE_STATE_INFO);
  ResponseBuffer = AllocateZeroPool (ResponseSize);

  if (ResponseBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "%a: fail to allocate buffer, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = GetEcInfo (SwapBytes16 (EC_CMD_GET_PD_UPDATE_STATE), &(Param->Index), sizeof (EC_PARAMS_PD_FW_UPDATE_STATE_INFO), (VOID *)ResponseBuffer, &ResponseSize);
  if (!EFI_ERROR (Status)) {
    Info->PdUpdateState = ResponseBuffer->PdUpdateState;

    DEBUG ((DEBUG_INFO, "EC response pd fw update state:\n"));
    DEBUG ((DEBUG_INFO, "\tPdUpdateState     : 0x%x\n", Info->PdUpdateState));
  }

  FreePool (ResponseBuffer);

  return Status;
}

#pragma pack(push, 1)
typedef struct {
  UINT8     CmdCode;
  UINT8     PktLen;
  UINT16    StartAddr;
  UINT8     DataLen;
} PD_FW_BIN_TRANS_HEADER;
#pragma pack(pop)

#define FLASH_SIZE_OF_BANK        0x10000
#define MAX_DATA_SIZE_ONCE_TRANS  0x1D
#define MAX_TRY_COUNT_ONCE_TRANS  3

STATIC CONST UINT8  WRITE_FLASH_COMMAND_CODE[] = { 0x04, 0x06, 0x13, 0x14 };

EFI_STATUS
EFIAPI
TransPdFwBin (
  IN  EC_PARAMS_PD_FW_BIN_TRANS_INFO  *Param
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       *RequestBuffer;
  UINTN       RequestSize;
  UINTN       ResponseBuffer;
  UINTN       ResponseSize = 0;
  UINT32      CurAddr      = Param->Address;
  UINT32      RestSize     = Param->Length;
  UINT8       *pData       = Param->Data;

  do {
    UINT16  StartAddr;
    UINT8   DataSize;
    UINT8   BankIndex;

    BankIndex = CurAddr / FLASH_SIZE_OF_BANK;
    StartAddr = CurAddr % FLASH_SIZE_OF_BANK;

    DataSize = MAX_DATA_SIZE_ONCE_TRANS;
    if (RestSize < MAX_DATA_SIZE_ONCE_TRANS) {
      DataSize = RestSize;
    }

    if (StartAddr + DataSize > FLASH_SIZE_OF_BANK) {
      DataSize = FLASH_SIZE_OF_BANK - StartAddr;
    }

    RequestSize   = sizeof (PD_FW_BIN_TRANS_HEADER) + DataSize;
    RequestBuffer = (UINT8 *)AllocatePool (RequestSize);
    PD_FW_BIN_TRANS_HEADER  *TransHeader = (PD_FW_BIN_TRANS_HEADER *)RequestBuffer;

    if (RestSize > DataSize) {
      StartAddr |= 0x8000;
    }

    TransHeader->CmdCode   = WRITE_FLASH_COMMAND_CODE[BankIndex];
    TransHeader->PktLen    = DataSize + 3;
    TransHeader->StartAddr = StartAddr;
    TransHeader->DataLen   = DataSize;
    CopyMem (RequestBuffer + sizeof (PD_FW_BIN_TRANS_HEADER), pData, DataSize);

    UINT8  TryCnt = 0;
    for (TryCnt = 0; TryCnt < MAX_TRY_COUNT_ONCE_TRANS; TryCnt++) {
      Status = GetEcInfo (SwapBytes16 (EC_CMD_TRANS_PD_FW_BIN), RequestBuffer, RequestSize, (VOID *)(&ResponseBuffer), &ResponseSize);
      if (!EFI_ERROR (Status)) {
        break;
      }
    }

    FreePool (RequestBuffer);

    if (TryCnt == MAX_TRY_COUNT_ONCE_TRANS) {
      DEBUG ((DEBUG_INFO, "EC transfer pd fw bin: %r\n", Status));
      DEBUG ((DEBUG_INFO, "\tSatrt Address : 0x%x\n", CurAddr));
      DEBUG ((DEBUG_INFO, "\tSize : 0x%x\n", DataSize));
      return Status;
    }

    CurAddr  += DataSize;
    RestSize -= DataSize;
    pData    += DataSize;
  } while (RestSize > 0);

  return Status;
}

EFI_STATUS
EFIAPI
ForceECMirror (
  IN EC_PARAMS_FORCE_MIRROR  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINT8       OutBuf[1];
  UINTN       InSize;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutBuf[0] = Info->Reserved;
  OutSize   = sizeof (EC_PARAMS_FORCE_MIRROR);
  InSize    = 0;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_FORCE_EC_FW_MIRROR), (VOID *)OutBuf, OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC request force mirror info:\n"));
    DEBUG ((DEBUG_INFO, "\tRsv       : 0x%x\n", Info->Reserved));
  }

  return Status;
}

EFI_STATUS
EFIAPI
SetAlsMode (
  IN EC_PARAMS_ALS_MODE_CTL  *Info
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       OutSize;
  UINT8       OutBuf[1];
  UINTN       InSize;
  UINTN       InBuf;

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OutBuf[0] = Info->Mode;
  OutSize   = sizeof (EC_PARAMS_ALS_MODE_CTL);
  InSize    = 0;

  Status = GetEcInfo (SwapBytes16 (EC_CMD_SET_AUTO_ALS_CTRL), (VOID *)OutBuf, OutSize, (VOID *)(&InBuf), &InSize);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Set als mode info:\n"));
    DEBUG ((DEBUG_INFO, "\tMode       : 0x%x\n", Info->Mode));
  }

  return Status;
}
