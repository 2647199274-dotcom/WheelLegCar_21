#ifndef _VMC_H_
#define _VMC_H_

#include "zf_common_headfile.h"
                                                            //         从上往下转
#define SERVO_MOTOR_PWM4            (TCPWM_CH57_P17_4 )     //CH3 17.4 180-0 右前腿                                                   
#define SERVO_MOTOR_PWM1            (TCPWM_CH30_P10_2 )     //CH2 10.2 0-180  右后腿            
#define SERVO_MOTOR_PWM2            (TCPWM_CH31_P10_3 )     //CH0 10.3 0-180  左前腿                
#define SERVO_MOTOR_PWM3            (TCPWM_CH58_P17_3 )     //CH1 17.3 180-0 左后腿                   

#define SERVO_MOTOR_FREQ            (300)                                       // 舵机电机PWM频率  单位Hz  范围 50-300

#define SERVO_MOTOR_L_MAX           (20.0f )                                       // 舵机电机左极限角度 单位度
#define SERVO_MOTOR_R_MAX           (160.0f)                                       // 舵机电机右极限角度 单位度

#define SERVO_MOTOR_DUTY(x) ((uint32_t)( roundf( (float)PWM_DUTY_MAX/(1000.0f/(float)SERVO_MOTOR_FREQ)*(0.5f+(float)(x)/90.0f) ) ))

#define L1  60.5f       //mm
#define L2  91.0f
#define L3  91.0f
#define L4  60.5f
#define L5  37.1f

#define xmid 18.55f    //中立位

//中立位y:25-150  x不好测
void servo_init(void);
void vmc_right(float x, float y);
void vmc_left(float x, float y);    





#endif