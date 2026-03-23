#ifndef _SYSTEM_CONTROL_H_
#define _SYSTEM_CONTROL_H_

#include "zf_common_headfile.h"

//====================================================系统状态定义====================================================
typedef enum
{
    SYSTEM_STATE_INIT = 0,
    SYSTEM_STATE_READY,
    SYSTEM_STATE_RUNNING,
    SYSTEM_STATE_JUMP,
    SYSTEM_STATE_ERROR,
    SYSTEM_STATE_STOP,
    SYSTEM_STATE_MAX
} system_state_enum;

//====================================================运行模式定义====================================================
typedef enum
{
    RUN_MODE_BALANCE = 0,
    RUN_MODE_JUMP,
    RUN_MODE_WALK,
    RUN_MODE_TEST,
    RUN_MODE_MAX
} run_mode_enum;

//====================================================全局变量声明====================================================
extern system_state_enum system_state;
extern run_mode_enum run_mode;

//====================================================时间与状态变量====================================================
extern uint32 sys_times;
extern bool jump_flag;
extern bool run_state;
extern int16 jump_time;

//====================================================运动控制变量====================================================
extern float car_speed;
extern float car_distance;
extern float target_speed;

//====================================================转向控制变量====================================================
extern float target_turn_angle;
extern float target_yaw;
extern float turn_output;

//====================================================电机控制变量====================================================
extern int16 left_motor_duty;
extern int16 right_motor_duty;

//====================================================时间状态数组====================================================
extern uint8_t system_time_state[20];

//====================================================IMU原始数据变量====================================================
extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;

//====================================================函数声明====================================================
void system_init(void);       // 系统初始化
void system_control_init(void);       // 系统初始化
void system_run_control(void);       // 系统运行控制

#endif
