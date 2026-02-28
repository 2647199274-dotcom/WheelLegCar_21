#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "zf_common_headfile.h"

// 四元数结构体
typedef struct
{
    float q0;   // w
    float q1;   // x
    float q2;   // y
    float q3;   // z
} Quaternion;

typedef struct
{
    float pitch;     // 俯仰角 
    float roll;    // 横滚角  
    float yaw;      // 偏航角 
} EulerAngle;

// 四元数算法结构体
typedef struct
{
    Quaternion q;           // 当前四元数
    EulerAngle euler;       // 当前欧拉角（弧度）
    EulerAngle euler_deg;   // 当前欧拉角（度）
    float dt;               // 采样周期（秒）
    float gyro_bias[3];     // 陀螺仪零偏补偿
    float kp;               // 比例增益
    float ki;               // 积分增益
    float exInt;            // 误差积分X
    float eyInt;            // 误差积分Y
    float ezInt;            // 误差积分Z
} Quaternion_AHRS;

extern Quaternion_AHRS ahrs;
extern EulerAngle angle;

void Quaternion_Init(void);
void Quaternion_Update(void);
void Quaternion_GetEuler(EulerAngle *angle);

#endif