/*
 * flashCtrl.c
 *
 *  Created on: 2016-6-22
 *      Author: chzu131
 */

#include "flashCtrl.h"
#include "rf_frame.h"
#include "../../proj/mcu/watchdog_i.h"
#include "sensor.h"
#include "../common/time.h"
#include "../../proj/drivers/flash.h"

u32 b_flash=0;


u32 flash_write(u32 addr, u16 id)
{

	u8 buf[2];

    buf[0] =id;
    buf[1] =id>>8;
   // buf[2] =id>>16;
   // buf[3] =id>>24;

	flash_write_page(addr, 2, buf);

	return 1;
}

u32 flash_read(u32 addr)
{
	u8 buf[2];
	u16 tmp;

	flash_read_page(addr, 2, buf);
	tmp = buf[0] | (buf[1]<<8);
		//|(buf[2]<<16) | (buf[3]<<24);

	return tmp;
}


u32 flash_writeAll(void)
{
	u32 address=2;
		u16 *p;
		u32 page;

	    flash_erase_sector(0xe000);
	    flash_write(0xe000,0x8888);

		p=&dectectData.g_agc;
		page=0;
		address=0xe002;
		while(1)
		{
		   wd_clear();
		   flash_write(address,p[page++]);
		   address=address+2;
		   if(page>=28)break;

		}

		 flash_write(address,(short)gyro_DCX);
		 address=address+2;
		 flash_write(address,(short)gyro_DCY);
	     address=address+2;
	     flash_write(address,(short)gyro_DCZ);
	     address=address+2;

	     flash_write(address,(short)acc_DCX);
	     address=address+2;
	     flash_write(address,(short)acc_DCY);
	     address=address+2;
	     flash_write(address,(short)acc_DCZ);
	     address=address+2;

	     return 1;
}


u32 flash_readAll(void)
{
	u32 address=2;
		u16 *p;
		u32 page;

		p=&dectectData.g_agc;
		page=0;
		address=0xe002;
		while(1)
		{
		   wd_clear();


		   p[page++]=flash_read(address);
		   address=address+2;
		   if(page>=28)break;

		}

		gyro_DCX=flash_read(address);
		address=address+2;
	    gyro_DCY=flash_read(address);
		address=address+2;
	    gyro_DCZ=flash_read(address);
		address=address+2;
	    acc_DCX=flash_read(address);
		address=address+2;
		acc_DCY=flash_read(address);
		address=address+2;
	    acc_DCZ=flash_read(address);
		address=address+2;

		return 1;
}


void flash_saveData(void)
{
	if(b_flash==0)return;
	if(time_checkMs(TIME_FLASH_UPDATA,1000)==0)return;
	b_flash=0;
	//gpio_write (GPIO_GP24, 0);
	flash_writeAll();

}

void flash_ramRst(void)
{
			   dectectData.g_agc=3;//3//陀螺仪负反馈
			   dectectData.g_p=3000;//3000;//1500;			//陀螺仪 pid 设置
			   dectectData.g_i=40;//40;
			   dectectData.g_d=12000;//15000;//5000;4000

			   dectectData.a_agc=10;//负反馈			//角度  pid 设置
			   dectectData.a_p=3500;//2700;220000  1600
			   dectectData.a_i=0; //50 150
			   dectectData.a_d=100;//50;



			   dectectData.z_p=5000;//3000			//z轴  pid 设置
			   dectectData.z_i=50;  //50
			   dectectData.z_d=11;//11
			   dectectData.z_angle=12;

			   dectectData.rc_spd1=8;//10 15		//手柄速度控制
			   dectectData.rc_spd2=5;//15
			   dectectData.rc_de1=15;				//z轴旋转速度
			   dectectData.rc_de2=15;

			   dectectData.atl_agc=0;				//高度负反馈
			   dectectData.atl_p=18;				//高度 pid 设置
			   dectectData.atl_i=19;
			   dectectData.atl_d=20;

			   dectectData.stunt1=400;//马达上升时间
			   dectectData.stunt2=1000;//pwm
			   dectectData.stunt3=600;//pwm 0~80~-80度 300
			   dectectData.stunt4=550;//speed pwm 0~80~-80度 600

			   dectectData.stunt5=600;////pwm 0~180~-180度 100
			   dectectData.stunt6=650;//speed pwm 0~180~-180度 780
			   dectectData.stunt7=13;//gyro 18
			   dectectData.stunt8=300;//自动回中的马达上升时间
}
