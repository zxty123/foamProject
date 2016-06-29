/*
 * comparator.h
 *
 *  Created on: Jul 25, 2014
 *      Author: xuzhen
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_

typedef enum {
	COMP_ANA0,
	COMP_ANA1,
	COMP_ANA2,
	COMP_ANA3,
	COMP_ANA4,
	COMP_ANA5,
	COMP_ANA6,
	COMP_ANA7,
	//PWM0,
	//PWM1,
	//PWM2,
	//COMP_AVDD=0xd,
} COMP_CH_e;

typedef enum {
	COMP_SCAL_1_QUARTER,
	COMP_SCAL_2_QUARTER,
	COMP_SCAL_3_QUARTER,
	COMP_SCAL_4_QUARTER,
} COMP_SCALING_FACT_e;

typedef enum {
	COMP_REF_NONE,
	COMP_REF_989MV,
	COMP_REF_937MV,
	COMP_REF_730MV,
	COMP_REF_832MV,
	COMP_REF_ANA3,
	COMP_REF_887MV,
	COMP_REF_AVDDH,
} COMP_REF_SEL_e;

typedef enum {
	COMP_REF_AMP_1,
	COMP_REF_AMP_1_5,
} COMP_REF_BOOST_e;

typedef struct{
	u8 comp_ref_sel;
	u8 comp_ref_boost;
	u8 comp_ref_scale;
} COMP_VL_SET_t;

typedef enum {
	COMP_VOL_0730,
	COMP_VOL_0832,
	COMP_VOL_0887,
	COMP_VOL_0937,
	COMP_VOL_0973,
	COMP_VOL_0989,
	COMP_VOL_1095,
	COMP_VOL_1109,
	COMP_VOL_1183,
	COMP_VOL_1249,
	COMP_VOL_1319,
	COMP_VOL_1331,
	COMP_VOL_1406,
	COMP_VOL_1460,
	COMP_VOL_1484,
	COMP_VOL_1664,
	COMP_VOL_1774,
	COMP_VOL_1874,
	COMP_VOL_1978,
	COMP_VOL_2190,
	COMP_VOL_2496,
	COMP_VOL_2661,
	COMP_VOL_2811,
	COMP_VOL_2920,
	COMP_VOL_2967,
	COMP_VOL_3328,
	COMP_VOL_LAST,
} COMP_PRE_SET_VOLTAGE_e;

/**
 *
 * @param channel   selected comparator channel
 * @param config    comparator configuration setting
 * @return if the input is less than the compare value, output 1, great is output 0
 */
int comparator_compare_voltage(int channel,COMP_VL_SET_t *config);

int comparator_compare_preset_voltage(int channel, COMP_PRE_SET_VOLTAGE_e voltage_index);


u32 comparator_read(int channel);

#endif /* COMPARATOR_H_ */
