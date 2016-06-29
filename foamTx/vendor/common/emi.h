/*
 * emi.h
 *
 *  Created on: Feb 14, 2014
 *      Author: xuzhen
 */

#ifndef EMI_H_
#define EMI_H_

enum {
	host_cmd_chn_l  = 0,
	host_cmd_chn_m  = 6,
	host_cmd_chn_h  = 14,
	host_cmd_mask	= 0xf0,
	//host_cmd_paring = BIT(8),
	host_cmd_carrier= BIT(4),
	host_cmd_cd		= BIT(5),
	host_cmd_rx		= BIT(6),
	host_cmd_tx		= BIT(7),
};

#define		TEST_MODE_CARRIER	(test_mode & host_cmd_carrier)
#define		TEST_MODE_CD		(test_mode & host_cmd_cd)
#define		TEST_MODE_RX		(test_mode & host_cmd_rx)
#define		TEST_MODE_TX		(test_mode & host_cmd_tx)


void emi_process(u8 cmd, u8 chn_idx, u8 test_mode_sel, u8 *tx_pkt, u8 tx_power_emi);
extern void (*pf_emi_rx_init) ( void );
extern void (*pf_emi_rx_recover) ( void );
extern void (*pf_emi_cust_init) ( void );

#endif /* EMI_H_ */
