/*++

Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

--*/

#ifndef ___TPM2_FIFO_H
#define ___TPM2_FIFO_H

#include <Library/BaseLib.h>

#define TPM2_BASE_ADDRESS  0xD40000

/* Register Space for FIFO TPM access where v represents locality */
#define TPM2_ACCESS(v)          (0x0000 | (v) << 12)
#define TPM2_INT_ENABLE(v)      (0x0008 | (v) << 12)
#define TPM2_INT_VECTOR(v)      (0x000c | (v) << 12)
#define TPM2_INT_STATUS(v)      (0x0010 | (v) << 12)
#define TPM2_INT_CAPABILITY(v)  (0x0014 | (v) << 12)
#define TPM2_STS(v)             (0x0018 | (v) << 12)
#define TPM2_DATA_FIFO(v)       (0x0024 | (v) << 12)
#define TPM2_INTERFACE_ID(v)    (0x0030 | (v) << 12)
#define TPM2_XDATA_FIFO(v)      (0x0080 | (v) << 12)
#define TPM2_DID_VID(v)         (0x0F00 | (v) << 12)
#define TPM2_RID(v)             (0x0F04 | (v) << 12)

/* Access Register */
#define TPM2_ACCESS_ESTABLISHMENT    BIT0
#define TPM2_ACCESS_REQUEST_USE      BIT1
#define TPM2_ACCESS_REQUEST_PENDING  BIT2
#define TPM2_ACCESS_ACTIVE_LOCALITY  BIT5
#define TPM2_ACCESS_VALID            BIT7

/* STS Register */
#define TPM2_STS_RESPONSE_RETRY          BIT1
#define TPM2_STS_SELF_TEST_DONE          BIT2
#define TPM2_STS_DATA_EXPECT             BIT3
#define TPM2_STS_DATA_AVAIL              BIT4
#define TPM2_STS_GO                      BIT5
#define TPM2_STS_COMMAND_READY           BIT6
#define TPM2_STS_VALID                   BIT7
#define TPM2_STS_COMMAND_CANCEL          BIT24
#define TPM2_STS_RESET_ESTABLISMENT_BIT  BIT25
#define TPM2_STS_FAMILY_TPM2             BIT26
#define TPM2_STS_FAMILY_MASK             3 << 26
#define TPM2_STS_BURST_COUNT_MASK        0xFFFF << 8
#define TPM2_STS_BURST_COUNT_SHIFT       8
#define TPM2_STS_READ_ZERO               0x23

/* INT_ENABLE Register */
#define TPM2_INT_DATA_AVAIL_INT       BIT0
#define TPM2_INT_STS_VALID_INT        BIT1
#define TPM2_INT_LOCALITY_CHANGE_INT  BIT2
#define TPM2_INT_CMD_READY_INT        BIT7
#define TPM2_GLOBAL_INT_ENABLE        BIT31

/* INTERFACE TYPE */
#define TPM2_FIFO_TYPE  0x00
#define TPM2_CRB_TYPE   0x01
#define TPM2_TIS_TYPE   0x0F

//
// Default TimeOut value
//
#define TPM2_TIMEOUT_A  (750  * 1000)               // 750ms
#define TPM2_TIMEOUT_B  (2000 * 1000)               // 2s
#define TPM2_TIMEOUT_C  (200  * 1000)               // 200ms
#define TPM2_TIMEOUT_D  (30   * 1000)               // 30ms
#define TPM2_CHECK_NOW  (30)                        // not wait

#define TPM_DISABLE  0
#define DTPM_ENABLE  1
#define FTPM_ENABLE  2

EFI_STATUS
Tpm2ActiveLocality (
  );

EFI_STATUS
EFIAPI
Tpm2RegisterRead (
  IN UINT32  registerOffset,
  IN UINT8   ReadLength,
  OUT UINT8  *Outdata
  );

EFI_STATUS
Tpm2WaitRegisterBits (
  IN      UINT32  RegisterOffset,
  IN      UINT32  BitSet,
  IN      UINT32  BitClear,
  IN      UINT32  TimeOut
  );

EFI_STATUS
EFIAPI
Tpm2RegisterWrite (
  IN UINT32  registerOffset,
  IN UINT8   WriteLength,
  IN UINT32  InputData
  );

#endif
