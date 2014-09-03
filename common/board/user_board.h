/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/** Name string macro */
#define BOARD_NAME                "USER_BOARD"

/**
 * \defgroup ANNALOGGER_WIFI_REVB_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** \name ANNA_LED_GRN definitions
 *  @{ */
#define ANNA_LED_GRN_NAME                "ANNA_LED_GRN"
#define ANNA_LED_GRN_PIN                 PIN_PA12
#define ANNA_LED_GRN_ACTIVE              false
#define ANNA_LED_GRN_INACTIVE            !ANNA_LED_GRN_ACTIVE
#define ANNA_LED_GRN_GPIO 				 ANNA_LED_GRN_PIN
/** @} */                                
/** \name ANNA_LED_ORG definitions       
 *  @{ */                                
#define ANNA_LED_ORG_NAME                "ANNA_LED_ORG"
#define ANNA_LED_ORG_PIN                 PIN_PA13
#define ANNA_LED_ORG_ACTIVE              false
#define ANNA_LED_ORG_INACTIVE            !ANNA_LED_ORG_ACTIVE
#define ANNA_LED_ORG_GPIO 				 ANNA_LED_ORG_PIN
/** @} */                                
/** \name ANNA_LED_WIFI definitions      
 *  @{ */                                
#define ANNA_LED_WIFI_NAME               "ANNA_LED_WIFI"
#define ANNA_LED_WIFI_PIN                PIN_PA27
#define ANNA_LED_WIFI_ACTIVE             false
#define ANNA_LED_WIFI_INACTIVE           !ANNA_LED_WIFI_ACTIVE
#define ANNA_LED_WIFI_GPIO 				 ANNA_LED_WIFI_PIN
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 3

/** \name ANNA_BUTTON_GRN definitions
 *  @{ */
#define ANNA_BUTTON_GRN_NAME                  "ANNA_BUTTON_GRN"
#define ANNA_BUTTON_GRN_PIN                   PIN_PB02
#define ANNA_BUTTON_GRN_ACTIVE                false
#define ANNA_BUTTON_GRN_INACTIVE              !ANNA_BUTTON_GRN_ACTIVE
#define ANNA_BUTTON_GRN_EIC_PIN               PIN_PB02A_EIC_EXTINT2
#define ANNA_BUTTON_GRN_EIC_MUX               MUX_PB02A_EIC_EXTINT2
#define ANNA_BUTTON_GRN_EIC_PINMUX            PINMUX_PB02A_EIC_EXTINT2
#define ANNA_BUTTON_GRN_EIC_LINE              2
/** @} */

/** \name ANNA_BUTTON_ORG definitions
 *  @{ */
#define ANNA_BUTTON_ORG_NAME                  "ANNA_BUTTON_ORG"
#define ANNA_BUTTON_ORG_PIN                   PIN_PB03
#define ANNA_BUTTON_ORG_ACTIVE                false
#define ANNA_BUTTON_ORG_INACTIVE              !ANNA_BUTTON_ORG_ACTIVE
#define ANNA_BUTTON_ORG_EIC_PIN               PIN_PB03A_EIC_EXTINT3
#define ANNA_BUTTON_ORG_EIC_MUX               MUX_PB03A_EIC_EXTINT3
#define ANNA_BUTTON_ORG_EIC_PINMUX            PINMUX_PB03A_EIC_EXTINT3
#define ANNA_BUTTON_ORG_EIC_LINE              3
/** @} */

/** Number of on-board buttons */
#define BUTTON_COUNT 2


/** \name Annalogger UART definitions
 *  @{
 */
#define ANNA_UART_MODULE              SERCOM5
#define ANNA_UART_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define ANNA_UART_SERCOM_PINMUX_PAD0  PINMUX_UNUSED 
#define ANNA_UART_SERCOM_PINMUX_PAD1  PINMUX_UNUSED 
#define ANNA_UART_SERCOM_PINMUX_PAD2  PINMUX_PB22D_SERCOM5_PAD2 //UART_TXD USB_RXD
#define ANNA_UART_SERCOM_PINMUX_PAD3  PINMUX_PB23D_SERCOM5_PAD3 //UART_RXD USB_TXD
/** @} */


/** \name SDMICRO_DETECT_N IRQ/External interrupt definitions
 *  @{
 */
#define SDMICRO_DETECT_PIN		            PIN_PA20
#define SDMICRO_DETECT_IRQ_MODULE           EIC
#define SDMICRO_DETECT_IRQ_INPUT            4
#define SDMICRO_DETECT_IRQ_PIN              PIN_PA20A_EIC_EXTINT4
#define SDMICRO_DETECT_IRQ_MUX              MUX_PA20A_EIC_EXTINT4
#define SDMICRO_DETECT_IRQ_PINMUX           PINMUX_PA20A_EIC_EXTINT4
/** @} */

/** \name SDMICRO SPI definitions
 *  @{
 */
#define ANNA_SDMICRO_SPI_MODULE              SERCOM0
#define ANNA_SDMICRO_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA08C_SERCOM0_PAD0 //SDMICRO_MOSI
#define ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA09C_SERCOM0_PAD1 //SDMICRO_SCK
#define ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA10C_SERCOM0_PAD2 //SDMICRO_CS_N
#define ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA11C_SERCOM0_PAD3 //SDMICRO_MISO

#define ANNA_SDMICRO_CS_N_PIN              	 PIN_PA10
/** @} */

/** \name CARD SPI definitions
 *  @{
 */
#define ANNA_CARD_A0_PIN                  PIN_PA02
#define ANNA_CARD_A1_PIN                  PIN_PA03
#define ANNA_CARD_A2_PIN                  PIN_PA04
#define ANNA_CARD_A3_PIN                  PIN_PA05
#define ANNA_CARD_A4_PIN                  PIN_PA06 
 
#define ANNA_CARD_SPI_MODULE              SERCOM1
#define ANNA_CARD_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define ANNA_CARD_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0 //CARD_MOSI
#define ANNA_CARD_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA17C_SERCOM1_PAD1 //CARD_SCK
#define ANNA_CARD_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA18C_SERCOM1_PAD2 //CARD_CS_N
#define ANNA_CARD_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA19C_SERCOM1_PAD3 //CARD_MISO
/** @} */


/** \name WIFI_N IRQ/External interrupt definitions
 *  @{
 */
#define WIFI_IRQ_MODULE           EIC
#define WIFI_IRQ_INPUT            5
#define WIFI_IRQ_PIN              PIN_PA21A_EIC_EXTINT5
#define WIFI_IRQ_MUX              MUX_PA21A_EIC_EXTINT5
#define WIFI_IRQ_PINMUX           PINMUX_PA21A_EIC_EXTINT5

#define WIFI_IRQ_CHANNEL                  5

#define ANNA_WIFI_IRQ_PIN                 PIN_PA21

/** @} */

/** \name WIFI SPI definitions
 *  @{
 */
#define ANNA_WIFI_SPI_MODULE              SERCOM3
#define ANNA_WIFI_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define ANNA_WIFI_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA22C_SERCOM3_PAD0 //WIFI_MOSI
#define ANNA_WIFI_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA23C_SERCOM3_PAD1 //WIFI_SCK
#define ANNA_WIFI_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA24C_SERCOM3_PAD2 //WIFI_CS_N
#define ANNA_WIFI_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA25C_SERCOM3_PAD3 //WIFI_MISO

#define ANNA_WIFI_CS_N                    PIN_PA24

#define ANNA_WIFI_EN					  PIN_PA07
/** @} */

/** \name EEPROM SPI definitions
 *  @{
 */
#define ANNA_EEPROM_SPI_MODULE              SERCOM4
#define ANNA_EEPROM_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB08D_SERCOM4_PAD0 //EEPROM_MOSI
#define ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB09D_SERCOM4_PAD1 //EEPROM_SCK
#define ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB10D_SERCOM4_PAD2 //EEPROM_CS_N
#define ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB11D_SERCOM4_PAD3 //EEPROM_MISO
/** @} */

#ifdef __cplusplus
}
#endif

#endif // USER_BOARD_H
