
#include "../tl_common.h"
#include "../drivers/adc.h"
#include "syshw.h"

void syshw_init(){
	dma_init();
	gpio_init();
#if (MODULE_ADC_ENABLE)
	adc_init();
#endif

}


