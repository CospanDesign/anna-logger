/**
 * \file
 *
 * \brief Debug print configuration
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#ifndef CONF_DBG_PRINT_H
#define CONF_DBG_PRINT_H

#include <board.h>

#define CONF_DBG_PRINT_SERCOM        ANNA_UART_MODULE
#define CONF_DBG_PRINT_BUFFER_SIZE   128

//NOT USING THE CRYSTAL BECAUSE THE UART CONTROLLER DOESN"T SEEM TO GET A LOCK
//#define CONF_DBG_PRINT_GCLK_SOURCE   GCLK_GENERATOR_3
#define CONF_DBG_PRINT_GCLK_SOURCE   GCLK_GENERATOR_0
#define CONF_DBG_PRINT_BAUD_RATE     115200
// This BAUD value gives 9600 baud with 48 MHz GCLK
//#define CONF_DBG_PRINT_BAUD_VALUE    1024

#define CONF_DBG_PRINT_SERCOM_MUX    ANNA_UART_SERCOM_MUX_SETTING
#define CONF_DBG_PRINT_PINMUX_PAD0   ANNA_UART_SERCOM_PINMUX_PAD0
#define CONF_DBG_PRINT_PINMUX_PAD1   ANNA_UART_SERCOM_PINMUX_PAD1
#define CONF_DBG_PRINT_PINMUX_PAD2   ANNA_UART_SERCOM_PINMUX_PAD2
#define CONF_DBG_PRINT_PINMUX_PAD3   ANNA_UART_SERCOM_PINMUX_PAD3


#endif // CONF_DBG_PRINT_H
