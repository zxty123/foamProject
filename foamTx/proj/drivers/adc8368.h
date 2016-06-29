
#pragma once
#if (MCU_CORE_TYPE == MCU_CORE_8368)
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#include "../tl_common.h"

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

static inline void adc_clk_disable()
{
	BM_CLR(reg_adc_mod, FLD_ADC_CLK_EN);
}
static inline void adc_clk_enable()
{
	BM_SET(reg_adc_mod, FLD_ADC_CLK_EN);
}

static inline void adc_start(void)
{
	BM_SET(reg_adc_run,FLD_ADC_RUN);
}

static inline u32 adc_value_get()
{
	return (reg_adc_dat_val&0x3ff);
}

static inline u32 adc_status_busy()
{
	return( (reg_adc_dat_val&BIT(15))? 1:0);
}

void adc_init();

void adc_channel_config(unsigned int adc_gpio_input );

unsigned int adc_start_with_output( );


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif
