/** @file

  Copyright 2024 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DEVICE (SDEI) {
  Name (_HID, "CIXH100D")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
      Package () { "method", "smc" },
    }
  })
}