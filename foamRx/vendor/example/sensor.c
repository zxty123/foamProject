/*
 * sensor.c
 *
 *  Created on: 2015-9-1
 *      Author: Administrator
 */


#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"

#include "../common/time.h"


#if USING_HW_I2C
#include "../../proj/drivers/tl_i2c_master.h"
#define i2c_read       tl_i2c_read
#define i2c_write      tl_i2c_write
#define i2c_burst_read tl_i2c_burst_read
#else
#include "../../proj/drivers/i2c.h"
#endif

s16 ax_,ay_,az_,gx_,gy_,gz_;
s16 ax,ay,az,gx,gy,gz;
int gyro_DCX,gyro_DCY,gyro_DCZ;
int acc_DCX,acc_DCY,acc_DCZ;

u32 b_gyroOffset=0;


//
#define GYRO_LEN		3//2
unsigned char	Gyro_Buf_Index = 0;
unsigned char F_Gyro_Start_Aver = 0;
int hwAR_GyroData_Buf0[GYRO_LEN];
int hwAR_GyroData_Buf1[GYRO_LEN];
int hwAR_GyroData_Buf2[GYRO_LEN];


#define GSEN_LEN		15//;//5//20
unsigned char	Gsen_Buf_Index = 0;
unsigned char F_Gsen_Start_Aver = 0;
short hwAR_GsenData_Buf0[GSEN_LEN];
short hwAR_GsenData_Buf1[GSEN_LEN];
short hwAR_GsenData_Buf2[GSEN_LEN];

int sum_hwAR_GsenData_Buf0;
int sum_hwAR_GsenData_Buf1;
int sum_hwAR_GsenData_Buf2;


void sensor_gyroFiliter(void)
{

	long temp = 0,temp1 = 0,temp2 = 0;
	unsigned char i;


   if(Gyro_Buf_Index == GYRO_LEN)							//?????Buffer
	{
  	Gyro_Buf_Index = 0;
		F_Gyro_Start_Aver = 1;
	}
	hwAR_GyroData_Buf0[Gyro_Buf_Index] = gx_;
	hwAR_GyroData_Buf1[Gyro_Buf_Index] = gy_;
	hwAR_GyroData_Buf2[Gyro_Buf_Index] = gz_;

	Gyro_Buf_Index++;
	//????????????
	if(F_Gyro_Start_Aver == 1)
	{
		for(i=0;i<GYRO_LEN;i++)
			{
				temp += hwAR_GyroData_Buf0[i];
				temp1 += hwAR_GyroData_Buf1[i];
				temp2 += hwAR_GyroData_Buf2[i];
			}
			temp = temp/GYRO_LEN;
			temp1 = temp1/GYRO_LEN;
			temp2 = temp2/GYRO_LEN;

			gx_ =(short)temp;
			gy_ =(short)temp1;
			gz_ =(short)temp2;


	}


}



void sensor_gSensorFiliter(void)
{

   if(Gsen_Buf_Index == GSEN_LEN)							//?????Buffer
	{
  	    Gsen_Buf_Index = 0;
		F_Gsen_Start_Aver = 1;
	}

    if(F_Gsen_Start_Aver){
		sum_hwAR_GsenData_Buf0 -=(int)hwAR_GsenData_Buf0[Gsen_Buf_Index];
		sum_hwAR_GsenData_Buf1 -=(int)hwAR_GsenData_Buf1[Gsen_Buf_Index];
		sum_hwAR_GsenData_Buf2 -=(int)hwAR_GsenData_Buf2[Gsen_Buf_Index];

    }
	hwAR_GsenData_Buf0[Gsen_Buf_Index] = ax_;
	hwAR_GsenData_Buf1[Gsen_Buf_Index] = ay_;
	hwAR_GsenData_Buf2[Gsen_Buf_Index] = az_;

	Gsen_Buf_Index++;

	sum_hwAR_GsenData_Buf0 +=(int)ax_;
	sum_hwAR_GsenData_Buf1 +=(int)ay_;
	sum_hwAR_GsenData_Buf2 +=(int)az_;

	if(F_Gsen_Start_Aver == 1)
	{
		ax_ = sum_hwAR_GsenData_Buf0/GSEN_LEN;
		ay_ = sum_hwAR_GsenData_Buf1/GSEN_LEN;
		az_ = sum_hwAR_GsenData_Buf2/GSEN_LEN;
	}

}






void sensor_initial(void)
{
	time_ms(30);

	//acc
	i2c_write(0x32,0x0f,0x0c);//acc range
	i2c_write(0x32,0x10,0x02);//acc range

	//gyro
	i2c_write(0xd2,0x0f,0x00);//gyro range
	i2c_write(0xd2,0x10,0x02);//gyro bw


	 i2c_write(0xd2,0x15,0x80);	 //intrupter setting
	 i2c_write(0xd2,0x16,0x00);	 //intrupter setting
	 //i2c_write(0xd2,0x1e,0x00);	 //intrupter setting
	 //i2c_write(0xd2,0x21,0x80);

	/*
		  i2c_write(0xd0,0x7e,0x11);  //cmd
		  time_ms(30);
		  i2c_write(0xd0,0x7e,0x15);  //cmd
		  time_ms(30);
		  i2c_write(0xd0,0x43,0x00);//gyro range
		  time_ms(1);
		  i2c_write(0xd0,0x42,0x2a);//gyro conf
		  time_ms(1);
		  i2c_write(0xd0,0x41,0x0c);//acc range
		  time_ms(1);
		  i2c_write(0xd0,0x40,0x2a);//acc conf
		  time_ms(1);
	      i2c_write(0xd0,0x45,0x00);//fifi downs
	      time_ms(1);
	      i2c_write(0xd0,0x47,0x00);	 //off filo
	      time_ms(1);
	      i2c_write(0xd0,0x50,0x00);	 //intrupter setting
	      time_ms(1);
		  i2c_write(0xd0,0x51,0x00);	 //intrupter setting
		  time_ms(1);
		  i2c_write(0xd0,0x52,0x00);	 //intrupter setting
		  time_ms(1);

		  */
}

u8 sensor_read(u8 udata)
{
   return i2c_read(0x32,udata);
}


u8 sensor_getData(void)		//得到陀螺仪的六个数据
{
	u8 temp[12];
		if((i2c_read(0xd2,0x0a)&0x80)==0)return 0;
		//i2c_write(0xd2,0x21,0x80);

	//	if(time_checkUs(TIME_SENSOR_UPDATA,10000)==0)return 0;

		//return 1;
		//i2c_burst_read(0xd0,0x0c,&temp[6],6);

		i2c_burst_read(0xd2,0x02,&temp[6],6);
		i2c_burst_read(0x32,0x02,&temp[0],6);


		//得到陀螺仪数据:gx_,gy_,gz_ 是角度值，单位是角度。
		//得到加速度数据:ax_,ay_,az_ 是角速度数据，单位是弧度。

		    if(b_gyroOffset==0)
		    {
		       ax_	=((short)(temp[0]+(0x100*temp[1])));
		       ay_	=((short)(temp[2]+(0x100*temp[3])));
		       az_	=((short)(temp[4]+(0x100*temp[5])));

		       gx_  =-((short)(temp[6]+(0x100*temp[7])));//- gyro_DCX;
		       gy_	=-((short)(temp[8]+(0x100*temp[9])));//- gyro_DCY;
		       gz_	=-((short)(temp[10]+(0x100*temp[11])));//- gyro_DCZ;
		    }
		    else
		    {						//校正时需要调整陀螺仪、加速度的  xyz
		    	ax_	=((short)(temp[0]+(0x100*temp[1])))- (short)acc_DCX;
		    	ay_	=((short)(temp[2]+(0x100*temp[3])))- (short)acc_DCY;
		    	az_	=((short)(temp[4]+(0x100*temp[5])))- (short)acc_DCZ+2048;

		       gx_	=-(((short)(temp[6]+(0x100*temp[7])))- (short)gyro_DCX);
		       gy_	=-(((short)(temp[8]+(0x100*temp[9])))+(short)gyro_DCY);
		       gz_	=-(((short)(temp[10]+(0x100*temp[11])))- (short)gyro_DCZ);
		    }


		//  ax_	=((short)(temp[0]+(0x100*temp[1])));
		//		       ay_	=((short)(temp[2]+(0x100*temp[3])));
		//		       az_	=((short)(temp[4]+(0x100*temp[5])));

		//		       gx_  =((short)(temp[6]+(0x100*temp[7])));//- gyro_DCX;
		//		       gy_	=((short)(temp[8]+(0x100*temp[9])));//- gyro_DCY;
		//		       gz_	=((short)(temp[10]+(0x100*temp[11])));//- gyro_DCZ;


	    ax=ax_;
	    ay=ay_;
	    az=az_;
	    gx=gx_;
	    gy=gy_;
	    gz=gz_;

	    if(b_gyroOffset==1)
	    {
	    	sensor_gyroFiliter();	//陀螺仪平滑滤波
	    	sensor_gSensorFiliter();//加速度平滑滤波
	    }
/*
	    if(gz_<0)
	    {
	       if(gz_>-25){gz_=0;}
	    }
	    else
	    {
	        if(gz_<25){gz_=0;}
	    }
*/

	    return(1);
}


void   sensor_getMotion6(float *accgyroval)
{

	*(accgyroval+0) = ax_;
	*(accgyroval+1) = ay_;
	*(accgyroval+2) = az_;
	*(accgyroval+3) = gx_;
	*(accgyroval+4) = gy_;
	*(accgyroval+5) = gz_;

	return;

}

void sensor_offset(void)
{
    #define ERROR_TOLERANCE	30

	int gyro_samples = 0;
		b_gyroOffset=0;
		 gyro_samples = 0;
		while(gyro_samples<150)
		{
			if(time_checkMs(TIME_LED_UPDATA,50)==1)led_not(GPIO_GP24);
			if(sensor_getData()==1)		//得到陀螺仪的六个数据
			{
				if (gyro_samples==0)
				{
					//always keep the first sample
					gyro_DCX = (int)gx;
					gyro_DCY = (int)gy;
					gyro_DCZ = (int)gz;
				}
				else
				{
					if ((gyro_DCX/gyro_samples - (int)gx) > ERROR_TOLERANCE	||
					(gyro_DCY/gyro_samples - (int)gy) > ERROR_TOLERANCE	||
					(gyro_DCZ/gyro_samples - (int)gz) > ERROR_TOLERANCE)
					{
						//restart the sampling
						gyro_samples=0;
						gyro_DCX = (int)gx;
						gyro_DCY = (int)gy;
						gyro_DCZ = (int)gz;
					}
					else
				    {
						gyro_DCX += (int)gx;
						gyro_DCY += (int)gy;
						gyro_DCZ += (int)gz;

					}
				}
				gyro_samples++;


			}
		}
		gyro_samples=0;
		gyro_DCX=0;
		gyro_DCY=0;
		gyro_DCZ=0;
		while(gyro_samples<150)		//150次取陀螺仪平均值
		{
			if(time_checkMs(TIME_LED_UPDATA,50)==1)led_not(GPIO_GP24);
			if(sensor_getData()==1)
			{
		    gyro_DCX += (int)gx;
			gyro_DCY += (int)gy;
			gyro_DCZ += (int)gz;
			gyro_samples++;
			//if(gyro_samples>=100)break;
			}

		}
		gyro_DCX = gyro_DCX/150;
		gyro_DCY = gyro_DCY/150;
		gyro_DCZ = gyro_DCZ/150;


		       gyro_samples=0;
		       acc_DCX=0;
		       acc_DCY=0;
		       acc_DCZ=0;
				while(gyro_samples<150)		//150次取加速度平均值
				{
					if(time_checkMs(TIME_LED_UPDATA,50)==1)led_not(GPIO_GP24);
					if(sensor_getData()==1)
					{
						acc_DCX += (int)ax;
						acc_DCY += (int)ay;
						acc_DCZ += (int)az;
					    gyro_samples++;
					}

				}
				acc_DCX = acc_DCX/150;
				acc_DCY = acc_DCY/150;
				acc_DCZ = acc_DCZ/150;

		b_gyroOffset=1;			//内八校正时，执行此函数:sensor_offset(void)
		gpio_write (GPIO_GP24, 1);


}



