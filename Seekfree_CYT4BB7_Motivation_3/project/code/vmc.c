#include "vmc.h"

/* 1 ___x
 * |     1  _____  4
 * |y     / a    \b
 *      2 \     / 3
 *         \   /
 *          \./
 *           5
 *12为L1，逆时针
 逆运动学求解函数
 输入：目标点坐标 (x, y)，以及各杆长度 L1, L2, L3, L4, L5
 输出：角度 α 和 β（单位：度）
*/
void inverse_kinematics(float x, float y,float *alpha, float *beta)
{
    // 计算 α 角
    float a = 2 * x * L1;
    float b = 2 * y * L1;
    float c = x*x + y*y + L1*L1 - L2*L2;
    float sqrt_term_alpha = sqrtf(a*a + b*b - c*c);
    float tan_alpha1 = (b + sqrt_term_alpha) / (a + c);
    float tan_alpha2 = (b - sqrt_term_alpha) / (a + c);
    float alpha1 = 2 * atanf(tan_alpha1);
    float alpha2 = 2 * atanf(tan_alpha2);
    
    alpha1 = (alpha1 >= 0)?alpha1:(alpha1 + 2 * PI);
    alpha2 = (alpha2 >= 0)?alpha2:(alpha2 + 2 * PI);
    if(alpha1 >= PI/4) *alpha = alpha1;
    else *alpha = alpha2;
    *alpha = (float)(*alpha * 180.0 / PI);
    
    // 计算 β 角
    float d = 2 * (x - L5) * L4;
    float e = 2 * y * L4;
    float f = (x - L5)*(x - L5) + L4*L4 + y*y - L3*L3;
    float sqrt_term_beta = sqrtf(d*d + e*e - f*f);
    float tan_beta1 = (e + sqrt_term_beta) / (d + f);
    float tan_beta2 = (e - sqrt_term_beta) / (d + f);
    float beta1 = 2 * atanf(tan_beta1);
    float beta2 = 2 * atanf(tan_beta2);
    
    if(beta1 >= 0 && beta1 <= PI/4) *beta = beta1;
    else *beta = beta2;
    *beta = (float)(*beta * 180.0 / PI);
}


// 舵机初始化
void servo_init(void)
{
    pwm_init(SERVO_MOTOR_PWM1, SERVO_MOTOR_FREQ, SERVO_MOTOR_DUTY(90));
    pwm_init(SERVO_MOTOR_PWM2, SERVO_MOTOR_FREQ, SERVO_MOTOR_DUTY(90));
    pwm_init(SERVO_MOTOR_PWM3, SERVO_MOTOR_FREQ, SERVO_MOTOR_DUTY(90));
    pwm_init(SERVO_MOTOR_PWM4, SERVO_MOTOR_FREQ, SERVO_MOTOR_DUTY(90));
}

//右腿控制
void vmc_right(float x, float y)
{
    float alpharight, betaright;
    inverse_kinematics(x, y, &alpharight, &betaright);
    alpharight = 270.0 - alpharight;
    betaright = 90.0 - betaright;

    //printf("右后向下: %f, 右前向上: %f\n", (float)alpharight, (float)betaright);
    
    if (alpharight < SERVO_MOTOR_L_MAX) alpharight = SERVO_MOTOR_L_MAX;
    if (alpharight > SERVO_MOTOR_R_MAX) alpharight = SERVO_MOTOR_R_MAX;
    if (betaright < SERVO_MOTOR_L_MAX) betaright = SERVO_MOTOR_L_MAX;
    if (betaright > SERVO_MOTOR_R_MAX) betaright = SERVO_MOTOR_R_MAX;
    
    pwm_set_duty(SERVO_MOTOR_PWM1, SERVO_MOTOR_DUTY((float)alpharight));
    pwm_set_duty(SERVO_MOTOR_PWM4, SERVO_MOTOR_DUTY((float)betaright));    
}  

//左腿控制
void vmc_left(float x, float y) 
{
    float alphaleft, betaleft;
    inverse_kinematics(x, y, &alphaleft, &betaleft);
    alphaleft = 270.0 - alphaleft;
    betaleft = 90.0 - betaleft; 
    
    //printf("左前向下: %f, 左后向上: %f\n", (float)alphaleft,(float)betaleft);
    
    if (alphaleft < SERVO_MOTOR_L_MAX) alphaleft = SERVO_MOTOR_L_MAX;
    if (alphaleft > SERVO_MOTOR_R_MAX) alphaleft = SERVO_MOTOR_R_MAX;
    if (betaleft < SERVO_MOTOR_L_MAX) betaleft = SERVO_MOTOR_L_MAX;
    if (betaleft > SERVO_MOTOR_R_MAX) betaleft = SERVO_MOTOR_R_MAX;
   
    pwm_set_duty(SERVO_MOTOR_PWM2, SERVO_MOTOR_DUTY((float)alphaleft));
    pwm_set_duty(SERVO_MOTOR_PWM3, SERVO_MOTOR_DUTY((float)betaleft));
}  


//servo_init();
    //system_delay_ms(5000);
    //vmc_right(xmid,80.0);
    //system_delay_ms(5000);
    //vmc_left(xmid,80.0);