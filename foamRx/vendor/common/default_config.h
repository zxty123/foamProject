
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define CHIP_TYPE				CHIP_TYPE_8568		// 8866-24, 8566-32
#define APPLICATION_DONGLE		0			// or else APPLICATION_DEVICE
#define	FLOW_NO_OS				1


#include "../../proj/mcu/config.h"

/////////////////// MODULE /////////////////////////////////
#ifndef MODULE_WATCHDOG_ENABLE
#define MODULE_WATCHDOG_ENABLE	0
#endif

#ifndef MODULE_RF_ENABLE
#define MODULE_RF_ENABLE		1
#endif
#ifndef MODULE_PM_ENABLE
#define MODULE_PM_ENABLE		0			//  power management
#endif
#ifndef MODULE_ETH_ENABLE
#define MODULE_ETH_ENABLE		0
#endif

///////////////////  interrupt  //////////////////////////////
#ifndef IRQ_TIMER1_ENABLE
#define	IRQ_TIMER1_ENABLE  	0
#endif
#ifndef IRQ_USB_PWDN_ENABLE
#define	IRQ_USB_PWDN_ENABLE  	0
#endif
#ifndef IRQ_GPIO_ENABLE
#define IRQ_GPIO_ENABLE 		0
#endif
#ifndef IRQ_GPIO0_ENABLE
#define IRQ_GPIO0_ENABLE 		0
#endif

#if(MODULE_ETH_ENABLE)
#define	IRQ_MAC_ENABLE		0
#endif

#ifndef IRQ_RF_RTX_ENABLE
#define IRQ_RF_RTX_ENABLE		MODULE_RF_ENABLE
#endif
//////////////////    RF configuration //////////////
#define RF_PKT_BUFF_LEN			256			// telink proprietary protocal

#define RF_RX_PKT_BUFF_COUNT	4		//  pingpong mode, use 2 buffers
#define RF_RX_USE_DMA_IRQ		0		//  pingpong mode, use 2 buffers
#define RF_TX_USE_DMA_IRQ		0		//  pingpong mode, use 2 buffers


////////////////  ethernet /////////////////
#define	ETH_RX_PKT_BUFF_LEN			512			//  including the dam_length,  that is the max ip packet size is ETH_RX_PKT_BUFF_SIZE - 4
#define	ETH_RX_PKT_BUFF_COUNT		4 			//
#define ETH_RX_USE_DMA_IRQ			1			//  pingpong mode, use 2 buffers
#define ETH_TX_USE_DMA_IRQ			0			//  pingpong mode, use 2 buffers



#ifndef WATCHDOG_INIT_TIMEOUT
#define WATCHDOG_INIT_TIMEOUT		200		//  in ms
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

