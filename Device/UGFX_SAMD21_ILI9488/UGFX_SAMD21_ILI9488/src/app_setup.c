/*
 * app_setup.c
 *
 * Created: 16-02-2021 04:09:19 PM
 *  Author: YURI
 */ 

#include "app_setup.h"

// SPI Master_Instance
struct spi_module spi_master_instance;
// SPI Slave_Instance
struct spi_slave_inst slave_disp;
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
	struct spi_slave_inst_config slave_dev_config;

	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = CONF_MASTER_SS_PIN;
	spi_attach_slave(&slave_disp, &slave_dev_config);

	spi_get_config_defaults(&config_spi_master);
	//config_spi_master.mode_specific.master.baudrate = 100000;
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

void configure_spi_master_callbacks(void)
{
	spi_register_callback(&spi_master_instance, callback_spi_master, SPI_CALLBACK_BUFFER_TRANSCEIVED);
	spi_enable_callback(&spi_master_instance, SPI_CALLBACK_BUFFER_TRANSCEIVED);
}

void test_EXT3_SPI_write(void)
{
	// Configure callbacks
	configure_spi_master_callbacks();
	// Select SPI Slave device
	//spi_select_slave(&spi_master_instance, &slave_disp, true);
	// Start transceive job
	spi_write_buffer_job(&spi_master_instance, wr_buffer, BUF_LENGTH);
	//spi_transceive_buffer_job(&spi_master_instance, wr_buffer,rd_buffer,BUF_LENGTH);
	//while (!transrev_complete_spi_master) {	}
	//transrev_complete_spi_master = false;
	//spi_select_slave(&spi_master_instance, &slave_disp, false);
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