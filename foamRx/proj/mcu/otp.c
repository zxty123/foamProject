
#include "../tl_common.h"
#include "analog.h"
// only for write!
TODO("Pls refine these OTP function..");
NOTE("what's the meaning of 0x800019/0x80001a/0x800071");

static void otp_init_clock(void){
	reg_dcdc_clk = 0x1f;    // open DCDC 6.75 clk to 8M
	sleep_us(30000);
	reg_dcdc_clk = 0x13;    // set DCDC 6.75 clk to 60M
	sleep_us(1000);
	reg_otp_ctrl = FLD_OTP_OEN_PWDN | FLD_OTP_WEN_PWDN | FLD_OTP_PCEN_PWDN | FLD_OTP_CLK | FLD_OTP_OEN | FLD_OTP_FAST_CLK;
}
void otp_init_readwrite(void){
	/* open vpp 6.75V */
	rega_poweron_dcdc_6p75();
	sleep_us(1000);
	otp_init_clock();
}
void otp_init_read(void){
	otp_init_clock();
}

_attribute_ram_code_ void otp_write(u16 addr, u8 value){
	u32 i=0;

	while((analog_read(0xaf)&0x80) !=0x80){
		WaitUs(500);
		i++;
		if(i>10) break;
	}


	reg_otp_addr_para = addr;
	U8_SET(0x80001a, 0x02);
	reg_otp_byte_dat = value;
	sleep_us(100);
	reg_otp_byte_dat = value;
	U8_SET(0x80001a, 0x00);
	WaitUs (20);
}

void otp_stop_write(void)
{
	rega_powerdn_dcdc_6p75();
	WaitUs (20);
}

_attribute_ram_code_ u8 otp_read(u16 addr){
	volatile u8 value;
	addr = ((addr-4) & 0x3fff);
	reg_otp_addr_para = addr;
	U8_SET(0x80001a, 0x00);
	
	value = reg_otp_byte_dat;
	sleep_us(15);
	value = U8_GET(0x800019);
	sleep_us(15);
	return U8_GET(0x800019);
}

