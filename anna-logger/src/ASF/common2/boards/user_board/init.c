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
	
	struct port_config pc = {
		.direction	= PORT_PIN_DIR_OUTPUT_WTH_READBACK,
		.input_pull = PORT_PIN_PULL_NONE,
		.powersave	= false
	};
	//port_pin_set_output_level(LED_0_PIN, ~port_pin_get_input_level(LED_0_PIN));
	port_pin_set_config(LED_0_PIN, &pc);
	port_pin_set_output_level(LED_0_PIN, false);
}