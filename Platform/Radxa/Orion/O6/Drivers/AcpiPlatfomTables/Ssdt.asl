/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef LINUX_ACPI_CONFIG_OVERRIDE
#include "DefaultLinuxAcpiConfig.h"
#else
#include "LinuxAcpiConfig.h"
#endif

DefinitionBlock("SsdtTable.aml", "SSDT", 5, "CIXTEK", "SKY1EDK2", 1) {
  Scope(_SB) {
    include("Audio.asl")
    include("MipiCamera.asl")
    include("EC.asl")
    include("I2cPD.asl")
    include("Iomux.asl")
    include("GpioInt.asl")
    include("EfiRtc.asl")
    include("CdnsPciePwr.asl")
  }
}
