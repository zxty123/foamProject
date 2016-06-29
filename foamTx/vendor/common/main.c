
#include "../../proj/tl_common.h"

#if(!__PROJECT_PM_TEST__)


extern void user_init();
extern void cpu_wakeup_init();
volatile u32 dbg1,dbg2,dbg3;

int main (void) {

	cpu_wakeup_init();
	//usb_dp_pullup_en (1);     //mouse should disable dp pull-up

	clock_init();


	dma_init();

	gpio_init();

	irq_init();

#if (MODULE_USB_ENABLE)
	usb_init();
#endif

//	rf_drv_init(0);

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


