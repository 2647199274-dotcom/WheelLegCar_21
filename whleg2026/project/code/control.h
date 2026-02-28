#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"

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

extern Control_data control_data;

void car_start(void);

void balance_pid_init(void);
void control_balance(void);
















#endif