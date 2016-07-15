#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known 


/////////////////// Clock  /////////////////////////////////
#if 1
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_OSC	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	32000000
#endif
/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE	1
#define MODULE_RF_ENABLE		0
#define IRQ_GPIO0_ENABLE        1
#define USING_HW_I2C        1
#define	IRQ_TIMER1_ENABLE  	1

///////////////////  interrupt  //////////////////////////////

///////////////////  GPIO  /////////////////////////////////
//  only need to define those are not default
//  all gpios are default to be output disabled, input disabled, output to 0, output strength to 1

//#define	GPIO0_INPUT_ENABLE		1	// use as battery detection input, can not enable

///////////////////  ADC  /////////////////////////////////

///////////////////  battery  /////////////////////////////////


/*
 the should be the combination of the followings:
 DEEPSLEEP_WAKEUP_PIN_GPIO0 to DEEPSLEEP_WAKEUP_PIN_GPIO3
 DEEPSLEEP_WAKEUP_PIN_ANA01 to DEEPSLEEP_WAKEUP_PIN_ANA12
 */

///////////////////  USB   /////////////////////////////////

////////////////  ethernet /////////////////	

///////////////////  RF4CE   /////////////////////////////////

/////////////////// set default   ////////////////

#include "../common/default_config.h"


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

