/*
 * app_setup.c
 *
 * Created: 16-02-2021 04:09:19 PM
 *  Author: YURI
 */ 

#include "app_setup.h"

// SPI Master_Instance
struct spi_module spi_master_instance;
// TCC Instance
struct tcc_module tcc_instance;

// Test Variables from here
#define BUF_LENGTH 20
static uint8_t wr_buffer[BUF_LENGTH] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13
};
static uint8_t rd_buffer[BUF_LENGTH];
volatile bool transrev_complete_spi_master = false;
// Test Variables till here

static void callback_spi_master( struct spi_module *const module)
{
	transrev_complete_spi_master = true;
}

void configure_spi_EXT3(void)
{
	struct spi_config config_spi_master;

	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mode_specific.master.baudrate = 4000000;
	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;
	// Configure EXT3 SERCOM SPI as MASTER
	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);
	// Enable EXT3 SPI
	spi_enable(&spi_master_instance);
}

void test_EXT3_SPI_write(void)
{
	// Set SPI CS Pin low
	port_pin_set_output_level(CONF_MASTER_SS_PIN, false);
	
	// Set DCX Pin low
	port_pin_set_output_level(LCD_DCX_PIN, true);
	
	// Select SPI Slave device
	spi_write_buffer_wait(&spi_master_instance, wr_buffer, BUF_LENGTH);
	
	// Set DCX Pin low
	port_pin_set_output_level(LCD_DCX_PIN, false);
	
	// Set SPI CS Pin high
	port_pin_set_output_level(CONF_MASTER_SS_PIN, true);	
}

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

void configure_backlight_timer(void)
{
	struct tcc_config config_tcc;
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV4;
	config_tcc.counter.period = 1000;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.compare.match[CONF_PWM_CHANNEL] = 1;
	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT]        = CONF_PWM_OUT_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT]    = CONF_PWM_OUT_MUX;

	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
}

void lcd_backlight_brightness(uint16_t brightness)
{
	tcc_set_compare_value(&tcc_instance, CONF_PWM_CHANNEL, brightness*10);
}

void configure_gpio_pins(void)
{
	// Structs for pin configuration
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
	// Setup SPI LCD CS Pin
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(CONF_MASTER_SS_PIN, &pin_conf);
	// Set SPI CS Pin high (default value)
	port_pin_set_output_level(CONF_MASTER_SS_PIN, true);

	// Setup LCD Data/Command select Pin 
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LCD_DCX_PIN, &pin_conf);
	// Set DCX Pin low (default value)
	port_pin_set_output_level(LCD_DCX_PIN, false);	

	// Setup RESET Pin
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LCD_RESET_PIN, &pin_conf);
	// Set RESET Pin high (default value)
	port_pin_set_output_level(LCD_RESET_PIN, true);
}