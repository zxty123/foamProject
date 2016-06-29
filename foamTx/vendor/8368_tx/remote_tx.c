#include "remote_tx.h"
#include "../../proj/tl_common.h"
#include "../../proj/drivers/adc.h"
#include "../../proj/drivers/i2c.h"
#include "../common/rf_frame.h"
#include "key_scan.h"

#include "../common/time.h"
extern volatile u32 dbg1, dbg2,dbg3,dbg4;
extern u32 keyTbl[13];
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////


extern void (*gpio0_user_irq_handler)(void);
extern void (*gpio_user_irq_handler)(void);
extern void (*timer_irq1_handler)(void);
extern void (*rf_rx_irq_handler)(void);
extern void (*rf_tx_irq_handler)(void);
void cust_rf_init ();
//#define CONNECT_PIN GPIO_GP17
/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
//#define LED_PIN GPIO_GP5


#define ADC_PULL_DOWN_RESISTOR	27
#define ADC_VARISTOR_VALUE		50

#define RANDOM_ID (read_reg16(0x800630)^read_reg16(0x800448))
#define RELAY_DELAY ((RANDOM_ID&7)*500)
rf_packet_t tx_packet __attribute__ ((aligned (4)));
//dma_len = mode_bitrate_data == EMI_MODE_1M? rf_pkt_emi.rf_len + 2: mode_bitrate_data == EMI_MODE_2M? rf_pkt_emi.rf_len + 1 : rf_pkt_emi.rf_len - 1;
// 1M   mode, dma_len = rf_len+2;
// 2M   mode, dma_len = rf_len+1;
// 250K mode, dma_len = rf_len-1;

u32 adc_val;
u32 sending_time,sleep_time_clock;
s32 data[4];
s32 offset[4];
//u32 control_pin[]={GPIO_GP4,GPIO_GP5,GPIO_GP7,GPIO_GP10};
u32 control_pin[]={GPIO_GP30,GPIO_GP31,GPIO_GP10,GPIO_GP9};
u8 mode_bitrate_mode;
u8 sleep_time;
u8 rev_flag,rev_dbg;
u8 pairing_flag=1;
u8 bat_lower=0;
u8 correct_flag;
u8 swlistLast;
u8 stunt_flag;
volatile u8 chn;
const u8 rf_channel_select[9][4]= {
	{22,40,60,13},
	{26,45,65,13},
	{30,50,70,13},
	{40,55,76,13},
	{24,42,62,13},
	{28,47,67,13},
	{33,52,72,13},
	{37,57,74,13},
	{13,13,13,13},
};

u32 b_rf=0;




/*************************************************
 * 功能：发送数据
 * 参数：
 ************************************************/
static inline void rf_send_data(void)
{
	int i;
	for(i =0; i < 4; i++){
	//	chn=8;
		Rf_TrxStateSet(RF_MODE_TX,rf_channel_select[chn][i]);//更换频段
		WaitUs (200);
		Rf_TxPkt((void *) &tx_packet);//发送数据
		while(!(RF_TxFinish()==2));
		Rf_TxFinishClearFlag();
	}
}
/*************************************************
 * 功能：接收中断函数
 * 参数：
 ************************************************/
u8 p0,p1,p2;
u16 txaddr,rxaddr;
void irq_rf_rx(void)
{
	//log_event (TR_T_rf_irq_rx);
	rf_packet_t *rf_packet1;

	irq_host_rx_no++;

	u8 * raw_pkt = (u8 *) (cust_rx_buff + emi_rx_wptr * RCV_PKT_BUFF_SIZE);
	if(raw_pkt[0]<15)return;
	emi_rx_wptr = (emi_rx_wptr + 1) & 1;
	reg_dma_rf_rx_addr = (u16)(u32) (cust_rx_buff + emi_rx_wptr * RCV_PKT_BUFF_SIZE); //set next buffer
	p0=raw_pkt[0];
	p1=raw_pkt[12]+13;
	p2=raw_pkt[raw_pkt[0]+3];
	if	(	raw_pkt[0] >= 15 && (
			(mode_bitrate_mode == EMI_MODE_2M) && RF_PACKET_2M_OK(raw_pkt) ||
			(mode_bitrate_mode == EMI_MODE_1M) && RF_PACKET_1M_OK(raw_pkt) ||
			(mode_bitrate_mode == EMI_MODE_250K) && RF_PACKET_250K_OK(raw_pkt)||1
			) &&
			((raw_pkt[raw_pkt[0]+3] & 0x01) == 0x00) )
	{//硬件检测
		rf_packet1=(rf_packet_t *)(raw_pkt + 8);//转换成接收数据的数据结构
		txaddr=rf_packet1->addr_tx;
		if(tx_packet.addr_tx==rf_packet1->addr_tx){//地址一致
			tx_packet.vr_lr=rf_packet1->vr_lr;
			tx_packet.vr_rotor=rf_packet1->vr_rotor;
			tx_packet.addr_rx=rf_packet1->addr_rx;
			b_rf=1;
		}
		rev_flag=0;
		rev_dbg++;
	}
	raw_pkt[0] = 1;
}
/*************************************************
 * 功能：发送中断函数
 * 参数：
 ************************************************/
void irq_rf_tx(void)
{
	irq_host_tx_no++;
}
void rf_checksum(void)
{
	tx_packet.button_2=0;

	tx_packet.button_2=tx_packet.vr_lr^tx_packet.vr_rotor^tx_packet.vr_fb^tx_packet.vr_thr^tx_packet.vr_stunt;
	tx_packet.button_2=tx_packet.button_2^tx_packet.button_1^tx_packet.trim_fb^tx_packet.trim_lr^tx_packet.trim_rotor;

}
/**************************************************************
 * 功能：读手柄的ADC值
 * 参数：pin ：响应管脚
 * 返回：adc值
 *************************************************************/
u32 read_control_value(u32 pin)
{
	u8 i;
	u32 value=0;
	adc_channel_config(GPIO_GP18);
	for(i=0;i<4;i++)
	{
		if(pin==control_pin[i])
		{
			gpio_set_output_en(pin,1);
			gpio_set_input_en(pin,0);
			gpio_write(pin,1);
			value=adc_start_with_output();
			gpio_write(pin,0);
			gpio_set_output_en(pin,0);
			gpio_set_input_en(pin,1);
			break;
		}
	}
	return value;
}
/**************************************************************
 * 功能：读按键ADC值
 * 参数：
 * 返回：按键ADC值
 *************************************************************/
u32 read_button_value(void)
{
	adc_channel_config(GPIO_GP22);
	return adc_start_with_output();
}



/**************************************************************
 * 功能：读拨动开关ADC值
 * 参数：
 * 返回：拨动开关ADC值
 *************************************************************/
u32 read_sw_value(void)
{
	adc_channel_config(GPIO_GP17);
	return adc_start_with_output();
}



/**************************************************************
 * 功能：获取遥控手柄偏置
 * 参数：id：相应手柄的管脚ID，与control_pin数组相对应
 * 返回：偏置值
 *************************************************************/
s32 get_Offset(u8 id,s32 data)
{
	s32 temp,sum=0;
	u8 s_times;
	for(s_times=0;s_times<20;s_times++)
		sum+=read_control_value(control_pin[id]);
	sum=(sum/20);
	temp=adc_avr(sum);

	return temp-data;
}
/**************************************************************
 * 功能：读相应手柄的控制位置值
 * 参数：
 * 返回：
 *************************************************************/
void control_value_processing(void)
{
	u8 i;
	for(i=0;i<4;i++){
		data[i]=read_control_value(control_pin[i]);
		data[i]=adc_avr(data[i]);
		data[i]-=offset[i];
		if(data[i]<0)
			data[i]=0;
		else if(data[i]>0xff)
			data[i]=0xff;
	}

	tx_packet.vr_thr=data[1];
	tx_packet.vr_rotor=data[0];
	tx_packet.vr_fb=data[3]^0xff;
	tx_packet.vr_lr=data[2];

}
/**************************************************************
 * 功能：重置手动偏置值
 * 参数：
 * 返回：
 *************************************************************/
void imu_offset(void)
{
	static int b_run=0;
	static int times=0;
	if(tx_packet.vr_thr<0x40)//thro
	{
		if(tx_packet.vr_fb<0x40)
		{
			if(tx_packet.vr_rotor<0x40)
			{
				if(tx_packet.vr_lr>0xc0)
				{
					times++;
					if(times<100)return;
					if(b_run==0)
					{
						buzzer_play(300);
						time_ms(200);
						buzzer_play(300);
						time_ms(200);

						//correct_flag=1;

						tx_packet.vr_stunt=0xcc;
						b_run=1;
						tx_packet.trim_fb=trim_fb=0;
						tx_packet.trim_lr=trim_lr=0;
					}
					return;
				}
			}
		}
	}
	b_run=0;
	times=0;
}
/**************************************************************
 * 功能：读取油门控制情况，若有控制，则清零进入休眠时间的计算
 * 参数：
 * 返回：
 *************************************************************/
void vr_thrCheck(void)
{
	static u32 throttleLast,throttleLastCheckTimes;
	u32 temp;
	throttleLastCheckTimes++;
	if(throttleLastCheckTimes<30)return;
	throttleLastCheckTimes=0;
	if(throttleLast>tx_packet.vr_thr)
		temp=throttleLast-tx_packet.vr_thr;
	else
		temp=tx_packet.vr_thr-throttleLast;
	if(temp>6)sleep_time=0;//b_wakeup=1;}
	throttleLast=tx_packet.vr_thr;
}
/**************************************************************
 * 功能：查询是否进入休眠
 * 参数：
 * 返回：
 *************************************************************/
void sleep_mode(void)
{
	if(clock_time_exceed(sleep_time_clock,1000000)==0)return;
	sleep_time_clock=clock_time();
	sleep_time++;
	if(sleep_time<180)return;
	while(1)
	{
		LED_OFF;
		cpu_sleep_wakeup(0, 0, 0, 0);//	do not set wakeup from pin, for simplicity and safety
    }
}
/**************************************************************
 * 功能：读遥控器ID值
 * 参数：
 * 返回：
 *************************************************************/
u16 get_id(u16 addr)
{
	u16 id;
	id=otp_read(addr)|otp_read(addr+1)<<8;
	return id;
}
/**************************************************************
 * 功能：对码
 * 参数：
 * 返回：
 *************************************************************/
u32 pair_count;
u32 count;
void pairing_code1(void)
{
	u32 led_time,pairing_time;
	u8 led_s=0;
	chn=8;
	tx_packet.addr_rx=1111;
	led_time=clock_time();
	pairing_time=clock_time();
	while(1)
	{
		if(b_rf==1)break;
		if(clock_time_exceed(pairing_time,80000)){
//			led_not(LED_PIN); //80ms亮/灭一次；
			pairing_time=clock_time();
			rf_send_data();
			Rf_TrxStateSet(RF_MODE_RX,13);//更换频段
			pair_count++;
		}
		if(clock_time_exceed(led_time,500000)){
			led_time=clock_time();
			if(led_s){
				led_s=0;
				LED_ON;
			}else{
				led_s=1;
				LED_OFF;
			}
		}

#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
	}
	LED_ON;
	buzzer_play(1000);
	chn=tx_packet.addr_tx&0x07;
	sleep_time=0;
	Rf_TrxStateSet(RF_MODE_TX,rf_channel_select[chn][0]);//更换频段
	pairing_flag=0;
}


u32 vr_adc;
void pairing_code(void)
{
	u32 adc,pairring_time,led_time;
	u8 state=0,led_s=0;
	tx_packet.addr_rx=3333;

	//tx_packet.addr_tx=get_id(0x3fc0);
	rf_checksum();
	pairring_time=clock_time();
	led_time=clock_time();

	LED_ON;
	sleep_us(500000);
	buzzer_play(500);
	sleep_us(100000);
	buzzer_play(500);

	chn=8;
	while(1)
	{
		adc=read_control_value(control_pin[1]);
		vr_adc=adc;
		adc=adc_avr(adc);
		if(state==0)
		{
			if(adc<0x10)
			{
				state=1;
			}
		}
		else
			if(state==1)
			{
				if(adc>0xf0)
				{
				state=2;
				buzzer_play(500);
				}
			}
			else
				if(state==2)
				{
					if(adc<0x20)
					{
						buzzer_play(500);
						break;
					}
				}

		if(clock_time_exceed(pairring_time,8000))
		{
			pairring_time=clock_time();
			chn=8;
			rf_send_data();
		}

		if(clock_time_exceed(led_time,500000))
		{
			led_time=clock_time();
			if(led_s)
			{
				led_s=0;
			}
			else
			{
				led_s=1;
			}
		}

		vr_thrCheck();
		sleep_mode();
	}

	LED_ON;
	tx_packet.addr_rx=7777;
	chn=tx_packet.addr_tx&0x07;
	sleep_time=0;
}



u32 bat_value;
void check_bat(void)
{
	u32 batData;
	static u32 batTimes=0;

	if(bat_lower)return;
	if(time_checkMs(TIME_BAT_UPDATA,500)==0)return;

	adc_channel_config(GPIO_GP23);
	batData=adc_start_with_output()>>2;
	if(batData<178)
		{batTimes++;}
	else
		{batTimes=0;}

	if(batTimes>6)
		{bat_lower=1;
		batTimes=0;}

	//tx_packet.trim_rotor=bat_value;

}




/*************************************************
 * 功能：数据初始化
 * 参数：
 ************************************************/
unsigned char * lib_test(void);
void user_init()
{
	u8 i;
	u32 sum=0,temp;
	otp_init_read();
	//gpio_set_func(GPIO_SWS,AS_GPIO);
	for(i=0;i<4;i++){
		gpio_set_output_en(control_pin[i],0);//设置gp7为输出
		gpio_set_input_en(control_pin[i],1);
	}
	gpio_set_output_en(LED_PIN,1);
	gpio_set_input_en(LED_PIN,0);

	gpio_set_output_en(BUZZER_PIN,1);
	gpio_set_input_en(BUZZER_PIN,0);

	tx_packet.dma_len = sizeof(rf_packet_t) - sizeof(tx_packet.dma_len);
	Rf_Init(RF_OSC_16M,RF_RATE_SELECT);
#if (RF_RATE_SELECT == RF_MODE_2M)//速率为2m模式
	Rf_AccessCodeLengthSetting(5);
	Rf_AccessCodeSetting01(0,0x9539517671);
	tx_packet.rf_len=(u8)(tx_packet.dma_len - 1);
#elif (RF_RATE_SELECT == RF_MODE_250K)//速率为250K模式
	Rf_For5330zigbee();
	reg_rf_rx_hdx=0x8c;
	mode_bitrate_mode=8;
	tx_packet.rf_len=(u8)(tx_packet.dma_len + 1);
#endif
	cust_rf_init ();
	Rf_TrxStateSet(RF_MODE_TX,5);
	rf_rx_irq_handler = irq_rf_rx ;
	write_reg8(0x80042b,0xf8);
	Rf_PowerLevelSet(RF_POWER_8dBm);
//	rf_tx_irq_handler = irq_rf_tx;
	tx_packet.addr_rx=9999;
	//tx_packet.addr_tx=get_id(ID_ADDR);
	tx_packet.addr_tx=get_id(0x3fc0);
	tx_packet.vr_rotor=0x80;
	tx_packet.vr_fb=0x80;
	tx_packet.vr_lr=0x80;
	tx_packet.vr_stunt=0xaa;
	tx_packet.button_1=0;
	rf_checksum();

	adc_init();
	adc_channel_config(GPIO_GP18);		//设置gp18为ADC模式
	for(i=0;i<4;i++){
		if(i==1){
			offset[1]=get_Offset(i,0);
			if(offset[1]>0x15)offset[1]=0x15;
		}else{
			offset[i]=get_Offset(i,0x80);
		}
	}

	pairing_code();

	temp=read_sw_value();
	temp=temp>>2;

	if(temp>240)
	{
		//慢速档
		swlistLast=2;
		tx_packet.button_1&=0xcf;
		stunt_flag=0;
	}
	else
	{	if(temp<20)
		{
			//翻滚档
			swlistLast=0;
			stunt_flag=1;
		}
		else
		{
			//高速档
			swlistLast=1;
			tx_packet.button_1|=0x30;
			stunt_flag=0;
		}
	}


}
/*************************************************
 * 功能：主循环 程序
 * 参数：
 ************************************************/
s32 adc_Value;
s32 Sw_adcValue;
void main_loop (void)
{
	dbg2++;

	adc_Value=read_button_value();
	//tx_packet.trim_lr=adc_Value>>2;

	Sw_adcValue=read_sw_value();
	//tx_packet.trim_lr=Sw_adcValue>>2;

	if(clock_time_exceed(sending_time,8000)){
		sending_time=clock_time();
		control_value_processing();
		imu_offset();
		vr_thrCheck();
		key_scan(adc_Value>>2);
		sw_scan(Sw_adcValue>>2);
		rf_checksum();
		rf_send_data();

		//adc_channel_config(GPIO_GP23);
		//tx_packet.trim_rotor=adc_start_with_output()>>2;

		//adc_channel_config(GPIO_GP17);
		//tx_packet.trim_lr=adc_start_with_output()>>2;
	}

	buzzer_scan();
	check_bat();




	//sleep_mode();
	//sleep_us(RELAY_DELAY);
	return;

}

