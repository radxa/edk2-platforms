/** @file

  Copyright 2023 Cix Technology (Shanghai) Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Protocol/ClockId.h>

Device (CCLK) {
    Name (_HID, "CIXHA010")
    Name (_UID, 0)
    Name (_STA, 0xF)
    //For linux clock framwork
    Name (CLKT, Package(){
      Package() {CLK_TREE_GPU_CLK_CORE, "gpu_clk_core", \_SB.GPU},
      Package() {CLK_TREE_GPU_CLK_STACKS, "gpu_clk_stacks", \_SB.GPU},
      Package() {CLK_TREE_DPU0_ACLK, "aclk", \_SB.DPU0},
      Package() {CLK_TREE_DPU1_ACLK, "aclk", \_SB.DPU1},
      Package() {CLK_TREE_DPU2_ACLK, "aclk", \_SB.DPU2},
      Package() {CLK_TREE_DPU3_ACLK, "aclk", \_SB.DPU3},
      Package() {CLK_TREE_DPU4_ACLK, "aclk", \_SB.DPU4},
      Package() {CLK_TREE_CAMERA_MCLK0, "mclk", \_SB.I2C0.IIS0},
      Package() {CLK_TREE_CAMERA_MCLK1, "mclk", \_SB.I2C3.IIS1},
      Package() {CLK_TREE_CAMERA_MCLK2, "mclk", \_SB.I2C1.IIS2},
      Package() {CLK_TREE_CAMERA_MCLK3, "mclk", \_SB.I2C4.IIS3},

      /* GPIO */
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI0},
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI1},
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI2},
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI3},
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI4},
      Package() {CLK_TREE_FCH_GPIO_APB, "", \_SB.GPI5},

      /* uart */
      Package() {CLK_TREE_FCH_UART0_APB, "apb_pclk", \_SB.COM0},
      Package() {CLK_TREE_FCH_UART1_APB, "apb_pclk", \_SB.COM1},
      Package() {CLK_TREE_FCH_UART2_APB, "apb_pclk", \_SB.COM2},
      Package() {CLK_TREE_FCH_UART3_APB, "apb_pclk", \_SB.COM3},
      Package() {CLK_TREE_FCH_UART0_FUNC, "uartclk", \_SB.COM0},
      Package() {CLK_TREE_FCH_UART1_FUNC, "uartclk", \_SB.COM1},
      Package() {CLK_TREE_FCH_UART2_FUNC, "uartclk", \_SB.COM2},
      Package() {CLK_TREE_FCH_UART3_FUNC, "uartclk", \_SB.COM3},

      /* I2C */
      Package() {CLK_TREE_FCH_I2C0_APB, "", \_SB.I2C0},
      Package() {CLK_TREE_FCH_I2C1_APB, "", \_SB.I2C1},
      Package() {CLK_TREE_FCH_I2C2_APB, "", \_SB.I2C2},
      Package() {CLK_TREE_FCH_I2C3_APB, "", \_SB.I2C3},
      Package() {CLK_TREE_FCH_I2C4_APB, "", \_SB.I2C4},
      Package() {CLK_TREE_FCH_I2C5_APB, "", \_SB.I2C5},
      Package() {CLK_TREE_FCH_I2C6_APB, "", \_SB.I2C6},
      Package() {CLK_TREE_FCH_I2C7_APB, "", \_SB.I2C7},

      /* I3C */
      Package() {CLK_TREE_FCH_I3C0_APB, "pclk", \_SB.I3C0},
      Package() {CLK_TREE_FCH_I3C0_FUNC, "sysclk", \_SB.I3C0},
      Package() {CLK_TREE_FCH_I3C1_APB, "pclk", \_SB.I3C1},
      Package() {CLK_TREE_FCH_I3C1_FUNC, "sysclk", \_SB.I3C1},

      /* Auido System */
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.ADSS.ACLK},
      Package() {CLK_TREE_AUDIO_CLK1, "audio_clk1", \_SB.ADSS.ACLK},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.ADSS.ACLK},
      Package() {CLK_TREE_AUDIO_CLK3, "audio_clk3", \_SB.ADSS.ACLK},
      Package() {CLK_TREE_AUDIO_CLK4, "audio_clk4", \_SB.ADSS.ACLK},
      Package() {CLK_TREE_AUDIO_CLK5, "audio_clk5", \_SB.ADSS.ACLK},

      /* I2S */
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S0},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S0},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S1},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S1},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S2},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S2},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S3},
      Package() {CLK_TREE_AUDIO_CLK1, "audio_clk1", \_SB.I2S3},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S3},
      Package() {CLK_TREE_AUDIO_CLK3, "audio_clk3", \_SB.I2S3},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S4},
      Package() {CLK_TREE_AUDIO_CLK1, "audio_clk1", \_SB.I2S4},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S4},
      Package() {CLK_TREE_AUDIO_CLK3, "audio_clk3", \_SB.I2S4},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S5},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S5},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S6},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S6},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S7},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S7},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S8},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S8},
      Package() {CLK_TREE_AUDIO_CLK0, "audio_clk0", \_SB.I2S9},
      Package() {CLK_TREE_AUDIO_CLK2, "audio_clk2", \_SB.I2S9},

      /* FCH DMA */
      Package() {CLK_TREE_FCH_DMA_ACLK, "", \_SB.DMA0},

      /* SPI */
      Package() {CLK_TREE_FCH_SPI0_APB, "pclk", \_SB.SPI0},
      Package() {CLK_TREE_FCH_SPI0_APB, "ref_clk", \_SB.SPI0},
      Package() {CLK_TREE_FCH_SPI1_APB, "pclk", \_SB.SPI1},
      Package() {CLK_TREE_FCH_SPI1_APB, "ref_clk", \_SB.SPI1},

      /* XSPI */
      Package() {CLK_TREE_FCH_XSPI_APB, "pclk", \_SB.XSPI},
      Package() {CLK_TREE_FCH_XSPI_MACLK, "maclk", \_SB.XSPI},
      Package() {CLK_TREE_FCH_XSPI_FUNC, "funcclk", \_SB.XSPI},

      /* GMAC0 */
      Package() {CLK_TREE_GMAC0_ACLK, "aclk", \_SB.MAC0},
      Package() {CLK_TREE_GMAC0_PCLK, "pclk", \_SB.MAC0},
      Package() {CLK_TREE_GMAC0_DIV_TXCLK, "tx_clk", \_SB.MAC0},

      /* GMAC1 */
      Package() {CLK_TREE_GMAC1_ACLK, "aclk", \_SB.MAC1},
      Package() {CLK_TREE_GMAC1_PCLK, "pclk", \_SB.MAC1},
      Package() {CLK_TREE_GMAC1_DIV_TXCLK, "tx_clk", \_SB.MAC1},

      /* USB SS0 */
      Package() {CLK_TREE_USB3C_DRD_CLK_SOF, "sof_clk", \_SB.SUB0},
      Package() {CLK_TREE_USB3C_DRD_AXI_GATE, "usb_aclk", \_SB.SUB0},
      Package() {CLK_TREE_USB3C_DRD_CLK_LPM, "lpm_clk", \_SB.SUB0},
      Package() {CLK_TREE_USB3C_DRD_APB_GATE, "usb_pclk", \_SB.SUB0},

      /* USB SS1 */
      Package() {CLK_TREE_USB3C_H0_CLK_SOF, "sof_clk", \_SB.SUB1},
      Package() {CLK_TREE_USB3C_0_AXI_GATE, "usb_aclk", \_SB.SUB1},
      Package() {CLK_TREE_USB3C_H0_CLK_LPM, "lpm_clk", \_SB.SUB1},
      Package() {CLK_TREE_USB3C_0_APB_GATE, "usb_pclk", \_SB.SUB1},

      /* USB SS2 */
      Package() {CLK_TREE_USB3C_H1_CLK_SOF, "sof_clk", \_SB.SUB2},
      Package() {CLK_TREE_USB3C_1_AXI_GATE, "usb_aclk", \_SB.SUB2},
      Package() {CLK_TREE_USB3C_H1_CLK_LPM, "lpm_clk", \_SB.SUB2},
      Package() {CLK_TREE_USB3C_1_APB_GATE, "usb_pclk", \_SB.SUB2},

      /* USB SS3 */
      Package() {CLK_TREE_USB3C_H2_CLK_SOF, "sof_clk", \_SB.SUB3},
      Package() {CLK_TREE_USB3C_2_AXI_GATE, "usb_aclk", \_SB.SUB3},
      Package() {CLK_TREE_USB3C_H2_CLK_LPM, "lpm_clk", \_SB.SUB3},
      Package() {CLK_TREE_USB3C_2_APB_GATE, "usb_pclk", \_SB.SUB3},

      /* USB SS4 */
      Package() {CLK_TREE_USB3A_H0_CLK_SOF, "sof_clk", \_SB.SUB4},
      Package() {CLK_TREE_USB3A_0_AXI_GATE, "usb_aclk", \_SB.SUB4},
      Package() {CLK_TREE_USB3A_H0_CLK_LPM, "lpm_clk", \_SB.SUB4},
      Package() {CLK_TREE_USB3A_0_APB_GATE, "usb_pclk", \_SB.SUB4},

      /* USB SS5 */
      Package() {CLK_TREE_USB3A_H1_CLK_SOF, "sof_clk", \_SB.SUB5},
      Package() {CLK_TREE_USB3A_1_AXI_GATE, "usb_aclk", \_SB.SUB5},
      Package() {CLK_TREE_USB3A_H1_CLK_LPM, "lpm_clk", \_SB.SUB5},
      Package() {CLK_TREE_USB3A_1_APB_GATE, "usb_pclk", \_SB.SUB5},

      /* USB HS0 */
      Package() {CLK_TREE_USB2_0_CLK_SOF, "sof_clk", \_SB.HUB0},
      Package() {CLK_TREE_USB2_0_AXI_GATE, "usb_aclk", \_SB.HUB0},
      Package() {CLK_TREE_USB2_0_CLK_LPM, "lpm_clk", \_SB.HUB0},
      Package() {CLK_TREE_USB2_0_APB_GATE, "usb_pclk", \_SB.HUB0},

      /* USB HS1 */
      Package() {CLK_TREE_USB2_1_CLK_SOF, "sof_clk", \_SB.HUB1},
      Package() {CLK_TREE_USB2_1_AXI_GATE, "usb_aclk", \_SB.HUB1},
      Package() {CLK_TREE_USB2_1_CLK_LPM, "lpm_clk", \_SB.HUB1},
      Package() {CLK_TREE_USB2_1_APB_GATE, "usb_pclk", \_SB.HUB1},

      /* USB HS2 */
      Package() {CLK_TREE_USB2_2_CLK_SOF, "sof_clk", \_SB.HUB2},
      Package() {CLK_TREE_USB2_2_AXI_GATE, "usb_aclk", \_SB.HUB2},
      Package() {CLK_TREE_USB2_2_CLK_LPM, "lpm_clk", \_SB.HUB2},
      Package() {CLK_TREE_USB2_2_APB_GATE, "usb_pclk", \_SB.HUB2},

      /* USB HS3 */
      Package() {CLK_TREE_USB2_3_CLK_SOF, "sof_clk", \_SB.HUB3},
      Package() {CLK_TREE_USB2_3_AXI_GATE, "usb_aclk", \_SB.HUB3},
      Package() {CLK_TREE_USB2_3_CLK_LPM, "lpm_clk", \_SB.HUB3},
      Package() {CLK_TREE_USB2_3_APB_GATE, "usb_pclk", \_SB.HUB3},

      /* USB PHY */
      Package() {CLK_TREE_USB3C_DRD_PHY3_GATE, "pclk", \_SB.UCP0},
      Package() {CLK_TREE_USB3C_0_PHY3_GATE, "pclk", \_SB.UCP1},
      Package() {CLK_TREE_USB3C_1_PHY3_GATE, "pclk", \_SB.UCP2},
      Package() {CLK_TREE_USB3C_2_PHY3_GATE, "pclk", \_SB.UCP3},
      Package() {CLK_TREE_USB3A_PHY3_GATE, "apb_clk", \_SB.U3P4},
      Package() {CLK_TREE_USB3A_PHY_x2_REF, "ref_clk", \_SB.U3P4},

      /* Timer */
      Package() {CLK_TREE_FCH_TIMER_APB, "fch_timer_apb_clk", \_SB.TMR0},
      Package() {CLK_TREE_FCH_TIMER_FUN, "fch_timer_func_clk", \_SB.TMR0},

      /* PWM */
      Package() {CLK_TREE_FCH_TIMER_APB, "fch_pwm_apb_clk", \_SB.PWM0},
      Package() {CLK_TREE_FCH_TIMER_FUN, "fch_pwm_func_clk", \_SB.PWM0},
      Package() {CLK_TREE_FCH_TIMER_APB, "fch_pwm_apb_clk", \_SB.PWM1},
      Package() {CLK_TREE_FCH_TIMER_FUN, "fch_pwm_func_clk", \_SB.PWM1},

      /* VPU */
      Package() {CLK_TREE_VPU_APBCLK, "vpu_clk", \_SB.VPU0},

      /* DPU */
      Package() {CLK_TREE_DP0_PIXEL0, "pipeline@0", \_SB.DPU0},
      Package() {CLK_TREE_DP0_PIXEL1, "pipeline@1", \_SB.DPU0},
      Package() {CLK_TREE_DP1_PIXEL0, "pipeline@0", \_SB.DPU1},
      Package() {CLK_TREE_DP1_PIXEL1, "pipeline@1", \_SB.DPU1},
      Package() {CLK_TREE_DP2_PIXEL0, "pipeline@0", \_SB.DPU2},
      Package() {CLK_TREE_DP2_PIXEL1, "pipeline@1", \_SB.DPU2},
      Package() {CLK_TREE_DP3_PIXEL0, "pipeline@0", \_SB.DPU3},
      Package() {CLK_TREE_DP3_PIXEL1, "pipeline@1", \_SB.DPU3},
      Package() {CLK_TREE_DP4_PIXEL0, "pipeline@0", \_SB.DPU4},
      Package() {CLK_TREE_DP4_PIXEL1, "pipeline@1", \_SB.DPU4},

      /* DP00 */
      Package() {CLK_TREE_DPC0_VIDCLK0, "vid_clk0", \_SB.DP00},
      Package() {CLK_TREE_DPC0_VIDCLK1, "vid_clk1", \_SB.DP00},
      Package() {CLK_TREE_DPC0_APBCLK, "apb_clk", \_SB.DP00},

      /* DP01 */
      Package() {CLK_TREE_DPC1_VIDCLK0, "vid_clk0", \_SB.DP01},
      Package() {CLK_TREE_DPC1_VIDCLK1, "vid_clk1", \_SB.DP01},
      Package() {CLK_TREE_DPC1_APBCLK, "apb_clk", \_SB.DP01},

      /* DP02 */
      Package() {CLK_TREE_DPC2_VIDCLK0, "vid_clk0", \_SB.DP02},
      Package() {CLK_TREE_DPC2_VIDCLK1, "vid_clk1", \_SB.DP02},
      Package() {CLK_TREE_DPC2_APBCLK, "apb_clk", \_SB.DP02},

      /* DP03 */
      Package() {CLK_TREE_DPC3_VIDCLK0, "vid_clk0", \_SB.DP03},
      Package() {CLK_TREE_DPC3_VIDCLK1, "vid_clk1", \_SB.DP03},
      Package() {CLK_TREE_DPC3_APBCLK, "apb_clk", \_SB.DP03},

      /* DP04 */
      Package() {CLK_TREE_DPC4_VIDCLK0, "vid_clk0", \_SB.DP04},
      Package() {CLK_TREE_DPC4_VIDCLK1, "vid_clk1", \_SB.DP04},
      Package() {CLK_TREE_DPC4_APBCLK, "apb_clk", \_SB.DP04},
    })

    /*get clock method*/
    Method (GCLK, 1, Serialized)
    {
        Return(\_SB.PMMX.CLKG(Arg0))
    }

    /*set clock method*/
    Method (SCLK, 3, Serialized)
    {
        Return(\_SB.PMMX.CLKS(Arg0,Arg1,Arg2))
    }

    /*describe clock method*/
    Method (CLKD, 2, Serialized)
    {
        Return(\_SB.PMMX.CLKD(Arg0,Arg1))
    }

    /*config clock method*/
    Method (CLKC, 2, Serialized)
    {
        Return(\_SB.PMMX.CLKC(Arg0,Arg1))
    }
}
