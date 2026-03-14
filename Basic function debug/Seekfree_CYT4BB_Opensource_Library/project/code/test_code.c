/*********************************************************************************************************************
* CYT4BB Opensourec Library
* Copyright (c) 2022 SEEKFREE
*
* 本文件是 CYT4BB 开源库的一部分
*
* 文件名称          test_code
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
********************************************************************************************************************/

#include "test_code.h"

//====================================================测试状态实例====================================================
test_status_struct test_status = 
{
    .current_item = TEST_ITEM_NONE,    
    .result = TEST_RESULT_NONE,         
    .test_count = 0,                   
    .pass_count = 0,                   
    .fail_count = 0,                   
    .test_enable = 0,                  
    .auto_test = 0,                    
    .test_value = {0},                 
    .test_message = ""                 
};

//====================================================外部变量声明====================================================
extern steer_control_struct steer_1;
extern steer_control_struct steer_2;
extern steer_control_struct steer_3;
extern steer_control_struct steer_4;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     测试模块初始化
// 返回参数     void
// 使用示例     test_code_init();
// 备注信息     初始化测试状态结构体
//-------------------------------------------------------------------------------------------------------------------
void test_code_init(void)
{
    test_status.current_item = TEST_ITEM_NONE;     
    test_status.result = TEST_RESULT_NONE;         
    test_status.test_count = 0;                    
    test_status.pass_count = 0;                    
    test_status.fail_count = 0;                    
    test_status.test_enable = 0;                   
    test_status.auto_test = 0;                     
    
    for(int i = 0; i < 10; i++)
    {
        test_status.test_value[i] = 0.0f;          
    }
    
    test_status.test_message[0] = '\0';            
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     运行所有测试
// 返回参数     void
// 使用示例     test_run_all();
// 备注信息     依次执行所有测试项目
//-------------------------------------------------------------------------------------------------------------------
void test_run_all(void)
{
    test_result_enum result = TEST_RESULT_NONE;
    
    printf("\r\n========== Start All Tests ==========\r\n");
    
    test_status.test_count = 0;
    test_status.pass_count = 0;
    test_status.fail_count = 0;
    
    result = test_imu_sensor();
    test_status.test_count++;
    if(result == TEST_RESULT_PASS) test_status.pass_count++;
    else test_status.fail_count++;
    
    result = test_steer_control();
    test_status.test_count++;
    if(result == TEST_RESULT_PASS) test_status.pass_count++;
    else test_status.fail_count++;
    
    result = test_motor_control();
    test_status.test_count++;
    if(result == TEST_RESULT_PASS) test_status.pass_count++;
    else test_status.fail_count++;
    
    result = test_balance_control();
    test_status.test_count++;
    if(result == TEST_RESULT_PASS) test_status.pass_count++;
    else test_status.fail_count++;
    
    result = test_vmc_control();
    test_status.test_count++;
    if(result == TEST_RESULT_PASS) test_status.pass_count++;
    else test_status.fail_count++;
    
    printf("\r\n========== Test Summary ==========\r\n");
    printf("Total: %d, Pass: %d, Fail: %d\r\n", 
           test_status.test_count, 
           test_status.pass_count, 
           test_status.fail_count);
    printf("==================================\r\n");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU传感器测试
// 返回参数     test_result_enum - 测试结果
// 使用示例     test_result_enum result = test_imu_sensor();
// 备注信息     测试IMU传感器数据是否正常
//-------------------------------------------------------------------------------------------------------------------
test_result_enum test_imu_sensor(void)
{
    test_result_enum result = TEST_RESULT_RUNNING;
    int16 gyro_x, gyro_y, gyro_z;
    int16 acc_x, acc_y, acc_z;
    int valid_count = 0;
    
    printf("\r\n[TEST] IMU Sensor Test Start...\r\n");
    tft180_clear();
    tft180_show_string(10, 0, "IMU Test...");
    
    for(int i = 0; i < 100; i++)
    {
        imu660ra_get_gyro();
        imu660ra_get_acc();
        
        gyro_x = imu660ra_gyro_x;
        gyro_y = imu660ra_gyro_y;
        gyro_z = imu660ra_gyro_z;
        acc_x = imu660ra_acc_x;
        acc_y = imu660ra_acc_y;
        acc_z = imu660ra_acc_z;
        
        if(func_abs(gyro_x) < 32768 && func_abs(gyro_y) < 32768 && func_abs(gyro_z) < 32768)
        {
            valid_count++;
        }
        
        if(func_abs(acc_x) < 32768 && func_abs(acc_y) < 32768 && func_abs(acc_z) < 32768)
        {
            valid_count++;
        }
        
        system_delay_ms(10);
    }
    
    if(valid_count >= 180)
    {
        result = TEST_RESULT_PASS;
        printf("[PASS] IMU Sensor Test\r\n");
        printf("  Gyro: X=%d, Y=%d, Z=%d\r\n", gyro_x, gyro_y, gyro_z);
        printf("  Acc:  X=%d, Y=%d, Z=%d\r\n", acc_x, acc_y, acc_z);
        tft180_show_string(10, 16, "IMU: PASS");
    }
    else
    {
        result = TEST_RESULT_FAIL;
        printf("[FAIL] IMU Sensor Test\r\n");
        printf("  Valid count: %d / 200\r\n", valid_count);
        tft180_show_string(10, 16, "IMU: FAIL");
    }
    
    system_delay_ms(500);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     舵机控制测试
// 返回参数     test_result_enum - 测试结果
// 使用示例     test_result_enum result = test_steer_control();
// 备注信息     测试舵机控制功能
//-------------------------------------------------------------------------------------------------------------------
test_result_enum test_steer_control(void)
{
    test_result_enum result = TEST_RESULT_RUNNING;
    
    printf("\r\n[TEST] Steer Control Test Start...\r\n");
    tft180_clear();
    tft180_show_string(10, 0, "Steer Test...");
    
    int16 test_positions[] = {0, 500, -500, 0};
    int test_pass = 1;
    
    for(int i = 0; i < 4; i++)
    {
        int16 pos = test_positions[i];
        
        steer_duty_set(&steer_1, steer_1.center_num + pos);
        steer_duty_set(&steer_2, steer_2.center_num + pos);
        steer_duty_set(&steer_3, steer_3.center_num + pos);
        steer_duty_set(&steer_4, steer_4.center_num + pos);
        
        system_delay_ms(300);
        
        int16 diff1 = func_abs(steer_1.now_location - (steer_1.center_num + pos));
        int16 diff2 = func_abs(steer_2.now_location - (steer_2.center_num + pos));
        int16 diff3 = func_abs(steer_3.now_location - (steer_3.center_num + pos));
        int16 diff4 = func_abs(steer_4.now_location - (steer_4.center_num + pos));
        
        if(diff1 > 100 || diff2 > 100 || diff3 > 100 || diff4 > 100)
        {
            test_pass = 0;
        }
        
        printf("  Position %d: S1=%d, S2=%d, S3=%d, S4=%d\r\n", 
               pos, steer_1.now_location, steer_2.now_location, 
               steer_3.now_location, steer_4.now_location);
    }
    
    steer_duty_set(&steer_1, steer_1.center_num);
    steer_duty_set(&steer_2, steer_2.center_num);
    steer_duty_set(&steer_3, steer_3.center_num);
    steer_duty_set(&steer_4, steer_4.center_num);
    
    if(test_pass)
    {
        result = TEST_RESULT_PASS;
        printf("[PASS] Steer Control Test\r\n");
        tft180_show_string(10, 16, "Steer: PASS");
    }
    else
    {
        result = TEST_RESULT_FAIL;
        printf("[FAIL] Steer Control Test\r\n");
        tft180_show_string(10, 16, "Steer: FAIL");
    }
    
    system_delay_ms(500);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机控制测试
// 返回参数     test_result_enum - 测试结果
// 使用示例     test_result_enum result = test_motor_control();
// 备注信息     测试电机控制功能
//-------------------------------------------------------------------------------------------------------------------
test_result_enum test_motor_control(void)
{
    test_result_enum result = TEST_RESULT_RUNNING;
    
    printf("\r\n[TEST] Motor Control Test Start...\r\n");
    tft180_clear();
    tft180_show_string(10, 0, "Motor Test...");
    
    int16 test_duties[] = {0, 500, -500, 0};
    int test_pass = 1;
    
    for(int i = 0; i < 4; i++)
    {
        int16 duty = test_duties[i];
        
        small_driver_set_duty(duty, duty);
        system_delay_ms(200);
        
        small_driver_get_speed();
        system_delay_ms(100);
        
        printf("  Duty %d: L_Speed=%d, R_Speed=%d\r\n", 
               duty, motor_value.receive_left_speed_data, motor_value.receive_right_speed_data);
    }
    
    small_driver_set_duty(0, 0);
    
    if(test_pass)
    {
        result = TEST_RESULT_PASS;
        printf("[PASS] Motor Control Test\r\n");
        tft180_show_string(10, 16, "Motor: PASS");
    }
    else
    {
        result = TEST_RESULT_FAIL;
        printf("[FAIL] Motor Control Test\r\n");
        tft180_show_string(10, 16, "Motor: FAIL");
    }
    
    system_delay_ms(500);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     平衡控制测试
// 返回参数     test_result_enum - 测试结果
// 使用示例     test_result_enum result = test_balance_control();
// 备注信息     测试平衡控制功能
//-------------------------------------------------------------------------------------------------------------------
test_result_enum test_balance_control(void)
{
    test_result_enum result = TEST_RESULT_RUNNING;
    
    printf("\r\n[TEST] Balance Control Test Start...\r\n");
    tft180_clear();
    tft180_show_string(10, 0, "Balance Test...");
    
    float rol_sum = 0, pit_sum = 0;
    int sample_count = 100;
    
    for(int i = 0; i < sample_count; i++)
    {
        imu660ra_get_gyro();
        imu660ra_get_acc();
        quaternion_module_calculate(&roll_balance_cascade);
        
        rol_sum += func_abs(roll_balance_cascade.posture_value.rol);
        pit_sum += func_abs(roll_balance_cascade.posture_value.pit);
        
        system_delay_ms(10);
    }
    
    float rol_avg = rol_sum / sample_count;
    float pit_avg = pit_sum / sample_count;
    
    printf("  Average Rol: %.2f deg\r\n", rol_avg);
    printf("  Average Pit: %.2f deg\r\n", pit_avg);
    
    if(rol_avg < 30.0f && pit_avg < 30.0f)
    {
        result = TEST_RESULT_PASS;
        printf("[PASS] Balance Control Test\r\n");
        tft180_show_string(10, 16, "Balance: PASS");
    }
    else
    {
        result = TEST_RESULT_FAIL;
        printf("[FAIL] Balance Control Test\r\n");
        tft180_show_string(10, 16, "Balance: FAIL");
    }
    
    system_delay_ms(500);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     VMC运动学测试
// 返回参数     test_result_enum - 测试结果
// 使用示例     test_result_enum result = test_vmc_control();
// 备注信息     测试VMC虚拟运动链控制
//-------------------------------------------------------------------------------------------------------------------
test_result_enum test_vmc_control(void)
{
    test_result_enum result = TEST_RESULT_RUNNING;
    
    printf("\r\n[TEST] VMC Control Test Start...\r\n");
    tft180_clear();
    tft180_show_string(10, 0, "VMC Test...");
    
    float test_x = 18.55f;  
    float test_y[] = {60.0f, 80.0f, 100.0f, 80.0f};
    int test_pass = 1;
    
    servo_init();
    
    for(int i = 0; i < 4; i++)
    {
        vmc_right(test_x, test_y[i]);
        vmc_left(test_x, test_y[i]);
        
        system_delay_ms(300);
        
        printf("  VMC Position: X=%.2f, Y=%.2f\r\n", test_x, test_y[i]);
    }
    
    vmc_right(test_x, 80.0f);
    vmc_left(test_x, 80.0f);
    
    if(test_pass)
    {
        result = TEST_RESULT_PASS;
        printf("[PASS] VMC Control Test\r\n");
        tft180_show_string(10, 16, "VMC: PASS");
    }
    else
    {
        result = TEST_RESULT_FAIL;
        printf("[FAIL] VMC Control Test\r\n");
        tft180_show_string(10, 16, "VMC: FAIL");
    }
    
    system_delay_ms(500);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示测试结果
// 返回参数     void
// 使用示例     test_display_result();
// 备注信息     在显示屏上显示测试结果
//-------------------------------------------------------------------------------------------------------------------
void test_display_result(void)
{
    tft180_clear();
    tft180_show_string(10, 0, "Test Result:");
    
    tft180_show_string(10, 20, "Total:");
    tft180_show_int(70, 20, test_status.test_count, 2);
    
    tft180_show_string(10, 36, "Pass:");
    tft180_show_int(70, 36, test_status.pass_count, 2);
    
    tft180_show_string(10, 52, "Fail:");
    tft180_show_int(70, 52, test_status.fail_count, 2);
    
    if(test_status.fail_count == 0)
    {
        tft180_show_string(10, 80, "ALL PASS!");
    }
    else
    {
        tft180_show_string(10, 80, "SOME FAILED");
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     打印测试报告
// 返回参数     void
// 使用示例     test_print_report();
// 备注信息     通过串口打印测试报告
//-------------------------------------------------------------------------------------------------------------------
void test_print_report(void)
{
    printf("\r\n");
    printf("╔══════════════════════════════════════╗\r\n");
    printf("║         TEST REPORT SUMMARY          ║\r\n");
    printf("╠══════════════════════════════════════╣\r\n");
    printf("║ Total Tests:  %d                      ║\r\n", test_status.test_count);
    printf("║ Passed:       %d                      ║\r\n", test_status.pass_count);
    printf("║ Failed:       %d                      ║\r\n", test_status.fail_count);
    printf("╠══════════════════════════════════════╣\r\n");
    
    if(test_status.fail_count == 0)
    {
        printf("║         ALL TESTS PASSED!            ║\r\n");
    }
    else
    {
        printf("║       SOME TESTS FAILED!             ║\r\n");
    }
    printf("╚══════════════════════════════════════╝\r\n");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     测试任务处理
// 返回参数     void
// 使用示例     test_task_handler();
// 备注信息     在主循环中调用，处理测试任务
//-------------------------------------------------------------------------------------------------------------------
void test_task_handler(void)
{
    static uint32 test_timer = 0;
    
    if(!test_status.test_enable)
    {
        return;
    }
    
    test_timer++;
    
    if(test_status.auto_test && test_timer % 1000 == 0)
    {
        test_run_all();
        test_display_result();
        test_print_report();
        test_status.test_enable = 0;
    }
}
