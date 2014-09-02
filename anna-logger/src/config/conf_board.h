/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H
/** Name string macro */
//#define BOARD_NAME                "ANNA_LOGGER_REV_A"

//Clocks
/*
#define BOARD_FREQ_SLCK_XTAL      0 // Not Mounted
#define BOARD_FREQ_SLCK_BYPASS    0 // Not Mounted
#define BOARD_FREQ_MAINCK_XTAL    (14756400U)
#define BOARD_FREQ_MAINCK_BYPASS  (14756400U)
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
*/


//LEDs
/*


//name LED0 definitions
#define LED0_PIN                  PIN_PA12
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE

//name LED1 definitions
#define LED1_PIN                  PIN_PA27
#define LED1_ACTIVE               false
#define LED1_INACTIVE             !LED1_ACTIVE

//name LED2 definitions
#define LED2_PIN                  PIN_PA13
#define LED2_ACTIVE               false
#define LED2_INACTIVE             !LED2_ACTIVE



//name LED #0 definitions
#define LED_0_NAME                "LED0 (green)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
#define LED0_GPIO 				        LED0_PIN

//name LED #0 definitions
#define LED_1_NAME                "LED1 (orange)"
#define LED_1_PIN                 LED1_PIN
#define LED_1_ACTIVE              LED1_ACTIVE
#define LED_1_INACTIVE            LED1_INACTIVE
#define LED1_GPIO 				        LED1_PIN

//name LED #0 definitions
#define LED_2_NAME                "LED2 (WIFI)"
#define LED_2_PIN                 LED2_PIN
#define LED_2_ACTIVE              LED2_ACTIVE
#define LED_2_INACTIVE            LED2_INACTIVE
#define LED2_GPIO 				        LED2_PIN

// Number of on-board LEDs
#define LED_COUNT                 3



//Buttons


//name SW0 definitions
#define SW0_PIN                   PIN_PB02
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA15A_EIC_EXTINT15
#define SW0_EIC_MUX               MUX_PA15A_EIC_EXTINT15
#define SW0_EIC_PINMUX            PINMUX_PA15A_EIC_EXTINT15
#define SW0_EIC_LINE              2

//name SW1 definitions
#define SW1_PIN                   PIN_PB03
#define SW1_ACTIVE                false
#define SW1_INACTIVE              !SW1_ACTIVE
#define SW1_EIC_PIN               PIN_PA15A_EIC_EXTINT15
#define SW1_EIC_MUX               MUX_PA15A_EIC_EXTINT15
#define SW1_EIC_PINMUX            PINMUX_PA15A_EIC_EXTINT15
#define SW1_EIC_LINE              2



//name Button #0 definitions
#define BUTTON_0_NAME             "Button0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_EIC_PIN          SW0_EIC_PIN
#define BUTTON_0_EIC_MUX          SW0_EIC_MUX
#define BUTTON_0_EIC_PINMUX       SW0_EIC_PINMUX
#define BUTTON_0_EIC_LINE         SW0_EIC_LINE


//name Button #0 definitions
#define BUTTON_1_NAME             "Button1"
#define BUTTON_1_PIN              SW1_PIN
#define BUTTON_1_ACTIVE           SW1_ACTIVE
#define BUTTON_1_INACTIVE         SW1_INACTIVE
#define BUTTON_1_EIC_PIN          SW1_EIC_PIN
#define BUTTON_1_EIC_MUX          SW1_EIC_MUX
#define BUTTON_1_EIC_PINMUX       SW1_EIC_PINMUX
#define BUTTON_1_EIC_LINE         SW1_EIC_LINE



// Number of on-board buttons
#define BUTTON_COUNT 2


//FTDI UART Interface
#define FTDI_HOST_UART_RXD        PIN_PB22
#define FTDI_HOST_UART_TXD        PIN_PB23

#define FTDI_HOST_MODULE              SERCOM5
#define FTDI_HOST_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define FTDI_HOST_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define FTDI_HOST_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define FTDI_HOST_SERCOM_PINMUX_PAD2  PINMUX_PB22D_SERCOM5_PAD2
#define FTDI_HOST_SERCOM_PINMUX_PAD3  PINMUX_PB23D_SERCOM5_PAD3

//SD Card Defines
#define SD_SPI_SCK                PIN_PA09
#define SD_SPI_CS_N               PIN_PA10
#define SD_SPI_MISO               PIN_PA11
#define SD_SPI_MOSI               PIN_PA08

#define SD_CARD_DETECT_N          PIN_PA20

//SD Card Notes:
#define SD_CARD_DETECT_N_EIC      4

#define SD_SPI_MODULE              SERCOM0
#define SD_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define SD_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA08C_SERCOM0_PAD0
#define SD_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA09C_SERCOM0_PAD1
#define SD_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA10C_SERCOM0_PAD2
#define SD_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA11C_SERCOM0_PAD3

//EEPROM Defines
#define EEPROM_SPI_MOSI           PIN_PB08
#define EEPROM_SPI_SCK            PIN_PB09
#define EEPROM_SPI_CS_N           PIN_PB10
#define EEPROM_SPI_MISO           PIN_PB11

//WIFI Defines
#define WIFI_EN					  PIN_PA07
#define WIFI_MOSI                 PIN_PA22
#define WIFI_SCK                  PIN_PA23
#define WIFI_CS_N                 PIN_PA24
#define WIFI_MISO                 PIN_PA24

#define WIFI_IRQ_N                PIN_PA21

//WIFI Notes:
#define WIFI_IRQ_N_EIC            5

//Expansion Card Defines
#define CARD_A0                   PIN_PA02
#define CARD_A1                   PIN_PA03
#define CARD_A2                   PIN_PA04
#define CARD_A3                   PIN_PA05
#define CARD_A4                   PIN_PA06

#define CARD_SCK                  PIN_PA17
#define CARD_CS_N                 PIN_PA18
#define CARD_MISO                 PIN_PA19
#define CARD_MOSI                 PIN_PA16
*/
#endif // CONF_BOARD_H
