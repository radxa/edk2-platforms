/**

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RedriverLib.h>
#include <Protocol/I2cMaster.h>
#include <Protocol/I2cDevicePath.h>
#include <Pi/PiI2c.h>

#define I2C_FLAG_WRITE           0

#define PI3DPX1207C_CONF			                                0x03
#define PI3DPX1207C_CONF_OPEN			                            0x02
#define PI3DPX1207C_CONF_SWAP			                            0x10
#define PI3DPX1207C_CONF_4LANE_DP                             0x22
#define PI3DPX1207C_CONF_USB3			                            0x42
#define PI3DPX1207C_CONF_USB3_AND_2LANE_DP	                  0x62
#define PI3DPX1207C_CONF_4LANE_DP_FLIP_AUX_NOT_FLIP           0x22
#define PI3DPX1207C_CONF_4LANE_DP_FLIP_AUX_FLIP               0x32
#define PI3DPX1207C_CONF_USB3_FLIP                            0x52
#define PI3DPX1207C_CONF_USB3_AND_2LANE_DP_FILP               0x72
#define PI3DPX1207C_CONF_USB3_AND_2LANE_DP_FILP_AUX_NOT_FILP  0x82

UINT8  *pRdDevBuses        = PcdGetPtr (PcdRedriverDevI2cBuses);
UINT8  *pRdDevSlaveAddress = PcdGetPtr (PcdRedriverDevI2cSlaveAddresses);
UINT8  *pNeedAuxFlip       = PcdGetPtr (PcdRedriverNeedAuxFlip);
UINT8  *pForceHpd          = PcdGetPtr (PcdRedriverForceHpd);

EFI_STATUS
PI3DPX1207_I2C_Block_Write (
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster,
  UINTN                    SlaveAddress,
  UINT8                    *WriteData,
  UINT32                   WiteLength
  )
{
  EFI_STATUS              Status         = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET  *RequestPacket = NULL;

  RequestPacket                 = (EFI_I2C_REQUEST_PACKET *)AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET));
  RequestPacket->OperationCount = 1;

  RequestPacket->Operation[0].Flags         = I2C_FLAG_WRITE;
  RequestPacket->Operation[0].LengthInBytes = WiteLength;
  RequestPacket->Operation[0].Buffer        = WriteData;

  Status = I2cMaster->StartRequest (I2cMaster, SlaveAddress, (void *)RequestPacket, NULL, NULL);

  if (RequestPacket) {
    FreePool (RequestPacket);
  }

  return Status;
}

EFI_STATUS
PI3DPX1207_SetConfig (
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster,
  UINTN                    SlaveAddress,
  UINT8                    NewConfig,
  BOOLEAN                  ForceHpd
)
{
	UINT8 write_conf[5] = {0};
	write_conf[0] = 0x13;
	write_conf[1] = 0x11;
	write_conf[2] = 0x20;
	write_conf[3] = NewConfig;
	if(ForceHpd) {
	  write_conf[4] = 0x06;
	} else {
	  write_conf[4] = 0;
	}
	return PI3DPX1207_I2C_Block_Write(I2cMaster, SlaveAddress, write_conf, sizeof(write_conf));
}

EFI_STATUS
GetRdDevConfig (
  UINT8  RdDevIdx,
  UINT8  *I2cBusId,
  UINT8  *SlaveAddress
  )
{
  if (!pRdDevBuses || !pRdDevSlaveAddress) {
    return EFI_INVALID_PARAMETER;
  }

  *I2cBusId     = pRdDevBuses[RdDevIdx];
  *SlaveAddress = pRdDevSlaveAddress[RdDevIdx];

  if ((*I2cBusId == 0xFF) || (*SlaveAddress == 0xFF)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetRdI2cDeviceHandle (
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
      "I2c Bus %d Locate Handle Buffer %g fail, status %r\n",
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

BOOLEAN
IsRdDeviceValid (
  UINT8  RdDevIdx
  )
{
  EFI_STATUS  Status       = EFI_SUCCESS;
  UINT8       I2cBus       = 0xFF;
  UINT8       SlaveAddress = 0xFF;
  EFI_HANDLE  I2cDevHandle;

  Status = GetRdDevConfig (RdDevIdx, &I2cBus, &SlaveAddress);
  DEBUG ((
    DEBUG_INFO,
    "Redriver Device %d get device config, I2C bus: %x, Slave Address:%x\n",
    RdDevIdx,
    I2cBus,
    SlaveAddress
    ));
  if (EFI_ERROR(Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Redriver Device %d i2c config is not valid\n",
      RdDevIdx
      ));
    return FALSE;
  }

  Status = GetRdI2cDeviceHandle (I2cBus, &I2cDevHandle);
  if (EFI_ERROR(Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Redriver Device %d i2c protocol in not installed\n",
      RdDevIdx
      ));
    return FALSE;
  }

  return TRUE;
}

EFI_STATUS
EFIAPI
RedriverSetConf (
  IN      UINT8             RdDevIdx,
  IN      TYPEC_MODE        Mode,
  IN      TYPEC_ORIENTATION Orientation
  )
{
  EFI_STATUS               Status       = EFI_SUCCESS;
  UINT8                    I2cBus       = 0;
  UINT8                    SlaveAddress = 0;
  EFI_I2C_MASTER_PROTOCOL  *I2cMaster   = NULL;
  EFI_HANDLE               I2cDevHandle;
  UINT8                    new_conf = 0;

  Status = GetRdDevConfig (RdDevIdx, &I2cBus, &SlaveAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Redriver Device %d is not supported\n",
      RdDevIdx
      ));
    return EFI_UNSUPPORTED;
  }

  Status = GetRdI2cDeviceHandle (I2cBus, &I2cDevHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Redriver Device %d get I2c Bus %d device handle %r\n",
      I2cBus,
      Status
      ));
    return Status;
  }

  Status = gBS->HandleProtocol (I2cDevHandle, &gEfiI2cMasterProtocolGuid, (VOID **)&I2cMaster);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Redriver Device %d Handle Protocol %g status %r\n",
      RdDevIdx,
      &gEfiI2cMasterProtocolGuid,
      Status
      ));
    return Status;
  }

  switch (Orientation) {
	case TYPEC_ORIENTATION_NONE:
		new_conf = PI3DPX1207C_CONF_OPEN;
		break;
	case TYPEC_ORIENTATION_NORMAL:
		new_conf &= ~PI3DPX1207C_CONF_SWAP;
		break;
	case TYPEC_ORIENTATION_REVERSE:
		new_conf |= PI3DPX1207C_CONF_SWAP;
		break;
	}

  switch (Mode) {
    case TYPEC_STATE_SAFE:
    case TYPEC_STATE_DISCONNECT:
      new_conf = (new_conf & PI3DPX1207C_CONF_SWAP) |
          PI3DPX1207C_CONF_OPEN;
      break;
    case TYPEC_STATE_USB:
      new_conf = (new_conf & PI3DPX1207C_CONF_SWAP) |
          PI3DPX1207C_CONF_USB3;
      break;
    case TYPEC_DP_STATE_D:
      new_conf = (new_conf & PI3DPX1207C_CONF_SWAP) |
          PI3DPX1207C_CONF_USB3_AND_2LANE_DP;
      break;
    case TYPEC_DP_STATE_E:
      new_conf = (new_conf & PI3DPX1207C_CONF_SWAP) |
          PI3DPX1207C_CONF_4LANE_DP;
      break;
    default:
      break;
  }

	if (!pNeedAuxFlip[RdDevIdx]) {
		if (Orientation == TYPEC_ORIENTATION_REVERSE) {
			switch (Mode) {
			case TYPEC_DP_STATE_C:
			case TYPEC_DP_STATE_E:
				new_conf = PI3DPX1207C_CONF_4LANE_DP_FLIP_AUX_NOT_FLIP;
				break;
			case TYPEC_DP_STATE_D:
				new_conf = PI3DPX1207C_CONF_USB3_AND_2LANE_DP_FILP_AUX_NOT_FILP;
				break;
			default:
				break;
			}
		}
	}

  return PI3DPX1207_SetConfig(I2cMaster, SlaveAddress, new_conf, pForceHpd[RdDevIdx]);
}
