/*
 * time.h
 *
 *  Created on: 2015-7-25
 *      Author: Administrator
 */

#ifndef TIME_H_
#define TIME_H_

#define TIME_CLOCK_MS  48000//CLOCK_TYPE_OSC/1000//32000
#define TIME_CLOCK_US  48//CLOCK_TYPE_OSC/1000000

#define    TIME_DELAY 0
#define    TIME_SERIAL 1
#define    TIME_MAGNET_UPDATA 2
#define    TIME_UART_UPDATA 3
#define    TIME_LED_UPDATA 4
#define    TIME_RF_UPDATA 5
#define    TIME_DEBUG_UPDATA 6
#define    TIME_RF_LOST_UPDATA 7
#define    TIME_STUNT_UPDATA 8
#define    TIME_AHRS_UPDATA 9
#define    TIME_SENSOR_UPDATA 10
#define    TIME_RF_SEND_UPDATA 11
#define    TIME_FLASH_UPDATA 12
#define    TIME_COMPARTOR_UPDATA 13
#define    TIME_CAMERA_PHOTO 14

#define    TIME_MOTOR_PROTECT 15

#define    TIME_IMU_RST 16
#define    TIME_PAIRING_TIMEOUT 17
#define    TIME_ALL   18

/*
enum
{
	TIME_DELAY,
    TIME_SERIAL,
    TIME_MAGNET_UPDATA,
    TIME_UART_UPDATA,
    TIME_LED_UPDATA,
    TIME_RF_UPDATA,
    TIME_DEBUG_UPDATA,
    TIME_RF_LOST_UPDATA,
	TIME_ALL
};
*/

extern u32 time[TIME_ALL];



void time_us(u32 Time);
void time_ms(u32 Time);
u8 time_checkMs(u8 Number,u32 Time);
u8 time_checkUs(u8 Number,u32 Time);

u32 time_getUs(u8 Number);


#endif /* TIME_H_ */
