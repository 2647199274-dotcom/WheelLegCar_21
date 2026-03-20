/*
 * filter.h
 *
 *  Created on: 2025年12月23日
 *      Author: ZhuanZ（无密码）
 */

#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_


#include "zf_common_typedef.h"

/***********************************一阶低通滤波**********************************/
typedef struct
{
    float alpha;
    float out;
} LPF;
void LPF_Init(LPF *lpf, float alpha);
float LPF_Update(LPF *lpf, float in);


// 一阶低通滤波器
extern float alpha;// 低通滤波系数，取值范围0~1，值越小滤波效果越明显
float filter_FirstOrderLowPassFilter(float input, float lastOutput);

/***********************************一阶互补滤波**********************************/
typedef struct
{
    float alpha;
    float angle;
} ComplementaryFilter;
void Complementary_Init(ComplementaryFilter* angle_cf);
float Complementary_Update(ComplementaryFilter angle_cf,float gyro,float acc_angle,float dt);

/***********************************巴特沃斯滤波**********************************/
 #define M_PI_F 3.141592653589793f
typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;

typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;
//平均滑动滤波
typedef struct
{
    uint8 index;            //下标
    uint8 buffer_size;      //buffer大小
    float data_buffer[32];  //缓冲区
    float data_sum;         //数据和
    float data_average;     //数据平均值
}move_filter_struct;



struct KFPTypeS
{
   float P; //估算协方差
   float G; //卡尔曼增益
   float Q; //过程噪声协方差,Q增大，动态响应变快，收敛稳定性变坏
   float R; //测量噪声协方差,R增大，动态响应变慢，收敛稳定性变好
   float Output; //卡尔曼滤波器输出
}; //Kalman Filter parameter type Struct
typedef struct KFPTypeS KFPTypeS_Struct;


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



typedef KFPTypeS_Struct *KFPType_Struct;

////2.定义卡尔曼结构体参数并初始化
//KFPTypeS kfpVar =
//{
//    0.020, //估算协方差. 初始化值为 0.02
//    0, //卡尔曼增益. 初始化值为 0
//    0.001, //过程噪声协方差,Q增大，动态响应变快，收敛稳定性变坏. 初始化值为 0.001
//    1.000, //测量噪声协方差,R增大，动态响应变慢，收敛稳定性变好. 初始化值为 1
//    0 //卡尔曼滤波器输出. 初始化值为 0
//};


float KalmanFilter( float input);
float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter);
void Yijielvbo(float angle,float angle_m, float gyro_m);
void move_filter_calc(move_filter_struct *move_filter, int32 new_data);
void move_filter_init(move_filter_struct *move_filter);
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF);

extern Butter_BufferData gyro_filter_buf[3],accel_filter_buf[3];



#endif /* CODE_FILTER_H_ */
