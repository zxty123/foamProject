
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known 


#define APPLICATION_DONGLE		0			// or else APPLICATION_DEVICE

//////////////////    RF configuration //////////////
#define RF_PROTOCOL				RF_PROTO_PROPRIETARY		//  RF_PROTO_PROPRIETARY / RF_PROTO_RF4CE / RF_PROTO_ZIGBEE

///////////////////  ADC  /////////////////////////////////

///////////////////  battery  /////////////////////////////////

#define BATT_ADC_CHANNEL		0
#define BATT_FULL_VOLT			(4100)	//  mV
#define BATT_LOW_VOLT			(3700)	//  mV
#define BATT_NO_PWR_VOLT		(3400)	//  mV
#define	ADC_CHN0_ANA_INPUT		ADC_CHN_INP_ANA_7
#define ADC_CHN0_REF_SRC		ADC_REF_SRC_INTERNAL


/*
the should be the combination of the followings:
DEEPSLEEP_WAKEUP_PIN_GPIO0 to DEEPSLEEP_WAKEUP_PIN_GPIO3
DEEPSLEEP_WAKEUP_PIN_ANA01 to DEEPSLEEP_WAKEUP_PIN_ANA12
*/
/////////////////// set default   ////////////////

#include "../common/default_config.h"

/////////////////// main loop, event loop  ////////////////
enum{ 
	EV_FIRED_EVENT_MAX = 8
};

typedef enum{
	EV_SUSPEND_NOTIFY,
	EV_WAKEUP_NOTIFY,
	EV_KEY_PRESS,
#if(MOUSE_USE_RAW_DATA)
	EV_MOUSE_RAW_DATA,
#endif	
	EV_RF_PKT_RECV,
	EV_PAIRING_START,
	EV_PAIRING_STOP,
	EV_MOUSE_EVENT,
	EV_KEYBOARD_EVENT,
#if(MODULE_SOMATIC_ENABLE)	
	EV_SOMATIC_EVENT,	
#endif
	EV_EVENT_MAX,
}ev_event_e;

typedef enum{
	EV_POLL_MOUSE_EVENT,
	EV_POLL_KEYBOARD_EVENT,
#if(MODULE_SOMATIC_ENABLE)	
	EV_POLL_SOMATIC_EVENT,
#endif	
	EV_POLL_RF_RECV,
	EV_POLL_DEVICE_PKT,
	EV_POLL_RF_CHN_HOPPING,
	EV_POLL_IDLE, //  Must be the last item in ev_poll_e
	EV_POLL_MAX,
}ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

