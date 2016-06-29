/*
 * led_rf_frame.h
 *
 *  Created on: Jan 13, 2014
 *      Author: xuzhen
 */

#ifndef _RF_FRAME_H_
#define _RF_FRAME_H_

#define RF_DATA_LENGTH     6

#define			EMI_MODE_MASK		0x0c
#define			EMI_MODE_2M			0
#define			EMI_MODE_1M			4
#define			EMI_MODE_250K		8


#define		RF_PACKET_2M_OK(p)		(p[0] == (p[12]&0x3f)+15)
#define		RF_PACKET_1M_OK(p)		(p[0] == (p[13]&0x3f)+17)
#define		RF_PACKET_250K_OK(p)	(p[0] == p[12]+13)
#define		RCV_PKT_BUFF_SIZE		48

u8 cust_rx_buff[RCV_PKT_BUFF_SIZE*2] __attribute__((aligned(4)));
u32 irq_host_rx_no, irq_host_tx_no;
u8 emi_rx_wptr;
typedef struct{

	u32 dma_len;
	u8  rf_len;
	//自定义数据
	u8 rf_seq_no;
	u16 vid;          //0,1
	u32 pid;          //2,3,4,5

	u8 data[10];

}rf_data_t;

typedef struct{

	u32 dma_len;
	u8  rf_len;

	u32 src_addr;          //0,1
	u32 dst_addr;          //2,3,4,5

	u8  frame_type;   //6
	u8  control_key;  //7
	u8  rf_seq_no;    //8
	u8  car_speed;
//	u8  reserve;      //9

//	u16  button_keep_counter; //10, 11

}rf_packet_remote_t;


typedef struct{
	u32 dma_len;
	u8  rf_len;

	u16 addr_rx;
	u16 addr_tx;
//	u16 offest;
//	u16 value;
	u8 vr_lr;
	u8 vr_rotor;
	u8 vr_fb;
	u8 vr_thr;
	u8 vr_stunt;
	u8 button_1;
	u8 button_2;
	u8 trim_fb;
	u8 trim_lr;
	u8 trim_rotor;

	//u8  data[RF_DATA_LENGTH];
	//u32 checksum;

}rf_packet_t;
#endif /* LED_RF_FRAME_H_ */
