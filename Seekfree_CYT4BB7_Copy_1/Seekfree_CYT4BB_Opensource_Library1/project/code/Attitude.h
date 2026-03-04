#ifndef _ATTITUDE_H_
#define _ATTITUDE_H_

#include "zf_common_headfile.h"


void Attitude_Init(void);
void Attitude_calibration(void);
void Attitude_filter(void);
void Attitude_get(void);

typedef struct
{
    float ax; // 校准加速度(原始单位）
    float ay; 
    float az; 
    float gx; // 校准角速度（真实rad/s）
    float gy; 
    float gz; 
}Imu_data;

extern Imu_data imudata;




#endif