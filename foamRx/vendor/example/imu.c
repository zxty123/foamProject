


#include "../../proj/tl_common.h"
//#include "mathft.h"
#include "../lib/mathft.h"
#include "sensor.h"
#include "../common/time.h"

#define M_PI  (float)3.1415926535f

float exInt, eyInt, ezInt;  // ????
float q0, q1, q2, q3; // ?????
float integralFBhand,handdiff;
unsigned int lastUpdate, now; // ?????? ?? us
unsigned short sysytem_time_ms=0;
int IMU_Pitch, IMU_Roll, IMU_Yaw;
int IMU_Pitch_, IMU_Roll_, IMU_Yaw_;
float  IMU_GYROx, IMU_GYROy, IMU_GYROz;
unsigned char IMU_inited = 0;
unsigned short imu_clce =0;

float pitch, roll,yaw;
float imuKp,imuKi=0.00f;

int gps_pitch,gps_roll;

extern u32 stunt_type;

float stuntP=0;




void IMU_init(void)		//四元数初始设置，用于调整飞行姿态
{
	 //   pitch=0;
		//roll=0;
		//imuKp=25.0f;//25
		//imuKi=0.0f;//0053;//0.0053;//0.0053f;//0.053

		q0 = 1.0f;  //??????
	  	q1 = 0.0f;
	  	q2 = 0.0f;
	  	q3 = 0.0f;
	  	exInt = 0.0f;
	  	eyInt = 0.0f;
	  	ezInt = 0.0f;
}



/**************************????********************************************
*????:	   void IMU_AHRSupdate
*?  ?:	 ??AHRS ?????
????: ???????
????:??
*******************************************************************************/
#define Kp 0.3f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.53f   // integral gain governs rate of convergence of gyroscope biases


void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {

  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez,halfT;
  float temp0,temp1,temp2,temp3;

  //halfT = (float)time_getUs(TIME_AHRS_UPDATA) / 2000000.f;
  //halfT=0.00125f;
  int t_tmp = time_getUs(TIME_AHRS_UPDATA);
 // halfT = (float)t_tmp/ 2000000.f;
  halfT = (float)t_tmp/ (2000000.f-stuntP);

  norm = invSqrt(ax*ax + ay*ay + az*az);
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;


  // estimated direction of gravity and flux (v and w)
  vx = 2.0f*(q1*q3 - q0*q2);
  vy = 2.0f*(q0*q1 + q2*q3);
  vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy);// + (my*wz - mz*wy);
  ey = (az*vx - ax*vz);// + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx);// + (mx*wy - my*wx);

if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
  exInt = exInt + ex * imuKi * halfT;
  eyInt = eyInt + ey * imuKi * halfT;
  ezInt = ezInt + ez * imuKi * halfT;

  // adjusted gyroscope measurements
  gx = gx + (imuKp*ex + exInt);
  gy = gy + (imuKp*ey + eyInt);
  gz = gz;

  }


  // integrate quaternion rate and normalise
  temp0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  temp1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  temp2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  temp3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = invSqrt(temp0*temp0 + temp1*temp1 + temp2*temp2 + temp3*temp3);
  q0 = temp0 * norm;
  q1 = temp1 * norm;
  q2 = temp2 * norm;
  q3 = temp3 * norm;

}



/*
#define sampleFreq    200.0f        // sample frequency in Hz

#define betaDef    0.05f        // 2 * proportional gain


//volatile float beta = imuKp;

void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

//	float sampleFreq;
//	int t_tmp = time_getUs(TIME_AHRS_UPDATA);
//	sampleFreq = (0.004f-stuntP); //(float)t_tmp/ 1000000.f;
	//sampleFreq=1.0f/sampleFreq;


	//float sampleFreq=0.00004f;
	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
				_4q0 = 4.0f * q0;
				_4q1 = 4.0f * q1;
				_4q2 = 4.0f * q2;
				_8q1 = 8.0f * q1;
				_8q2 = 8.0f * q2;
				q0q0 = q0 * q0;
				q1q1 = q1 * q1;
				q2q2 = q2 * q2;
				q3q3 = q3 * q3;

				// Gradient decent algorithm corrective step
				s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
				s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
				s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
				s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
				recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
				s0 *= recipNorm;
						s1 *= recipNorm;
						s2 *= recipNorm;
						s3 *= recipNorm;

						// Apply feedback step
						qDot1 -= imuKp * s0;
						qDot2 -= imuKp * s1;
						qDot3 -= imuKp * s2;
						qDot4 -= imuKp * s3;
					}

					// Integrate rate of change of quaternion to yield quaternion
					q0 += qDot1 *  (1.0f/sampleFreq);
					q1 += qDot2 *  (1.0f/sampleFreq);
					q2 += qDot3 *  (1.0f/sampleFreq);
					q3 += qDot4 *  (1.0f/sampleFreq);

					// Normalise quaternion
					recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
					q0 *= recipNorm;
					q1 *= recipNorm;
					q2 *= recipNorm;
					q3 *= recipNorm;
}
*/




#define new_weight 0.35f
#define old_weight 0.65f

void IMU_getValues(float * values) {
	float accgyroval[6];
	static  float lastacc[3]= {0,0,0};
	int i;


	sensor_getMotion6(accgyroval);	//得到陀螺仪的六个数据放入数组


	for(i = 0; i<6; i++) {
      if(i < 3) {
       // values[i] = (float) accgyroval[i] * new_weight +lastacc[i] * old_weight ;
		//lastacc[i] = values[i];
		//values[i]=values[i];

    	  values[i]=accgyroval[i];

      }
      else {
        values[i] = ((float) accgyroval[i]) /16.4f;//16.4f; //?????

      }
    }


}



float mygetqval[6];	//??????????????
void IMU_getQ(float * q) {

  IMU_getValues(mygetqval);

	//得到陀螺仪数据:gx_,gy_,gz_ 是角度值，存放在 mygetqval[3],mygetqval[4],mygetqval[5]，单位是角度。
	//得到加速度数据:ax_,ay_,az_ 是角速度数据，存放在 mygetqval[0],mygetqval[1],mygetqval[2]，单位是弧度。

    //得到陀螺仪数据是角度值，单位是角度，1度=M_PI/180弧度，将函数前三个实参由角度转换为弧度 。

 IMU_AHRSupdate(mygetqval[3] * M_PI/180, mygetqval[4] * M_PI/180, mygetqval[5] * M_PI/180,
   mygetqval[0], mygetqval[1], mygetqval[2]);

  q[0] = q0; //?????
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
}



float safe_asin(float v)
{
//	if (isnan(v)) {
//		return 0.0;
//	}
	if (v >= 1.0) {
		return M_PI/2;
	}
	if (v <= -1.0) {
		return -M_PI/2;
	}
	return asin_f(v);
}
extern volatile float dbg_imu;

void IMU_getYawPitchRoll(void) {
  static float q[4]; // ???

  IMU_getQ(q); //???????


//  IMU_Roll=(int)(20000.f *(q[2] * q[3] + q[0] * q[1]));
//  IMU_Pitch=(int)(20000.f*(q[1]*q[3]-q[0]*q[2]));

  //IMU_Yaw=(int)(20000*(q[1]*q[1] + q[2]*q[2]));


	//得到陀螺仪数据:gx_,gy_,gz_ 是角度值，单位是角度。
	//得到加速度数据:ax_,ay_,az_ 是角速度数据，单位是弧度。

    //反三角函数得到是弧度数据，1弧度=180度/M_PI，将弧度转换为角度 。

  roll = (atan2_f(2.0f*(q[0]*q[1] + q[2]*q[3]),
                        1.0f - 2.0f*(q[1]*q[1] + q[2]*q[2])))* 180.0f/M_PI;
  IMU_Roll=(int)(roll*100.0f);


  pitch = (atan2_f(2.0f*(q[1]*q[3] - q[0]*q[2]),
                         1.0f - 2.0f*(q[1]*q[1] + q[2]*q[2])))* 180.0f/M_PI;

  //pitch = asin_f(2.0f*(q[1]*q[3] - q[0]*q[2]))* 180.0f/M_PI;

  IMU_Pitch=(int)(pitch*100.0f);


  yaw = -atan2_f(2.0f*(q[0]*q[3] + q[1]*q[2]),
                      1.0f - 2.0f*(q[2]*q[2] + q[3]*q[3]))* 180.0f/M_PI;

  if(yaw <0)yaw +=360.0f;
  yaw=-yaw;

  IMU_Yaw=(int)(yaw*100.0f);

  IMU_Pitch_=IMU_Pitch;
  IMU_Roll_=IMU_Roll;
  if(IMU_Pitch_ <0)IMU_Pitch_+=36000;
  if(IMU_Roll_ <0)IMU_Roll_+=36000;

  if(stunt_type!=0)
  {
	  pitch = asin_f(2.0f*(q[1]*q[3] - q[0]*q[2]))* 180.0f/M_PI;
	  pitch=pitch*1.5f;
	 roll = asin_f(2.0f*(q[0]*q[1] + q[2]*q[3]))* 180.0f/M_PI;
	 roll=roll*1.5f;
	  if(stunt_type==1)stuntP=0;//-30000.0f;;  //f IMU_Pitch_  f
	  	  			if(stunt_type==2)stuntP=0.0f;;//b IMU_Pitch_ 45 b
	  	  			if(stunt_type==3)stuntP=-0.0f;;// IMU_Roll_ 3000 r
	  	              if(stunt_type==4)stuntP=0.0f;  // IMU_Roll_  l
  }
  else stuntP=0.0f;


}


void IMU_noHeadMode(int x,int y)
{

   float sin_yaw = sin_f(yaw * 0.0174532925f);	//?????
   float cos_yaw = cos_f(yaw * 0.0174532925f);	//???????? ???cos

   gps_pitch = (int)((x*cos_yaw + y*sin_yaw) );

   gps_roll = (int)((x*sin_yaw - y*cos_yaw) );

}







