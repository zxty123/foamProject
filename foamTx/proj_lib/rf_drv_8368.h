//extern  void  just_a_test(void);
/**********************************************************************************************************************
* 文件名称： rf.h
* 内容摘要：8368_remington
* 当前版本： V1.0
* 作    者：陈凯鑫
* 完成日期： 2015.06.25
*
* 修改记录1：
**********************************************************************************************************************/
#ifndef		TL_DRV_RF_H
#define     TL_DRV_RF_H
/**********************************************************************************************************************
 *                                            全局常量定义                                                            *
 *********************************************************************************************************************/
//#define		RF_CRYSTAL_12M_OR_16M        0x01
//#define		RF_LONG_PACKET_EN			 0x02
//#define		RF_MANUAL_AGC                0x04
//#define RF_CHN_TABLE 		0x8000
//#define RF_SET_TX_MANAUL	0x4000
enum  M_OSC_SEL {
  RF_OSC_16M = 0,  
  RF_OSC_12M = 1
};
enum  M_RF_TR_MODE {
	STX = 0,
	STR = 1,
	SRT = 2,
};
enum  M_RF_MODE {     // now only 2M mode is supported
	RF_MODE_NORDIC_2M = 0,
	RF_MODE_BLE_1M = 1,
	RF_MODE_ZIGBEE_250K = 2
};
enum  M_RF_STATE {
  RF_MODE_TX = 0,
  RF_MODE_RX = 1,
  RF_MODE_AUTO=2,
  RF_MODE_OFF = 3

};
enum M_RF_POWER{
	RF_POWER_8dBm	= 0,
	RF_POWER_7dBm,
	RF_POWER_5dBm,
	RF_POWER_4dBm,
	RF_POWER_3dBm,
	RF_POWER_2dBm,
	RF_POWER_0dBm,
	RF_POWER_OFF,
};

/**********************************************************************************************************************
 *                                             全局函数原型                                                           *
 *********************************************************************************************************************/

extern	int	 Rf_Init( enum  M_OSC_SEL  osc_sel,enum M_RF_MODE m);
extern	 void Rf_ModeSet(enum M_RF_MODE m);
extern void Rf_BaseBandReset (void);
extern   unsigned char  Rf_TrxStateSet(enum M_RF_STATE  state ,signed char channel);
extern  void Rf_TxPkt (unsigned char* addr);
extern	void Rf_PowerLevelSet(enum M_RF_POWER level);
//extern   void  Rf_SetMaxRcvLen(unsigned char len);
extern void  Rf_RxBufferSet(unsigned char *  addr, int size, unsigned char  pingpong_en);
extern unsigned char Rf_RxBufferRequest(void);
extern void  Rf_RxBufferClearFlag(unsigned char  idx);
extern unsigned char  RF_TxFinish(void);
extern void Rf_TxFinishClearFlag (void);


extern void Rf_StartStx  (unsigned char* tx_addr,unsigned int tick);
extern void Rf_StartStxToRx  ( unsigned char* tx_addr ,unsigned int tick,unsigned short timeout_us);
extern void Rf_StartSrxToTx  (unsigned char* tx_addr  ,unsigned int tick,unsigned int timeout_us);

extern void Rf_TxAccessCodeSelect (unsigned char  idx);
extern void Rf_AccessCodeSetting01 (unsigned char  idx,unsigned long long  access_code);
extern void Rf_AccessCodeSetting2345 (unsigned char  idx,unsigned char  prefix);
extern void Rf_RxAccessCodeEnable (unsigned char enable);
extern void Rf_AccessCodeLengthSetting (unsigned char length);


extern void	Rf_EmiCarrierOnlyTest(enum M_RF_POWER power_level,signed char rf_chn);
extern void	Rf_EmiCarrierDataTest(enum M_RF_POWER power_level,signed char rf_chn);

void Rf_For5330zigbee(void);
int rf_drv_init (void);
#endif
