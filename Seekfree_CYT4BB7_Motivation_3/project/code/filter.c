#include"filter.h"

//----------------------------------------------------------------
//  @brief      一阶低通滤波 主要是简单且速度快，延迟低
//  @param
//  @param
//  @return     float       滤波后的角度
//----------------------------------------------------------------
LPF gyro_lpf[3];
LPF acc_lpf[3];

void LPF_Init(LPF *lpf, float alpha)
{
    lpf->alpha = alpha;
    lpf->out = 0.0f;
}

float LPF_Update(LPF *lpf, float in)
{
    lpf->out += lpf->alpha * (in - lpf->out);
    return lpf->out;
}
//------------------------自制一介互补滤波-------------------------

ComplementaryFilter angle_cf_buf[3];
void Complementary_Init(ComplementaryFilter* angle_cf)
{
    angle_cf->alpha = 0.98f;   // 200Hz 下通用
    angle_cf->angle = 0.02f;
}
float Complementary_Update(ComplementaryFilter angle_cf,float gyro,float acc_angle,float dt)
{
    angle_cf.angle = angle_cf.alpha * (angle_cf.angle + gyro * dt) +
         (1.0f - angle_cf.alpha) * acc_angle;

    return angle_cf.angle;
}

// 一阶低通滤波器
float alpha = 0.15;
float filter_FirstOrderLowPassFilter(float input, float lastOutput)
{
    float output = alpha * input + (1.0f - alpha) * lastOutput;
    return output;
}

/*************************************************
函数名:    LPButterworth
说明: 二阶巴特沃斯低通滤波器
入口: float curr_input    当前输入加速度计
出口: 无
备注: 2阶Butterworth低通滤波器
        Cutoff Fre. 5Hz
*************************************************/

Butter_BufferData gyro_filter_buf[3],accel_filter_buf[3];

float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
        /* 加速度计Butterworth滤波 */
    /* 获取最新x(n) */
        static int LPB_Cnt=0;
        Buffer->Input_Butter[2]=curr_input;
        if(LPB_Cnt>=100)
        {
    /* Butterworth滤波 */
        Buffer->Output_Butter[2]=
         Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
        +Parameter->b[2] * Buffer->Input_Butter[0]
        -Parameter->a[1] * Buffer->Output_Butter[1]
        -Parameter->a[2] * Buffer->Output_Butter[0];
        }
        else
        {
          Buffer->Output_Butter[2]=Buffer->Input_Butter[2];
          LPB_Cnt++;
        }
    /* x(n) 序列保存 */
        Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
        Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
    /* y(n) 序列保存 */
        Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
        Buffer->Output_Butter[1]=Buffer->Output_Butter[2];


        return Buffer->Output_Butter[2];
}


/*************************************************
函数名:    Set_Cutoff_Frequency
说明: 设置二阶巴特沃斯低通滤波器的截止频率和采样频率
*************************************************/
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF)
{
  float fr = sample_frequent / cutoff_frequent;
  float ohm = tanf(M_PI_F / fr);
  float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
  if (cutoff_frequent <= 0.0f) {
    // no filtering
    return;
  }
  LPF->b[0] = ohm * ohm / c;
  LPF->b[1] = 2.0f * LPF->b[0];
  LPF->b[2] = LPF->b[0];
  LPF->a[0] = 1.0f;
  LPF->a[1] = 2.0f * (ohm * ohm - 1.0f) / c;
  LPF->a[2] = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;
}
/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
float K1 =0.02;
void Yijielvbo(float angle,float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.005);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      滑动平均滤波计算
//  @param      void
//  @return     void
//  @since      主要用于对数据滤波，存储目标数据最近的n个数据，并求出平均值
//-------------------------------------------------------------------------------------------------------------------
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)
{
    //加上新的数值 减去最末尾的数值 求得最新的和
    move_filter->data_sum = move_filter->data_sum + new_data - move_filter->data_buffer[move_filter->index];
    //重新求平均值
    move_filter->data_average = move_filter->data_sum / move_filter->buffer_size;

    //将数据写入缓冲区
    move_filter->data_buffer[move_filter->index] = new_data;
    move_filter->index++;
    if(move_filter->buffer_size <= move_filter->index)
    {
        move_filter->index = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      滑动平均滤波初始化
//  @param      void
//  @return     void
//  @since      主要用于对数据滤波，存储目标数据最近的n个数据，并求出平均值
//-------------------------------------------------------------------------------------------------------------------
void move_filter_init(move_filter_struct *move_filter)
{
    move_filter->data_average = 0;
    move_filter->data_sum = 0;
    move_filter->index = 0;
    //设置缓冲区大小
    move_filter->buffer_size = 32;

    uint8 i;
    for(i=0; i < move_filter->buffer_size; i++)
    {
        move_filter->data_buffer[i] = 0;
    }
}



/*************************************************************
** Function name:      MCU_Open_interrupt
** Descriptions:       卡尔曼滤波器 函数
** Input parameters:   *kfp    - 卡尔曼结构体参数
** Input parameters:   input   - 需要滤波的参数的测量值（即传感器的采集值）
** Output parameters:  卡尔曼滤波器输出值（最优值）
** Returned value:     卡尔曼滤波器输出值（最优值）
*************************************************************/

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


float K_P=0.020f; //估算协方差
float K_G=0; //卡尔曼增益
float K_Q=0.010f; //过程噪声协方差,Q增大，动态响应变快，收敛稳定性变坏
float K_R=0.40f; //测量噪声协方差,R增大，动态响应变慢，收敛稳定性变好
float K_Output; //卡尔曼滤波器输出

float KalmanFilter( float input)
{
    //估算协方差方程：当前 估算协方差 = 上次更新 协方差 + 过程噪声协方差
    K_P = K_P +K_Q;

    //卡尔曼增益方程：当前 卡尔曼增益 = 当前 估算协方差 / （当前 估算协方差 + 测量噪声协方差）
    K_G = K_P / (K_P + K_R);

    //更新最优值方程：当前 最优值 = 当前 估算值 + 卡尔曼增益 * （当前 测量值 - 当前 估算值）
    K_Output = K_Output + K_G * (input - K_Output); //当前 估算值 = 上次 最优值

    //更新 协方差 = （1 - 卡尔曼增益） * 当前 估算协方差。
    K_P = (1 - K_G) * K_P;

     return K_Output;
}


