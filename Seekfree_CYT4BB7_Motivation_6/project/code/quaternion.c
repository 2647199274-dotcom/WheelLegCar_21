#include "quaternion.h"

Euler_angle angle = {0.0f, 0.0f, 0.0f};

static float quaternion[4] = {1.0f, 0.0f, 0.0f, 0.0f};

static float invSqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

void Quaternion_Init(void)
{
    quaternion[0] = 1.0f;
    quaternion[1] = 0.0f;
    quaternion[2] = 0.0f;
    quaternion[3] = 0.0f;
    angle.pitch = 0.0f;
    angle.roll = 0.0f;
    angle.yaw = 0.0f;
}

void Quaternion_Update(void)
{
    const float dt = 0.004f;  //对应中断周期
    float beta = 0.012f; // 当 dt 变小时，beta 值应相应减小-如果静态漂移严重，增加 beta 到 0.03-0.05 如果动态响应慢，减小 beta 到 0.01-0.015
    float gx = imudata.gx;
    float gy = imudata.gy;
    float gz = imudata.gz;
    float ax = imudata.ax;
    float ay = imudata.ay;
    float az = imudata.az;
    
    float gyro_magnitude = sqrtf(gx*gx + gy*gy + gz*gz);
    if (gyro_magnitude > 2.0f) {                        // 快速运动
        beta = 0.006f;                                  // 减小加速度计权重
    } else {                                            // 慢速运动或静止
        beta = 0.018f;                                  // 增大加速度计权重
    }

    float q0 = quaternion[0];
    float q1 = quaternion[1];
    float q2 = quaternion[2];
    float q3 = quaternion[3];
    
    // 归一化四元数
    float norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;
    
    quaternion[0] = q0;
    quaternion[1] = q1;
    quaternion[2] = q2;
    quaternion[3] = q3;
    
    // 陀螺仪更新
    float dq0 = 0.5f * (-q1*gx - q2*gy - q3*gz) * dt;
    float dq1 = 0.5f * (q0*gx + q2*gz - q3*gy) * dt;
    float dq2 = 0.5f * (q0*gy - q1*gz + q3*gx) * dt;
    float dq3 = 0.5f * (q0*gz + q1*gy - q2*gx) * dt;
    
    // 加速度计修正
    if (ax != 0.0f || ay != 0.0f || az != 0.0f)
    {
        float acc_norm = invSqrt(ax*ax + ay*ay + az*az);
        ax *= acc_norm;
        ay *= acc_norm;
        az *= acc_norm;
        
        float vx = 2.0f * (q1*q3 - q0*q2);
        float vy = 2.0f * (q0*q1 + q2*q3);
        float vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
        
        float ex = (ay*vz - az*vy);
        float ey = (az*vx - ax*vz);
        float ez = (ax*vy - ay*vx);
        
        ex *= beta;
        ey *= beta;
        ez *= beta;
        
        gx += ex;
        gy += ey;
        gz += ez;
        
        dq0 = 0.5f * (-q1*gx - q2*gy - q3*gz) * dt;
        dq1 = 0.5f * (q0*gx + q2*gz - q3*gy) * dt;
        dq2 = 0.5f * (q0*gy - q1*gz + q3*gx) * dt;
        dq3 = 0.5f * (q0*gz + q1*gy - q2*gx) * dt;
    }
    
    quaternion[0] += dq0;
    quaternion[1] += dq1;
    quaternion[2] += dq2;
    quaternion[3] += dq3;
    
    norm = invSqrt(quaternion[0]*quaternion[0] + 
                   quaternion[1]*quaternion[1] + 
                   quaternion[2]*quaternion[2] + 
                   quaternion[3]*quaternion[3]);
    quaternion[0] *= norm;
    quaternion[1] *= norm;
    quaternion[2] *= norm;
    quaternion[3] *= norm;
}

void Quaternion_GetEuler(Euler_angle *euler)
{
    float q0 = quaternion[0];
    float q1 = quaternion[1];
    float q2 = quaternion[2];
    float q3 = quaternion[3];
    
    float roll = -asinf(2.0f * (q0*q2 - q1*q3));
    float pitch = -atan2f(2.0f * (q0*q1 + q2*q3), 
                        1.0f - 2.0f * (q1*q1 + q2*q2));
    float yaw = atan2f(2.0f * (q0*q3 + q1*q2), 
                       1.0f - 2.0f * (q2*q2 + q3*q3));
    
    euler->pitch = pitch * 180.0f / PI;
    euler->roll = roll * 180.0f / PI;
    euler->yaw = yaw * 180.0f / PI;
}
