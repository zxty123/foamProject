#ifndef _RF_COM_H_
#define _RF_COM_H_


#define  WAIT_TX_DONE               while(REG_ADDR8(0x41c)&BIT(1))
#define  TX_IS_DONE                 (!(REG_ADDR8(0x41c)& BIT(1)))

#define	 RX_BUFF_SIZE		        256
#define  MAX_RF_PACKAGE_LENGTH      80

void RF_init();

void RF_send(u8* pkt);
void RF_sendDebug(u8* pkt1);

void RF_rcv_process(void(* user_rcv)(u8*p));

#endif

