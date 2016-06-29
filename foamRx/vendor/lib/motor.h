/*
 * motor.h
 *
 *  Created on: 2015-8-30
 *      Author: Administrator
 */

#ifndef MOTOR_H_
#define MOTOR_H_

extern s16 s_pwmM1;
extern s16 s_pwmM2;
extern s16 s_pwmM3;
extern s16 s_pwmM4;
extern s16 s_pwmM1int;
extern s16 s_pwmM2int;
extern s16 s_pwmM3int;
extern s16 s_pwmM4int;
extern u32 b_motorPwm;

void pwm_init(u32 config);
//void pwm_speedSet(void);

void pwm_speedSet(u32 pwmM1, u32 pwmM2, u32 pwmM3, u32 pwmM4 );



#endif /* MOTOR_H_ */
