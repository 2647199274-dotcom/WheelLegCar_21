#include "control.h"


// 全局变量
Control_data control_data = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
PID_Positional_Data balance_pitch_g;
PID_Positional_Data balance_pitch_angle;
float angle_control=0.0;

// 新增全局变量
Balance_Cascade roll_balance_cascade = {0};
uint8 run_state = 0;          // 运行状态
int16 left_motor_duty = 0;     // 左电机占空比
int16 right_motor_duty = 0;    // 右电机占空比
int car_speed = 0;             // 车辆速度（RPM）
float car_distance = 0.0f;     // 累计行驶距离（米）

int start = 0;  // 按键状态计数，用于启动/停止电机
void car_start(void)
 {
     if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
     {
         start++;  // 按键短按一次，状态计数加1
         run_state = start % 2;  // 奇数次按下：启动，偶数次按下：停止
     }
     if (start != 0)
     {
         if (start % 2 == 1)  // 奇数次按下：启动电机
         {
             motor_control(500, 500);
         }
         else  // 偶数次按下：停止电机
         {
             motor_control(0, 0);
         }
     }
 }

void balance_pid_init(void)
{
  // 角速度
  PID_Positional_Init(&balance_pitch_g, 1.1f, 0.0f, 0.0f, 0.001f, 10000.0f, -10000.0f, 1000.0f, 0.0f);
  // 角度
  PID_Positional_Init(&balance_pitch_angle, 700.0f, 2.0f, 50.0f, 0.001f, 10000.0f, -10000.0f, 1000.0f, 0.0f);
}

float attitude_zero=0.0;
void control_balance(void)
 {
    static uint32 sys_times = 0;
    sys_times++;
    
    //Attitude_get();
    
    if(sys_times % 5 == 0)
    {
        float angle_control = PID_Positional_Calculate(&balance_pitch_angle, 0.0f-attitude_zero, angle.pitch);
    }
    
    float gyro_control = PID_Positional_Calculate(&balance_pitch_g, angle_control, imu660ra_gyro_x);
    
    // 更新平衡级联控制输出
    roll_balance_cascade.angular_speed_cycle.out = gyro_control;
    
    //printf("%f,%d\n",gyro_control,imu660ra_gyro_x);
    
    motor_control((int)gyro_control, (int)gyro_control);
 }

/**
 * @brief 车辆电机占空比控制
 */
void car_motor_control(void)
{
    // 累计行驶距离 = 上次距离 + (当前转速 / 60.0) * 车轮周长 * π * 0.001
    // car_speed 单位：转/分钟(RPM)；除以60转为转/毫秒；乘以周长得到毫米；乘0.001转为米
    car_distance += ((float)car_speed / 60.0f * WHEEL_CIRCUMFERENCE * PI * 0.001f);

    // 当运行状态为 1 时，计算电机占空比
    if (run_state)
    {
        // 左电机占空比：取平衡姿态环输出，限幅在 [-balance_duty_max, balance_duty_max]
        left_motor_duty = func_limit_ab((int16)roll_balance_cascade.angular_speed_cycle.out, -balance_duty_max, balance_duty_max);
        // 右电机占空比：同左电机，同步输出平衡控制量
        right_motor_duty = func_limit_ab((int16)roll_balance_cascade.angular_speed_cycle.out, -balance_duty_max, balance_duty_max);

        // 叠加转向控制：左电机 += 陀螺仪Z轴角速度/3，再限幅在 [-turn_duty_max, turn_duty_max]
        left_motor_duty = func_limit_ab(left_motor_duty + imu660ra_gyro_z / 3, -turn_duty_max, turn_duty_max);
        // 叠加转向控制：右电机 -= 陀螺仪Z轴角速度/3，再限幅在 [-turn_duty_max, turn_duty_max]
        right_motor_duty = func_limit_ab(right_motor_duty - imu660ra_gyro_z / 3, -turn_duty_max, turn_duty_max);
    }
    // 当运行状态为 0 时，电机关闭
    else
    {
        left_motor_duty = 0;  // 左电机占空比设为 0
        right_motor_duty = 0; // 右电机占空比设为 0
    }

    // 设置驱动的电机占空比（左电机取反，适配安装方向）
    small_driver_set_duty(-left_motor_duty, right_motor_duty);
}