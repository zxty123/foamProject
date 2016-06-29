
#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"


void (*rf_rx_irq_handler)(void);
void (*rf_tx_irq_handler)(void);
void (*gpio0_user_irq_handler)(void);
void (*gpio_user_irq_handler)(void);
void (*timer_irq1_handler)(void);
void (*i2c_irq_handler)(void);

u8 HostCmdStatus,HostPacketStatus;
u32 hostcmdirq_w,hostcmdirq_r;
u32 irq_num;
u32 src;
#if 1
_attribute_ram_code_ void irq_handler(void)
{

	u8  src_rf = reg_rf_irq_status;
	src = reg_irq_src;
	HostCmdStatus = read_reg8(0x800021);
	HostPacketStatus = read_reg8(0x800022);
	irq_num++;
	if (HostPacketStatus & 0x06) {
		if(i2c_irq_handler)
			i2c_irq_handler();
		if(HostPacketStatus&0x04){
		    hostcmdirq_w++;
		    write_reg8(0x800022,0x04);
		}
		if(HostPacketStatus&0x02){
			hostcmdirq_r++;
			write_reg8(0x800022,0x02);
		}
		write_reg8(0x800020, 0x00);
	}
	if(IRQ_TIMER1_ENABLE && (src & FLD_IRQ_TMR1_EN)){
		if(timer_irq1_handler)
			timer_irq1_handler();
		reg_irq_src = FLD_IRQ_TMR1_EN;
	}

	// should use FLD_IRQ_GPIO_RISC2_EN for compatibility with 86xx/85xx
	if(IRQ_GPIO_ENABLE && (src & FLD_IRQ_GPIO_RISC0_EN)){
	#ifndef WIN32
		if(gpio_user_irq_handler)
			gpio_user_irq_handler();
	#endif
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
	}

#if(!APPLICATION_DONGLE)
	if(IRQ_GPIO0_ENABLE && (src & FLD_IRQ_GPIO_RISC2_EN)){
	#ifndef WIN32
		if(gpio0_user_irq_handler)
			gpio0_user_irq_handler();
	#endif
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
	}
#endif

	if((src_rf & FLD_RF_IRQ_RX)){
		if(rf_rx_irq_handler)
			rf_rx_irq_handler();
		reg_rf_irq_status = FLD_RF_IRQ_RX;
	}

	if((src_rf & FLD_RF_IRQ_TX)){
		if(rf_tx_irq_handler)
			rf_tx_irq_handler();
		reg_rf_irq_status = FLD_RF_IRQ_TX;
	}

}
#endif
