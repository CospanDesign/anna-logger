/*
 * main.h
 *
 * Created: 8/28/2014 10:43:29 AM
 *  Author: cospan
 */ 


#ifndef MAIN_H_
#define MAIN_H_


#include <board.h>

#define CONF_STDIO_USART        FTDI_HOST_MODULE

#define CONF_STDIO_GCLK_SOURCE   GCLK_GENERATOR_3

#define CONF_STDIO_BAUDRATE     115200
// This BAUD value gives 9600 baud with 48 MHz GCLK
//#define CONF_DBG_PRINT_BAUD_VALUE    1024

#define CONF_STDIO_MUX_SETTING   FTDI_HOST_SERCOM_MUX_SETTING
#define CONF_STDIO_PINMUX_PAD0   FTDI_HOST_SERCOM_PINMUX_PAD0
#define CONF_STDIO_PINMUX_PAD1   FTDI_HOST_SERCOM_PINMUX_PAD1
#define CONF_STDIO_PINMUX_PAD2   FTDI_HOST_SERCOM_PINMUX_PAD2
#define CONF_STDIO_PINMUX_PAD3   FTDI_HOST_SERCOM_PINMUX_PAD3




#endif /* MAIN_H_ */