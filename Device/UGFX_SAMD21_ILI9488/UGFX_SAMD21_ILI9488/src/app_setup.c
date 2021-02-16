/*
 * app_setup.c
 *
 * Created: 16-02-2021 04:09:19 PM
 *  Author: YURI
 */ 

#include "app_setup.h"

void configure_gclk2_out(void)
{
	// PA16 as GCLK[2] output
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
	pin_conf.mux_position = MUX_PA16H_GCLK_IO2;
	system_pinmux_pin_set_config(PIN_PA16H_GCLK_IO2, &pin_conf);
	
	/* 
	GCLK[2] Freq	= 48 MHz
	Div				= 24
	Output freq		= 2 MHz
	*/
}