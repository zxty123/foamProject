/*
 * cust_rf.c
 *
 *  Created on: Feb 17, 2015
 *      Author: xuzhenli
 */

#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"
#include "rf_frame.h"

//#define		RF_PACKET_CRC_OK(p)		((p[p[0]+3] & 0x51) == 0x40)



int rf_rx_response (u8 * raw_pkt)
{
//	rf_packet_emi_t *p_pkt = (rf_packet_emi_t *) (raw_pkt + 8);

	return 1;
}


void cust_rf_init (void)
{

	reg_dma_rf_rx_addr = (u16)(u32) (cust_rx_buff);
	reg_dma2_ctrl = FLD_DMA_WR_MEM | (RCV_PKT_BUFF_SIZE>>4);   // rf rx buffer enable & size
	reg_dma_chn_irq_msk = 0;

	reg_irq_mask |= FLD_IRQ_ZB_RT_EN;    //enable RF & timer1 interrupt
	reg_rf_irq_mask = FLD_RF_IRQ_RX;

}
