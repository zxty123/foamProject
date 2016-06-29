/*
 * led_rf_frame.h
 *
 *  Created on: Jan 13, 2014
 *      Author: xuzhen
 */

#ifndef LED_RF_FRAME_H_
#define LED_RF_FRAME_H_

#define RF_DATA_LENGTH     6



typedef struct {

	u16 pitch            ;
	u16 roll     ;
	u16 yaw    ;
	u16 gx     ;
	u16 gy              ;
	u16 gz              ;
	u16 ax       ;
	u16 ay        ;
	u16 az       ;
	u16 mx                      ;
	u16 my                      ;
	u16 mz                      ;
	u16 baro      ;
	u16 alt       ;
	u16 vel;
	u16 thr;
	u16 fb;
	u16 lr;
	u16 rotor;
	u16 fn1;
			u16 fn2;
			u16 cmd1;
			u16 cmd2;
			u16 cmd3;
	u16 frq;
//=====================================================
	u16 g_agc;
	u16 g_p;
	u16 g_i;
	u16 g_d;

	u16 a_agc;
	u16 a_p;
	u16 a_i;
	u16 a_d;

	u16 z_angle;
	u16 z_p;
	u16 z_i;
	u16 z_d;


	u16 rc_spd1;
	u16 rc_spd2;
	u16 rc_de1;
	u16 rc_de2;

	u16 atl_agc;
	u16 atl_p;
	u16 atl_i;
	u16 atl_d;

	u16 stunt4;
	u16 stunt1;
	u16 stunt2;
	u16 stunt3;


	u16 stunt5;
	u16 stunt6;
	u16 stunt7;
	u16 stunt8;

}Flying_Control_t;



Flying_Control_t dectectData;











typedef struct{
	u32 dma_len;
	u8  rf_len;

	u32 pid;

	u8  data[RF_DATA_LENGTH];
	u32 reserved;

}rf_packet_t;

typedef struct{
	u32 dma_len;
		u8  rf_len;

		u16 addr_rx;
		u16 addr_tx;
		u8 vr_lr;
		u8 vr_rotor;
		u8 vr_fb;
		u8 vr_thr;
		u8 vr_stunt;
		u8 button_1;
		u8 button_2;
		u8 trim_fb;
		u8 trim_lr;
		u8 trim_rotor;

}rf_packet_z_t;



typedef struct{
	u32 dma_len;
		u8  rf_len;

		u16 x;
				u16 y;
				u16 z;

				u16 ix;
				u16 iy;
				u16 iz;

				u8 debug9;
				u8 debug10;

}rf_packet_debug;



#define PWM_BASE1 8
#define PWM_BASE2 4
#define PWM_BASE3 3
#define PWM_BASE4 2
#define PWM_BASE5 2
#define PWM_BASE6 1
#define PWM_BASE7 1
#define PWM_BASE8 1
#define PWM_BASE9 2
#define PWM_BASE10 3
#define PWM_BASE11 4
#define PWM_BASE12 5
#define PWM_BASE13 6
#define PWM_BASE14 7


/*
#define PWM_BASE1 7
#define PWM_BASE2 6
#define PWM_BASE3 5
#define PWM_BASE4 4
#define PWM_BASE5 4
#define PWM_BASE6 3
#define PWM_BASE7 3
#define PWM_BASE8 2
#define PWM_BASE9 2
#define PWM_BASE10 2
#define PWM_BASE11 2
#define PWM_BASE12 2
#define PWM_BASE13 3
#define PWM_BASE14 3
*/



#define PWM0  0
#define PWM1  20
#define PWM2  PWM1+PWM_BASE1
#define PWM3  PWM2+PWM_BASE1
#define PWM4  PWM3+PWM_BASE1
#define PWM5  PWM4+PWM_BASE1
#define PWM6  PWM5+PWM_BASE1
#define PWM7  PWM6+PWM_BASE2
#define PWM9  PWM8+PWM_BASE2
#define PWM8  PWM7+PWM_BASE2
#define PWM10 PWM9+PWM_BASE2
#define PWM11 PWM10+PWM_BASE3
#define PWM12 PWM11+PWM_BASE3
#define PWM13 PWM12+PWM_BASE3
#define PWM14 PWM13+PWM_BASE3
#define PWM15 PWM14+PWM_BASE4
#define PWM16 PWM15+PWM_BASE4
#define PWM17 PWM16+PWM_BASE4
#define PWM18 PWM17+PWM_BASE4
#define PWM19 PWM18+PWM_BASE5
#define PWM20 PWM19+PWM_BASE5
#define PWM21 PWM20+PWM_BASE5
#define PWM22 PWM21+PWM_BASE5
#define PWM23 PWM22+PWM_BASE5
#define PWM24 PWM23+PWM_BASE5
#define PWM25 PWM24+PWM_BASE6
#define PWM26 PWM25+PWM_BASE6
#define PWM27 PWM26+PWM_BASE6
#define PWM28 PWM27+PWM_BASE6
#define PWM29 PWM28+PWM_BASE6
#define PWM30 PWM29+PWM_BASE6
#define PWM31 PWM30+PWM_BASE6
#define PWM32 PWM31+PWM_BASE7
#define PWM33 PWM32+PWM_BASE7
#define PWM34 PWM33+PWM_BASE7
#define PWM35 PWM34+PWM_BASE7
#define PWM36 PWM35+PWM_BASE7
#define PWM37 PWM36+PWM_BASE8
#define PWM38 PWM37+PWM_BASE8
#define PWM39 PWM38+PWM_BASE8
#define PWM40 PWM39+PWM_BASE8
#define PWM41 PWM40+PWM_BASE8
#define PWM42 PWM41+PWM_BASE8
#define PWM43 PWM42+PWM_BASE9
#define PWM44 PWM43+PWM_BASE9
#define PWM45 PWM44+PWM_BASE9
#define PWM46 PWM45+PWM_BASE9
#define PWM47 PWM46+PWM_BASE9
#define PWM48 PWM47+PWM_BASE10
#define PWM49 PWM48+PWM_BASE10
#define PWM50 PWM49+PWM_BASE10
#define PWM51 PWM50+PWM_BASE10
#define PWM52 PWM51+PWM_BASE11
#define PWM53 PWM52+PWM_BASE11
#define PWM54 PWM53+PWM_BASE11
#define PWM55 PWM54+PWM_BASE11
#define PWM56 PWM55+PWM_BASE12
#define PWM57 PWM56+PWM_BASE12
#define PWM58 PWM57+PWM_BASE12
#define PWM59 PWM58+PWM_BASE13
#define PWM60 PWM59+PWM_BASE13
#define PWM61 PWM60+PWM_BASE13
#define PWM62 PWM61+PWM_BASE14
#define PWM63 PWM62+PWM_BASE14






#endif /* LED_RF_FRAME_H_ */
