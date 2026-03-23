#include "turn.h"

//====================================================转向控制结构实例====================================================
turn_control_struct turn_control = 
{
    .target_angle = 0.0f,
    .current_angle = 0.0f,
    .angle_error = 0.0f,
    .last_angle_error = 0.0f,
    .angle_error_sum = 0.0f,
    
    .kp = 1.0f,
    .ki = 0.01f,
    .kd = 0.1f,
    
    .turn_output = 0.0f,
    .turn_output_max = 1000.0f,
    
    .turn_enable = false,
    .turn_complete = true,
    .turn_threshold = 2.0f,
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     转向控制初始化
// 返回参数     void
// 使用示例     turn_control_init();
// 备注信息     初始化转向控制参数
//-------------------------------------------------------------------------------------------------------------------
void turn_control_init(void)
{
    turn_control.target_angle = 0.0f;
    turn_control.current_angle = 0.0f;
    turn_control.angle_error = 0.0f;
    turn_control.last_angle_error = 0.0f;
    turn_control.angle_error_sum = 0.0f;
    
    turn_control.kp = 1.0f;
    turn_control.ki = 0.01f;
    turn_control.kd = 0.1f;
    
    turn_control.turn_output = 0.0f;
    turn_control.turn_output_max = 1000.0f;
    
    turn_control.turn_enable = false;
    turn_control.turn_complete = true;
    turn_control.turn_threshold = 2.0f;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置转向目标角度
// 参数说明     target_angle - 目标转向角度（度）
// 返回参数     void
// 使用示例     turn_set_target_angle(90.0f);
// 备注信息     设置转向目标角度并开始转向
//-------------------------------------------------------------------------------------------------------------------
void turn_set_target_angle(float target_angle)
{
    // 获取当前偏航角作为起始角度
    turn_control.current_angle = pitch_balance_cascade.posture_value.yaw;
    
    // 设置目标角度（相对角度）
    turn_control.target_angle = turn_control.current_angle + target_angle;
    
    // 归一化到 -180 到 180 度
    if(turn_control.target_angle > 180.0f)
        turn_control.target_angle -= 360.0f;
    else if(turn_control.target_angle < -180.0f)
        turn_control.target_angle += 360.0f;
    
    // 重置转向状态
    turn_control.angle_error = 0.0f;
    turn_control.last_angle_error = 0.0f;
    turn_control.angle_error_sum = 0.0f;
    turn_control.turn_output = 0.0f;
    turn_control.turn_enable = true;
    turn_control.turn_complete = false;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     转向控制计算
// 返回参数     float - 转向输出值
// 使用示例     float output = turn_control_calculate();
// 备注信息     计算转向控制输出，调用此函数进行转向控制
//-------------------------------------------------------------------------------------------------------------------
float turn_control_calculate(void)
{
    if(!turn_control.turn_enable)
    {
        turn_control.turn_output = 0.0f;
        return 0.0f;
    }
    
    // 获取当前偏航角
    turn_control.current_angle = pitch_balance_cascade.posture_value.yaw;
    
    // 计算角度误差（考虑角度 wrap-around）
    turn_control.angle_error = turn_control.target_angle - turn_control.current_angle;
    
    // 归一化误差到 -180 到 180 度
    if(turn_control.angle_error > 180.0f)
        turn_control.angle_error -= 360.0f;
    else if(turn_control.angle_error < -180.0f)
        turn_control.angle_error += 360.0f;
    
    // 检查转向是否完成
    if(func_abs(turn_control.angle_error) < turn_control.turn_threshold)
    {
        turn_control.turn_complete = true;
        turn_control.turn_enable = false;
        turn_control.turn_output = 0.0f;
        return 0.0f;
    }
    
    // 计算误差积分（限幅）
    turn_control.angle_error_sum += turn_control.angle_error * 0.004f;  // 4ms 采样周期
    turn_control.angle_error_sum = func_limit_ab(turn_control.angle_error_sum, -100.0f, 100.0f);
    
    // 计算误差微分
    float angle_error_diff = (turn_control.angle_error - turn_control.last_angle_error) / 0.004f;
    
    // PID 计算
    turn_control.turn_output = 
        turn_control.kp * turn_control.angle_error +
        turn_control.ki * turn_control.angle_error_sum +
        turn_control.kd * angle_error_diff;
    
    // 限幅
    turn_control.turn_output = func_limit_ab(turn_control.turn_output, -turn_control.turn_output_max, turn_control.turn_output_max);
    
    // 保存上一次误差
    turn_control.last_angle_error = turn_control.angle_error;
    
    return turn_control.turn_output;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取转向状态
// 返回参数     bool - 转向完成返回true，否则返回false
// 使用示例     bool complete = turn_is_complete();
// 备注信息     检查转向是否完成
//-------------------------------------------------------------------------------------------------------------------
bool turn_is_complete(void)
{
    return turn_control.turn_complete;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     停止转向
// 返回参数     void
// 使用示例     turn_stop();
// 备注信息     停止转向控制
//-------------------------------------------------------------------------------------------------------------------
void turn_stop(void)
{
    turn_control.turn_enable = false;
    turn_control.turn_complete = true;
    turn_control.turn_output = 0.0f;
    turn_control.angle_error_sum = 0.0f;
}
