#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__
#include "../../proj/tl_common.h"



#define no_head_mode 	0
#define CAMERA 			1
#define Reserve1		2
#define Reserve2		3
#define L_ROTOR_TRIM 	4
#define R_ROTOR_TRIM 	5
#define Reserve3		6
#define FBLR_STUNT 		7
#define F_TRIM 			8
#define B_TRIM 			9
#define R_TRIM 			10
#define L_TRIM 			11
#define ONE_KEY_RETURN 	12



#define FAST_SPEED 		15
#define RIGHT_BUTTON 	16
#define LEFT_BUTTON 	17
#define PHOTO 			18
#define LIGHT_CONTROR 	19



u8 buzzer_list;
u8 b_noHeadMode;
u8 b_led;

s32 offset_key;
s32 trim_lr;
s32 trim_fb;
s32 trim_rotor;

void buzzer_play(s32 playTime);
void buzzer_play1(s32 playTime);
void buzzer_scan(void);
void key_scan(s32 adc);
void sw_scan(s32 swadc);
u32 read_button_value(void);
u32 read_sw_value(void);
#endif
