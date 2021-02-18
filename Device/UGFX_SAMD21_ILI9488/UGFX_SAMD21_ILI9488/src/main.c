#include <asf.h>
#include "app_setup.h"
#include "gfx.h"
#include "peripheral_test.h"

void configure_app(void);

static void taskInit(void* pvParameters)
{
	(void)pvParameters;
	
	gfxInit();
}

void configure_app(void)
{
	// PORT MUX GCLk2 output on PA16
	configure_gclk2_out();	
	// Configure EXT3 SPI
	configure_spi_EXT3();
	// Configure TCC2
	configure_backlight_timer();
	// Configure GPIO pins
	configure_gpio_pins();
}

int main (void)
{
	system_init();
	configure_app();
	add_peripheral_tests();
    
	// Create the initialization task
    xTaskCreate(taskInit, "Initialization", 512, 0, tskIDLE_PRIORITY, 0);
		
	vTaskStartScheduler();
	while(1);
}
