/*
 * time.c
 *
 *  Created on: 2015-7-25
 *      Author: Administrator
 */

#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"


#include "time.h"

u32 time[TIME_ALL];


void time_ms(u32 Time)
{
	u32 newTime;
	u32 updataTime;
		updataTime= clock_time();
		while(1)
		{
			 wd_clear();
			 newTime = clock_time() - updataTime;

			 if(newTime>=(TIME_CLOCK_MS*Time))break;

		}
}

void time_us(u32 Time)
{
	u32 newTime;
	u32 updataTime;
	updataTime= clock_time();
	while(1)
	{
		 wd_clear();
		 newTime = clock_time() - updataTime;

		 if(newTime>=(TIME_CLOCK_US*Time))break;

	}

}

u8 time_checkMs(u8 Number,u32 Time)
{
     u32 newTime;
     wd_clear();
	 newTime = clock_time() - time[Number];

	 if(newTime<(TIME_CLOCK_MS*Time))return 0;
	 time[Number]=clock_time();
     return 1;

}

u8 time_checkUs(u8 Number,u32 Time)
{
     u32 newTime;
     wd_clear();
	 newTime = clock_time() - time[Number];

	 if(newTime<(TIME_CLOCK_US*Time))return 0;
	 time[Number]=clock_time();
     return 1;

}


u32 time_getUs(u8 Number)
{
	u32 NewTime;
	wd_clear();
	NewTime = clock_time()-time[Number];

	time[Number] = clock_time();

	  return NewTime/TIME_CLOCK_US;

}


