#include <asf.h>
#include "app_setup.h"
#include "gfx.h"

TaskHandle_t xHandle = NULL;
TaskHandle_t xHandleBlink = NULL;

void configure_app(void);
static void blink_task(void *p);
static void spi_test_task(void *p);

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

void configure_app(void)
{
	// PORT MUX GCLk2 output on PA16
	configure_gclk2_out();	
	// Configure EXT3 SPI
	configure_spi_EXT3();
	// Configure TCC2
	configure_backlight_timer();
}


int main (void)
{
	system_init();
	configure_app();
	xTaskCreate( blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, xHandleBlink );
	xTaskCreate( spi_test_task, "spi_test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, xHandle );
	xTaskCreate( brightness_test_task, "backlight_test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, xHandle );
	vTaskStartScheduler();
	while(1);
}
