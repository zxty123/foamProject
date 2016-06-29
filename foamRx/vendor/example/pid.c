/*
 * pid.c
 *
 *  Created on: 2015-9-8
 *      Author: Administrator
 */

#include "../../proj/tl_common.h"
#include "pid.h"
#include "rf_frame.h"

#define X 0;
#define y 1;

int PID_GP,PID_GI,PID_GD,PID_GA;
int PID_AP,PID_AI,PID_AD;

int PID_ZP,PID_ZI,PID_ZD;


float pid_integrator[3];
int pid_lastError[2];
float pid_gyro_integrator[3];
int pid_gyroLast[2];
float pid_lastAngle[2];
int Differ[2][4];
int pid_integratorZ;

extern u32 stunt_type;

extern u32 startflytime;

//float pid_integratorFloat[2];



//inline int Math_fConstrain(int value, int min, int max){
//if(value > max)value = max;
//	else if(value < min)value = min;
//return value;
//}

#define  Constrain(x,l,h) (x < l ? l:(x > h ? h : x)





void pid_gyroSet(int p, int i, int d)
{
	PID_GP=p;
	PID_GI=i;
	PID_GD=d;
}

void pid_angleSet(int p, int i, int d)
{
	PID_AP=p;
	PID_AI=i;
	PID_AD=d;
}

void pid_zSet(int p, int i, int d)
{
	PID_ZP=p;
	PID_ZI=i;
	PID_ZD=d;
}


void pid_normalMode(void)
{
	PID_GA=dectectData.g_agc;
	PID_GP=dectectData.g_p;
	PID_GI=dectectData.g_i;
	PID_GD=dectectData.g_d;

	PID_AP=dectectData.a_p;
	PID_AI=dectectData.a_i;
	PID_AD=dectectData.a_d;

	PID_ZP=dectectData.z_p;
	PID_ZI=dectectData.z_i;
		//PID_ZD=100;   //0.05


/*
	PID_GP=1500; //0.15 //p
	PID_GI=40;  //
	PID_GD=5000;  //1.3 //d

	PID_AP=2700;  //0.3
	PID_AI=50;  //100000*0.5=5000 degree
	PID_AD=50; //5

	PID_ZP=3000;  //0.29
	PID_ZI=50;   //0.02   max 5000
	//PID_ZD=100;   //0.05
*/
}


s16 pid_xyz(float pid_imu,s32 gyro,s32 pid_target,u8 type)
{

   int deltaSum;
   float error,kp,kd,ki;

   static float accgyroval[3]={0,0,0};;
        static float lastacc[3]={0,0,0};;

       accgyroval[type]=gyro;
        gyro = (float) accgyroval[type] * 0.85f +lastacc[type] * 0.15f ;
        lastacc[type] = gyro;

    //    gyro = Math_fConstrain(gyro,-15000,15000);//100

   if(type==2)
   {
   	   error=gyro-pid_target;
   	   kp = error * (float)PID_ZP * 0.0001f;

       pid_gyro_integrator[type] += error * 0.0001f * (float)PID_ZI;
   	   pid_gyro_integrator[type] = Math_fConstrain(pid_gyro_integrator[type],-280,280);//100

   	   ki=pid_gyro_integrator[type];

   	   //ki=0;
   	   error=kp+ki;

   }
   else
   {
	   //pid_imuLimit
/*
	   if(stunt_type==0)
	  	   {

	  	   startflytime++;
	  	     if(startflytime>200)
	  	     {
	  	    	startflytime=10000;
	  		  if((pid_imu>1.1f)||(pid_imu<-1.1f))
	  		  {
	  		    if(pid_imu>1.1f)pid_imu=pid_imu-1.1f;
	  		    if(pid_imu<-1.1f)pid_imu=pid_imu+1.1f;
	  		  }
	      	   else pid_imu=0;
	      	  }
	  	   }
*/

      error = pid_imu + (float)pid_target/100.0f;

      error = Math_fConstrain(error,-70,70);//50

      gyro=gyro-((s32)(pid_gyro_integrator[type]*PID_GA));


      kp = error * (float)PID_AP * 0.01f;//0.01f

      pid_integrator[type] += error * 0.0001f * (float)PID_AI;
      pid_integrator[type] = 0;//Math_fConstrain(pid_integrator[type],-100,100);//100

      pid_gyro_integrator[type] += (float)gyro * 0.0001f * (float)PID_GI;
      pid_gyro_integrator[type] = Math_fConstrain(pid_gyro_integrator[type],-500,500);//100

      ki=pid_integrator[type]+pid_gyro_integrator[type];

      kp += (float)gyro * (float)PID_GP  * 0.0001f;

      //error = gyro - pid_gyroLast[type];
     // pid_gyroLast[type] =gyro;

     // kd = error  * (float)PID_GD * 0.0001f;//陀螺新数据和旧数据误差乘以比例

      Differ[type][0] = gyro - pid_gyroLast[type];	//抑制正在进行中的动作
                    pid_gyroLast[type] = gyro;
                    deltaSum = Differ[type][0]+Differ[type][1]+Differ[type][2];
                    Differ[type][2] = Differ[type][1];
                    Differ[type][1] = Differ[type][0];
                    error=deltaSum;
                    kd = error  * (float)PID_GD * 0.0001f;




      kd += ((pid_imu-pid_lastAngle[type])*(float)PID_AD);//新旧角度误差乘以比例
      pid_lastAngle[type]=pid_imu;

      //ki=0;
     // kd=0;
      error=kp+ki+kd;

   }


   return (s16)error;



}










