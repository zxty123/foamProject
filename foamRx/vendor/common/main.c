
#include "../../proj/tl_common.h"

#if(!__PROJECT_PM_TEST__)

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"

#include "../../proj_lib/rf_drv.h"

extern void user_init();
extern void proc_button_ahead();


const TBLCMDSET  tbl_cpu_modules_init[] = {
	0x0060, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE,	//open all the clk,disable all the rst
	0x0061, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE,	//open all the clk,disable all the rst
	0x0062, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE,	//open all the clk,disable all the rst
	0x0063, 0x7f,		TCMD_UNDER_BOTH | TCMD_WRITE,	//open all the clk,disable all the rst
	0x0064, 0xff,		TCMD_UNDER_BOTH | TCMD_WRITE,
	0x0065, 0x02,		TCMD_UNDER_BOTH | TCMD_WRITE,	//open all the clk,disable all the rst
	0x0620, 0x01,		TCMD_UNDER_BOTH | TCMD_WRITE,

	0x0001, 0x80,		TCMD_UNDER_BOTH | TCMD_WAREG,	////32K RC need be on
	0x0005, 0x82,		TCMD_UNDER_BOTH | TCMD_WAREG,	////32K RC need be on
	0x0006, 0x00,		TCMD_UNDER_BOTH | TCMD_WAREG,	//LDO
};

int main (void) {


	LoadTblCmdSet (tbl_cpu_modules_init, sizeof (tbl_cpu_modules_init)/sizeof (TBLCMDSET));

	clock_init();

	dma_init();

	gpio_init();

	irq_init();


#if (MODULE_USB_ENABLE)
	usb_init();
#endif


	rf_drv_init(0);

    user_init ();

    irq_enable();

	while (1) {
#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
		main_loop ();
	}
}

#endif


