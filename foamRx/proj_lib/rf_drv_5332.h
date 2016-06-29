#ifndef _RF_DRV_H_
#define _RF_DRV_H_

#define DEBUG_COARSETUNE 0
#define DEBUG_DCOC 0
#define DEBUG_TP 0
#define DEBUG_TBL 0

#define RF_CHN_AUTO_CAP 	0xff00
#define RF_CHN_TABLE 		0x8000

#define FRE_OFFSET 	0
#define FRE_STEP 	5
#define MAX_RF_CHANNEL  16

#define RF_CHANNEL_MAX			16
#define RF_CHANNEL_MASK			(RF_CHANNEL_MAX - 1)

STATIC_ASSERT_POW2(RF_CHANNEL_MAX);		//  if not, may fail in rf_get_msk_chn and rf_get_sub_chn


extern unsigned char	rfhw_tx_power;
extern unsigned char	rf_chn[RF_CHANNEL_MAX*2];
extern unsigned char	cap_tp[RF_CHANNEL_MAX];

enum{
	RF_TX_MODE_NORMAL = 0,
	RF_TX_MODE_CARRIER,
	RF_TX_MODE_CONTINUE,
};

void CalibTOS32M (void);

int rf_drv_init (int wakeup_from_suspend);
void RF_resume(void);
void SetRxMode (signed char chn, unsigned short set);
void SetTxMode (signed char chn, unsigned short set);
unsigned char TpCal(signed char chn);
unsigned char TxCoarseTunning ();
unsigned char RxCoarseTunning ();
void TxPkt (void* addr);
//#define TxSendPkt	write_reg8 (0x800524, 0x08)
#define TxSendPkt	TxPkt ()


#define		RF_DCOC_DCI_SET(v)	write_reg32 (RM_DCI-3, ((v)<<24) | ((v)<<16) | ((v)<<8) | (v))
#define		RF_DCOC_DCQ_SET(v)	write_reg32 (RM_DCQ-3, ((v)<<24) | ((v)<<16) | ((v)<<8) | (v))
#define		RF_RM_DCI_VALUE		read_reg8(RM_DCI)
#define		RF_RM_DCQ_VALUE		read_reg8(RM_DCQ)

#define FR_TX_PA_MAX_POWER	0x10
#define FR_TX_PA_MIN_POWER	0x11

//#define	RF_TX_PA_POWER_LOW		WriteAnalogReg (0x9e, 0x02)
//#define	RF_TX_PA_POWER_HIGH		WriteAnalogReg (0x9e, 0xf2)
#define	RF_TX_PA_POWER_LEVEL(high)		WriteAnalogReg (0x9e, high ? 0xfd : 0x02)
#define	RF_TX_PA_POWER_LOW		rfhw_tx_power = FR_TX_PA_MIN_POWER
#define	RF_TX_PA_POWER_HIGH		rfhw_tx_power = FR_TX_PA_MAX_POWER
//#define	RF_TX_PA_POWER_LEVEL(high)		rfhw_tx_power = high ? FR_TX_PA_MAX_POWER : FR_TX_PA_MIN_POWER;
#define	RF_TX_PA_POWER_ADJUST(level)		{WriteAnalogReg (0x9e, level);}while (0)

#define	SET_RF_TX_DMA_ADR(a)	write_reg16 (0x80050c, a)

extern u8 rf_tx_mode;
#define RF_TX_MODE_CARRIER_ENABLE			do {rf_tx_mode = RF_TX_MODE_CARRIER;} while(0)
#define RF_TX_MODE_CONTINUE_ENABLE			do {rf_tx_mode = RF_TX_MODE_CONTINUE;} while(0)
#define RF_TX_MODE_NORMAL_ENABLE			do {rf_tx_mode = RF_TX_MODE_NORMAL;} while(0)

extern unsigned char reset_flag;

extern  unsigned long  pll_offset;
extern	void	(*g_rf_func_wait)(void);


#define		SET_RF_TX_DMA_ADR(a)	write_reg16 (0x80050c, a)

#define	CAL_OSC_CTRL	0x800e0d
#define CAL_OSC_ST0		0x800e12
#define	CAL_OSC_ST1		0x800e13
#define MODE_CAL32K		1
#define MODE_CAL32M		0
#define CAL_STATUS		read_reg8(CAL_OSC_ST1)
#define CAL_CAP			read_reg8(CAL_OSC_ST0)

extern u8 rf_channel;
#endif
