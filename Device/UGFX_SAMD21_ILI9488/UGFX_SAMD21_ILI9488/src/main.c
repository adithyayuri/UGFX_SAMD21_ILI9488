#include <asf.h>
#include "app_setup.h"
#include "gfx.h"
//#include "ugfx/src/gfx_mk.c"
#include "peripheral_test.h"

void configure_app(void);

static void GFX_init_task(void* pvParameters)
{
	(void)pvParameters;
	
	gfxInit();
	
	while(1);
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
    xTaskCreate(GFX_init_task, "Initialization", 512, 0, tskIDLE_PRIORITY, 0);
		
	vTaskStartScheduler();
	while(1);
}
