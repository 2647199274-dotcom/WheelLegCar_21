#include "PID.h"

#ifndef NULL
    #define NULL ((void *)0)
#endif


/****
******************** 位置式PID ********************
****/
void PID_Positional_Init(PID_Positional_Data *pid, float Kp, float Ki, float Kd, float dt,
                         float output_max, float output_min, float integral_max, float derivative_max)
{
    if (pid == NULL) return;

    // 初始化PID参数
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->dt = dt;

    // 初始化状态变量
    pid->SP = 0.0f;           // 设定值
    pid->FB = 0.0f;           // 反馈值
    pid->error = 0.0f;        // 当前误差
    pid->last_error = 0.0f;   // 上一周期误差
    pid->last_error_integral = 0.0f; // 积分项累计值
    pid->output = 0.0f;       // PID输出

    // 初始化限幅参数
    pid->output_max = output_max;
    pid->output_min = output_min;
    pid->integral_max = integral_max;
    pid->integral_min = -integral_max; // 积分上下限对称
    pid->derivative_max = derivative_max;
}

float PID_Positional_Calculate(PID_Positional_Data *pid, float set_point, float feedback)
{
    if (pid == NULL) return 0.0f;

    pid->FB = feedback;
    pid->SP = set_point;
    pid->error = pid->SP - pid->FB;

    // 1. 比例项
    float proportional = pid->Kp * pid->error;

    // 2. 积分项（梯形积分，带限幅）
    float integral = pid->last_error_integral + (pid->error + pid->last_error) * pid->dt * 0.5f;
    integral = (integral > pid->integral_max) ? pid->integral_max : integral;
    integral = (integral < pid->integral_min) ? pid->integral_min : integral;
    float integral_term = pid->Ki * integral;

    // 3. 微分项（带限幅）
    float derivative = (pid->error - pid->last_error) / pid->dt;
    derivative = (derivative > pid->derivative_max) ? pid->derivative_max : derivative;
    derivative = (derivative < -pid->derivative_max) ? -pid->derivative_max : derivative;
    float derivative_term = pid->Kd * derivative;

    // 计算总输出（带输出限幅）
    pid->output = proportional + integral_term + derivative_term;
    pid->output = (pid->output > pid->output_max) ? pid->output_max : pid->output;
    pid->output = (pid->output < pid->output_min) ? pid->output_min : pid->output;

    // 更新状态变量
    pid->last_error = pid->error;
    pid->last_error_integral = integral;

    return pid->output;
}

void PID_Positional_Clear(PID_Positional_Data *pid)
{
    if (pid == NULL) return;

    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->last_error_integral = 0.0f;
    pid->output = 0.0f;
}


/****
******************** 增量式PID ********************
****/
void PID_Incremental_Init(PID_Incremental_Data *pid, float Kp, float Ki, float Kd, float dt,
                          float inc_max, float inc_min)
{
    if (pid == NULL) return;

    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->dt = dt;

    pid->SP = 0.0f;           // 设定值
    pid->FB = 0.0f;           // 反馈值
    pid->error[0] = 0.0f;     // 当前误差
    pid->error[1] = 0.0f;     // 上一周期误差
    pid->error[2] = 0.0f;     // 上两周期误差
    pid->increment = 0.0f;    // PID增量输出

    pid->inc_max = inc_max;
    pid->inc_min = inc_min;
}


float PID_Incremental_Calculate(PID_Incremental_Data *pid, float set_point, float feedback)
{
    if (pid == NULL) return 0.0f;

    // 更新设定值和反馈值
    pid->SP = set_point;
    pid->FB = feedback;

    // 移位更新误差队列（error[2] = 上上次, error[1] = 上次, error[0] = 当前）
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->error[0] = pid->SP - pid->FB;

    // 1. 比例增量
    float delta_p = pid->Kp * (pid->error[0] - pid->error[1]);
    // 2. 积分增量
    float delta_i = pid->Ki * pid->error[0] * pid->dt;
    // 3. 微分增量
    float delta_d = pid->Kd * (pid->error[0] - 2 * pid->error[1] + pid->error[2]) / pid->dt;

    // 计算总增量
    pid->increment = delta_p + delta_i + delta_d;
    pid->increment = (pid->increment > pid->inc_max) ? pid->inc_max : pid->increment;
    pid->increment = (pid->increment < pid->inc_min) ? pid->inc_min : pid->increment;

    return pid->increment;
}


void PID_Incremental_Clear(PID_Incremental_Data *pid)
{
    if (pid == NULL) return;

    pid->error[0] = 0.0f;
    pid->error[1] = 0.0f;
    pid->error[2] = 0.0f;
    pid->increment = 0.0f;
}