
#pragma once

#if (MCU_CORE_TYPE == MCU_CORE_8266)
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

enum{
	ADC_DONE_SIGNAL_COUNTER,
	ADC_DONE_SIGNAL_RISING,
	ADC_DONE_SIGNAL_FALLING,
};

enum{
	ADC_AUDIO_MODE_NONE,
	ADC_AUDIO_MODE_MONO,
	ADC_AUDIO_MODE_STEREO,
};

enum{
	ADC_SAMPLING_SYCLE_3 = 0,
	ADC_SAMPLING_SYCLE_6 = 1,
	ADC_SAMPLING_SYCLE_9 = 2,
	ADC_SAMPLING_SYCLE_12 = 3,
	ADC_SAMPLING_SYCLE_18 = 4,
	ADC_SAMPLING_SYCLE_24 = 5,
	ADC_SAMPLING_SYCLE_48 = 6,
	ADC_SAMPLING_SYCLE_144 = 7,
};

enum{
	ADC_SAMPLING_RES_7BIT = 0,
	ADC_SAMPLING_RES_9BIT = 1,
	ADC_SAMPLING_RES_10BIT = 2,
	ADC_SAMPLING_RES_11BIT = 3,
	ADC_SAMPLING_RES_12BIT = 4,
	ADC_SAMPLING_RES_13BIT = 5,
	//ADC_SAMPLING_RES_14BIT = 6,
	ADC_SAMPLING_RES_14BIT = 7,

};

enum{
	FLD_ADC_VREF_1P3V = 0,
	ADC_REF_SRC_AVDD  = 1,
};

extern void adc_clk_en(int);
extern void adc_set_chn0_sampling(int, int);
extern void adc_set_ch12_sampling(int, int);
extern void adc_set_clk_freq(unsigned char);

#define ADC_BATTERY_RESOLUTION	ADC_SAMPLING_RES_8BIT

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif
