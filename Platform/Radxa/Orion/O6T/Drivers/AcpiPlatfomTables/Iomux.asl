/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <sky1-iomux.h>
#include "IoConfig.h"

// MUX0 For iomux S0 pins
Device (MUX0) {
  Name (_HID, "CIXHA016")
  Name (_UID, 0x0)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x04170000, 0x1000)

// Platform defined pinctrl

    PinGroup ("pinctrl_fch_i2c0", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2C0_CLK, 0x00, 0x47,
            IO_S0_I2C0_SDA, 0x00, 0x47,
        })
        {
            SKY1_IOMUXC_I2C0_CLK,
            SKY1_IOMUXC_I2C0_SDA,
        }

    PinGroup ("pinctrl_fch_i2c2", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2C2_SCL, 0x00, 0x5c,
            IO_S0_I2C2_SDA, 0x00, 0x5c,
        })
        {
            SKY1_IOMUXC_I2C2_SCL,
            SKY1_IOMUXC_I2C2_SDA,
        }

    PinGroup ("pinctrl_fch_uart2", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_UART2_TXD, 0x00, 0x27,
            IO_S0_UART2_RXD, 0x00, 0x27,
        })
        {
            SKY1_IOMUXC_UART2_TXD,
            SKY1_IOMUXC_UART2_RXD,
        }

    PinGroup ("pinctrl_substrate_i2s1", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2S1_MCLK,        0x00, 0x3c,
            IO_S0_I2S1_SCK,         0x00, 0x3c,
            IO_S0_I2S1_WS,          0x00, 0x5c,
            IO_S0_I2S1_DATA_IN,     0x00, 0x3c,
            IO_S0_I2S1_DATA_OUT,    0x00, 0x3c,
        })
        {
            SKY1_IOMUXC_I2S1_MCLK,
            SKY1_IOMUXC_I2S1_SCK,
            SKY1_IOMUXC_I2S1_WS,
            SKY1_IOMUXC_I2S1_DATA_IN,
            SKY1_IOMUXC_I2S1_DATA_OUT,
        }

// Device defined pinctrl

    PinGroup ("oculink_pwren", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2S2_MCLK, 0x00, 0xD8,
        })
        {
            SKY1_IOMUXC_I2S2_MCLK,
        }

    PinGroup ("3v3_bkey", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2S2_TSCK,    0x00, 0x44,
        })
        {
            SKY1_IOMUXC_I2S2_TSCK,
        }

    PinGroup ("bkey_reset", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_I2S2_TWS,     0x00, 0x44,
        })
        {
            SKY1_IOMUXC_I2S2_TWS,
        }

    PinGroup ("pinctrl_cam0_hw", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_CSI0_MCLK0,       0x00, 0x3c,
            IO_S0_I2S3_DATA_IN1,    0x00, 0x9c,
            IO_S0_I2S4_MCLK_LB,     0x00, 0x1c,
            IO_S0_I2S4_SCK_LB,      0x00, 0x0c,
            IO_S0_I2S4_DATA_IN_LB,  0x00, 0x1c,
        })
        {
            SKY1_IOMUXC_CSI0_MCLK0,
            SKY1_IOMUXC_I2S3_DATA_IN1,
            SKY1_IOMUXC_I2S4_MCLK_LB,
            SKY1_IOMUXC_I2S4_SCK_LB,
            SKY1_IOMUXC_I2S4_DATA_IN_LB,
        }

    PinGroup ("pinctrl_cam1_hw", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S0_CSI0_MCLK1,       0x00, 0x3c,
            IO_S0_I2S4_WS_LB,       0x00, 0x1c,
            IO_S0_I2S4_DATA_OUT_LB, 0x00, 0x1c,
        })
        {
            SKY1_IOMUXC_CSI0_MCLK1,
            SKY1_IOMUXC_I2S4_WS_LB,
            SKY1_IOMUXC_I2S4_DATA_OUT_LB,
        }
  })
}

// MUX1 For iomux S5 pins
Device (MUX1) {
  Name (_HID, "CIXHA017")
  Name (_UID, 0x1)
  Name (_STA, 0xF)
  Name (_CRS, ResourceTemplate () {
    Memory32Fixed (ReadWrite, 0x16007000, 0x1000)

// Platform defined pinctrl

    PinGroup ("pinctrl_fch_spi0", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_SPI1_MISO,    0x00, 0x5c,
            IO_S5_SPI1_CS0,     0x00, 0x5c,
            IO_S5_SPI1_CS1,     0x00, 0x5c,
            IO_S5_SPI1_MOSI,    0x00, 0x5c,
            IO_S5_SPI1_CLK,     0x00, 0x1c,
        })
        {
            SKY1_IOMUXC_SPI1_MISO,
            SKY1_IOMUXC_SPI1_CS0,
            SKY1_IOMUXC_SPI1_CS1,
            SKY1_IOMUXC_SPI1_MOSI,
            SKY1_IOMUXC_SPI1_CLK,
        }

    PinGroup ("pinctrl_fch_xspi", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_SE_QSPI_CLK,      0x00, 0xdc,
            IO_S5_SE_QSPI_CS_L,     0x00, 0xdc,
            IO_S5_SE_QSPI_DATA0,    0x00, 0xdc,
            IO_S5_SE_QSPI_DATA1,    0x00, 0xdc,
            IO_S5_SE_QSPI_DATA2,    0x00, 0xdc,
            IO_S5_SE_QSPI_DATA3,    0x00, 0xdc,
        })
        {
            SKY1_IOMUXC_SE_QSPI_CLK,
            SKY1_IOMUXC_SE_QSPI_CS_L,
            SKY1_IOMUXC_SE_QSPI_DATA0,
            SKY1_IOMUXC_SE_QSPI_DATA1,
            SKY1_IOMUXC_SE_QSPI_DATA2,
            SKY1_IOMUXC_SE_QSPI_DATA3,
        }

    PinGroup ("pinctrl_usb4", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_USB_DRIVE_VBUS4, 0x00, 0xa4,
        })
        {
            SKY1_IOMUXC_DRIVE_VBUS4,
        }

    PinGroup ("pinctrl_usb5", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_USB_DRIVE_VBUS5, 0x00, 0xa4,
        })
        {
            SKY1_IOMUXC_DRIVE_VBUS5,
        }
    PinGroup ("pinctrl_pcie_x8_rc", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO002, 0x00, 0x24,
        })
        {
            SKY1_IOMUXC_GPIO2,
        }

    PinGroup ("pinctrl_pcie_x4_rc", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO004, 0x00, 0x24,
        })
        {
            SKY1_IOMUXC_GPIO4,
        }

    PinGroup ("pinctrl_pcie_x2_rc", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO005, 0x00, 0x24,
        })
        {
            SKY1_IOMUXC_GPIO5,
        }

    PinGroup ("pinctrl_pcie_x1_1_rc", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO006, 0x00, 0x24,
        })
        {
            SKY1_IOMUXC_GPIO6,
        }

    PinGroup ("pinctrl_pcie_x1_0_rc", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO003, 0x00, 0x24,
        })
        {
            SKY1_IOMUXC_GPIO3,
        }

// Device defined pinctrl

    PinGroup ("lom_pwren", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO010, 0x00, 0x44,
        })
        {
            SKY1_IOMUXC_GPIO10,
        }

    PinGroup ("rtc_alarm_l", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO011, 0x00, 0x44,
        })
        {
            SKY1_IOMUXC_GPIO11,
        }

    PinGroup ("wlan_pwren", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO013, 0x00, 0x5c,
        })
        {
            SKY1_IOMUXC_GPIO13,
        }

    PinGroup ("ufs_5v_en", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_GPIO014, 0x00, 0x44,
        })
        {
            SKY1_IOMUXC_GPIO14,
        }

    PinGroup ("phy2_pwr_en", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_SFI_GPIO7, 0x00, 0xD7,
        })
        {
            SKY1_IOMUXC_SFI_GPIO7,
        }

    PinGroup ("tpm_pwr_en", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_SFI_GPIO8, 0x00, 0xD7,
        })
        {
            SKY1_IOMUXC_SFI_GPIO8,
        }
    
    PinGroup ("gpio_leds", ResourceProducer, ,
        RawDataBuffer ()
        {
            IO_S5_SFI_GPIO0, 0x00, 0x57,
            IO_S5_SFI_GPIO6, 0x00, 0xD7,
        })
        {
            SKY1_IOMUXC_SFI_GPIO0,
            SKY1_IOMUXC_SFI_GPIO6,
        }
  })
}
