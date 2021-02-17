/*
 * app_setup.h
 *
 * Created: 16-02-2021 04:08:49 PM
 *  Author: YURI
 */ 


#ifndef APP_SETUP_H_
#define APP_SETUP_H_

#include <asf.h>

// TCC Defines
/** PWM module to use */
#define CONF_PWM_MODULE      TCC2
/** PWM channel */
#define CONF_PWM_CHANNEL     0
/** PWM output */
#define CONF_PWM_OUTPUT      0
/** PWM output pin */
#define CONF_PWM_OUT_PIN     PIN_PA12E_TCC2_WO0
/** PWM output pinmux */
#define CONF_PWM_OUT_MUX     MUX_PA12E_TCC2_WO0


void configure_gclk2_out(void);
void configure_spi_EXT3(void);
void test_EXT3_SPI_write(void);
void configure_backlight_timer(void);
void lcd_backlight_brightness(uint16_t brightness);

#endif /* APP_SETUP_H_ */