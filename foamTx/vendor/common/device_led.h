/*
 * device_led.h
 *
 *  Created on: Feb 11, 2014
 *      Author: xuzhen
 */

#pragma once
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


typedef struct {
	u32 gpio;
	u8  level_on;
	u8  is_on;
	u32  repeat_count;

	u32 onoff_repeat;

	u32 on_time;
	u32 off_time;

	u32 clock;

}LED_t;

void device_led_on(void);

void device_led_off(void);

void device_led_init(u32 led_pin, u32 level_on);

void device_led_process(void);

int device_led_setup(u32 on_time, u32 off_time, u32 onoff_repeat);


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
