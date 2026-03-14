/*********************************************************************************************************************
* CYT4BB Opensourec Library
* Copyright (c) 2022 SEEKFREE
*
* 本文件是 CYT4BB 开源库的一部分
*
* 文件名称          system_control
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
********************************************************************************************************************/

#include "system_control.h"

//====================================================测试模式开关====================================================
#define TEST_MODE_ENABLE        (1)

//====================================================系统状态====================================================
static system_state_enum system_state = SYSTEM_STATE_INIT;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统初始化
// 返回参数     void
// 使用示例     system_control_init();
// 备注信息     初始化所有系统组件
//-------------------------------------------------------------------------------------------------------------------
void system_control_init(void)
{
    clock_init(SYSTEM_CLOCK_250M);
    debug_init();
    
    system_hardware_init();
    system_sensor_init();
    system_control_system_init();
    
    printf("System Init Complete!\r\n");
    printf("Ready to run...\r\n");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统主循环
// 返回参数     void
// 使用示例     system_control_loop();
// 备注信息     系统主循环处理函数
//-------------------------------------------------------------------------------------------------------------------
void system_control_loop(void)
{
    system_display_status();
    
    if(TEST_MODE_ENABLE)
    {
        system_run_tests();
    }
    
    system_delay_ms(10);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     硬件初始化
// 返回参数     void
// 使用示例     system_hardware_init();
// 备注信息     初始化硬件外设
//-------------------------------------------------------------------------------------------------------------------
void system_hardware_init(void)
{
    uart_init(UART_0, 115200, UART0_TX_P00_1, UART0_RX_P00_0);
    
    tft180_set_dir(TFT180_CROSSWISE);
    tft180_init();

    tft180_clear();
    tft180_show_string(10, 0, "System Init...");
    
    switch_init();
    
    ble6a20_init();
    
    tft180_show_string(10, 16, "Hardware OK!");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     传感器初始化
// 返回参数     void
// 使用示例     system_sensor_init();
// 备注信息     初始化传感器
//-------------------------------------------------------------------------------------------------------------------
void system_sensor_init(void)
{
    tft180_show_string(10, 32, "IMU Init...");
    
    imu660ra_init();
    
    tft180_show_string(10, 48, "IMU OK!");
    
    system_delay_ms(100);
    
    imu660ra_get_gyro();
    imu660ra_get_acc();
    
    quaternion_module_init(&roll_balance_cascade);
    quaternion_module_init(&pitch_balance_cascade);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     控制系统初始化
// 返回参数     void
// 使用示例     system_control_system_init();
// 备注信息     初始化控制系统
//-------------------------------------------------------------------------------------------------------------------
void system_control_system_init(void)
{
    tft180_show_string(10, 64, "Control Init...");
    
    steer_control_init();
    
    small_driver_uart_init();
    
    balance_cascade_init();
    
    user_values_init();
    
    pit_ms_init(PIT_CH0, 1);
    
    tft180_show_string(10, 80, "Control OK!");
    
    system_delay_ms(500);
    
    tft180_clear();
    tft180_show_string(10, 0, "Ready!");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     系统状态更新
// 返回参数     void
// 使用示例     system_state_update();
// 备注信息     更新系统状态
//-------------------------------------------------------------------------------------------------------------------
void system_state_update(void)
{
    switch(system_state)
    {
        case SYSTEM_STATE_INIT:
            system_state = SYSTEM_STATE_READY;
            break;
            
        case SYSTEM_STATE_READY:
            if(run_state == 1)
            {
                system_state = SYSTEM_STATE_RUNNING;
            }
            break;
            
        case SYSTEM_STATE_RUNNING:
            if(func_abs(roll_balance_cascade.posture_value.rol) > 45.0f || 
               func_abs(roll_balance_cascade.posture_value.pit) > 45.0f)
            {
                system_state = SYSTEM_STATE_ERROR;
            }
            break;
            
        case SYSTEM_STATE_ERROR:
            if(func_abs(roll_balance_cascade.posture_value.rol) < 30.0f && 
               func_abs(roll_balance_cascade.posture_value.pit) < 30.0f)
            {
                system_state = SYSTEM_STATE_READY;
                run_state = 0;
            }
            break;
            
        case SYSTEM_STATE_STOP:
            break;
            
        default:
            system_state = SYSTEM_STATE_INIT;
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示系统状态
// 返回参数     void
// 使用示例     system_display_status();
// 备注信息     显示系统运行状态
//-------------------------------------------------------------------------------------------------------------------
void system_display_status(void)
{
    static uint32 display_count = 0;
    
    display_count++;
    
    if(display_count % 50 == 0)
    {
        tft180_show_string(10, 0, "State:");
        
        switch(system_state)
        {
            case SYSTEM_STATE_INIT:
                tft180_show_string(60, 0, "INIT  ");
                break;
            case SYSTEM_STATE_READY:
                tft180_show_string(60, 0, "READY ");
                break;
            case SYSTEM_STATE_RUNNING:
                tft180_show_string(60, 0, "RUN   ");
                break;
            case SYSTEM_STATE_ERROR:
                tft180_show_string(60, 0, "ERROR ");
                break;
            case SYSTEM_STATE_STOP:
                tft180_show_string(60, 0, "STOP  ");
                break;
            default:
                tft180_show_string(60, 0, "UNKNOW");
                break;
        }
        
        tft180_show_int(10, 144, sys_times, 6);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     运行测试
// 返回参数     void
// 使用示例     system_run_tests();
// 备注信息     运行系统测试
//-------------------------------------------------------------------------------------------------------------------
void system_run_tests(void)
{
    static uint32 test_count = 0;
    
    test_count++;
    
    if(test_count % 10 == 0)
    {
        tft180_show_int(10, 16, imu660ra_gyro_x, 6);
        tft180_show_int(10, 32, imu660ra_gyro_y, 6);
        tft180_show_int(10, 48, imu660ra_gyro_z, 6);
        
        tft180_show_int(10, 64, imu660ra_acc_x, 6);
        tft180_show_int(10, 80, imu660ra_acc_y, 6);
        tft180_show_int(10, 96, imu660ra_acc_z, 6);
        
        tft180_show_float(10, 112, roll_balance_cascade.posture_value.rol, 4, 2);
        tft180_show_float(10, 128, roll_balance_cascade.posture_value.pit, 4, 2);
    }
    
    if(test_count % 1000 == 0)
    {
        ble6a20_send_string("System Test Data\r\n");
        char buffer[64];
        sprintf(buffer, "Rol: %.2f, Pit: %.2f\r\n", 
                roll_balance_cascade.posture_value.rol,
                roll_balance_cascade.posture_value.pit);
        ble6a20_send_string(buffer);
    }
}
