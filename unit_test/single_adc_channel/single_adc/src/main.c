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
#include <main.h>
#include <ft2248.h>

static void initialize_stdio(void);

//! Structure for UART module connected to EDBG (used for unit test output)
struct usart_module ftdi_uart_module;

static void initialize_stdio(void){
	
	struct usart_config usart_conf;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;
	//usart_conf.generator_source = CONF_STDIO_GCLK_SOURCE;

	stdio_serial_init(&ftdi_uart_module, CONF_STDIO_USART, &usart_conf);
	usart_enable(&ftdi_uart_module);	
}


int main (void)
{
	system_init();
	delay_init();
	
	initialize_stdio();
	
	printf("Hello world!\r\n");
	
	sensor_t * adc = new_ft2248(0);
	
	
	
	get_all_sensor_data(adc, NULL);
	get_sensor_data(adc, 0, NULL);
	update_all_sensor_data(adc);
	update_sensor_data(adc, 0);
	set_all_sensor_data(adc, NULL);
	set_sensor_data(adc, 0, NULL);
	
	//get a reference to the local sensor structure
	//Type is the LTC2448 Sensor Type and the address is 0
	//sensor_type *s = new_sensor_struct(LTC2448_SENSOR, 0);

	// Insert application code here, after the board has been initialized.
	while (1){
		port_pin_toggle_output_level(LED_0_PIN);
		delay_cycles_ms(200);
			
	}
}
