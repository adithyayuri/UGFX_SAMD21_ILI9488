/*
 * PERIPHERALS_TEST_H_.c
 *
 * Created: 18-02-2021 08:50:06 PM
 *  Author: YURI
 */ 

#include "peripheral_test.h"

static void blink_task(void *p)
{
	(void)p;
	while (1) {
		port_pin_set_output_level(LED_0_PIN, !port_pin_get_output_level(LED_0_PIN));
		vTaskDelay(1);
	}
}

static void spi_test_task(void *p)
{
	(void)p;
	while (1) {
		test_EXT3_SPI_write();
		vTaskDelay(1000);
	}
}

static void brightness_test_task(void *p)
{
	(void)p;
	uint8_t brightness = 0;
	bool flip = false;
	while (1) {
		if (flip)
		{
			brightness--;
		}
		else
		{
			brightness++;
		}
		if (brightness == 100 || brightness == 0)
		{
			flip = !flip;
		}
		lcd_backlight_brightness(brightness);
		vTaskDelay(10);
	}
}

void add_peripheral_tests(void)
{
	//xTaskCreate( blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( spi_test_task, "spi_test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( brightness_test_task, "backlight_test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );	
}