/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef LINUX_ACPI_CONFIG_OVERRIDE
#include "DefaultLinuxAcpiConfig.h"
#else
#include "LinuxAcpiConfig.h"
#endif

DefinitionBlock("SsdtTable.aml", "SSDT", 2, "Radxa", "OrionO6N", 1) {
  Scope(_SB) {
    include("MipiCamera.asl")
    include("I2cPD.asl")
    include("Iomux.asl")
    include("GpioInt.asl")
    include("GpioLeds.asl")
    include("EfiRtc.asl")
    include("CdnsPciePwr.asl")
    include("UsbPwr.asl")
  }
}
