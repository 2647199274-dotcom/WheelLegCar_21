#include "quaternion.h"


Quaternion_AHRS ahrs = {
    .q = {1.0f, 0.0f, 0.0f, 0.0f},
    .euler = {0.0f, 0.0f, 0.0f},
    .euler_deg = {0.0f, 0.0f, 0.0f},
    .dt = 0.005f,                        // 匹配采样周期
    .gyro_bias = {0.0f, 0.0f, 0.0f},
    .kp = 0.5f,                          // 比例增益
    .ki = 0.001f,                        // 积分增益
    .exInt = 0.0f,
    .eyInt = 0.0f,
    .ezInt = 0.0f
};
EulerAngle angle= {0.0f, 0.0f, 0.0f};

static float yaw_integral_rad = 0.0f;// Yaw角纯积分累加值


// 快速平方根倒数
static float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


/**
 * @brief 四元数初始化
 */
void Quaternion_Init(void)
{
    ahrs.q.q0 = 1.0f;
    ahrs.q.q1 = 0.0f;
    ahrs.q.q2 = 0.0f;
    ahrs.q.q3 = 0.0f;
    
    ahrs.exInt = 0.0f;
    ahrs.eyInt = 0.0f;
    ahrs.ezInt = 0.0f;
    
    ahrs.euler.roll = 0.0f;
    ahrs.euler.pitch = 0.0f;
    ahrs.euler.yaw = 0.0f;
    
    ahrs.euler_deg.roll = 0.0f;
    ahrs.euler_deg.pitch = 0.0f;
    ahrs.euler_deg.yaw = 0.0f;
    
    yaw_integral_rad = 0.0f;
}


/**
 * @brief 四元数更新
 */
void Quaternion_Update(void)
{
    float gx = imudata.gx;
    float gy = imudata.gy;
    float gz = imudata.gz;
    
    float q0 = ahrs.q.q0;
    float q1 = ahrs.q.q1;
    float q2 = ahrs.q.q2;
    float q3 = ahrs.q.q3;
    
    float dt = ahrs.dt;
    
    // 一阶龙格-库塔法更新四元数
    // dq/dt = 0.5 * q ? [0, gx, gy, gz]
    float q0_dot = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    float q1_dot = 0.5f * ( q0 * gx + q2 * gz - q3 * gy);
    float q2_dot = 0.5f * ( q0 * gy - q1 * gz + q3 * gx);
    float q3_dot = 0.5f * ( q0 * gz + q1 * gy - q2 * gx);
    
    // 积分
    q0 += q0_dot * dt;
    q1 += q1_dot * dt;
    q2 += q2_dot * dt;
    q3 += q3_dot * dt;
    
    // 归一化
    float norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;
    
    ahrs.q.q0 = q0;
    ahrs.q.q1 = q1;
    ahrs.q.q2 = q2;
    ahrs.q.q3 = q3;
    
    // 计算欧拉角
    // roll (x-axis rotation)
    ahrs.euler.roll = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2));
    
    // pitch (y-axis rotation)
    float sin_pitch = 2.0f * (q0 * q2 - q3 * q1);
    if (sin_pitch > 1.0f) sin_pitch = 1.0f;
    if (sin_pitch < -1.0f) sin_pitch = -1.0f;
    ahrs.euler.pitch = asinf(sin_pitch);
    
    // yaw (z-axis rotation)
    //ahrs.euler.yaw = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3));
    
    yaw_integral_rad += gz * dt;  
    while (yaw_integral_rad > PI) yaw_integral_rad -= 2 * PI;
    while (yaw_integral_rad < -PI) yaw_integral_rad += 2 * PI;
    ahrs.euler.yaw = yaw_integral_rad; 
    
    ahrs.euler_deg.pitch = -ahrs.euler.roll * 180.0f / PI;
    ahrs.euler_deg.roll  = -ahrs.euler.pitch * 180.0f / PI;//交换pr取负
    ahrs.euler_deg.yaw   = ahrs.euler.yaw * 180.0f / PI; 
}


/**
 * @brief 获取欧拉角数据
 */
void Quaternion_GetEuler(EulerAngle *angle)
{
    if (angle != NULL)
    {
        angle->roll  = ahrs.euler_deg.roll;
        angle->pitch = ahrs.euler_deg.pitch;
        angle->yaw   = ahrs.euler_deg.yaw;
    }
}