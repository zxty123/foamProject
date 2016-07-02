#include "../../proj/tl_common.h"
#include "../../proj/drivers/adc.h"
#include "key_scan.h"
#include "../common/time.h"
#include "../common/rf_frame.h"

//#define LED_PIN		GPIO_GP5
//#define LED_PIN GPIO_GP22
extern rf_packet_t tx_packet;

//const u32 keyTbl[15]={
//0x00,0x47,0x91,0xdb,0x125,0x16e,0x1b3,0x1ff,0x243,0x28a,0x2d3,0x315,0x360,0x3a2,0x3a2
//};

//const u32 keyTbl[13]={
//0x00,52,110,180,246,319,388,464,535,598,656,706,763
//};
const u32 keyTbl[15]={
	0x00,0x0c,0x1b,0x2c,0x3c,0x4e,0x5f,0x71,0x83,0x93,0xa2,0xaf,0xbd,0x3ff,0x3ff
};
//const u32 keyTbl[10]={
//0x00,52,110,180,246,319,388,464,535,598,656,706,763
//};

static u32 b_highTurn=0;
static u32 b_highTurn1=0;
static u8 onekeyreturn_state=0;
u8 f_b_trim_flag;
extern u8 stunt_flag;
extern u8 stunt_key_f;
extern u8 bat_lower;
extern u8 correct_flag;
extern u8 swlistLast;
void led_not(u32 chn)
{
	gpio_write (chn, b_led);//B PWM
	b_led=!b_led;
}
void stunt_check(void)
{
	static u32 debonce1;
	static u32 debonce2;
	static u32 debonce3;
	static u32 debonce4;

    if( tx_packet.vr_fb>0xf0) //f stunt b0
	{
    	debonce1++;
    	if(debonce1>5)
    	{
    		tx_packet.vr_stunt=0x44;
    		//buzzer_list=0;
    		stunt_flag=0;
    	}
	}else
    	debonce1=0;

//==================================================
	if( tx_packet.vr_fb<0x10) //b stunt
	{
		debonce2++;
		if(debonce2>5)
		{
			tx_packet.vr_stunt=0x33;
			//buzzer_list=0;
			stunt_flag=0;
		}
	}else
    	debonce2=0;

//==================================================
    if( tx_packet.vr_lr<0x10) //left stunt 30
    {
    	debonce3++;
    	if(debonce3>5)
    	{
    		tx_packet.vr_stunt=0x22;
    		//buzzer_list=0;
    		stunt_flag=0;
    	}
    }else
    	debonce3=0;

//==================================================
	if( tx_packet.vr_lr>0xf0) //right stunt
	{
		debonce4++;
		if(debonce4>5)
		{
			tx_packet.vr_stunt=0x11;
			//buzzer_list=0;
			stunt_flag=0;
		}
	}
	else
		debonce4=0;
}



/**************************************************************
 * 功能：蜂鸣器处理
 * 参数：
 * 返回：
 *************************************************************/
void buzzer_play(s32 playTime)
{

	while(1)
	{
		sleep_us(250);
	   gpio_write (BUZZER_PIN, b_led);
	   b_led=!b_led;
	   playTime--;
	   if(playTime<=0)break;

	}
	gpio_write (BUZZER_PIN, 0);
}
/**************************************************************
 * 功能：蜂鸣器处理
 * 参数：
 * 返回：
 *************************************************************/
void buzzer_play1(s32 playTime)
{

	while(1)
	{
		sleep_us(150);
	   gpio_write (BUZZER_PIN, b_led);
	   b_led=!b_led;
	   playTime--;
	   if(playTime<=0)break;

	}
	gpio_write (BUZZER_PIN, 0);
}
void stunt_check2(void)
{
	static u32 debonce1;
	static u32 debonce2;
	static u32 debonce3;
	static u32 debonce4;

	if(tx_packet.vr_thr<15){
		buzzer_list=0;
		return;
	}
	if(time_checkMs(TIME_DEBUG_UPDATA,2000)==1)
	{
		buzzer_list=0;
		return;
	}
	if( tx_packet.vr_fb>0xc0) //f stunt b0
	{
		debonce1++;
		if(debonce1>5)
		{
			tx_packet.vr_stunt=0x88;
			buzzer_list=0;
		}
	}
	else
		debonce1=0;
//==================================================
	if( tx_packet.vr_fb<0x30) //b stunt
	{
		debonce2++;
		if(debonce2>5)
		{
			tx_packet.vr_stunt=0x77;
			buzzer_list=0;
		}
	}
	else
		debonce2=0;
//==================================================
	if( tx_packet.vr_lr<0x30) //left stunt 30
	{
		debonce3++;
		if(debonce3>5)
		{
			tx_packet.vr_stunt=0x66;
			buzzer_list=0;
		}
	}
	else
		debonce3=0;
    //==================================================
	if( tx_packet.vr_lr>0xc0) //right stunt
	{
		debonce4++;
		if(debonce4>5)
		{
			tx_packet.vr_stunt=0x55;
			buzzer_list=0;
		}
	}
	else
		debonce4=0;
}
/**************************************************************
 * 功能：蜂鸣器扫描
 * 参数：
 * 返回：
 *************************************************************/
void buzzer_scan(void)
{
	static u32 time=0;//static前缀不受全局变量的影响
	static u32 i=0;  //初始化值

	//低电
	if(bat_lower)
	{
		if(time_checkMs(TIME_BAT_UPDATA,1000)==1)
		{
			led_not(LED_PIN);
			if(b_led)buzzer_play(100);
		}
	}

	//校正
	if(correct_flag)
	{
		if(tx_packet.vr_thr>0x40||tx_packet.vr_lr>0x40||tx_packet.vr_rotor>0x40||tx_packet.vr_fb>0x40)correct_flag=0;
		if(time_checkMs(TIME_CORRECT_UPDATA,300)==1)
		{
			buzzer_play(100);
		}
	}

	if(swlistLast==0)		//翻滚档，只要油门大于34，stunt_flag 置 1
	{
		if(tx_packet.vr_thr>=35)
			{
			stunt_flag=1;
			}
	}

	//翻滚
	if(stunt_flag)
	{
		static u32 state_g_r=1;
		static u32 burn_debonce;

/*
		if((tx_packet.vr_thr<0x40)&&(tx_packet.vr_rotor>0xc0)&&(tx_packet.vr_fb<0x40)&&(tx_packet.vr_lr<0x40))
		{
			burn_debonce++;
	    	if(burn_debonce>5)
	    	{
	    		tx_packet.vr_stunt=0xdd;
	    		buzzer_list=0;
	    		stunt_flag=0;
				LED_ON;
				state_g_r=1;
	    	}
			return;
		}
		else
			burn_debonce=0;
	//==================================================
*/

		if(tx_packet.vr_thr<35)
		{
				//buzzer_list=0;
				stunt_flag=0;
				stunt_key_f=0;
		}
		else
		{
			//翻滚档，只要油门大于35，可以根据方向手柄控制前后左右方向，达到阀值即可前后左右翻滚，无需按翻滚健
			if(swlistLast==0)
			{
				if(stunt_key_f==1)
				{
		    		tx_packet.vr_stunt=0x44;
		    		//buzzer_list=1;
		    		stunt_flag=0;
		    		stunt_key_f=0;

				}
				else
				{
					stunt_check();
				}
				/*
				if(time_checkMs(TIME_STUNT_UPDATA,300)==1)
				{
					if(state_g_r)
					{
						LED_ON;
						state_g_r=0;
						buzzer_play1(200);
					}
					else
					{
						LED_OFF;
						state_g_r=1;
					}
				}
				*/
			}
			else
			{
	    		tx_packet.vr_stunt=0x44;
	    		//buzzer_list=1;
	    		stunt_flag=0;
	    		stunt_key_f=0;
			}
		}

		if(stunt_flag==0)
		{
			LED_ON;
			state_g_r=1;
		}
	}



	if(buzzer_list==4)
	{
		if(tx_packet.vr_thr<20)buzzer_list=0;
		if(tx_packet.vr_lr<0x60)buzzer_list=0;
		if(tx_packet.vr_lr>0xa0)buzzer_list=0;
		if(tx_packet.vr_rotor<0x60)buzzer_list=0;
		if(tx_packet.vr_rotor>0xa0)buzzer_list=0;
		if(tx_packet.vr_fb<0x60)buzzer_list=0;
		if(tx_packet.vr_fb>0xa0)buzzer_list=0;

		if(time_checkMs(TIME_BUZZER_UPDATA,time)==1)
		{
			time=300;
			buzzer_play(200);
		}
		return;
	}

	tx_packet.button_1&=0xfd;
	if(tx_packet.vr_thr<20)	//油门拉下时
	{
		tx_packet.button_1&=0x3f;
		b_highTurn=0;
		b_highTurn1=0;
	}

	//无头
	/*
	if(b_noHeadMode==1)
	{
		if(time_checkMs(TIME_BUZZER_UPDATA,time)==1)
		{
			time=1000;
			led_not(LED_PIN);
			buzzer_play(200);
		}
		return;
	}
	*/

	if(buzzer_list==7)
	{
		if(time_checkMs(TIME_BUZZER_UPDATA,time)==1)
		{
			time=1000;
			led_not(LED_PIN);
			buzzer_play(50);
		}
		return;
	}

	if(buzzer_list==6)
	{
		stunt_check2();
		if(time_checkMs(TIME_BUZZER_UPDATA,time)==1)
		{
			if(time==50)
			{
				buzzer_play(300);
				time_ms(200);
				buzzer_play(300);
				time_ms(200);
				buzzer_play(300);
			}
			time=500;
		}
		i=0;
		//buzzer_list=0;
		return;
	}

	time=50;
	if(buzzer_list==1)buzzer_play1(300);

	if(buzzer_list==2)buzzer_play(1500);

	if(buzzer_list==5)
	{
		buzzer_play1(300);
		time_ms(200);
		buzzer_play1(300);
	}

	if(buzzer_list==10)
	{
		buzzer_play1(300);
		time_ms(200);
		buzzer_play1(300);
		time_ms(200);
		buzzer_play1(300);
	}

	buzzer_list=0;

	if(time_checkMs(TIME_BUZZER_UPDATA,300)==1)
	{
		i++;
		if(i>2)
		{
			i=0;
			tx_packet.vr_stunt=0xaa;
			buzzer_list=0;
		}
	}
}



/**************************************************************
 * 功能：无头模式
 * 参数：
 * 返回：
 *************************************************************/
void key_onHeadMode(void)
{
	static u32 b_turn=0;
	if(b_turn==0)
	{
		b_turn=1;
		tx_packet.button_1|=0x01;
		b_noHeadMode=1;
		buzzer_list=1;
	}else{
		b_turn=0;
		tx_packet.button_1&=0xfe;
		buzzer_list=1;
		b_noHeadMode=0;
		gpio_write (LED_PIN, 1);
	}
}
/**************************************************************
 * 功能：一键返回
 * 参数：
 * 返回：
 *************************************************************/
void key_oneKeyReturn(void)
{
	if(onekeyreturn_state==0){
		if(tx_packet.vr_thr<0x20){
			buzzer_list=1;
		}else{
			onekeyreturn_state=1;
			tx_packet.button_1|=0x02;
			buzzer_list=4;
		}
	}else{
		onekeyreturn_state=0;
		tx_packet.button_1|=0x02;
		buzzer_list=4;
	}
}
u8 state_l,state_r;
void set_autorotation_L(void)
{
	if(state_l){
		state_l=0;
		buzzer_list=0;
		tx_packet.trim_rotor&=0xfe;
	}else{
		state_l=1;
		state_r=0;
		buzzer_list=7;
		tx_packet.trim_rotor&=0xfd;
		tx_packet.trim_rotor|=0x01;
	}
}
void set_autorotation_R(void)
{
	if(state_r){
		state_r=0;
		buzzer_list=0;
		tx_packet.trim_rotor&=0xfd;
	}else{
		state_r=1;
		buzzer_list=7;
		state_l=0;
		tx_packet.trim_rotor&=0xfe;
		tx_packet.trim_rotor|=0x02;
	}
}
/**************************************************************
 * 功能：速度模式设置
 * 参数：
 * 返回：
 *************************************************************/
void key_speedMode(void)
{
	static u32 b_turn=0;
	if(b_turn==0)
	{
		b_turn=1;
		tx_packet.button_1|=0x30;
		buzzer_list=5;
	}else{
		b_turn=0;
		tx_packet.button_1&=0xcf;
		buzzer_list=1;
	}
}
/**************************************************************
 * 功能：
 * 参数：
 * 返回：
 *************************************************************/
void key_lightControl(void)
{
	static u32 b_turn=0;
	if(b_turn==0)
	{
		b_turn=1;
		tx_packet.button_1|=0x08;
		buzzer_list=1;
	}else{
		b_turn=0;
		tx_packet.button_1&=0xf7;
		buzzer_list=0;
		buzzer_list=1;
	}
}

void right_highSpeedControl(void)
{
	b_highTurn1=0;
	if(b_highTurn==0)
	{
		b_highTurn=1;
		tx_packet.button_1|=0x40;
		tx_packet.button_1&=0x7f;
		buzzer_list=1;
	}else{
		b_highTurn=0;
		tx_packet.button_1&=0xbf;
		buzzer_list=0;
		buzzer_list=1;
	}
}

void left_highSpeedControl(void)
{
	b_highTurn=0;
	if(b_highTurn1==0)
	{
		b_highTurn1=1;
		tx_packet.button_1|=0x80;
		tx_packet.button_1&=0xbf;
		buzzer_list=1;
	}else{
		b_highTurn1=0;
		tx_packet.button_1&=0x7f;

		buzzer_list=1;
	}
}


/**************************************************************
 * 功能：按键命令处理
 * 参数：cmd ：对应的按键值
 * 返回：
 *************************************************************/
void key_cmd(u32 cmd)
{
	switch(cmd)
	{
		case PHOTO:
			tx_packet.vr_stunt=0x99;
			buzzer_list=1;
			break;

		case CAMERA:
			tx_packet.vr_stunt=0xbb;
			buzzer_list=1;
			break;

		case FBLR_STUNT:
/*
			if(stunt_flag==0)
			{
				buzzer_list=3;
				stunt_flag=1;
			}
			else
			{
				buzzer_list=0;
				stunt_flag=0;
				LED_ON;
			}
*/
			buzzer_list=1;
			stunt_flag=1;
			stunt_key_f=1;
			break;

		case L_TRIM:
			buzzer_list=1;
			trim_lr-=1;
			if(trim_lr==0)
				buzzer_list=2;
			if(trim_lr<-50){
				buzzer_list=0;
				trim_lr=-50;
			}
			tx_packet.trim_lr=trim_lr;
			break;

	   //-----------------------------------------------
		case R_TRIM:
			buzzer_list=1;
			trim_lr+=1;
			if(trim_lr==0)buzzer_list=2;
			if(trim_lr>50){
				buzzer_list=0;
				trim_lr=50;
			}
			tx_packet.trim_lr=trim_lr;
			break;

	   //-----------------------------------------------
		case F_TRIM:
			buzzer_list=1;
			trim_fb+=1;
			if(trim_fb==0)buzzer_list=2;
			if(trim_fb>50){
				buzzer_list=0;
				trim_fb=50;
			}
			tx_packet.trim_fb=trim_fb;
			break;

	   //-----------------------------------------------
		case B_TRIM:
			buzzer_list=1;
			trim_fb-=1;
			if(trim_fb==0)buzzer_list=2;
			if(trim_fb<-50){
				buzzer_list=0;
				trim_fb=-50;
			}
			tx_packet.trim_fb=trim_fb;
			break;

		//-----------------------------------------------
		case R_ROTOR_TRIM:
			buzzer_list=1;
			trim_rotor-=1;
			if(trim_rotor==0)buzzer_list=2;
			if(trim_rotor<-50){
				buzzer_list=0;
				trim_rotor=-50;
			}
			tx_packet.trim_rotor=trim_rotor;
			break;


		//-----------------------------------------------
		case L_ROTOR_TRIM:
			buzzer_list=1;
			trim_rotor+=1;
			if(trim_rotor==0)buzzer_list=2;
			if(trim_rotor>50){
				buzzer_list=0;
				trim_rotor=50;
				}
			tx_packet.trim_rotor=trim_rotor;
			break;

	   //-----------------------------------------------
		case no_head_mode:
			key_onHeadMode();
			break;
		case ONE_KEY_RETURN:
			key_oneKeyReturn();
			break;
	   //------------------------------------------------------
		case FAST_SPEED:
			//key_speedMode();
			break;

		case LIGHT_CONTROR:
			key_lightControl();
			break;

		case RIGHT_BUTTON:
			right_highSpeedControl();
			break;

		case LEFT_BUTTON:

		    left_highSpeedControl();
		    break;


	   //-----------------------------------------------
		default: break;
   }
}



u32 calculate_adc_value(void)
{
	u32 temp1,value;
	temp1=read_button_value();  //GPIO读出按键值
	value=(5*temp1*0x3ff)/(13*(0x3ff-temp1));
	return value;
}
/********************************************************
 * 功能：按键扫描
 * 参数：adc		读出ADC的值
 *******************************************************/
void key_scan(s32 adc)
{
	static u32 holdTimes=0;
	static u32 hold_Interval_Times=0;
	static u32 hold_flag=0;   		//保持标志位

	static u32 listLast=20;
	s32 list=0;
	static u32 u32_debonce=0;

	s32 temp;
	static u32 temp1,temp2;

	temp1=adc;
	temp2=adc;

	if(adc<0)adc=0;
	else if(adc>0xff)adc=0xff;     //限值256内，防止出错

	while(1)
	{
		temp=keyTbl[list];			//list初始为0

		if(adc>temp)
		{
			temp=adc-temp;
		}
		else
		{
			temp=temp-adc;
		}

		if(temp<8)					//8是误差最大值
		{
			break;
		}

		list++;
		if(list>=13)
		{
			break;
		}
	}

	if(listLast==list)            		//识别到按键时候
	{
	    if(list>=13)//快慢开关长时间状态
		{
		    holdTimes=0;         		//长按有效时间清零
		    hold_flag=0;
		    hold_Interval_Times=0;
		    return;
		}

	    holdTimes++;              		//时间开始计数

		if(holdTimes>50)   				//长按有效时间
		{
			hold_flag=1;
		}

	    if(hold_flag==1)
	    {
	    	hold_Interval_Times++;

			if(hold_Interval_Times>25)   			//长按有效时间
			{
				hold_Interval_Times=0;

				if(list==F_TRIM)
				{
					buzzer_list=1;
					trim_fb+=1;
					if(trim_fb==0)buzzer_list=2;
					if(trim_fb>50){
						buzzer_list=0;
						trim_fb=50;
					}
					tx_packet.trim_fb=trim_fb;
					return;
				}

				if(list==B_TRIM)
				{
					buzzer_list=1;
					trim_fb-=1;
					if(trim_fb==0)buzzer_list=2;
					if(trim_fb<-50){
						buzzer_list=0;
						trim_fb=-50;
					}
					tx_packet.trim_fb=trim_fb;
					return;
				}

				if(list==L_TRIM)
				{
					buzzer_list=1;
					trim_lr-=1;
					if(trim_lr==0)
						buzzer_list=2;
					if(trim_lr<-50){
						buzzer_list=0;
						trim_lr=-50;
					}
					tx_packet.trim_lr=trim_lr;
					return;
				}

				if(list==R_TRIM)
				{
					buzzer_list=1;
					trim_lr+=1;
					if(trim_lr==0)buzzer_list=2;
					if(trim_lr>50){
						buzzer_list=0;
						trim_lr=50;
					}
					tx_packet.trim_lr=trim_lr;
					return;
				}

				if(list==R_ROTOR_TRIM)
				{
					buzzer_list=1;
					trim_rotor-=1;
					if(trim_rotor==0)buzzer_list=2;
					if(trim_rotor<-50){
						buzzer_list=0;
						trim_rotor=-50;
					}
					tx_packet.trim_rotor=trim_rotor;
					return;
				}

				if(list==L_ROTOR_TRIM)
				{
					buzzer_list=1;
					trim_rotor+=1;
					if(trim_rotor==0)buzzer_list=2;
					if(trim_rotor>50){
						buzzer_list=0;
						trim_rotor=50;
						}
					tx_packet.trim_rotor=trim_rotor;
					return;
				}
			}
	    }

		return;
		}

	u32_debonce++;
	if(u32_debonce<5)
	{
		return;
	}

	u32_debonce=0;
	listLast=list;
//===============================================================
	key_cmd(list);
	//tx_packet.trim_rotor=list;
	return;
}



void	sw_scan(s32 swadc)
{

	s32 swlist;
	static u32 sw_debonce=0;

	if(swadc>240)
	{
		//慢速档
		swlist=2;
	}
	else
	{	if(swadc<20)
		{
		//翻滚档
		swlist=0;
		}
		else
		{
		//高速档
		swlist=1;
		}
	}

	if(swlistLast==swlist)		//识别到开关时候
	{
		return;
	}

	sw_debonce++;
	if(sw_debonce<5)
	{
		return;
	}

	sw_debonce=0;
	swlistLast=swlist;
//===============================================================


	if(swlist==0)
	{
		//翻滚档

		buzzer_list=10;
		stunt_flag=1;
		stunt_key_f=0;
		return;
	}

	if(swlist==1)
	{
		//高速档
		tx_packet.button_1|=0x30;
		buzzer_list=5;
		stunt_flag=0;
		stunt_key_f=0;
		return;
	}

	if(swlist==2)
	{
		//慢速档
		tx_packet.button_1&=0xcf;
		buzzer_list=1;
		stunt_flag=0;
		stunt_key_f=0;
		return;
	}

	//tx_packet.trim_lr=list;

}


