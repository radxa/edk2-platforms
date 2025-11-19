/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef LINUX_ACPI_CONFIG_OVERRIDE
#include "DefaultLinuxAcpiConfig.h"
#else
#include "LinuxAcpiConfig.h"
#endif

DefinitionBlock("SsdtTable.aml", "SSDT", 2, "RADXA", "ORIONO6N", 1) {
  Scope(_SB) {
    include("Audio.asl")
    include("EEPROM.asl")
    include("MipiCamera.asl")
    include("I2cPD.asl")
    include("Iomux.asl")
    include("GpioBtns.asl")
    include("GpioInt.asl")
    include("GpioLeds.asl")
    include("GpioLines.asl")
    include("EfiRtc.asl")
    include("CdnsPciePwr.asl")
    include("TPM.asl")
    include("UsbPwr.asl")
    include("Wireless.asl")
  }
}
