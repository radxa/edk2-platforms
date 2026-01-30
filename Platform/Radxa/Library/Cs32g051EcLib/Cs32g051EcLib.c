/**

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.
  Copyright 2025 Radxa Computer (Shenzhen) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/Cs32g051EcLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/I2cLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Pi/PiI2c.h>
#include <Protocol/I2cDevicePath.h>
#include <Protocol/I2cMaster.h>

UINT8  *pPdDevBuses        = PcdGetPtr (PcdPdDevEcI2cBuses);
UINT8  *pPdDevSlaveAddress = PcdGetPtr (PcdPdDevEcI2cSlaveAddresses);

EFI_STATUS
I2C_Block_WriteThenRead (
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster,
  UINTN                    SlaveAddress,
  UINT8                    *WriteData,
  UINT32                   WriteLength,
  UINT8                    *ReadData,
  UINT32                   ReadLength
  )
{
  EFI_STATUS              Status         = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET  *RequestPacket = NULL;

  RequestPacket                 = (EFI_I2C_REQUEST_PACKET *)AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET) + sizeof (EFI_I2C_OPERATION));
  RequestPacket->OperationCount = I2C_WR_THEN_RD_OP_COUNT;

  RequestPacket->Operation[0].Flags         = I2C_FLAG_WRITE;
  RequestPacket->Operation[0].LengthInBytes = WriteLength;
  RequestPacket->Operation[0].Buffer        = WriteData;

  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = ReadLength;
  RequestPacket->Operation[1].Buffer        = ReadData;

  Status = I2cMaster->StartRequest (I2cMaster, SlaveAddress, (void *)RequestPacket, NULL, NULL);

  if (RequestPacket) {
    FreePool (RequestPacket);
  }

  return Status;
}

EFI_STATUS
I2C_Block_Write (
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster,
  UINTN                    SlaveAddress,
  UINT8                    *WriteData,
  UINT32                   WriteLength
  )
{
  EFI_STATUS              Status         = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET  *RequestPacket = NULL;

  RequestPacket                 = (EFI_I2C_REQUEST_PACKET *)AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET));
  RequestPacket->OperationCount = I2C_WR_OP_COUNT;

  RequestPacket->Operation[0].Flags         = I2C_FLAG_WRITE;
  RequestPacket->Operation[0].LengthInBytes = WriteLength;
  RequestPacket->Operation[0].Buffer        = WriteData;

  Status = I2cMaster->StartRequest (I2cMaster, SlaveAddress, (void *)RequestPacket, NULL, NULL);

  if (RequestPacket) {
    FreePool (RequestPacket);
  }

  return Status;
}

EFI_STATUS
I2C_Block_Read (
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster,
  UINTN                    SlaveAddress,
  UINT8                    *ReadData,
  UINT32                   ReadLength
  )
{
  EFI_STATUS              Status         = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET  *RequestPacket = NULL;

  RequestPacket                 = (EFI_I2C_REQUEST_PACKET *)AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET));
  RequestPacket->OperationCount = I2C_RD_OP_COUNT;

  RequestPacket->Operation[0].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[0].LengthInBytes = ReadLength;
  RequestPacket->Operation[0].Buffer        = ReadData;

  Status = I2cMaster->StartRequest (I2cMaster, SlaveAddress, (void *)RequestPacket, NULL, NULL);

  if (RequestPacket) {
    FreePool (RequestPacket);
  }

  return Status;
}

EFI_STATUS
PDWriteData (
  EFI_I2C_MASTER_PROTOCOL *I2cMaster,
  UINTN                   SlaveAddress,
  UINT8                   *WriteData,
  UINT32                  WriteLength,
  UINT32                  *ReadLength
  )
{
  EFI_STATUS            Status      = EFI_SUCCESS;
  CS32G051_PING_STATUS  PingStatus  = { 0 };

  Status = I2C_Block_Write (I2cMaster, SlaveAddress, WriteData, WriteLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to write initial data, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = I2C_Block_Read (I2cMaster, SlaveAddress, (UINT8 *)&PingStatus, sizeof (PingStatus));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to read ping status, status %r\n", __FUNCTION__, Status));
    return Status;
  }

  switch (PingStatus.Bits.CMD_STS) {
    case Wait:
      DEBUG ((DEBUG_ERROR, "%a: FIXME: wait and retry PING_STATUS\n", __FUNCTION__));
      return EFI_NOT_READY;
    case Complete:
      DEBUG ((DEBUG_ERROR, "%a: operation completed, DATA_LEN = %d\n", __FUNCTION__, PingStatus.Bits.DATA_LEN));
      if (ReadLength != NULL) {
        *ReadLength = PingStatus.Bits.DATA_LEN;
      }
      return EFI_SUCCESS;
    case Busy:
      DEBUG ((DEBUG_ERROR, "%a: PD busy, caller should wait and retry full operation\n", __FUNCTION__));
      return EFI_NOT_READY;
    case Error:
    default:
      DEBUG ((DEBUG_ERROR, "%a: PD has internal error: 0x%x\n", __FUNCTION__, PingStatus.Uint8));
      return EFI_DEVICE_ERROR;
  }
}

EFI_STATUS
PDReadData (
  EFI_I2C_MASTER_PROTOCOL *I2cMaster,
  UINTN                   SlaveAddress,
  UINT8                   *ReadData,
  UINT32                  ReadLength
  )
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  UINT8       ReadRequest[] = { 0x80 };
  
  Status = I2C_Block_WriteThenRead (I2cMaster, SlaveAddress, ReadRequest, sizeof(ReadRequest), ReadData, ReadLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to read data, status %r\n", __FUNCTION__, Status));
  }
  return Status;
}

EFI_STATUS
GetPdDevConfig (
  UINT8  PdDevIdx,
  UINT8  *I2cBusId,
  UINT8  *SlaveAddress
  )
{
  if (!pPdDevBuses || !pPdDevSlaveAddress) {
    return EFI_INVALID_PARAMETER;
  }

  *I2cBusId     = pPdDevBuses[PdDevIdx];
  *SlaveAddress = pPdDevSlaveAddress[PdDevIdx];

  if ((*I2cBusId == 0xFF) || (*SlaveAddress == 0xFF)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

BOOLEAN
IsPdDeviceValid (
  UINT8  PdDevIdx
  )
{
  EFI_STATUS  Status       = EFI_SUCCESS;
  UINT8       I2cBus       = 0xFF;
  UINT8       SlaveAddress = 0xFF;

  Status = GetPdDevConfig (PdDevIdx, &I2cBus, &SlaveAddress);

  DEBUG ((
    DEBUG_INFO,
    "%a: Pd Device %d get device config, I2C bus: %x, Slave Address:%x\n",
    __FUNCTION__,
    PdDevIdx,
    I2cBus,
    SlaveAddress
    ));

  return Status == EFI_SUCCESS;
}

EFI_STATUS
GetI2cDeviceHandle (
  IN  UINT8          I2cBus,
  IN OUT EFI_HANDLE  *DevHandle
  )
{
  EFI_STATUS                Status        = EFI_SUCCESS;
  UINTN                     HandleCount   = 0;
  EFI_HANDLE                *HandleBuffer = NULL;
  UINT32                    Index;
  EFI_DEVICE_PATH           *DevicePath;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiI2cMasterProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: I2c Bus %d Locate Handle Buffer %g fail, status %r\n",
      __FUNCTION__,
      I2cBus,
      &gEfiI2cMasterProtocolGuid,
      Status
      ));
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    DevicePath = DevicePathFromHandle (HandleBuffer[Index]);
    if (!DevicePath) {
      continue;
    }

    if ((DevicePath->Type != HARDWARE_DEVICE_PATH) || (DevicePath->SubType != HW_VENDOR_DP)) {
      continue;
    }

    if (!CompareGuid (&(((VENDOR_DEVICE_PATH *)DevicePath)->Guid), &gCixI2cDevicePathGuid)) {
      continue;
    }

    if (((I2C_DEVICE_PATH*)DevicePath)->Bus == I2cBus) {
      *DevHandle = HandleBuffer[Index];
      break;
    }
  }

  if (Index == HandleCount) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: I2C bus %d is not found\n",
      __FUNCTION__,
      I2cBus
      ));
    Status = EFI_NOT_FOUND;
  }

  if (HandleBuffer) {
    FreePool (HandleBuffer);
  }

  return Status;
}

EFI_STATUS
EFIAPI
PdGetIcStatus (
  IN      UINT8 PdDevIdx,
  IN OUT  VOID  *IcStatus
  )
{
  EFI_STATUS               Status       = EFI_SUCCESS;
  UINT8                    I2cBus       = 0;
  UINT8                    SlaveAddress = 0;
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster   = NULL;
  EFI_HANDLE               I2cDevHandle;

  Status = GetPdDevConfig (PdDevIdx, &I2cBus, &SlaveAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d is not supported\n",
      __FUNCTION__,
      PdDevIdx
      ));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: Pd Device %d I2C Bus %d, Slave Address %x, read data status\n",
    __FUNCTION__,
    PdDevIdx,
    I2cBus,
    SlaveAddress
    ));

  Status = GetI2cDeviceHandle (I2cBus, &I2cDevHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d get I2c Bus %d device handle %r\n",
      __FUNCTION__,
      I2cBus,
      Status
      ));
    return Status;
  }

  Status = gBS->HandleProtocol (I2cDevHandle, &gEfiI2cMasterProtocolGuid, (VOID **)&I2cMaster);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d Handle Protocol %g status %r\n",
      __FUNCTION__,
      PdDevIdx,
      &gEfiI2cMasterProtocolGuid,
      Status
      ));
    return Status;
  }

  CS32G051_REQUEST_CMD_GET_IC_STATUS  req = CS32G051_CMD_GET_IC_STATUS_PACKET;
  UINT32 ReadLength = 0;

  Status = PDWriteData (I2cMaster, SlaveAddress, (UINT8 *)&req, sizeof(req), &ReadLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d PDWriteData status %r\n",
      __FUNCTION__,
      PdDevIdx,
      Status
      ));
    return Status;
  }

  if (ReadLength != sizeof(CS32G051_RESULT_CMD_GET_IC_STATUS)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d read length %d mismatch expected %d\n",
      __FUNCTION__,
      PdDevIdx,
      ReadLength,
      sizeof(CS32G051_RESULT_CMD_GET_IC_STATUS)
      ));
    return EFI_DEVICE_ERROR;
  }

  Status = PDReadData (I2cMaster, SlaveAddress, (UINT8*)IcStatus, ReadLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Pd Device %d PDReadData status %r\n",
      __FUNCTION__,
      PdDevIdx,
      Status
      ));
  }

  if (((CS32G051_RESULT_CMD_GET_IC_STATUS*)IcStatus)->ByteCount != ReadLength) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: PD has returned %d bytes instead of expected %d\n",
      __FUNCTION__,
      ((CS32G051_RESULT_CMD_GET_IC_STATUS*)IcStatus)->ByteCount,
      ReadLength
      ));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}
