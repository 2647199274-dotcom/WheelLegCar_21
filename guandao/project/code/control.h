#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"

// 配置参数
#define WHEEL_CIRCUMFERENCE 0.1f      // 车轮周长（米）
#define balance_duty_max 10000        // 平衡控制最大占空比
#define turn_duty_max 10000           // 转向控制最大占空比

// 控制相关变量结构体
typedef struct
{
    int speed_target_left;     // 目标左速度(pwm)
    int speed_target_right;     // 目标右速度(pwm)
    float angle_target;     // 目标角度
    float gyro_target;      // 目标角速度
    int speed_FB_left;      //当前左速度
    int speed_FB_right;    //当前右速度
    float angle_FB;     // 当前角度
    float gyro_FB;      // 当前角速度    
}Control_data;

// 平衡级联控制结构体
typedef struct {
    struct {
        float out;  // 角速度环输出
    } angular_speed_cycle;
} Balance_Cascade;

extern Control_data control_data;
extern Balance_Cascade roll_balance_cascade;
extern uint8 run_state;
extern int16 left_motor_duty;
extern int16 right_motor_duty;
extern int car_speed;
extern float car_distance;

// 函数声明
void car_start(void);
void balance_pid_init(void);
void control_balance(void);
void car_motor_control(void);

#endif