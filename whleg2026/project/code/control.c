#include "control.h"
#include "Quaternion.h"
Control_data control_data = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
PID_Positional_Data balance_pitch_g;
PID_Positional_Data balance_pitch_angle;
float angle_control=0.0;

int start = 0;  // 按键状态计数，用于启动/停止电机
void car_start(void)
 {
     if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
     {
         start++;  // 按键短按一次，状态计数加1
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
    
    Attitude_get();
    
    if(sys_times % 5 == 0)
    {
      float angle_control = PID_Positional_Calculate(&balance_pitch_angle, 0.0f-attitude_zero, angle.pitch);
    }
    
    float gyro_control = PID_Positional_Calculate(&balance_pitch_g, angle_control, imu660ra_gyro_x);
    
    //printf("%f,%d\n",gyro_control,imu660ra_gyro_x);
    
    motor_control((int)gyro_control, (int)gyro_control);
 }