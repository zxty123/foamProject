/*
 * div_mod.c
 *
 *  Created on: Sep 14, 2015
 *      Author: xuzhenli
 */


#define _attribute_ram_code_  	__attribute__((section(".ram_code")))

#define 	UDIV		0
#define 	SDIV		1
#define 	UMOD		0
#define 	SMOD		1

#define REG_BASE_ADDR			0x800000
#define REG_ADDR8(a)			(*(volatile unsigned char*) (REG_BASE_ADDR + a))
#define REG_ADDR16(a)			(*(volatile unsigned short*)(REG_BASE_ADDR + a))
#define REG_ADDR32(a)			(*(volatile unsigned int*)(REG_BASE_ADDR + a))


#define reg_div_ctl			            REG_ADDR8(0x660)
                                        // Write it to star DIV, 1 signed division 0 unsigned division.
                                        // reading for status check , 1 busy, 0 idle/done
#define reg_dividend_quotient 			REG_ADDR32(0x664)
                                        //dividend for writing and quotient for reading
#define reg_divisor_remain		        REG_ADDR32(0x668)
                                        //divisor for writing and remain for reading


_attribute_ram_code_ int __divsi3(int dividend,int divisor )
{

	reg_dividend_quotient = dividend;
	reg_divisor_remain = divisor;
	reg_div_ctl = SDIV;
	while(reg_div_ctl);

	return reg_dividend_quotient;

}
