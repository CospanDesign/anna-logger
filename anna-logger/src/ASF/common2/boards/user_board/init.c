/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
volatile bool transfer_complete_spi_master = false;
void configure_spi_master_callbacks( void );
void configure_spi_master( void );
static void callback_spi_master( const struct spi_module *const module )
{
	transfer_complete_spi_master = true;
}
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
	port_pin_set_config(LED_0_PIN, &pco);
	port_pin_set_output_level(LED_0_PIN, false);
	
	
	port_pin_set_config(LED_1_PIN, &pco);
	port_pin_set_output_level(LED_1_PIN, true);	
	
	port_pin_set_config(LED_2_PIN, &pco);
	port_pin_set_output_level(LED_2_PIN, true);	
	
	//Buttons
	port_pin_set_config(BUTTON_0_PIN, &pci);
	port_pin_set_config(BUTTON_1_PIN, &pci);
	
	//Setup WIFI Interface
	port_pin_set_config(WIFI_EN, &pco);
	port_pin_set_output_level(WIFI_EN, false);
	
	port_pin_set_config(WIFI_IRQ_N, &pci);	
	
	//Setup the LTC2448 (ADC Interface)
	port_pin_set_config(CARD_A0, &pco);
	port_pin_set_config(CARD_A1, &pco);
	port_pin_set_config(CARD_A2, &pco);
	port_pin_set_config(CARD_A3, &pco);
	port_pin_set_config(CARD_A4, &pco);	
	
	port_pin_set_output_level(CARD_A0, false);
	port_pin_set_output_level(CARD_A1, false);
	port_pin_set_output_level(CARD_A2, false);
	port_pin_set_output_level(CARD_A3, false);
	port_pin_set_output_level(CARD_A4, false);
}

struct spi_module spi_sdmicro_instance;

void configure_spi_master( void )
{
	struct spi_config config_spi_master;
	spi_get_config_defaults( &config_spi_master );
	config_spi_master.mux_setting = ANNA_SDMICRO_SPI_SERCOM_MUX_SETTING;
	config_spi_master.pinmux_pad0 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad2 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = ANNA_SDMICRO_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &spi_sdmicro_instance, ANNA_SDMICRO_SPI_MODULE, &config_spi_master );
	spi_enable( &spi_sdmicro_instance );

	// configure_sdmicro_spi_master_callbacks
	spi_register_callback( &spi_sdmicro_instance, callback_spi_master, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &spi_sdmicro_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}

struct spi_module spi_wifi_instance;

void configure_spi_master( void )
{
	struct spi_config config_spi_master;
	spi_get_config_defaults( &config_spi_master );
	config_spi_master.mux_setting = ANNA_WIFI_SPI_SERCOM_MUX_SETTING;
	config_spi_master.pinmux_pad0 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad2 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = ANNA_WIFI_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &spi_wifi_instance, ANNA_WIFI_SPI_MODULE, &config_spi_master );
	spi_enable( &spi_wifi_instance );

	// configure_wifi_spi_master_callbacks
	spi_register_callback( &spi_wifi_instance, callback_spi_master, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &spi_wifi_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}


struct spi_module spi_card_instance;

void configure_spi_master( void )
{
	struct spi_config config_spi_master;
	spi_get_config_defaults( &config_spi_master );
	config_spi_master.mux_setting = ANNA_CARD_SPI_SERCOM_MUX_SETTING;
	config_spi_master.pinmux_pad0 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad2 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = ANNA_CARD_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &spi_card_instance, ANNA_CARD_SPI_MODULE, &config_spi_master );
	spi_enable( &spi_card_instance );

	// configure_card_spi_master_callbacks
	spi_register_callback( &spi_card_instance, callback_spi_master, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &spi_card_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}

struct spi_module spi_eeprom_instance;

void configure_spi_master( void )
{
	struct spi_config config_spi_master;
	spi_get_config_defaults( &config_spi_master );
	config_spi_master.mux_setting = ANNA_EEPROM_SPI_SERCOM_MUX_SETTING;
	config_spi_master.pinmux_pad0 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad2 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = ANNA_EEPROM_SPI_SERCOM_PINMUX_PAD3;
	spi_init( &spi_eeprom_instance, ANNA_EEPROM_SPI_MODULE, &config_spi_master );
	spi_enable( &spi_eeprom_instance );

	// configure_eeprom_spi_master_callbacks
	spi_register_callback( &spi_eeprom_instance, callback_spi_master, SPI_CALLBACK_BUFFER_TRANSMITTED );
	spi_enable_callback( &spi_eeprom_instance, SPI_CALLBACK_BUFFER_TRANSMITTED );
}


