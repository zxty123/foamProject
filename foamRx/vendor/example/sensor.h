/*
 * SENSOR_H_.h
 *
 *  Created on: 2015-9-1
 *      Author: Administrator
 */

#ifndef SENSOR_H_
#define SENSOR_H_

extern s16 ax_,ay_,az_,gx_,gy_,gz_;
extern s16 ax,ay,az,gx,gy,gz;

extern int gyro_DCX,gyro_DCY,gyro_DCZ;
extern int acc_DCX,acc_DCY,acc_DCZ;
extern u32 b_gyroOffset;


void sensor_initial(void);
u8 sensor_read(u8 udata);
u8 sensor_getData(void);
void sensor_offset(void);
void   sensor_getMotion6(float *accgyroval);

#endif /* SENSOR_H_ */
