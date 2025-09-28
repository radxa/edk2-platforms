/** @file

  Copyright 2022-2024 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.
  Copyright (c) 2016-2021, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef MM_COMMUNICATE_H_
#define MM_COMMUNICATE_H_

#define MM_MAJOR_VER_MASK   0xEFFF0000
#define MM_MINOR_VER_MASK   0x0000FFFF
#define MM_MAJOR_VER_SHIFT  16

#define MM_MAJOR_VER(x)  (((x) & MM_MAJOR_VER_MASK) >> MM_MAJOR_VER_SHIFT)
#define MM_MINOR_VER(x)  ((x) & MM_MINOR_VER_MASK)

#define MM_CALLER_MAJOR_VER  0x1UL
#define MM_CALLER_MINOR_VER  0x0

#define PTA_STMM_CMDID_COMMUNICATE  0

#define PTA_STMM_UUID  { 0x1BC61315, 0x4254, 0x4DAE, {\
      0xA3, 0xEE, 0x6C, 0xFD, 0xFE, 0xAB, 0xBC, 0x83 } }

#endif /* MM_COMMUNICATE_H_ */
