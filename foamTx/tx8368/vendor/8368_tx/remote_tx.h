#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0
//#define	__DEBUG_PRINT__			0
//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880C
// If ID_PRODUCT left undefined, it will default to be combination of ID_PRODUCT_BASE and the USB config USB_SPEAKER_ENABLE/USB_MIC_ENABLE...
// #define ID_PRODUCT			0x8869

#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"2.4G Wireless Light"
#define STRING_SERIAL			L"TLSR8266"

#define CHIP_TYPE				CHIP_TYPE_8368		// 8866-24, 8566-32
//#define CHIP_TYPE				CHIP_TYPE_8366		// 8866-24, 8566-32

#define ID_ADDR					0x3fc4

//Interrupt configuration
#define IRQ_TIMER1_ENABLE      0         //Timer 1
#define IRQ_GPIO_ENABLE        0         //GPIO
#define IRQ_RF_RTX_ENABLE      1         //RF
#define MODULE_ETH_ENABLE      0         //MAC


#define RF_MODE_2M			0
#define RF_MODE_1M			1
#define RF_MODE_250K		2
#define RF_RATE_SELECT		RF_MODE_250K
///////////////////  Hardware  //////////////////////////////
/////////////////// Clock  /////////////////////////////////
#if 1
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	32000000
#else
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PAD	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	12000000
#endif
/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE	0

#define     TX_DONE               while(REG_ADDR8(0x41c)&BIT(1))

//#define PIN_I2C_SCL			GPIO_GP22
//#define PIN_I2C_SDA			GPIO_GP23

#define LED_PIN GPIO_GP5
#define LED_ON	gpio_write(LED_PIN,1)
#define LED_OFF gpio_write(LED_PIN,0)

#define LED_G		GPIO_GP22
#define LED_ON_G	gpio_write(LED_G,1)
#define LED_0FF_G	gpio_write(LED_G,0)

//#define BUZZER_PIN			GPIO_SWS//GPIO_GP9
#define BUZZER_PIN			GPIO_GP24

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
