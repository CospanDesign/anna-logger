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
	
	//Setup WIFI Interface
	port_pin_set_config(WIFI_EN, &pco);
	port_pin_set_config(WIFI_IRQ_N, &pci);
	port_pin_set_output_level(WIFI_EN, false);
	
	//port_pin_set_config(WIFI_SCK, &pco);
	//port_pin_set_config(WIFI_CS_N, &pco);
	//port_pin_set_config(WIFI_MOSI, &pco);
	
	//port_pin_set_config(WIFI_MISO, &pci);
	
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
	
	port_pin_set_config(CARD_SCK, &pco);
	port_pin_set_output_level(CARD_SCK, false);
	port_pin_set_config(CARD_CS_N, &pco);
	port_pin_set_output_level(CARD_CS_N, true);
	port_pin_set_config(CARD_MOSI, &pco);
	port_pin_set_output_level(CARD_MOSI, false);
		
	port_pin_set_config(CARD_MISO, &pci);
}