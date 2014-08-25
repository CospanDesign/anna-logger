/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <port.h>




uint32_t main_counter;
char main_string[] = "Main task iteration: 0x00000000\r\n";

static void main_task(void *params)
{
	do {

		dbg_print_str("Main task loop executing\r\n");
		dbg_sprint_hexint(&main_string[23], main_counter++);
		dbg_print_str(main_string);
				
		port_pin_toggle_output_level(LED_0_PIN);
		vTaskDelay(1000 / portTICK_RATE_MS);
	} while(1);
}



int main (void)
{
	system_init();
	dbg_init();
	

	
	
	xTaskCreate(&main_task,
		(const signed char *)"Main task",
		configMINIMAL_STACK_SIZE + 100,
		NULL,
		tskIDLE_PRIORITY + 2,
		NULL);
	vTaskStartScheduler();
	/*
	do {
		
	} while (1);
	*/
	// Insert application code here, after the board has been initialized.
}
