/*
 * time.h
 *
 *  Created on: 2015-7-25
 *      Author: Administrator
 */

#ifndef TIME_H_
#define TIME_H_


#define TIME_CLOCK_MS  32000//CLOCK_TYPE_OSC/1000//32000
#define TIME_CLOCK_US  32//CLOCK_TYPE_OSC/1000000

#define    TIME_DELAY 0
#define    TIME_SERIAL 1
#define    TIME_BUZZER_UPDATA 2
#define    TIME_SLEEP_UPDATA 3
#define    TIME_LED_UPDATA 4
#define    TIME_RF_UPDATA 5
#define    TIME_DEBUG_UPDATA 6
#define    TIME_RF_LOST_UPDATA 7
#define    TIME_STUNT_UPDATA 8
#define    TIME_BAT_UPDATA 9
#define    TIME_CORRECT_UPDATA 10
#define    TIME_ALL   11

/*
enum
{
	TIME_DELAY,
    TIME_SERIAL,
    TIME_BUZZER_UPDATA,
    TIME_SLEEP_UPDATA,
    TIME_LED_UPDATA,
	TIME_ALL
};
*/

void time_us(u32 Time);
void time_ms(u32 Time);
u8 time_checkMs(u8 Number,u32 Time);
u32 time_getUs(u8 Number);
extern u32 TimeArray[TIME_ALL];

#endif /* TIME_H_ */
