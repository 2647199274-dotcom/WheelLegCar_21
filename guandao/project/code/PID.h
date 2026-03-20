#ifndef PID_H
#define PID_H

// PID类型枚举
typedef enum {
    PID_POSITIONAL,    // 位置式PID
    PID_INCREMENTAL    // 增量式PID
} PID_Type;

// 位置式PID数据结构体
typedef struct {

    float Kp;
    float Ki;
    float Kd;
    float dt;

    float SP;            // 设定值
    float FB;            // 反馈值
    float error;         
    float last_error;    
    float last_error_integral; 
    float output;        

    // 限幅参数
    float output_max;   
    float output_min;    
    float integral_max;  
    float integral_min;  
    float derivative_max;
} PID_Positional_Data;

// 增量式PID数据结构体
typedef struct {

    float Kp;
    float Ki;
    float Kd;
    float dt;

    float SP;            // 设定值
    float FB;            // 反馈值
    float error[3];      // 误差队列 [当前, 上一周期, 上两周期]
    float increment;     

    // 限幅参数
    float inc_max;     
    float inc_min;     
} PID_Incremental_Data;

//电机闭环控制pid
extern PID_Positional_Data motor_pid_left; 
extern PID_Positional_Data motor_pid_right;  

// 位置式PID函数声明
void PID_Positional_Init(PID_Positional_Data *pid, float Kp, float Ki, float Kd, float dt,
                         float output_max, float output_min, float integral_max, float derivative_max);
float PID_Positional_Calculate(PID_Positional_Data *pid, float set_point, float feedback);
void PID_Positional_Clear(PID_Positional_Data *pid);

// 增量式PID函数声明
void PID_Incremental_Init(PID_Incremental_Data *pid, float Kp, float Ki, float Kd, float dt,
                          float inc_max, float inc_min);
float PID_Incremental_Calculate(PID_Incremental_Data *pid, float set_point, float feedback);
void PID_Incremental_Clear(PID_Incremental_Data *pid);

#endif /* PID_H */