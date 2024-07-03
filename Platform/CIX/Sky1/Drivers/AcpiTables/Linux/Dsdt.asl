/** @file

  Copyright 2023 - 2024 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/Acpi.h>
#include <Library/AcpiLib.h>
#include <Library/PcdLib.h>
#include <AcpiRamVariable.h>
#include "Include/MemoryMap.h"
#include "Include/InterruptId.h"
#include "Include/DMACommon.h"
#include "../Common/CommonDefines.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 5, "CIXTEK", "SKY1EDK2", 1) {
  Scope(_SB) {
    include("../Common/Dsdt-Debug.asl")
    include("../Common/Dsdt-CPU.asl")
    include("Dsdt-iomux.asl")
    include("Dsdt-Fch-Uart.asl")
    include("Dsdt-dst.asl")
    include("Dsdt-PDC.asl")
    include("../Common/Dsdt-Clock.asl")
    include("Dsdt-ResLookup.asl")
    include("Dsdt-Reset.asl")
    include("Dsdt-Gmac.asl")
    //include("../Common/Dsdt-Thermal.asl")
    include("../Common/Dsdt-ScmiMailbox.asl")
    include("Dsdt-Audss.asl")
    include("../Common/Dsdt-Gpio.asl")
    include("Dsdt-Pwm.asl")
    //include("Dsdt-Wdt.asl")
    include("Dsdt-Timer.asl")
    include("Dsdt-Mailbox.asl")
    include("Dsdt-HDA.asl")
    include("Dsdt-Dsp.asl")
    include("Dsdt-Dma.asl")
    include("Dsdt-Xspi.asl")
    include("Dsdt-I2c.asl")
    include("Dsdt-Spi.asl")
    include("Dsdt-I3c.asl")
    include("../Common/Dsdt-Pcie.asl")
    include("Dsdt-Vpu.asl")
    include("Dsdt-Dpu.asl")
    include("Dsdt-GPU.asl")
    include("Dsdt-NPU.asl")
    include("Dsdt-Wlan.asl")
    include("Dsdt-I2s.asl")
    include("Dsdt-SndCard.asl")
    // include("Dsdt-SUSB.asl")
    include("../Common/Dsdt-USB.asl")
    include("../Common/Dsdt-EC.asl")
    // include("Dsdt-ISP.asl")
    // include("Dsdt-CSI-DMA.asl")
    include("../Common/Dsdt-AcpiRam.asl")
  }
}
