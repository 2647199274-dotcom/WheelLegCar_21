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

#include "user_values.h"

//====================================================系统状态变量====================================================
system_state_enum system_state = SYSTEM_STATE_INIT;    
run_mode_enum run_mode = RUN_MODE_BALANCE;             

//====================================================时间与状态变量====================================================
uint32 sys_times = 0;                                   
uint8_t jump_flag = 0;                                 
uint8_t run_state = 0;                                 
int16 jump_time = 0;                                    

//====================================================运动控制变量====================================================
float car_speed = 0.0f;                                
float car_distance = 0.0f;                             
float target_speed = 0.0f;                             

//====================================================电机控制变量====================================================
int16 left_motor_duty = 0;                             
int16 right_motor_duty = 0;                            

//====================================================时间状态数组====================================================
uint8_t system_time_state[20] = {0};                   

//====================================================IMU原始数据变量====================================================
                             
                             
                             
                              
                              
                              

//====================================================电机数据结构实例====================================================
motor_data_struct motor_data = 
{
    .receive_left_speed_data = 0,                      
    .receive_right_speed_data = 0,                     
    .target_left_speed = 0,                            
    .target_right_speed = 0,                           
    .left_duty = 0,                                    
    .right_duty = 0                                    
};

//====================================================姿态数据结构实例====================================================
posture_data_struct posture_data = 
{
    .rol = 0.0f,                                       
    .pit = 0.0f,                                       
    .yaw = 0.0f,                                       
    .rol_speed = 0.0f,                                 
    .pit_speed = 0.0f,                                 
    .yaw_speed = 0.0f,                                 
    .mechanical_zero_rol = -4.0f,                      
    .mechanical_zero_pit = 0.0f                        
};

//====================================================控制参数结构实例====================================================
control_param_struct control_param = 
{
    .balance_kp = 0.0f,                              
    .balance_ki = 0.0f,                                
    .balance_kd = 0.0f,                                
    .speed_kp = 0.0f,                                 
    .speed_ki = 0.0f,                                  
    .speed_kd = 0.0f,                                  
    .turn_kp = 0.0f,                                  
    .turn_kd = 0.0f,                                   
    .balance_duty_max = 3000,                          
    .turn_duty_max = 3000,                             
    .speed_limit = 100                                 
};

//====================================================测试参数结构实例====================================================
test_param_struct test_param = 
{
    .test_enable = 0,                                  
    .test_item = 0,                                    
    .test_count = 0,                                   
    .test_value = {0},                                 
    .test_result = 0                                   
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     用户变量初始化
// 返回参数     void
// 使用示例     user_values_init();
// 备注信息     初始化所有用户自定义的全局变量和参数
//-------------------------------------------------------------------------------------------------------------------
void user_values_init(void)
{
    system_state = SYSTEM_STATE_INIT;                   
    run_mode = RUN_MODE_BALANCE;                        
    
    sys_times = 0;                                      
    jump_flag = 0;                                      
    run_state = 0;                                      
    jump_time = 0;                                      
    
    car_speed = 0.0f;                                   
    car_distance = 0.0f;                                
    target_speed = 0.0f;                                
    
    left_motor_duty = 0;                                
    right_motor_duty = 0;                               
    
    for(int i = 0; i < 20; i++)
    {
        system_time_state[i] = 0;                       
    }
    
    
    motor_data.receive_left_speed_data = 0;             
    motor_data.receive_right_speed_data = 0;            
    motor_data.target_left_speed = 0;                   
    motor_data.target_right_speed = 0;                  
    motor_data.left_duty = 0;                           
    motor_data.right_duty = 0;                          
    
    posture_data.rol = 0.0f;                            
    posture_data.pit = 0.0f;                            
    posture_data.yaw = 0.0f;                            
    posture_data.rol_speed = 0.0f;                      
    posture_data.pit_speed = 0.0f;                      
    posture_data.yaw_speed = 0.0f;                      
    posture_data.mechanical_zero_rol = -2.0f;           
    posture_data.mechanical_zero_pit = 0.0f;            
    
    control_param_reset();                              
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     控制参数重置
// 返回参数     void
// 使用示例     control_param_reset();
// 备注信息     重置控制参数到默认值
//-------------------------------------------------------------------------------------------------------------------
void control_param_reset(void)
{
    control_param.balance_kp = 0.0f;                  
    control_param.balance_ki = 0.0f;                    
    control_param.balance_kd = 0.0f;                    
    control_param.speed_kp = 0.0f;                     
    control_param.speed_ki = 0.0f;                      
    control_param.speed_kd = 0.0f;                      
    control_param.turn_kp = 0.0f;                      
    control_param.turn_kd = 0.0f;                       
    control_param.balance_duty_max = 3000;              
    control_param.turn_duty_max = 3000;                 
    control_param.speed_limit = 1000;                    
}
