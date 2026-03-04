#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_

#include "zf_common_headfile.h"


// 一阶低通滤波器
extern float alpha;// 低通滤波系数，取值范围0~1，值越小滤波效果越明显
float filter_FirstOrderLowPassFilter(float input, float lastOutput);


//卡尔曼滤波器 
typedef struct
{
    float LastP;  // 上一时刻的状态方差（或协方差）
    float Now_P;  // 当前时刻的状态方差（或协方差）
    float out;    // 滤波器的输出值，即估计的状态
    float Kg;     // 卡尔曼增益，用于调节预测值和测量值之间的权重
    float Q;      // 过程噪声的方差，反映系统模型的不确定性
    float R;      // 测量噪声的方差，反映测量过程的不确定性
} KalmanFilter_Struct;

extern KalmanFilter_Struct kfs1[3];//加速度滤波
extern KalmanFilter_Struct kfs2[3];//欧拉角滤波
double filter_KalmanFilter(KalmanFilter_Struct *kf, double input);

#endif
