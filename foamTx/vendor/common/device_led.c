/*
 * device_led.c
 *
 *  Created on: Feb 11, 2014
 *      Author: xuzhen
 */

#include "../../proj/tl_common.h"
#include "device_led.h"



static LED_t led;

void device_led_on( )
{
	led.is_on = 1;
	led.clock = clock_time();
	gpio_write(led.gpio, led.level_on? 1:0);
}

void device_led_off( )
{
	led.is_on = 0;
	led.clock = clock_time();
	gpio_write(led.gpio, led.level_on? 0:1);
}


void device_led_init(u32 led_pin, u32 level_on)
{
	led.gpio = led_pin;
	led.level_on = level_on;
}

void device_led_process( )
{

	if(led.repeat_count ==0)
		return;  //led flash finished

	if(led.is_on && led.on_time ){
		if(clock_time_exceed(led.clock,led.on_time)){
			device_led_off();
			led.repeat_count--;
		}
	}
	else if((led.is_on==0) && led.off_time){
		if(clock_time_exceed(led.clock,led.off_time))
			device_led_on();
	}
}

int device_led_setup(u32 on_time, u32 off_time, u32 onoff_repeat)
{
	led.on_time = on_time;
	led.off_time = off_time;
	led.onoff_repeat = onoff_repeat;
	led.repeat_count = onoff_repeat;
	device_led_on( );
	return 1;
}
