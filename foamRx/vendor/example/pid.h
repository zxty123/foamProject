/*
 * pid.h
 *
 *  Created on: 2015-9-8
 *      Author: Administrator
 */

#ifndef PID_H_
#define PID_H_

extern float pid_integrator[3];
//int pid_lastError[2];
extern float pid_gyro_integrator[3];
//int pid_gyroLast[2];
//int pid_lastAngle[2];
//int Differ[2][4];
extern int pid_integratorZ;
extern int PID_AP,PID_AI,PID_AD;
extern int PID_GP,PID_GI,PID_GD,PID_GA;
extern int PID_ZP,PID_ZI;

#define Math_fConstrain(val, min, max)  ((val>max)? max:(val<min)? min:val)

//extern int Math_fConstrain(int value, int min, int max);

void pid_gyroSet(int p, int i, int d);
void pid_angleSet(int p, int i, int d);
void pid_zSet(int p, int i, int d);

void pid_normalMode(void);

s16 pid_xyz(float pid_imu,s32 gyro,s32 pid_target,u8 type);
//s16 pid_xy(s32 pid_imu,s32 gyro,s32 pid_target,u8 type);
//s16 pid_z(s32 gyro,s32 target);

#endif /* PID_H_ */
