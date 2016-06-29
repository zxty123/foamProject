#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"
#include "../example/rf_frame.h"
#include "rf_com.h"
//#include "ota_sample.h"
#include "../lib/ota_sample.h"
unsigned char  		pkt_rcv_buff[RX_BUFF_SIZE*2] __attribute__((aligned(4)));  //Pingpong RX buffer.


rf_packet_t	pkt_rf __attribute__ ((aligned (4))) ;

extern void user_rcv_proc();


int g_rf_chn=0;

void RF_init()
{

	pkt_rf.dma_len = sizeof(rf_packet_t) - sizeof(pkt_rf.dma_len);//
	pkt_rf.rf_len = (u8)(pkt_rf.dma_len + 1);

	//set receiving buffer
	reg_dma2_addr = (unsigned short)pkt_rcv_buff;

	reg_rf_rx_chn_dc = MAX_RF_PACKAGE_LENGTH; //sizeof(rf_packet_led_remote_t);
	assert(MAX_RF_PACKAGE_LENGTH<=(RX_BUFF_SIZE - 16));

	reg_dma2_ctrl = FLD_DMA_WR_MEM | FLD_DMA_PINGPONG_EN | (RX_BUFF_SIZE>>4);   // rf rx buffer enable & size


}


void RF_send(u8* pkt)
{
	SetTxMode (g_rf_chn, RF_CHN_TABLE);	// set tx again

	reg_rf_tx_mode1 = reg_rf_tx_mode1&(~0x0c);

	TxPkt (pkt);

	while(!TX_IS_DONE);
}


void RF_sendDebug(u8* pkt1)
{
	SetTxMode (22, RF_CHN_TABLE);	// set tx again

	reg_rf_tx_mode1 = reg_rf_tx_mode1&(~0x0c);

	TxPkt (pkt1);

	//while(!TX_IS_DONE);
}




u32 RF_RX_timing;
void RF_rcv_process(void(* user_rcv)(u8*p))
{

	u8  src_rf = reg_rf_rx_status;
	u32 rcv_data_valid=0;
	unsigned char *p=0;

	if(src_rf & FLD_RF_RX_INTR)
	{
		reg_rf_rx_status = FLD_RF_RX_INTR;
		{

			if(reg_dma_rx_rdy0 & FLD_DMA_RF_RX)
			{
				p = (unsigned char *)(pkt_rcv_buff );
				reg_dma_rx_rdy0 = FLD_DMA_RF_RX;
			}
			else if(reg_dma_rx_rdy1 & FLD_DMA_RF_RX)
			{
				p = (unsigned char *)(pkt_rcv_buff + RX_BUFF_SIZE );
				reg_dma_rx_rdy1 = FLD_DMA_RF_RX;
			}
			RF_RX_timing = clock_time();
			if(p==0)
				return;


		    if ((p[0]>=15) && (p[0]==p[12]+13) && (p[p[0]+3]== 0x10) )
		    {
		    	p[0] = 1;
		    	rcv_data_valid = 1;
			}

		}

	}

	if(rcv_data_valid)	user_rcv(p);


}
