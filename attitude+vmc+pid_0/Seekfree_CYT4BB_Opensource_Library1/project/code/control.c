#include "control.h"

Control_data control_data = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

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
             motor_control(80, 80);
         }
         else  // 偶数次按下：停止电机
         {
             motor_control(0, 0);
         }
     }
 }