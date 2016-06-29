#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "rf_frame.h"
#include "../common/rf_com.h"
#include "../common/time.h"

#include "../lib/ota_sample.h"
#include "../lib/comparator.h"
#include "../lib/uart.h"
#include "../lib/motor.h"
#include "sensor.h"
#include "imu.h"
#include "pid.h"
#include "flashCtrl.h"






#define USE_SUSPEND   1

extern rf_packet_t	pkt_rf ;

rf_packet_z_t	* ptr_pkt_rf;

rf_packet_debug	* ptr_debug;

extern int g_rf_chn;



const u8 rf_channel_0[8][3] =
{
 4, 8, 12,  //0
 5, 9, 13,  //1
 6, 10, 14,  //2
 7, 11, 15, //3
 20, 24, 28, //4
 21, 25, 29, //5
 22, 26, 30,  //6
 23, 27, 31,  //7
};//频段
u8 rf_cha0,rf_cha1,rf_cha2;


int gps_x,gps_y,gps_z;
volatile float dbg_imu;


u16 addr_rx_tmp;
u16 addr_tx_tmp;
u8 vr_lr_tmp;
u8 vr_rotor_tmp;
u8 vr_fb_tmp;
u8 vr_thr_tmp;
u8 vr_stunt_tmp;
u8 button_1_tmp;
u8 button_2_tmp;
u8 trim_fb_tmp;
u8 trim_lr_tmp;
u8 trim_rotor_tmp;



u32 b_bat=0;
u32 m_stop=0;
u32 P_bat=0;
u32 stunt_bat=0;
u32 b_motorProtect=0;
u32 b_led=0;
u32 b_led1=0;
u32 b_rf=0;
u32 b_rf1=0;
u16 tx_addr_u16;
u16 rx_addr_u16;
u32 rf_lostTime;
s16 thr=0;


u32 stunt_type=0;
u32 stunt_type_last=0;

u32 stunt_imu=0;
u16 imuPersec;


u32 code_updata=0;

u32 code_updata1=0;

//u32 b_flash=0;

u16 voltage;


s32 x_userSum=0;
s32 y_userSum=0;
s32 xy_userIndex=0;
s32 xy_userIndex1=0;
s32 pid_reset;

u32 time_cameraPhoto=200;
u8 b_noHead=0;	//无头标志位

u32 startflytime;

//const unsigned char pwmTbl[64]={
//PWM0, PWM1, PWM2, PWM3, PWM4, PWM5, PWM6, PWM7,
//PWM8, PWM9, PWM10, PWM11, PWM12, PWM13, PWM14, PWM15,
//PWM16, PWM17, PWM18, PWM19, PWM20, PWM21, PWM22, PWM23,
//PWM24, PWM25, PWM26, PWM27, PWM28, PWM29, PWM30, PWM31,
//PWM32, PWM33, PWM34, PWM35, PWM36, PWM37, PWM38, PWM39,
//PWM40, PWM41, PWM42, PWM43, PWM44, PWM45, PWM46, PWM47,
//PWM48, PWM49, PWM50, PWM51, PWM52, PWM53, PWM54, PWM55,
//PWM56, PWM57, PWM58, PWM59, PWM60, PWM61, PWM62, PWM63,
//
//};


s32 pwm_m1,pwm_m2,pwm_m3,pwm_m4;

//优化程序后油门曲线表在此调整
const unsigned short  throttle_table[12] = { 0, 180, 320, 410, 430, 450, 500, 550, 600, 650, 850,0};
//const unsigned short  throttle_table[12] = { 0, 320, 380, 410, 430, 450, 500, 550, 600, 650, 850,0};



void throttle_curves(s32 tmp)
{
	s32 tmp2;
	tmp=tmp*4;
	 	 	 	if( tmp < 0 )
	            {
	                tmp = 0;
	            }
	            if( tmp > 1000 )
	            {
	                tmp = 1000;
	            }
	            tmp = tmp * 2559 / 1000;
	            tmp2 = tmp/256; // range [0;9]
	            tmp =throttle_table[tmp2] + (tmp-tmp2*256) * (throttle_table[tmp2+1]-throttle_table[tmp2]) / 256; // [0;2559]
	            thr = ( thr * 7 +  tmp * 3 ) / 10;
}

void led_not(u32 chn)
{

	gpio_write (chn, b_led);//B PWM
	b_led=!b_led;
}

void led_not1(u32 chn)
{

	gpio_write (chn, b_led1);//B PWM
	b_led1=!b_led1;
}


volatile u32 gpio_irq_cnt;
_attribute_ram_code_  void gpio_irq_handler(void)
{
	gpio_irq_cnt++;

	//led_not(GPIO_GP24);
}

extern void (*gpio0_user_irq_handler)(void);




unsigned int vendor_id_get()
{
	//return 0x1134; //0x1234; //0x5678; //0x5453;

	otp_init_read();
		return otp_read(0x3f04);
}

void platform_init()
{


		gpio_set_input_en(GPIO_GP12,1);
		gpio_set_output_en(GPIO_GP12, 0);

		gpio_set_input_en(GPIO_GP13,1);
		gpio_set_output_en(GPIO_GP13, 0);

		gpio_set_input_en(GPIO_GP14,1);
		gpio_set_output_en(GPIO_GP14, 0);

		gpio_set_input_en(GPIO_GP16,1);
		gpio_set_output_en(GPIO_GP16, 0);

		/*

		reg_gpio_g_2risc2 |= GPIO_GP12|GPIO_GP13|GPIO_GP14|GPIO_GP16; //Select interrupt GPIO

				reg_gpio_g_pol &= ~(GPIO_GP12|GPIO_GP13|GPIO_GP14|GPIO_GP16); //Control High level interrupt triggle


		//reg_gpio_g_2risc2 |= GPIO_GP12|GPIO_GP13|GPIO_GP14|GPIO_GP16; //Select interrupt GPIO

		//reg_gpio_g_pol &= ~(GPIO_GP12|GPIO_GP13|GPIO_GP14|GPIO_GP16); //Control High level interrupt triggle

		gpio0_user_irq_handler = gpio_irq_handler;
		reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EN;	    //FLD_IRQ_GPIO_RISC2;
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;		//clear GPIO interrupt flag

        */


}
void device_info_load()
{

}







static inline void system_enter_suspend(void)
{

	irq_disable();
//	cpu_sleep_wakeup(DEEPSLEEP_MODE, 10, 0,  0); //for 10ms per one package.
	cpu_sleep_wakeup(SUSPEND_MODE, 10, 0,  0); //for 10ms per one package.

	clock_init();
	rf_drv_init(1);
	irq_enable();
	WaitUs (800);
}


//void rf_checksum(void)
//{

//}

void user_rcv_proc(u8* p)
{
	static u32 b_photo=0;
	static u32 b_camra=0;
	//pkt_rf.data[0]++;
	u32 page;
	u16 *p1;
	u32 i;
	rf_packet_z_t *ppkt_rf = (struct rf_packet_z_t *) (p + 8);

	ptr_pkt_rf->addr_tx=ppkt_rf->addr_tx;
	ptr_pkt_rf->addr_rx=ppkt_rf->addr_rx;
	ptr_pkt_rf->vr_lr=ppkt_rf->vr_lr;
	b_rf=1;
	if(ptr_pkt_rf->addr_rx==7777)//接收到遥控地址
	{
		if(ptr_pkt_rf->addr_tx==tx_addr_u16)//发给遥控地址也相同
		{
			button_2_tmp=ppkt_rf->vr_lr^ppkt_rf->vr_rotor^ppkt_rf->vr_fb^ppkt_rf->vr_thr^ppkt_rf->vr_stunt;
			button_2_tmp=button_2_tmp^ppkt_rf->button_1^ppkt_rf->trim_fb^ppkt_rf->trim_lr^ppkt_rf->trim_rotor;
			if(button_2_tmp==ppkt_rf->button_2)
			{

		       vr_lr_tmp=ppkt_rf->vr_lr;
		       vr_rotor_tmp=ppkt_rf->vr_rotor;
		       vr_fb_tmp=ppkt_rf->vr_fb;
		       vr_thr_tmp=ppkt_rf->vr_thr;
		       vr_stunt_tmp=ppkt_rf->vr_stunt;
	           button_1_tmp=ppkt_rf->button_1;
	           button_2_tmp=ppkt_rf->button_2;
	           trim_fb_tmp=ppkt_rf->trim_fb*2;
	           trim_lr_tmp=ppkt_rf->trim_lr*2;
	           trim_rotor_tmp=ppkt_rf->trim_rotor*2;//取遥控数据

	           if(vr_stunt_tmp==0x99)
	           {
	        	   if(b_photo==0)
	        	   {
	        		   b_photo=1;

	        		   gpio_write (GPIO_GP19, 0);
	        		   time[TIME_CAMERA_PHOTO]=clock_time();
	        		   time_cameraPhoto=200;
	        	   }
	           }
	           else b_photo=0;

	           if(vr_stunt_tmp==0xbb)
	           {
	           	    if(b_camra==0)
	           	    {
	           	      b_camra=1;
	           	      gpio_write (GPIO_GP19, 0);
	           	      time[TIME_CAMERA_PHOTO]=clock_time();
	           	      time_cameraPhoto=1000;
	           	    }
	           }
	           else b_camra=0;



	           if(vr_thr_tmp<35){b_motorProtect=1;}//解遥控数据b_rf=b_rf1=2
	           //  rf_lostTime=0;
	           b_rf=2;
	           b_rf1=2;




			}
		}
	}
	//==================debug parameter
/*
	if(ptr_pkt_rf->addr_rx==0x5555)
	{
		if(ptr_pkt_rf->addr_tx==0xaaaa)
		{
			//gpio_write (GPIO_GP24, 1);
			led_not(GPIO_GP24);

			rf_packet_debug *ppkt_rf = (struct rf_packet_z_t *) (p + 8);
			 p1 = &dectectData.g_agc;

			 page=ppkt_rf->debug10-12;
			 p1[page++]=ppkt_rf->iz;
			 p1[page++]=ppkt_rf->z;
			 p1[page++]=ppkt_rf->ix;
			 p1[page++]=ppkt_rf->iy;

			 b_flash=1;
			 time[TIME_FLASH_UPDATA]=clock_time();
			 pid_normalMode();



		}
	}*/



}


void io_init(void)
{
	gpio_set_func(GPIO_GP24, AS_GPIO);
	gpio_set_output_en(GPIO_GP24, 1);

	gpio_set_func(GPIO_GP3, AS_GPIO);
		gpio_set_output_en(GPIO_GP3, 1);
		gpio_write (GPIO_GP3, 0);

	gpio_set_func(GPIO_GP23, AS_GPIO);
	gpio_set_output_en(GPIO_GP23, 1);

	gpio_set_func(GPIO_GP22, AS_GPIO);
	gpio_set_output_en(GPIO_GP22, 1);

	gpio_set_func(GPIO_GP25, AS_GPIO);
	gpio_set_output_en(GPIO_GP25, 0);
	gpio_set_input_en(GPIO_GP25, 1);

	gpio_set_func(GPIO_GP17, AS_GPIO);
    gpio_set_output_en(GPIO_GP17, 0);
	gpio_set_input_en(GPIO_GP17, 1);


	gpio_set_func(GPIO_GP19, AS_GPIO);
	gpio_set_output_en(GPIO_GP19, 1);
	gpio_write (GPIO_GP19, 1);

}



extern u32 RF_RX_timing;
void debug_codeUpdata(void)
{
	time_ms(100);
	    wd_stop();
		irq_disable();
		RF_init();
		SetRxMode (0, RF_CHN_TABLE);
	gpio_write (GPIO_GP24, 1);
	//while(!clock_time_exceed(start_time,5000000)){
	while(1){
		RF_rcv_process(ota_main_proc);
		if(clock_time_exceed(RF_RX_timing,1000000)){

					RF_sendDebug(ptr_debug);
					WaitUs(1000);
			        SetRxMode (0, RF_CHN_TABLE);
					RF_RX_timing = clock_time();
				}


		if(time_checkMs(TIME_LED_UPDATA,40)==1)
		{
			led_not (GPIO_GP24);

	    }

	}
	gpio_write (GPIO_GP24, 1);


}




void debug_send(u16 x,u16 y,u16 z)
{
	        static u32 page=0;
			static u32 set_rxMode=0;
			static u32 size=0;
			u16 *p;
			u16 temp1;


			if(page>=size )
			{
				if(time_checkMs(TIME_UART_UPDATA,1000)==1)
				{
					dectectData.frq=imuPersec;
					imuPersec=0;

					size=53;//sizeof(Flying_Control_t);
				}
				else size=25;//(sizeof(Flying_Control_t)/2)-28;

				if(time_checkUs(TIME_RF_SEND_UPDATA,1000)==1)
				{
					if(set_rxMode==1)
					{
						SetRxMode (g_rf_chn, RF_CHN_TABLE);
						set_rxMode=0;
					    return;
					}
			   }



			   if(time_checkMs(TIME_DEBUG_UPDATA,50)==1)
			   {
				   page=0;

				   dectectData.pitch=(short)(pitch*100);
				   dectectData.roll=(short)(roll*100);
				   dectectData.yaw=(short)(yaw*100);

				   dectectData.gx=gx;
				   dectectData.gy=gy;
				   dectectData.gz=gz;

				   dectectData.ax=ax;//IMU_Pitch_;//ax;
				   dectectData.ay=ay;//IMU_Roll_;//ay;
				   dectectData.az=az;

				   dectectData.mx=s_pwmM1;//voltage;//size;//ax;
				   dectectData.my=s_pwmM2;//b_bat;//ay;
				   dectectData.mz=s_pwmM3;//P_bat;//az;

				   dectectData.thr=thr;
				   dectectData.fb=trim_fb_tmp;//vr_fb_tmp;//gps_x;
				   dectectData.lr=trim_lr_tmp;//vr_lr_tmp;//gps_y;
				   dectectData.rotor=gps_z;

				   dectectData.cmd1=x;
				   dectectData.cmd2=y*10;
				   dectectData.cmd3=z;

				   dectectData.fn1=44;
				   dectectData.alt=s_pwmM4;
				  // dectectData.temperature=(sensor_velocity*100.f);
				  // dectectData.pressure=(sensor_altitude*100.f);


			   }
			   else return;
			}

		//----------------------------------------------------------------------
			if(time_checkUs(TIME_RF_SEND_UPDATA,1000)==0)return;

			set_rxMode=1;
			time[TIME_RF_SEND_UPDATA]= clock_time();

			ptr_debug->debug9=0x88;
			ptr_debug->debug10=page;

			p = &dectectData.pitch;

			ptr_debug->x=p[page++];//IMU_Pitch/10; //mpu6050_read(117);
		    ptr_debug->y=p[page++];//IMU_Roll/10;
			ptr_debug->z=p[page++];//IMU_Yaw/10;

			ptr_debug->ix=p[page++];
			ptr_debug->iy=p[page++];
			ptr_debug->iz=p[page++];

			 temp1=0;

			temp1=ptr_debug->x^ptr_debug->y^ptr_debug->z^ptr_debug->ix^ptr_debug->iy^ptr_debug->iz;


			ptr_debug->debug9=((temp1&0xff)^(temp1>>8));

		    RF_sendDebug(ptr_debug);
}






void rf_matchAddr(void)
{
	static u16 tx_addr;
    u32 times;
    u8 temp11;
    rx_addr_u16=vendor_id_get();//0x99;客户ID

//while(1)
{
	g_rf_chn=2;
    times=0;
    /*
	while(1)
	{
	   if(time_checkMs(TIME_LED_UPDATA,10)==1)
	   {
	      ptr_pkt_rf->addr_tx=tx_addr_u16;//0x99  发射地址
	      ptr_pkt_rf->addr_rx=rx_addr_u16;//0x99
	      ptr_pkt_rf->vr_lr = 0x55;//迷你四轴0x44 普通四轴0x55

	      if(rf_lostTime>15)ptr_pkt_rf->vr_rotor=0x22;
	      else ptr_pkt_rf->vr_rotor=0x11;


	      RF_send(ptr_pkt_rf);//发射ptr_pkt_rf给遥控
	      times++;
	      if(times>50)break;//50x10=500ms
	   }
	   if(time_checkMs(TIME_RF_UPDATA,250)==1)led_not(GPIO_GP24);
	}
	*/
//-----------------------------------------------------------------
	g_rf_chn=2;//通道2对码
	SetRxMode (g_rf_chn, RF_CHN_TABLE);
	times=0;
	ptr_pkt_rf->addr_tx=0;
	ptr_pkt_rf->addr_rx=0;
	while(1)
	{
		RF_rcv_process(user_rcv_proc);
		if(time_checkMs(TIME_RF_UPDATA,80)==1){led_not(GPIO_GP24);times++;}
		//if(times>3)break;//750ms退出
		if(b_rf==1)
		{
		   b_rf=0;
		   tx_addr_u16=ptr_pkt_rf->addr_tx;
		   if(ptr_pkt_rf->addr_rx==3333)break;
		   //break;
		}

	}
	//ptr_pkt_rf->addr_tx=0;
	//	ptr_pkt_rf->addr_rx=0;
	temp11=tx_addr_u16&0x07;
	 rf_cha0=rf_channel_0[temp11][0];
		rf_cha1=rf_channel_0[temp11][1];
		rf_cha2=rf_channel_0[temp11][2];
		g_rf_chn=rf_cha2;
		SetRxMode (g_rf_chn, RF_CHN_TABLE);

		gpio_write (GPIO_GP24, 1);
		time_ms(1500);
	while(1)
	{
		RF_rcv_process(user_rcv_proc);
				if(time_checkMs(TIME_RF_UPDATA,500)==1){led_not(GPIO_GP24);times++;}
				//if(times>3)break;//750ms退出
				if(b_rf1==2)
				{
				   b_rf1=0;
				   if(tx_addr_u16==ptr_pkt_rf->addr_tx)
				   {
				       if(ptr_pkt_rf->addr_rx==7777)break;
				   }

				}

	}




//	if(ptr_pkt_rf->addr_rx==1111)break;
	/*
	if(rf_lostTime>15)
	{
       if(tx_addr_u16==tx_addr)
       {
    	   if(ptr_pkt_rf->addr_rx==rx_addr_u16)
    	   {
    		   m_stop=0;
    	       break;
    	   }
       }
	}
	else
	{
	    if(ptr_pkt_rf->addr_rx==rx_addr_u16)break;
	}
	*/
}




gpio_write (GPIO_GP24, 1);

SetRxMode (g_rf_chn, RF_CHN_TABLE);
tx_addr=tx_addr_u16;
}



void rf_dataUpdate(void)
{
	static u32 rstAll=0;
	short x,y,z,speed,z_speed;
    static u32 thrLast=0;
    u32 temp;

    static short thrLast1=0;

    	    //if((rf_lostTime>7)||(bat_lowTime>1800)||(motor_protect==1))
            if((rf_lostTime>7)||(P_bat))
    		{
    			static u32 times=0;
    			if(thr<thrLast1)thrLast1=thr;//上一笔油门数据大于最新油门数据，上一笔油门数据<=最新油门数据
    			if(thrLast1>0)
    			{
    				rf_lostTime=10;
    				//thrLast1--;
    				times++;//yfpeng缓降
    				if(times>2){thrLast1--;times=0;}
    				if(thrLast1<100)
    					{
    					thrLast1=0;
    					if(P_bat==1)m_stop=1;
    					}


    			}
    			//else b_motorProtect=0;

    			thr=thrLast1;

    		}
    		else thrLast1=thr;

    thrLast=thr;
    pwm_m1=thrLast;
    pwm_m2=thrLast;
    pwm_m3=thrLast;
    pwm_m4=thrLast;

	if(b_rf1!=2);	//RF接收函数(RF_rcv_process())中的函数 user_rcv_proc() 中会将 b_rf1 赋值 2
	else
	{
		rf_lostTime=0;
	    b_rf1=0;
	    b_rf=0;
	}

	if(stunt_type==0)
	{
	   if(vr_stunt_tmp==0x44){stunt_type=2; stunt_type_last=0;}//f
	   if(vr_stunt_tmp==0x33){stunt_type=1; stunt_type_last=0;}//b
	   if(vr_stunt_tmp==0x11){stunt_type=4; stunt_type_last=0;}//R
	   if(vr_stunt_tmp==0x22){stunt_type=3; stunt_type_last=0;}//
	}

	// if(vr_stunt_tmp==0x99)

	 if((vr_thr_tmp<0x40)&&(vr_rotor_tmp>0xb0)&&(vr_fb_tmp<0x40)&&(vr_lr_tmp<0x40)) //左斜45度进入无线烧录模式
	{
		code_updata1++;

	}
	else code_updata1=0;

	//rf_lostTime=0;
	if(vr_stunt_tmp==0xdd)			//进入无线烧录模式
	{
		code_updata1=100;
	}



	if((button_1_tmp&0x1)==0x1)			//进入无头模式
	{
		b_noHead=1;
	}
	else
	{
		b_noHead=0;
	}

	//接收遥控端IMU校准
	if(vr_stunt_tmp==0xcc)
	{
		rf_lostTime=200;	//校准标志
	}

	 throttle_curves(vr_thr_tmp);					//油门算法函数

	if(vr_fb_tmp>0x80)			//手柄 x,y,z计算
	{
		y=vr_fb_tmp-0x80;
	}
	else
	{
		y=-(0x80-vr_fb_tmp);
	}

	if(vr_lr_tmp>0x80)
	{
		x=vr_lr_tmp-0x80;
	}
	else
	{
		x=-(0x80-vr_lr_tmp);
	}

	if(vr_rotor_tmp>0x80)
	{
		z=(vr_rotor_tmp-0x80);
	}
	else
	{
		z=-(0x80-vr_rotor_tmp);
	}

	if((z>45)||(z<-45))								//防止自转刷锅动作，锁定 z 轴程序
	{
		startflytime=190;
		pid_gyro_integrator[0]=0;
		pid_gyro_integrator[1]=0;
		pid_gyro_integrator[2]=0;
		pid_integrator[0]=0;
		pid_integrator[1]=0;
	}
	else
	{
		z=0;
	}

	//if((x>55)||(x<-55));
	//else x=0;

	//if((y>55)||(y<-55));
	//else y=0;

	if((x>35)||(x<-35)||(y>35)||(y<-35))	//x,y轴，刹车力度设置
	{
		//pid_gyro_integrator[0]=0;
			//		pid_gyro_integrator[1]=0;
				//	pid_integrator[0]=0;
				//	pid_integrator[1]=0;

		pid_reset=200;
		if(xy_userIndex<100)//600ms
		{
			xy_userIndex++;
			xy_userIndex1=xy_userIndex;
			x_userSum=x_userSum+x;
			y_userSum=y_userSum+y;

		}
	}

	else
	{
		x=0,y=0;

		/*
		if(time_checkMs(TIME_IMU_RST,100)==1)
		{
			//if(stunt_type==0)IMU_init();
		}

		pid_reset++;
		if(pid_reset>500)
		{
			x=0,y=0;
			pid_reset=600;
			xy_userIndex=0;
			x_userSum=0;
			y_userSum=0;
		}
		else
		{
			xy_userIndex=0;
			x=-(x_userSum/xy_userIndex1)*50/100;
			y=-(y_userSum/xy_userIndex1)*50/100;//yfpeng翘头，*参数越大，翘头越厉害
			x_userSum-=(x_userSum/xy_userIndex1);
			y_userSum-=(y_userSum/xy_userIndex1);
		}
        */

	}

	if(trim_fb_tmp>0x80)
	{
		y = y-(0xff-trim_fb_tmp);
	}
	else
	{
		y = y + trim_fb_tmp;
	}

	if(trim_lr_tmp>0x80)
	{
		x=x-(0xff-trim_lr_tmp);
	}
	else
	{
		x=x+trim_lr_tmp;
	}

	if(trim_rotor_tmp>0x80)
	{
		z=z-(0xff-trim_rotor_tmp);
	}
	else
	{
		z=z+trim_rotor_tmp;
	}


	if((button_1_tmp&0x30)==0x30)//slow speed
    {
		speed=dectectData.rc_spd1;
		z_speed=dectectData.rc_de1;
	}
	else
	{
		speed=dectectData.rc_spd2;
		z_speed=dectectData.rc_de2;
	}

    if((button_1_tmp&0x02)==0x02)// one key return;
	{
		IMU_noHeadMode(0,80);
		y=gps_roll;
		x=gps_pitch;
	}
    else if(b_noHead==1)
    {
		IMU_noHeadMode(x,-y);		//无头模式设置
		y=gps_roll;
		x=gps_pitch;
    }

	x=Math_fConstrain(x,-100,100);//50
	y=Math_fConstrain(y,-100,100);//50

	gps_y=x*speed;
	gps_x=y*speed;
	gps_z=-z*z_speed;


}


void motor_Check(short x,short y,short z)
{

	static u32 imu_rest=200;
	static u32 motorProtectTime;

		if(time_checkMs(TIME_RF_LOST_UPDATA,100)==1){

			if(rf_lostTime<50)
			rf_lostTime++;
			motorProtectTime++;
		}

       // if(vr_thr_tmp<85)motorProtectTime=0;
		//if(motorProtectTime<5)gpio_irq_cnt=0;
		if(motorProtectTime>2)
		{
			//if(gpio_irq_cnt>800)b_motorProtect=0;//7000
			if(time_checkMs(TIME_MOTOR_PROTECT,250)==1)
				{
				/*
					 pwm_speedSet(20000,20000,20000,20000);
					 time_us(1000);
					 pwm_speedSet(0,0,0,0);
					 time_us(500);
					 if(gpio_read(GPIO_GP12)==GPIO_GP12)b_motorProtect=0;
					 if(gpio_read(GPIO_GP13)==GPIO_GP13)b_motorProtect=0;
					 if(gpio_read(GPIO_GP14)==GPIO_GP14)b_motorProtect=0;
					 if(gpio_read(GPIO_GP16)==GPIO_GP16)b_motorProtect=0;
					 time_us(500);
					 */
				}


			//gpio_irq_cnt=0;
			motorProtectTime=11;
		}

		imu_rest++;

		// if(gpio_read(GPIO_GP14)==0) gpio_write (GPIO_GP24, 0);
		// else  gpio_write (GPIO_GP24, 1);

	//	led_not(GPIO_GP24);

		if(b_motorProtect==0)
		{
		   vr_thr_tmp=0;
		   if(time_checkMs(TIME_RF_UPDATA,150)==1)led_not(GPIO_GP24);
		}
		if((vr_thr_tmp<20)||(rf_lostTime>13)||(m_stop==1))//35
		{
		//	m_stop=0;
			if(P_bat==1)m_stop=1;
            motorProtectTime=0;
            pwm_m1=0;
            pwm_m2=0;
            pwm_m3=0;
            pwm_m4=0;

			thr=0;
			vr_thr_tmp=0;
			pid_gyro_integrator[0]=0;
			pid_gyro_integrator[1]=0;
			pid_integrator[0]=0;
			pid_integrator[1]=0;

			startflytime=0;

			if(imu_rest<100) imuKp=0.30f;
			else
			{
				imuKp=2.5f;
				pid_gyro_integrator[2]=0;
			}
            if(imu_rest>300)imuKp=0.8f;

			return;
		}
		imuKp=0.3f;
		imu_rest=0;

		 // x = Math_fConstrain(x,-7000,7000);
		//	   y = Math_fConstrain(y,-7000,7000);
		//	   z = Math_fConstrain(z,-7000,7000);

		pwm_m1=pwm_m1+x+y-z;

		pwm_m2=pwm_m2+x-y+z;

		pwm_m3=pwm_m3-x-y-z;

		pwm_m4=pwm_m4-x+y+z;


		pwm_m1 = Math_fConstrain(pwm_m1,30,980);
		pwm_m2 = Math_fConstrain(pwm_m2,30,980);
		pwm_m3 = Math_fConstrain(pwm_m3,30,980);
		pwm_m4 = Math_fConstrain(pwm_m4,30,980);



}


void motor_set(short x,short y,short z)
{
	pwm_m1=pwm_m1+x+y-z;

			pwm_m2=pwm_m2+x-y+z;

			pwm_m3=pwm_m3-x-y-z;

			pwm_m4=pwm_m4-x+y+z;


			pwm_m1 = Math_fConstrain(pwm_m1,30,980);
			pwm_m2 = Math_fConstrain(pwm_m2,30,980);
			pwm_m3 = Math_fConstrain(pwm_m3,30,980);
			pwm_m4 = Math_fConstrain(pwm_m4,30,980);

}



void stunt_timePwm(u32 tm,s16 spd)
{
	s16 x,y,z;
	s16 temp1;
	temp1=spd;
	time[TIME_STUNT_UPDATA]= clock_time();

	while(1)
	{
		if(time_checkMs(TIME_STUNT_UPDATA,tm)==1)break;

		if(sensor_getData()==1)
		{

			//if(temp1>950)temp1=950;
			//if(temp1<650)temp1=650;


			pwm_m1=temp1;
			pwm_m2=temp1;
			pwm_m3=temp1;
			pwm_m4=temp1;

		//    pid_gyro_integrator[0]=0;
		  //  pid_gyro_integrator[1]=0;
		 //   pid_integrator[0]=0;
		  //  pid_integrator[1]=0;

		    gps_x=0;

		    gps_y=0;
		    gps_z=0;

			IMU_getYawPitchRoll();
			x=pid_xyz(roll,gx_,gps_x,0);
		    y=pid_xyz(-pitch,gy_,gps_y,1);
			z=pid_xyz(0,gz_,gps_z,2);





            stunt_imu=2;
            motor_set(x,y,z);
            pwm_speedSet(pwm_m1,pwm_m2,pwm_m3,pwm_m4);
		}
	}
}






void stunt_autoPlay(void)
{
	s16 x,y,z;
	s16 temp1;
	temp1=500;
	time[TIME_STUNT_UPDATA]= clock_time();

	while(1)
	{
		if(time_checkMs(TIME_STUNT_UPDATA,500)==1)break;

		if(sensor_getData()==1)
		{

			//if(temp1>950)temp1=950;
			//if(temp1<650)temp1=650;



			pwm_m1=temp1;
			pwm_m2=temp1;
			pwm_m3=temp1;
			pwm_m4=temp1;

		//    pid_gyro_integrator[0]=0;
		  //  pid_gyro_integrator[1]=0;
		 //   pid_integrator[0]=0;
		  //  pid_integrator[1]=0;

		    if(stunt_type==2){if(IMU_Roll_<1600)break;}//IMU_Pitch_ 170
			if(stunt_type==1){if(IMU_Roll_>20000)break;}//IMU_Pitch_ 190

			if(stunt_type==3){if(IMU_Pitch_<1600)break;}//IMU_Roll_ 170
			if(stunt_type==4){if(IMU_Pitch_>20000)break;}//IMU_Roll_ 190


		    gps_x=0;

		    gps_y=0;
		    gps_z=0;

			IMU_getYawPitchRoll();
			x=pid_xyz(roll,gx_,gps_x,0);
		    y=pid_xyz(-pitch,gy_,gps_y,1);
			z=pid_xyz(0,gz_,gps_z,2);


            stunt_imu=2;
            motor_set(x,y,z);
            pwm_speedSet(pwm_m1,pwm_m2,pwm_m3,pwm_m4);
		}
	}
}



void stunt_playTurn(void)
{
	s16 x,y,z;


	s16 pwm;
	int spd;
	u32 tim;
    //int itemp;

	stunt_imu=0;


	time[TIME_STUNT_UPDATA]= clock_time();
	if(stunt_type==3){pwm=500;gps_y=2;gps_x=0;}//r
	if(stunt_type==4){pwm=500;gps_y=-2;gps_x=0;}//l

	if(stunt_type==1){pwm=500;gps_y=0;gps_x=2;} //b
	if(stunt_type==2){pwm=500;gps_y=0;gps_x=-2;}//f


	//pid_gyroSet(8,1,1);
	//pid_angleSet(90,1,1);

	tim=0;

	while(1)
	{
		//if(time_checkMs(TIME_STUNT_UPDATA,2000)==1)break;
		if(sensor_getData()==1)
		{




			IMU_getYawPitchRoll();



			tim++;
			if(tim<8)
			{
				spd=600;
			}
			//spd=spd-300;
			//if(spd<20)spd=20;
			if(spd>600)spd=600;

			//pid_angleSet(spd,1,1);



			if(stunt_imu==0)
			{
				pwm=800;//dectectData.stunt3;//400
				spd=dectectData.stunt4;//600
                if(stunt_type==1){if((IMU_Roll>8000)||(IMU_Roll<-8000))stunt_imu=1;}//IMU_Pitch
				if(stunt_type==2){if((IMU_Roll>8000)||(IMU_Roll<-8000))stunt_imu=1;}//IMU_Pitch
				if(stunt_type==3){if((IMU_Pitch>8000)||(IMU_Pitch<-8000))stunt_imu=1;}//IMU_Roll
				if(stunt_type==4){if((IMU_Pitch>8000)||(IMU_Pitch<-8000))stunt_imu=1;}//IMU_Roll

				//itemp=0;
			}
			if(stunt_imu==1)
			{
				pwm=50;//dectectData.stunt5;//200

				//spd=dectectData.stunt6;//650
				
																	//前后左右翻滚时间结束的角度
               if(stunt_type==2){spd=dectectData.stunt5;if(IMU_Roll_<17000)break;}//IMU_Pitch_ 170
			   if(stunt_type==1){spd=dectectData.stunt5;if(IMU_Roll_>19000)break;}//IMU_Pitch_ 190


			   if(stunt_type==3){spd=dectectData.stunt6;if(IMU_Pitch_<17000)break;}//IMU_Roll_ 170
			   if(stunt_type==4){spd=dectectData.stunt6;if(IMU_Pitch_>19000)break;}//IMU_Roll_ 190
				//itemp=itemp+(gx/100)+(gy/100);
				//if(itemp>dectectData.stunt8)break;

				    //if(itemp<-(dectectData.stunt8))break;

			}

		    gps_z=0;

			pwm_m1=pwm;
			pwm_m2=pwm;
			pwm_m3=pwm;
			pwm_m4=pwm;
		//    pid_gyro_integrator[0]=0;
		//    pid_gyro_integrator[1]=0;

		//    pid_integrator[0]=0;
		 //   pid_integrator[1]=0;

		  //  x=pid_xyz(roll,gx,gps_x,0);	//(横滚角，陀螺仪x，手柄x，x)
		 	//	     y=pid_xyz(-pitch,gy,gps_y,1);  //(俯仰角(负值)，陀螺仪y，手柄y,y)
		 	//	   	 z=pid_xyz(0,gz,gps_z,2);		//(0，陀螺仪z，手柄z，z)


		    x=(gx_/dectectData.stunt7)-(gps_x*spd);//-(roll*300.f);//-spd; //gx/23  17
		    y=(gy_/dectectData.stunt7)-(gps_y*spd);



		   // x=-(gps_x*spd);//-spd;
		   // y=-(gps_y*spd);

		   // x=x/15;
		  //  y=y/15;

		   // z=pid_xyz(0,gz,gps_z,2);



            motor_set(x,y,z);
            pwm_speedSet(pwm_m1,pwm_m2,pwm_m3,pwm_m4);
		}
	}
	gps_x=0;
	gps_y=0;
    gps_z=0;

   // pid_gyroSet(8,1,20);
   // pid_angleSet(22,1,100);
}

void stunt_play(void)
{
   if(stunt_type==0)return;
   if(stunt_type==10)				//翻滚之后为 10
   {
	   pid_gyro_integrator[0]=0;	//陀螺仪积分x数值
	   pid_gyro_integrator[1]=0;	//陀螺仪积分y数值

	   pid_integrator[0]=0;
	   pid_integrator[1]=0;
	   startflytime=160;
	 // pid_reset=200;
      if(time_checkMs(TIME_STUNT_UPDATA,300)==1)
      {
    	  stunt_type=stunt_type_last;
    	  stunt_type_last=0;
      }
      return;
   }
      if(stunt_bat==1)dectectData.stunt2=910;
      else dectectData.stunt2=910;//750;

      //imuKp=0.01f;//25
	   stunt_timePwm(400,dectectData.stunt2);
	   stunt_timePwm(20,50);

	   PID_AP=700;						//翻滚加速度设置
	   PID_GP=2300;
	  // PID_AP=1800;
	//   if(stunt_type==3||stunt_type==4)PID_AP=2000;
	   stunt_playTurn();
	//   stunt_autoPlay();

//	       PID_GP=2000;
//	  	   PID_GD=17000;
	   if(stunt_bat==1)dectectData.stunt8=500;
	         else dectectData.stunt8=500;
	   //stunt_timePwm(dectectData.stunt8,dectectData.stunt2);	//翻滚结束前上升函数
	   stunt_timePwm(100,800);
	   stunt_timePwm(400,900);
	   stunt_timePwm(100,800);

	   pid_normalMode();			//回复到正常模式  pid 数据

	  // stunt_timePwm(700,500);//stunt8
	  // stunt_playTurn();
	   //stunt_timePwm(400,800);


	   //pid_normalMode();

	   time[TIME_STUNT_UPDATA]= clock_time();

	   vr_rotor_tmp=0x80;
	      vr_fb_tmp=0x80;
	      vr_lr_tmp=0x80;

/*
	      if(stunt_type==1)
	     	      {
	     	        x_userSum=0;
	     	        y_userSum=9000;
	     	      }

	      if(stunt_type==2)
	      	     	      {
	      	     	        x_userSum=0;
	      	     	        y_userSum=-9000;
	      	     	      }

	      if(stunt_type==3)
	      {
	        x_userSum=5000;
	        y_userSum=0;
	      }
	      if(stunt_type==4)
	      {
	      	        x_userSum=-5000;
	      	        y_userSum=0;
	      }


	      xy_userIndex=100;
	      xy_userIndex1=100;
	      pid_reset=100;
*/

	      stunt_type=10;

}







void app_quad(void)
{

	      if(sensor_getData()==1)		//得到陀螺仪的六个数据
		  {
		     s16 x,y,z;
		     imuPersec++;

		     IMU_getYawPitchRoll();		//利用陀螺仪的六个数据，得到飞行姿态三个角度数值
		     rf_dataUpdate();			//本函数中可以得到手柄的 x,y,z



			//x=pid_xyz(-pitch,gy,gps_x,0);//x=pid_xyz(roll,gx,gps_x,0);
		    //y=pid_xyz(-roll,-gx,gps_y,1);//y=pid_xyz(-pitch,gy,gps_y,1);
		   	//z=pid_xyz(0,gz,gps_z,2);  //6881用gy_抖的厉害
		     	 	 	 	 	 	 	 	//利用前面三个函数得到 pid_xyz 函数的实参值
		     x=pid_xyz(roll,gx,gps_x,0);	//(横滚角，陀螺仪x，手柄x，x)
		     y=pid_xyz(-pitch,gy,gps_y,1);  //(俯仰角(负值)，陀螺仪y，手柄y,y)
		   	 z=pid_xyz(0,gz,gps_z,2);		//(0，陀螺仪z，手柄z，z)

		   	// x=(gx/5);
		   //	 y=(gy/5);


		     motor_Check(x,y,z);  //检测马达是否工作

		     pwm_speedSet(pwm_m1,pwm_m2,pwm_m3,pwm_m4);
             if(b_bat==0)
	         stunt_play();
         }
}


void rf_matchChn(void)
{
	static u32 chn=0;
	if(rf_lostTime<2)return;
	if(time_checkMs(TIME_RF_UPDATA,50)==0)return;

	chn++;
	if(chn==1)g_rf_chn=2;//rf_cha0;
	if(chn==2)g_rf_chn=rf_cha1;
	if(chn==3){g_rf_chn=rf_cha2;chn=0;}
	if(chn>3)chn=0;
	SetRxMode (g_rf_chn, RF_CHN_TABLE);
}

void bat_check(void)
{
	static u32 bat_time=0,p_bat_time=0,stunt_bat_time=0;
	static u32 bat_limit=0;

//	if(time_checkMs(TIME_CAMERA_PHOTO,time_cameraPhoto)==1) gpio_write (GPIO_GP19, 1);



	if(time_checkMs(TIME_LED_UPDATA,100)==0)return;
	//stunt_bat

	if((P_bat==1)&&(m_stop==1))
	{
		while(1)
		{
			if(time_checkMs(TIME_LED_UPDATA,1500)==1){pwm_speedSet(0,0,0,0); led_not(GPIO_GP24);}
		}

	}

	// if(vr_stunt_tmp==0xaa)
	 //{
	//	 gpio_write (GPIO_GP24, 0);
	//	 return;
	 //}


	 if(rf_lostTime>15)
	 {
		 //if(rf_lostTime>200)rf_lostTime=200;
		 static u32 batled=0;
		 batled++;
		 if(batled>=5)
		 		{
		 			batled=0;

		 		    led_not(GPIO_GP24);
		 		}
		 		return;
	 }

//	 if((button_1_tmp&0x08)==0x08)//led off from rc
//	 {

//		 gpio_write (GPIO_GP24, 0);
//			 		return;
//	}




		if(voltage<39)// stunt check
		{
			stunt_bat_time++;
			if(stunt_bat_time>5)stunt_bat=1;
		}
		else
		{
			stunt_bat_time=0;
		}

	if(voltage<31)// motor stop check
	{
		p_bat_time++;
		if(p_bat_time>20)P_bat=1;
	}
	else
	{
		p_bat_time=0;
	}
	if(b_bat==1)
	{
		static u32 batled=0;

		batled++;
		if(batled>10)
		{
			bat_limit++;
		    if(bat_limit>=30)P_bat=1;
			batled=0;

		    led_not(GPIO_GP24);
		}
		return;
	}

	//if(stunt_bat==1)gpio_write (GPIO_GP24,0);


	if(voltage<34)// low bat check//yfpeng低电量闪烁时间
	{
		bat_time++;
		if(bat_time>20)b_bat=1;
		//led_not(GPIO_GP24);
	}
	else
	{
		bat_time=0;
		if(b_motorProtect==1)
		{
			//if(stunt_bat==0)

			//if((button_1_tmp&0x02)==0x08)//

			if(b_noHead==0)	gpio_write (GPIO_GP24, 1);
			//else gpio_write (GPIO_GP24, 0);


		}
		else led_not(GPIO_GP24);

	}

	if(b_noHead)//no head mode
		{
			//led_not1(GPIO_GP3);
			//gpio_write (GPIO_GP24, 0);
		    static u32 batled1=0;
				batled1++;
				if(batled1==1)led_not1(GPIO_GP24);
				if(batled1==2)led_not1(GPIO_GP24);
				if(batled1==3)led_not1(GPIO_GP24);
				if(batled1==4)led_not1(GPIO_GP24);
				if(batled1==5)led_not1(GPIO_GP24);
				if(batled1==6)gpio_write (GPIO_GP24, 0);


				if(batled1>10)batled1=0;



		}
		//else gpio_write (GPIO_GP3, 0);

}





void flash_BankCheck(void)
{
//	rf_lostTime=100;
	if(flash_read(0xe000)==0x8888)
	{

	   flash_readAll();
	}
	else
	{

	   //flash_ramRst();
	   //flash_writeAll();
		rf_lostTime=200;	//第一次上电设置为200,在main_loop中执行函数flash_writeAll()
	}


}


void  user_init(void)
{
	ptr_pkt_rf = &pkt_rf;
	ptr_debug=&pkt_rf;

	tl_i2c_init(400000);
	platform_init();
	device_info_load ();

	reg_wakeup_en = FLD_WAKEUP_SRC_GPIO;
	RF_init();
	SetRxMode (g_rf_chn, RF_CHN_TABLE);
	gpio_set_func(GPIO_SWM, AS_GPIO);
    gpio_set_output_en(GPIO_SWM, 1);
    //debug_codeUpdata();

    io_init();
    i2c_init();
    sensor_initial();
    rf_lostTime=0;

    gpio_irq_cnt=sensor_read(0);

   //

    flash_BankCheck();
    gpio_write (GPIO_GP24, 1);
    time_ms(1000);

    rf_matchAddr();
    //rf_lostTime=200;
}


void main_loop(void)
{

    if((rf_lostTime>=100))//||(rf_lostTime==0))
    {
    	   sensor_offset();
    	   flash_ramRst();
    	   //if(rf_lostTime>=200) flash_ramRst();
           flash_writeAll();

    }
   vr_stunt_tmp=0;
   vr_thr_tmp=0;
   vr_rotor_tmp=0x80;
   vr_fb_tmp=0x80;
   vr_lr_tmp=0x80;


    b_gyroOffset=1;
    IMU_init();

    pwm_speedSet(0,0,0,0);
    pwm_init(5);	//7:750		6:1.5k	5:3k	4:6k
    pid_normalMode();
	rf_lostTime=0;     //初始化

	g_rf_chn=2;
	SetRxMode (g_rf_chn, RF_CHN_TABLE);
	while(1)
	{
	  RF_rcv_process(user_rcv_proc);//循环中处理
	  debug_send(vr_thr_tmp,trim_rotor_tmp,voltage);	//上位机调试功能，量产时注释掉

	  app_quad();								//四轴飞机应该程序：飞行姿态，翻滚
	  rf_matchChn();		//rf 选频段

	  //flash_saveData();

	  bat_check();


	  if(time_checkMs(TIME_COMPARTOR_UPDATA,20))
	  voltage=comparator_read(COMP_ANA5);

	  if(rf_lostTime>=100)break;


	  //if(gpio_read(GPIO_GP14)==0)gpio_write (GPIO_GP24, 0);
	  //else gpio_write (GPIO_GP24, 1);

	  if(code_updata1>50)
	  {
		 pwm_speedSet(0,0,0,0);
		 time_ms(20);
	 	 debug_codeUpdata();
	  }

	}

}











