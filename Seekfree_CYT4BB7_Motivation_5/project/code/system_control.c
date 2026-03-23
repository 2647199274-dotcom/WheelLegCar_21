#include "system_control.h"


//====================================================系统状态变量====================================================
system_state_enum system_state = SYSTEM_STATE_INIT;
run_mode_enum run_mode = RUN_MODE_BALANCE;

//====================================================时间与状态变量====================================================
uint32 sys_times = 0;
bool jump_flag = 0;
bool run_state = 0;
int16 jump_time = 0;

//====================================================运动控制变量====================================================
float car_speed = 0.0f;
float car_distance = 0.0f;
float target_speed = 0.0f;

//====================================================转向控制变量====================================================
float target_turn_angle = 0.0f;
float target_yaw = 0.0f;
float turn_output = 0.0f;

//====================================================电机控制变量====================================================
int16 left_motor_duty = 0;
int16 right_motor_duty = 0;

//====================================================时间状态数组====================================================
uint8_t system_time_state[20] = {0};




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

    target_turn_angle = 0.0f;
    target_yaw = 0.0f;
    turn_output = 0.0f;

    left_motor_duty = 0;
    right_motor_duty = 0;

    for(int i = 0; i < 20; i++)
    {
        system_time_state[i] = 0;
    }

    imu660ra_gyro_x = 0;
    imu660ra_gyro_y = 0;
    imu660ra_gyro_z = 0;
    imu660ra_acc_x = 0;
    imu660ra_acc_y = 0;
    imu660ra_acc_z = 0;

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统控制初始化
// 返回参数     void
// 使用示例     system_control_init();
// 备注信息     初始化所有系统控制相关的模块和硬件
//-------------------------------------------------------------------------------------------------------------------
void system_control_init(void)
{
    // 硬件初始化
    tft180_set_dir(TFT180_CROSSWISE);
    tft180_init();
    tft180_clear();
    
    tft180_show_string(10, 0, "System Control Init...");
    
    // 传感器初始化
    tft180_show_string(10, 16, "IMU Init...");
    Attitude_Init();
    tft180_show_string(10, 32, "IMU OK!");
    
    // 控制模块初始化
    tft180_show_string(10, 48, "Control Init...");
    balance_cascade_init_test();    // 平衡数据初始化
    turn_control_init();            // 转向控制初始化
    motor_filter_init();           // 电机滤波初始化
    steer_control_init();          // 舵机初始化
    small_driver_uart_init();      // 电机驱动初始化
    
    // 用户变量初始化
    user_values_init();
    
    // 定时器初始化
    pit_ms_init(PIT_CH0, 4);
    
    tft180_show_string(10, 64, "Control OK!");
    tft180_show_string(10, 80, "Ready!");
    
    system_delay_ms(1000);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统运行控制
// 返回参数     void
// 使用示例     system_run_control();
// 备注信息     根据当前运行模式和系统状态执行相应的控制逻辑
//-------------------------------------------------------------------------------------------------------------------
void system_run_control(void)
{
    switch(system_state)
    {
        case SYSTEM_STATE_INIT:
            // 初始化状态，等待进入就绪状态
            system_state = SYSTEM_STATE_READY;
            break;
            
        case SYSTEM_STATE_READY:
            // 就绪状态，等待开始运行
            if(run_state)
            {
                system_state = SYSTEM_STATE_RUNNING;
            }
            break;
            
        case SYSTEM_STATE_RUNNING:
            // 运行状态，根据运行模式执行不同控制
            switch(run_mode)
            {
                case RUN_MODE_BALANCE:
                    // 平衡模式 - 执行平衡控制
                    
                    // // 1. 计算车辆速度
                    // if (sys_times % 10 == 0)
                    // {
                    //     car_speed = (motor_value.receive_left_speed_data - motor_value.receive_right_speed_data) / 2;
                    //     // 速度环 PID 控制
                    //     pid_control(&pitch_balance_cascade.speed_cycle, 0.0f, (float)car_speed);
                    // }
                    
                    // // 2. 角度环 PID 控制
                    // if (sys_times % 3 == 0)
                    // {
                    //     pid_control(&pitch_balance_cascade.angle_cycle, 0.0 - pitch_balance_cascade.posture_value.mechanical_zero, pitch_balance_cascade.posture_value.pit);
                    // }
                    
                    // // 3. 角速度环 PID 控制
                    // pid_control(&pitch_balance_cascade.angular_speed_cycle, pitch_balance_cascade.angle_cycle.out, GYRO_DATA_Y);
                    
                    // 4. 计算车辆状态
                    car_state_calculate();
                    
                    // 5. 控制舵机
                    car_steer_control();
                    
                    // 6. 控制电机
                    car_motor_control();
                    
                    break;
                    
                case RUN_MODE_JUMP:
                    // 跳跃模式 - 执行跳跃控制
                    system_state = SYSTEM_STATE_JUMP;
                    break;
                    
                case RUN_MODE_WALK:
                    // 行走模式 - 执行行走控制
                    // 这里可以添加行走控制逻辑
                    break;
                    
                case RUN_MODE_TEST:
                    // 测试模式 - 执行测试控制
                    // 这里可以添加测试控制逻辑
                    break;
                    
                default:
                    break;
            }
            break;
            
        case SYSTEM_STATE_JUMP:
            // 跳跃状态
            // 执行跳跃相关控制
            car_steer_control();  // 跳跃时的舵机控制
            if(!jump_flag)
            {
                system_state = SYSTEM_STATE_RUNNING;
            }
            break;
            
        case SYSTEM_STATE_ERROR:
            // 错误状态
            // 处理错误情况
            run_state = 0;
            left_motor_duty = 0;
            right_motor_duty = 0;
            break;
            
        case SYSTEM_STATE_STOP:
            // 停止状态
            // 执行停止相关操作
            run_state = 0;
            left_motor_duty = 0;
            right_motor_duty = 0;
            break;
            
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统初始化
// 返回参数     void
// 使用示例     system_init();
// 备注信息     初始化所有系统控制相关的模块和硬件
//-------------------------------------------------------------------------------------------------------------------
void system_init(void)
{
    tft180_set_dir(TFT180_CROSSWISE);
    tft180_init();
    tft180_clear();			// 屏幕初始化
    
    tft180_show_string(10, 0, "System Init...");
    balance_cascade_init_test();		//平衡数据初始化
    menu_init(); 				// 菜单初始化
    tft180_show_string(10, 16, "menu OK!");
    
    tft180_show_string(10, 32, "IMU Init...");
    Attitude_Init();
    tft180_show_string(10, 48, "IMU OK!");
    system_delay_ms(100);
    
    tft180_show_string(10, 64, "Control Init...");//控制初始化
    balance_cascade_init_test();		//平衡数据初始化
    turn_control_init();            // 转向控制初始化
    motor_filter_init();
    steer_control_init();		//舵机
    small_driver_uart_init();		//电机
    
    user_values_init();		
    pit_ms_init(PIT_CH0, 4); 

    tft180_show_string(10, 80, "Control OK!");

    tft180_show_string(10, 16, "Hardware OK!");

    tft180_show_string(10, 0, "Ready!");

    system_delay_ms(1000);
}
