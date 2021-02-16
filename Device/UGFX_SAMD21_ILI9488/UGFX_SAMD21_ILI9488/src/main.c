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
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "app_setup.h"

TaskHandle_t xHandle = NULL;
TaskHandle_t xHandleBlink = NULL;

void configure_app(void);
static void blink_task(void *p);

static void blink_task(void *p)
{
	(void)p;
	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
		port_pin_set_output_level(LED_0_PIN, !port_pin_get_output_level(LED_0_PIN));
		vTaskDelay(1);
	}
}

void configure_app(void)
{
	// PORT MUX GCLk2 output on PA16
	configure_gclk2_out();	
}

int main (void)
{
	system_init();
	configure_app();
	xTaskCreate( blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, xHandleBlink );
	vTaskStartScheduler();
	while(1);
}
