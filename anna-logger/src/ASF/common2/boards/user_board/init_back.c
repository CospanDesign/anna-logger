/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include <user_board.h>
#include <usart.h>


void configure_anna_sdmicro_spi( void );
void configure_anna_wifi_spi( void );
void configure_anna_card_spi( void );
void configure_anna_eeprom_spi( void );
void configure_anna_uart( void );

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	struct port_config pco = {
		.direction	= PORT_PIN_DIR_OUTPUT_WTH_READBACK,
		.input_pull = PORT_PIN_PULL_NONE,
		.powersave	= false
	};
	struct port_config pci = {
		.direction	= PORT_PIN_DIR_INPUT,
		.input_pull = PORT_PIN_PULL_NONE,
		.powersave	= false
	};	
	//port_pin_set_output_level(LED_0_PIN, ~port_pin_get_input_level(LED_0_PIN));
	port_pin_set_config(ANNA_LED_GRN_PIN, &pco);
	port_pin_set_output_level(ANNA_LED_GRN_PIN, LOW);
	
	
	port_pin_set_config(ANNA_LED_ORG_PIN, &pco);
	port_pin_set_output_level(ANNA_LED_ORG_PIN, HIGH);	
	
	port_pin_set_config(ANNA_LED_WIFI_PIN, &pco);
	port_pin_set_output_level(ANNA_LED_WIFI_PIN, HIGH);	
	
	//Setup the LTC2448 (ADC Interface)
	port_pin_set_config(ANNA_CARD_A0_PIN, &pco);
	port_pin_set_config(ANNA_CARD_A1_PIN, &pco);
	port_pin_set_config(ANNA_CARD_A2_PIN, &pco);
	port_pin_set_config(ANNA_CARD_A3_PIN, &pco);
	port_pin_set_config(ANNA_CARD_A4_PIN, &pco);
	
	port_pin_set_output_level(ANNA_CARD_A0_PIN, false);
	port_pin_set_output_level(ANNA_CARD_A1_PIN, false);
	port_pin_set_output_level(ANNA_CARD_A2_PIN, false);
	port_pin_set_output_level(ANNA_CARD_A3_PIN, false);
	port_pin_set_output_level(ANNA_CARD_A4_PIN, false);

	
    //configure_anna_sdmicro_spi();
    configure_anna_wifi_spi();
    //configure_anna_card_spi();
    //configure_anna_eeprom_spi();
    //configure_anna_uart();
}

/*
struct spi_module anna_sdmicro_instance;

void configure_anna_sdmicro_spi( void )
{
	struct spi_config config_spi;
	
	spi_get_config_defaults( &config_spi );
	config_spi.mux_setting = ANNA_SDMICRO_SPI_SERCOM_MUX_SETTING;
	config_spi.pinmux_pad0 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD0;
	config_spi.pinmux_pad1 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD1;
	config_spi.pinmux_pad2 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD2;
	config_spi.pinmux_pad3 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &anna_sdmicro_instance, ANNA_SDMICRO_SPI_MODULE, &config_spi );
	spi_enable( &anna_sdmicro_instance );

	// configure_sdmicro_spi_callbacks
	spi_register_callback( &anna_sdmicro_instance, callback_spi, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &anna_sdmicro_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}
*/
struct spi_module anna_wifi_instance;

void configure_anna_wifi_spi( void )
{
	struct spi_config config_spi;
	
	spi_get_config_defaults( &config_spi );
	config_spi.mux_setting = ANNA_WIFI_SPI_SERCOM_MUX_SETTING;
	config_spi.pinmux_pad0 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD0;
	config_spi.pinmux_pad1 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD1;
	config_spi.pinmux_pad2 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD2;
	config_spi.pinmux_pad3 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &anna_wifi_instance, ANNA_WIFI_SPI_MODULE, &config_spi );
	spi_enable( &anna_wifi_instance );

	// configure_wifi_spi_callbacks
	
	//spi_register_callback( &anna_wifi_instance, callback_spi, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &anna_wifi_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}
/*

struct spi_module anna_card_instance;

void configure_anna_card_spi( void )
{
	struct spi_config config_spi;
	
	spi_get_config_defaults( &config_spi );
	config_spi.mux_setting = ANNA_CARD_SPI_SERCOM_MUX_SETTING;
	config_spi.pinmux_pad0 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD0;
	config_spi.pinmux_pad1 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD1;
	config_spi.pinmux_pad2 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD2;
	config_spi.pinmux_pad3 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &anna_card_instance, ANNA_CARD_SPI_MODULE, &config_spi );
	spi_enable( &anna_card_instance );

	// configure_card_spi_callbacks
	spi_register_callback( &anna_card_instance, callback_spi, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &anna_card_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}

struct spi_module anna_eeprom_instance;

void configure_anna_eeprom_spi( void )
{
	struct spi_config config_spi;
	
	spi_get_config_defaults( &config_spi );
	config_spi.mux_setting = ANNA_EEPROM_SPI_SERCOM_MUX_SETTING;
	config_spi.pinmux_pad0 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD0;
	config_spi.pinmux_pad1 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD2;
	config_spi.pinmux_pad2 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD2;
	config_spi.pinmux_pad3 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &anna_eeprom_instance, ANNA_EEPROM_SPI_MODULE, &config_spi );
	spi_enable( &anna_eeprom_instance );

	// configure_eeprom_spi_callbacks
	spi_register_callback( &anna_eeprom_instance, callback_spi, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &anna_eeprom_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}


struct usart_module anna_uart_instance;

void configure_anna_uart( void )
{
	struct usart_config config_usart;
	
	usart_get_config_defaults( &config_usart );
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = ANNA_UART_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = ANNA_UART_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = ANNA_UART_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad2 = ANNA_UART_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = ANNA_UART_SERCOM_PINMUX_PAD3;
	usart_init( &anna_uart_instance, ANNA_UART_MODULE, &config_usart );
	usart_enable( &anna_uart_instance );

	// configure_usart_callbacks
	usart_register_callback( &anna_uart_instance, callback_usart, USART_CALLBACK_BUFFER_TRANSMITTED );
	usart_enable_callback( &anna_uart_instance, USART_CALLBACK_BUFFER_TRANSMITTED );
}
*/

