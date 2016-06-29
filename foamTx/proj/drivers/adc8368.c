
#include "../tl_common.h"

#if (CHIP_TYPE==CHIP_TYPE_8368)
//#if (MCU_CORE_TYPE == MCU_CORE_8368)
#include "adc8368.h"
/**
 *
 */
void adc_init()
{
	analog_write(0x06,analog_read(0x06)&0xfe);
	//AD clk = src_clk * step/mod = 32M(OSC) * 4/32 = 4M    T = 0.25us
	//adc conver time = sample_time + bit_store_time = (cycle + res_len) * T =
	reg_adc_step_l = 4;  //step
	BM_SET_MASK_FLD(reg_adc_mod, FLD_ADC_MOD, CLK_FHS_MZ);   //set adc clock at 4M
	BM_SET(reg_adc_mod, FLD_ADC_CLK_EN);   //enable ADC clock

	BM_SET(reg_adc_ref, FLD_ADC_REFERENCE);  //vddH reference

	SET_FLD_V(reg_adc_samp_clk_res, FLD_ADC_CHN0_SAMP_CYCLE, 0, FLD_ADC_CHN0_SAMP_RESOL, 3); //10 bit 3cycle  13*0.25us = 3.25us

}

void adc_batInt(void)
{
	 gpio_set_func(GPIO_GP23,AS_GPIO);

	    gpio_set_input_en(GPIO_GP23,0);

	    gpio_set_output_en(GPIO_GP23,1);

	    gpio_write(GPIO_GP23,1);



	    REG_ADDR8(0x69) = 4;  //step

	    REG_ADDR8(0x6a) = CLK_FHS_MZ;  //mod

	    REG_ADDR8(0x6b) = 0x80;  //adc clk en



	    REG_ADDR8(0x33) = 0x00;  //no auto

	    REG_ADDR8(0x2b) = 0x00;  //ref:Vbg 1.224V

	    REG_ADDR8(0x2c) = 0x06;  //GP23  0x04->>0x06  0x07

	    REG_ADDR8(0x3c) = 0x18;  //10 bit 3cycle  13*0.25us = 3.25us

	    analog_write(0x02,analog_read(0x02)|0x08);
}


void adc_channel_config(u32 adc_gpio_input )
{
	const u32 adcmapping[]={GPIO_GP17, GPIO_GP18, GPIO_GP22, GPIO_GP23,};
	u32 i;

	gpio_set_input_en(adc_gpio_input,0);

	for(i=0; i<sizeof(adcmapping)/sizeof(u32);i++){
		if(adc_gpio_input == adcmapping[i] )
			break;
	}
	if(i!=sizeof(adcmapping)/sizeof(u32))

	BM_SET_MASK_FLD(reg_adc_chn_m_sel, FLD_ADC_INPUT_CH_SEL, (i+1));
}

u32 adc_start_with_output( )
{
	adc_start();
	sleep_us(5);
	return (reg_adc_dat_val&0x3ff);
}



#endif
