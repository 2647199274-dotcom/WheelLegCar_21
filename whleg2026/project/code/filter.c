#include "filter.h"


// 一阶低通滤波器
float alpha = 0.15;
float filter_FirstOrderLowPassFilter(float input, float lastOutput)
{
    float output = alpha * input + (1.0f - alpha) * lastOutput;
    return output;
}


// 卡尔曼滤波器
KalmanFilter_Struct kfs1[3] = {
    {0.02, 0, 0, 0, 0.001, 0.543},
    {0.02, 0, 0, 0, 0.001, 0.543},
    {0.02, 0, 9.80, 0, 0.001, 0.543}
};

KalmanFilter_Struct kfs2[3] = {
    {0.02, 0, 0, 0, 0.01, 0.3},
    {0.02, 0, 0, 0, 0.01, 0.3},
    {0.02, 0, 0, 0, 0.01, 0.3}
};

double filter_KalmanFilter(KalmanFilter_Struct *kf, double input)
{
    // 预测当前状态方差
    kf->Now_P = kf->LastP + kf->Q;
    // 计算卡尔曼增益
    kf->Kg = kf->Now_P / (kf->Now_P + kf->R);
    // 更新状态估计值
    kf->out = kf->out + kf->Kg * (input - kf->out);
    // 更新状态方差，用于下一时刻
    kf->LastP = (1 - kf->Kg) * kf->Now_P;

    return kf->out;
}