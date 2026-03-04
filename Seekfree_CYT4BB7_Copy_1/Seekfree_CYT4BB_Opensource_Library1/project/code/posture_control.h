#ifndef _POSTURE_CONTROL_H_
#define _POSTURE_CONTROL_H_

#include "zf_common_headfile.h"

// 宏定义
#define WHEEL_CIRCUMFERENCE   (6.4f)    // 车轮直径（cm）

// 全局变量声明
int16 balance_duty_max;
int16 turn_duty_max;

// 外部变量声明
extern int16 sys_times;
extern uint8_t jump_flag;
extern uint8_t run_state;
extern int16 jump_time;
extern float car_speed;
extern float car_distance;
extern int16 left_motor_duty;
extern int16 right_motor_duty;
extern float target_speed;
extern uint8_t system_time_state[20];

extern float imu660ra_gyro_x;
extern float imu660ra_gyro_z;

// 结构体前置声明及外部实例
typedef struct {
    int16 receive_left_speed_data;
    int16 receive_right_speed_data;
} motor_value_struct;

extern motor_value_struct motor_value;

typedef struct {
    float p;
    float i;
    float d;
    float out;
    float i_value;
} pid_cycle_struct;

typedef struct {
    float rol;
    float pit;
    float mechanical_zero;
} posture_value_struct;

typedef struct {
    pid_cycle_struct angle_cycle;
    pid_cycle_struct speed_cycle;
    pid_cycle_struct angular_speed_cycle;
    posture_value_struct posture_value;
} balance_cascade_struct;

extern balance_cascade_struct roll_balance_cascade;
extern balance_cascade_struct roll_balance_cascade_resave;
extern balance_cascade_struct pitch_balance_cascade;

typedef struct {
    pwm_channel_enum  pwm_pin;
    int16            control_frequency;
    int16            steer_dir;
    int16            center_num;
    int16            steer_state;
    int16            now_location;
} steer_control_struct;

extern steer_control_struct steer_1;
extern steer_control_struct steer_2;
extern steer_control_struct steer_3;
extern steer_control_struct steer_4;

// 函数声明
//-----------------------------------------------------------------------------
// 函数简介     计算并更新车辆状态标志
// 返回参数     void
// 使用示例     car_state_calculate();
// 备注信息     根据横滚角、转向电流、实际距离等参数切换车辆悬挂状态和运行状态，同时处理 PID 参数的渐变
//-----------------------------------------------------------------------------
void car_state_calculate(void);

//-----------------------------------------------------------------------------
// 函数简介     车辆舵机控制
// 返回参数     void
// 使用示例     car_steer_control();
// 备注信息     控制跳跃、左右倾斜、腿部前后倾斜、自动复位等操作
//-----------------------------------------------------------------------------
void car_steer_control(void);

//-----------------------------------------------------------------------------
// 函数简介     车辆电机占空比控制
// 返回参数     void
// 使用示例     car_motor_control();
// 备注信息     根据运行标志控制左右电机的占空比，通过限幅函数限制输出范围，最终设置电机驱动
//-----------------------------------------------------------------------------
void car_motor_control(void);

//-----------------------------------------------------------------------------
// 函数简介     PIT 定时器中断回调函数，处理定时任务
// 返回参数     void
// 使用示例     由 PIT 中断触发调用，无需手动调用
// 备注信息     周期性执行传感器数据采集、状态计算、PID 控制、电机控制等任务，各子任务按不同周期执行
//-----------------------------------------------------------------------------
void pit_callback(void);

#endif 