/*
 * app_setup.h
 *
 * Created: 16-02-2021 04:08:49 PM
 *  Author: YURI
 */ 


#ifndef APP_SETUP_H_
#define APP_SETUP_H_

#include <asf.h>

void configure_gclk2_out(void);

void configure_spi_EXT3(void);
void test_EXT3_SPI_write(void);



#endif /* APP_SETUP_H_ */