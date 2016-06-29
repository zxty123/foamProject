
#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"


void (*rf_rx_irq_handler)(void);
void (*rf_tx_irq_handler)(void);
void (*gpio0_user_irq_handler)(void);
void (*gpio_user_irq_handler)(void);
void (*timer_irq1_handler)(void);
void (*pwm_irq_handler)(void);


_attribute_ram_code_ void irq_handler(void)
{

	u32 src = reg_irq_src;


	if(IRQ_TIMER1_ENABLE && (src & FLD_IRQ_TMR1_EN)){
		if(timer_irq1_handler)
			timer_irq1_handler();
		reg_irq_src = FLD_IRQ_TMR1_EN;
	}

    if(src & BIT(22)){
    	//xz
    	if(pwm_irq_handler)
    		pwm_irq_handler();
    	reg_irq_src = BIT(22);
    }

    if(src & FLD_IRQ_GPIO_RISC2_EN){
    		if(gpio0_user_irq_handler)
    			gpio0_user_irq_handler();
    		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
    	}

}

