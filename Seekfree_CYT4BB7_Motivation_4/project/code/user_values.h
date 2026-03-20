/*********************************************************************************************************************
* CYT4BB Opensourec Library
* Copyright (c) 2022 SEEKFREE
*
* 本文件是 CYT4BB 开源库的一部分
*
* 文件名称          user_values
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
********************************************************************************************************************/

#ifndef _USER_VALUES_H_
#define _USER_VALUES_H_

#include "zf_common_headfile.h"

//====================================================系统状态定义====================================================
typedef enum
{
    SYSTEM_STATE_INIT = 0,      
    SYSTEM_STATE_READY,         
    SYSTEM_STATE_RUNNING,       
    SYSTEM_STATE_ERROR,         
    SYSTEM_STATE_STOP,          
            
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

extern uint32 sys_times;                          
extern uint8_t jump_flag;                        
extern uint8_t run_state;                        
extern int16 jump_time;                          

extern float car_speed;                          
extern float car_distance;                       
extern float target_speed;                       

extern int16 left_motor_duty;                    
extern int16 right_motor_duty;                   

extern uint8_t system_time_state[20];            

//====================================================IMU数据声明====================================================
extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;  
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;     

//====================================================电机数据结构====================================================
typedef struct
{
    int16 receive_left_speed_data;   
    int16 receive_right_speed_data;  
    int16 target_left_speed;         
    int16 target_right_speed;        
    int16 left_duty;                 
    int16 right_duty;                
} motor_data_struct;

extern motor_data_struct motor_data;             

//====================================================姿态数据结构====================================================
typedef struct
{
    float rol;                               
    float pit;                               
    float yaw;                               
    float rol_speed;                         
    float pit_speed;                         
    float yaw_speed;                         
    float mechanical_zero_rol;               
    float mechanical_zero_pit;               
} posture_data_struct;

extern posture_data_struct posture_data;         

//====================================================控制参数结构====================================================
typedef struct
{
    float balance_kp;                        
    float balance_ki;                        
    float balance_kd;                        
    float speed_kp;                          
    float speed_ki;                          
    float speed_kd;                          
    float turn_kp;                           
    float turn_kd;                           
    int16 balance_duty_max;                  
    int16 turn_duty_max;                     
    int16 speed_limit;                       
} control_param_struct;

extern control_param_struct control_param;       

//====================================================测试参数结构====================================================
typedef struct
{
    uint8_t test_enable;                      
    uint8_t test_item;                        
    uint32_t test_count;                      
    float test_value[10];                     
    uint8_t test_result;                      
} test_param_struct;

extern test_param_struct test_param;             

//====================================================函数声明====================================================
void user_values_init(void);                     
void system_state_update(void);                  
void control_param_reset(void);                  

#endif 
