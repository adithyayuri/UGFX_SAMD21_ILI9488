/*
 * PERIPHERALS_TEST_H_.h
 *
 * Created: 18-02-2021 08:50:29 PM
 *  Author: YURI
 */ 


#ifndef PERIPHERALS_TEST_H_
#define PERIPHERALS_TEST_H_

#include <asf.h>
#include "app_setup.h"

static void blink_task(void *p);
static void spi_test_task(void *p);
static void brightness_test_task(void *p);
void add_peripheral_tests(void);

#endif /* PERIPHERALS_TEST_H_ */