/*
 * uart.h
 *
 *  Created on: Nov 11, 2014
 *      Author: xuzhen
 */

#ifndef UART_H_
#define UART_H_

#define reg_uart_tx_data	REG_ADDR8(0x40)
#define reg_uart_rx_data	REG_ADDR8(0x41)
#define reg_uart_bsp_cfg	REG_ADDR8(0x42)           //baudrate = uart_sclk/(uart_baudrate_cfg+1)/2
#define reg_uart_status		REG_ADDR8(0x43)           //TX, RX both have 4 bytes FIFO, this status bytes reflect the FIFO status
#define reg_uart_clk_div	REG_ADDR8(0x44)           //uart_sclk = sclk/(uart_clk_div+1)/2

typedef enum{
	FLD_UART_RX_FIFO_FULL 		= 	BIT(3),
	FLD_UART_TX_FIFO_FULL 		= 	BIT(2),
	FLD_UART_RX_FIFO_N_EMPTY 	= 	BIT(1),
	FLD_UART_TX_FIFO_N_EMPTY 	= 	BIT(0),
} UART_FIFO_STATUS_E ;

typedef enum {
	UART_TX = BIT(0),
	UART_RX = BIT(1),
} UART_MODE_E;

typedef  s16  int16_t;

void UART_Init(u32 baudrate, u32 uart_mode);

void UART_PutChar(u32 c);

void UART_SendData(const u8 * data_addr, u32 data_len);

u32 UART_ReceiveData(u8* rcv_buff);

u32 UART_Status(void);

void uart_string(char * c);

void uart_8bit(u8 c);

void uart_16bit(u16 c);

void uart_32bit(u32 c);



#endif /* UART_H_ */
