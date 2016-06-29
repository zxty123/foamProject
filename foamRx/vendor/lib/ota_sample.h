/*
 * ota_sample.h
 *
 *  Created on: Jul 9, 2015
 *      Author: xuzhenli
 */

#ifndef OTA_SAMPLE_H_
#define OTA_SAMPLE_H_

typedef struct{
	u32 dma_len;
	u8  rf_len;

	u32 pid;

	u16 start_address;
	u16 crc16;
	u8  pkt_id;
	u8  pkt_length;

	u8  pkt_data[64];

}rf_packet_ota_t;


#define         OTA_RF_CH           0
#define			OTA_START			0x8000
#define         OTA_FLAG_ADDR       0xf000


int ota_check_start(u32 reserved_dword);
int is_ota_mode(void);
void ota_data_set(unsigned char *p);
void ota_process();
void ota_main_proc(u8 *p);
#endif /* OTA_SAMPLE_H_ */
