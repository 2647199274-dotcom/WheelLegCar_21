#include "my_menu.h"
#include "system_control.h"
#include "turn.h"
//=============================== 储存数据的起始指针 =============================
#define data_start beep_time_ms
//=============================== 储存数据的起始指针 =============================

//蜂鸣器
uint32 beep_time_ms = 300;

//=============================== 按键参数 =============================
//extern uint8 key1_flag;
//extern uint8 key2_flag;
//控制加减距离
#define SETUP_LEN               (8)
float SetupNumber[SETUP_LEN] = {0.01, 0.02, 0.1, 1, 5, 10, 30, 100};//控制加减大小
uint8_t SetupIndex;
//=============================== 按键参数 =============================

//=============================== 菜单参数 =============================
Menu_Item* key;
Menu_Item head;
#define DISPLAY_POSITION 95  //数据展示起始位置
#define FLASH_DATA_SIZE 28   // 存储数据的数量
//=============================== 菜单参数 =============================

////=============================== PID参数 =============================
//extern CascadePID balance_pid;

////=============================== PID参数 =============================

static void create_Menus(void)
{
//====================================== 主菜单参数 ========================================
    Menu_Item* Folder_Settings = Creat_Menu_Dynamic_Folder(&head,"1.Settings");
    Menu_Item* Folder_Balance = Creat_Menu_Dynamic_Folder(&head,"2.Balance");
    Menu_Item* Folder_Speed = Creat_Menu_Dynamic_Folder(&head,"3.Turn");
    Menu_Item* Folder_Steer = Creat_Menu_Dynamic_Folder(&head,"4.Steer");
    Menu_Item* Folder_Jump = Creat_Menu_Dynamic_Folder(&head,"5.Jump");
    Menu_Item* Folder_System = Creat_Menu_Dynamic_Folder(&head,"6.System");
//====================================== 主菜单参数 ========================================

//====================================== 设置菜单参数 =======================================
//    Menu_Item* Title_Settings = Creat_Menu_Dynamic_Folder(Folder_Settings,"-= Settings =-");
    Creat_Menu_Dynamic_Data(Folder_Settings,"Step:",&SetupIndex,uint8_Box);
    Creat_Menu_Dynamic_Data(Folder_Settings,"Beep:",&beep_time_ms,uint32_Box);
//====================================== 设置菜单参数 ==========================================

//====================================== 平衡参数菜单 =======================================  
//    Menu_Item* Title_Balance = Creat_Menu_Dynamic_Folder(Folder_Balance,"-= Balance Loop =-");
    Menu_Item* Folder_ZeroPoint = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.0 zero point");
    Menu_Item* Folder_PitchAngular = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.1 Pitch Angular");
    Menu_Item* Folder_PitchAngle = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.2 Pitch Angle");
    Menu_Item* Folder_PitchSpeed = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.3 Pitch Speed");
    Menu_Item* Folder_RollAngular = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.4 Roll Angular");
    Menu_Item* Folder_RollAngle = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.5 Roll Angle");
    Menu_Item* Folder_RollSpeed = Creat_Menu_Dynamic_Folder(Folder_Balance,"2.6 Roll Speed");
//====================================== 平衡菜单参数 =======================================  
       
//====================================== 机械零点参数 =======================================
    Creat_Menu_Dynamic_Data(Folder_ZeroPoint,"0_pitch:",&pitch_balance_cascade.posture_value.mechanical_zero,float_Box);
    Creat_Menu_Dynamic_Data(Folder_ZeroPoint,"0_roll:",&roll_balance_cascade.posture_value.mechanical_zero,float_Box);
//====================================== 机械零点参数 =======================================
    
//====================================== 俯仰角角速度环参数 =======================================
//    Menu_Item* Title_Angular_Loop = Creat_Menu_Dynamic_Folder(Folder_PitchAngular,"-= Pitch Angular =-");
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"KP:",&pitch_balance_cascade.angular_speed_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"KI:",&pitch_balance_cascade.angular_speed_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"KD:",&pitch_balance_cascade.angular_speed_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"KI_Max:",&pitch_balance_cascade.angular_speed_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"Out_Max:",&pitch_balance_cascade.angular_speed_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngular,"Output:",&pitch_balance_cascade.angular_speed_cycle.out,float_Box);
//====================================== 俯仰角角速度环参数 =======================================
    
//====================================== 俯仰角角度环参数 =======================================
//    Menu_Item* Title_Angle_Loop = Creat_Menu_Dynamic_Folder(Folder_AngleLoop,"-= Pitch Angle =-");
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"KP:",&pitch_balance_cascade.angle_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"KI:",&pitch_balance_cascade.angle_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"KD:",&pitch_balance_cascade.angle_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"KI_Max:",&pitch_balance_cascade.angle_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"Out_Max:",&pitch_balance_cascade.angle_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchAngle,"Output:",&pitch_balance_cascade.angle_cycle.out,float_Box);
//====================================== 俯仰角角度环参数 =======================================

//====================================== 俯仰角速度环参数 =======================================
//    Menu_Item* Title_Speed_Loop = Creat_Menu_Dynamic_Folder(Folder_PitchSpeed,"-= Pitch Speed =-");
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"KP:",&pitch_balance_cascade.speed_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"KI:",&pitch_balance_cascade.speed_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"KD:",&pitch_balance_cascade.speed_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"KI_Max:",&pitch_balance_cascade.speed_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"Out_Max:",&pitch_balance_cascade.speed_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_PitchSpeed,"Output:",&pitch_balance_cascade.speed_cycle.out,float_Box);
//====================================== 俯仰角速度环参数 =======================================

//====================================== 横滚角角速度环参数 =======================================
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"KP:",&roll_balance_cascade.angular_speed_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"KI:",&roll_balance_cascade.angular_speed_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"KD:",&roll_balance_cascade.angular_speed_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"KI_Max:",&roll_balance_cascade.angular_speed_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"Out_Max:",&roll_balance_cascade.angular_speed_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngular,"Output:",&roll_balance_cascade.angular_speed_cycle.out,float_Box);
//====================================== 横滚角角度环参数 =======================================

//====================================== 横滚角速度环参数 =======================================

    Creat_Menu_Dynamic_Data(Folder_RollAngle,"KP:",&roll_balance_cascade.angle_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngle,"KI:",&roll_balance_cascade.angle_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngle,"KD:",&roll_balance_cascade.angle_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngle,"KI_Max:",&roll_balance_cascade.angle_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngle,"Out_Max:",&roll_balance_cascade.angle_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollAngle,"Output:",&roll_balance_cascade.angle_cycle.out,float_Box);

//====================================== 横滚角角度环参数 =======================================

//====================================== 横滚角速度环参数 =======================================	
    // 速度环菜单
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"KP:",&roll_balance_cascade.speed_cycle.p,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"KI:",&roll_balance_cascade.speed_cycle.i,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"KD:",&roll_balance_cascade.speed_cycle.d,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"KI_Max:",&roll_balance_cascade.speed_cycle.i_value_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"Out_Max:",&roll_balance_cascade.speed_cycle.out_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_RollSpeed,"Output:",&roll_balance_cascade.speed_cycle.out,float_Box);
    
//====================================== 横滚角速度环参数 =======================================

//====================================== 舵机设置菜单 =======================================
    // Menu_Item* Title_Steer_Center = Creat_Menu_Dynamic_Folder(Folder_Steer,"-= Steer Center =-");
    Creat_Menu_Dynamic_Data(Folder_Steer,"St_1:",&steer_1.center_num,int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Steer,"St_2:",&steer_2.center_num,int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Steer,"St_3:",&steer_3.center_num,int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Steer,"St_4:",&steer_4.center_num,int16_Box);

//====================================== 舵机设置菜单 =======================================

//====================================== 转向设置菜单 =======================================
    // 转向PID参数
    Creat_Menu_Dynamic_Data(Folder_Speed,"KP:",&turn_control.kp,float_Box);
    Creat_Menu_Dynamic_Data(Folder_Speed,"KI:",&turn_control.ki,float_Box);
    Creat_Menu_Dynamic_Data(Folder_Speed,"KD:",&turn_control.kd,float_Box);
    Creat_Menu_Dynamic_Data(Folder_Speed,"Out_Max:",&turn_control.turn_output_max,float_Box);
    Creat_Menu_Dynamic_Data(Folder_Speed,"Threshold:",&turn_control.turn_threshold,float_Box);
    // 转向状态
    Creat_Menu_Dynamic_Data(Folder_Speed,"Enable:",&turn_control.turn_enable,bool_Box);
    // Creat_Menu_Dynamic_Data(Folder_Speed,"Complete:",&turn_control.turn_complete,bool_Box);
    Creat_Menu_Dynamic_Data(Folder_Speed,"Output:",&turn_control.turn_output,float_Box);
    // Creat_Menu_Dynamic_Data(Folder_Speed,"Target:",&turn_control.target_angle,float_Box);
    // Creat_Menu_Dynamic_Data(Folder_Speed,"Current:",&turn_control.current_angle,float_Box);

//====================================== 转向设置菜单 =======================================

//====================================== 跳跃设置菜单 =======================================
    // Menu_Item* Title_Jump_Time = Creat_Menu_Dynamic_Folder(Folder_Jump,"-= Jump Time =-");
    Creat_Menu_Dynamic_Data(Folder_Jump,"J_T_1:",&jump_time_num[0],int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Jump,"J_T_2:",&jump_time_num[1],int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Jump,"J_T_3:",&jump_time_num[2],int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Jump,"J_T_4:",&jump_time_num[3],int16_Box);
    Creat_Menu_Dynamic_Data(Folder_Jump,"Jump_Flag:",&jump_flag,bool_Box);

//====================================== 跳跃设置菜单 =======================================


//====================================== 系统菜单 =======================================

    Menu_Item* Folder_System_State = Creat_Menu_Dynamic_Folder(Folder_System,"6.1 System State");
    Menu_Item* Folder_System_Control = Creat_Menu_Dynamic_Folder(Folder_System,"6.2 Control");
    
    // 系统状态菜单
    Creat_Menu_Dynamic_Data(Folder_System_State,"State:",&system_state,uint8_Box);
    Creat_Menu_Dynamic_Data(Folder_System_State,"Mode:",&run_mode,uint8_Box);
    Creat_Menu_Dynamic_Data(Folder_System_State,"Run:",&run_state,bool_Box);
    Creat_Menu_Dynamic_Data(Folder_System_State,"Jump:",&jump_flag,bool_Box);
    
    // 控制参数菜单
    Creat_Menu_Dynamic_Data(Folder_System_Control,"Target_Speed:",&target_speed,float_Box);
    Creat_Menu_Dynamic_Data(Folder_System_Control,"Target_Turn:",&target_turn_angle,float_Box);
    Creat_Menu_Dynamic_Data(Folder_System_Control,"Target_Yaw:",&target_yaw,float_Box);

//====================================== 系统菜单 =======================================

    // // 数据菜单
    // Menu_Item* Item_Save = Creat_Menu_Dynamic_Folder(Folder_System,"Save");
    // Menu_Item* Item_Read = Creat_Menu_Dynamic_Folder(Folder_System,"Read");
    // Menu_Item* Item_Clear = Creat_Menu_Dynamic_Folder(Folder_System,"Clear");
    
    // // 系统菜单
    // Menu_Item* Item_Status = Creat_Menu_Dynamic_Folder(Folder_System,"Status");
    
    // // 保存数据功能
    // Item_Save->kind = funtion_Box;
    // Item_Save->data = (void *)flash_write_data;
    
    // // 读取数据功能
    // Item_Read->kind = funtion_Box;
    // Item_Read->data = (void *)flash_read_data;
    
    // // 清除数据功能
    // Item_Clear->kind = funtion_Box;
    // Item_Clear->data = (void *)flash_erase_data;
    

}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     菜单初始化
// 返回参数     void
// 使用示例     menu_init();
// 备注信息     初始化菜单系统，包括按键、FLASH和菜单结构
//-------------------------------------------------------------------------------------------------------------------
void menu_init(void)
{
    switch_init();                                      //拨码开关初始化
    flash_init(); 				// 初始化FLASH
    my_key_init(10);                                    //按键初始化     PIT1
    
    // 初始化菜单结构
    head.data = NULL;
    head.father = NULL;
    head.first_son = NULL;
    head.kind = MENU_Folder;
    head.last_brother = NULL;
    head.name = "Main_MENU";
    head.next_brother = NULL;
    head.number = 0;
    head.sons = 0;

    create_Menus();
    
    if(head.first_son != NULL)
        key = head.first_son;
    
    All_Folder_Menu_Init(&head);
    
    // 检查是否需要读取FLASH数据
    while (!Switch_Get(SWITCH_1) && !Switch_Get(SWITCH_2))
    {
        system_delay_ms(100);
    }
    
    if(Switch_Get(SWITCH_1))
    {
        flash_read_data();                                  // 读取FLASH数据
    }
    else
    {
        system_delay_ms(500);
    }
    
    tft180_clear();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示菜单数据
// 返回参数     void
// 使用示例     menu_show_num();
// 备注信息     显示当前菜单中的数据值
//-------------------------------------------------------------------------------------------------------------------
void menu_show_num(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;
    
    for(int i = 0;i < h->sons; i++)
    {
        // 确保显示位置在屏幕范围内 (TFT180屏幕高度为128像素)
        if(i * 16 < 128)
        {
            switch(s->kind)
            {
                case int8_Box:
                    tft180_show_int(DISPLAY_POSITION+2,i*16,*(int8 *)s->data,4);
                    break;
                case int16_Box:
                    tft180_show_int(DISPLAY_POSITION,i*16,*(int16 *)s->data,4);
                    break;
                case int32_Box:
                    tft180_show_int(DISPLAY_POSITION,i*16,*(int32 *)s->data,5);
                    break;
                case int64_Box:
                    tft180_show_int(DISPLAY_POSITION,i*16,*(int64 *)s->data,5);
                    break;
                case uint8_Box:
                    tft180_show_uint(DISPLAY_POSITION+10,i*16,*(uint8 *)s->data,4);
                    break;
                case uint16_Box:
                    tft180_show_uint(DISPLAY_POSITION,i*16,*(uint16 *)s->data,4);
                    break;
                case uint32_Box:
                    tft180_show_uint(DISPLAY_POSITION,i*16,*(uint32 *)s->data,5);
                    break;
                case uint64_Box:
                    tft180_show_uint(DISPLAY_POSITION,i*16,*(uint64 *)s->data,5);
                    break;
                case float_Box:
                    tft180_show_float(DISPLAY_POSITION,i*16,*(float *)s->data,4,2);
                    break;
                case double_Box:
                    tft180_show_float(DISPLAY_POSITION,i*16,*(double *)s->data,4,2);
                    break;
                case funtion_Box:
                    tft180_clear();
                    void (*func)() = (void (*)())s->data;
                    func();
                    if(i * 16 < 128)
                        tft180_show_string(DISPLAY_POSITION,i*16,s->name);
                    break;
                case bool_Box:
                    if(i * 16 < 128)
                    {
                        if(*(bool *)s->data != false)
                            tft180_show_char(DISPLAY_POSITION+3,i*16,'T');
                        else
                            tft180_show_char(DISPLAY_POSITION+3,i*16,'F');
                    }
                    break;
                default:
                    break;
            }
        }
        s = s->next_brother;
    }
}

extern key_select_enum my_key_select;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示菜单
// 返回参数     void
// 使用示例     menu_show();
// 备注信息     显示当前菜单内容
//-------------------------------------------------------------------------------------------------------------------
void menu_show(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;
        
    if(my_key_select != KEY_SELECT_NULL)		
    {
	tft180_clear();
    }

    // 确保显示位置在屏幕范围内 (TFT180屏幕高度为128像素)
    for(int i = 0;i < h->sons && i * 16 < 128; i++)
    {
        tft180_show_string(16,i*16,s->name);
        s = s->next_brother;
    }
    
    show_key();
    menu_show_num();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示按键选中状态
// 返回参数     void
// 使用示例     show_key();
// 备注信息     显示当前选中的菜单项
//-------------------------------------------------------------------------------------------------------------------
void show_key(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;

    // 确保显示位置在屏幕范围内 (TFT180屏幕高度为128像素)
    for(int i = 0;i < h->sons && i * 16 < 128; i++)
    {
        if(s == key)
        {
            tft180_show_string(0,i*16,"->");
        }
        else
        {
            tft180_show_string(0,i*16,"  ");
        }
        s = s->next_brother;
    }
}


// ==================== FLASH DATA ====================

// 存储数据到FLASH的变量指针数组
uint32 *FLASH_DATA[] = {
    (uint32 *)&beep_time_ms,
    (uint32 *)&SetupIndex,

    //机械零点参数
    (uint32 *)&pitch_balance_cascade.posture_value.mechanical_zero,
    (uint32 *)&roll_balance_cascade.posture_value.mechanical_zero,
    (uint32 *)&pitch_balance_cascade_resave.posture_value.mechanical_zero,
    (uint32 *)&roll_balance_cascade_resave.posture_value.mechanical_zero,
    //储存俯仰角平衡角速度环PID参数
    (uint32 *)&pitch_balance_cascade.angular_speed_cycle.p,
    (uint32 *)&pitch_balance_cascade.angular_speed_cycle.i,
    (uint32 *)&pitch_balance_cascade.angular_speed_cycle.d,
    (uint32 *)&pitch_balance_cascade.angular_speed_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade.angular_speed_cycle.out_max,
    
    (uint32 *)&pitch_balance_cascade_resave.angular_speed_cycle.p,
    (uint32 *)&pitch_balance_cascade_resave.angular_speed_cycle.i,
    (uint32 *)&pitch_balance_cascade_resave.angular_speed_cycle.d,
    (uint32 *)&pitch_balance_cascade_resave.angular_speed_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade_resave.angular_speed_cycle.out_max,
    //储存俯仰角平衡角度环PID参数
    (uint32 *)&pitch_balance_cascade.angle_cycle.p,
    (uint32 *)&pitch_balance_cascade.angle_cycle.i,
    (uint32 *)&pitch_balance_cascade.angle_cycle.d,
    (uint32 *)&pitch_balance_cascade.angle_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade.angle_cycle.out_max,
    
    (uint32 *)&pitch_balance_cascade_resave.angle_cycle.p,
    (uint32 *)&pitch_balance_cascade_resave.angle_cycle.i,
    (uint32 *)&pitch_balance_cascade_resave.angle_cycle.d,
    (uint32 *)&pitch_balance_cascade_resave.angle_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade_resave.angle_cycle.out_max,
    //储存俯仰角平衡速度环PID参数
    (uint32 *)&pitch_balance_cascade.speed_cycle.p,
    (uint32 *)&pitch_balance_cascade.speed_cycle.i,
    (uint32 *)&pitch_balance_cascade.speed_cycle.d,
    (uint32 *)&pitch_balance_cascade.speed_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade.speed_cycle.out_max,
    
    (uint32 *)&pitch_balance_cascade_resave.speed_cycle.p,
    (uint32 *)&pitch_balance_cascade_resave.speed_cycle.i,
    (uint32 *)&pitch_balance_cascade_resave.speed_cycle.d,
    (uint32 *)&pitch_balance_cascade_resave.speed_cycle.i_value_max,
    (uint32 *)&pitch_balance_cascade_resave.speed_cycle.out_max,
    
    //储存横滚角平衡角速度环PID参数
    (uint32 *)&roll_balance_cascade.angular_speed_cycle.p,
    (uint32 *)&roll_balance_cascade.angular_speed_cycle.i,
    (uint32 *)&roll_balance_cascade.angular_speed_cycle.d,
    (uint32 *)&roll_balance_cascade.angular_speed_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade.angular_speed_cycle.out_max,
    
    (uint32 *)&roll_balance_cascade_resave.angular_speed_cycle.p,
    (uint32 *)&roll_balance_cascade_resave.angular_speed_cycle.i,
    (uint32 *)&roll_balance_cascade_resave.angular_speed_cycle.d,
    (uint32 *)&roll_balance_cascade_resave.angular_speed_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade_resave.angular_speed_cycle.out_max,
    //储存横滚角平衡角度环PID参数    
    (uint32 *)&roll_balance_cascade.angle_cycle.p,
    (uint32 *)&roll_balance_cascade.angle_cycle.i,
    (uint32 *)&roll_balance_cascade.angle_cycle.d,
    (uint32 *)&roll_balance_cascade.angle_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade.angle_cycle.out_max,
    
    (uint32 *)&roll_balance_cascade_resave.angle_cycle.p,
    (uint32 *)&roll_balance_cascade_resave.angle_cycle.i,
    (uint32 *)&roll_balance_cascade_resave.angle_cycle.d,
    (uint32 *)&roll_balance_cascade_resave.angle_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade_resave.angle_cycle.out_max,
    //储存横滚角平衡角速度环PID参数    
    (uint32 *)&roll_balance_cascade.speed_cycle.p,
    (uint32 *)&roll_balance_cascade.speed_cycle.i,
    (uint32 *)&roll_balance_cascade.speed_cycle.d,
    (uint32 *)&roll_balance_cascade.speed_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade.speed_cycle.out_max,
    
    (uint32 *)&roll_balance_cascade_resave.speed_cycle.p,
    (uint32 *)&roll_balance_cascade_resave.speed_cycle.i,
    (uint32 *)&roll_balance_cascade_resave.speed_cycle.d,
    (uint32 *)&roll_balance_cascade_resave.speed_cycle.i_value_max,
    (uint32 *)&roll_balance_cascade_resave.speed_cycle.out_max,
    //储存舵机参数
    (uint32 *)&steer_1.center_num,
    (uint32 *)&steer_2.center_num,
    (uint32 *)&steer_3.center_num,
    (uint32 *)&steer_4.center_num,
    //储存跳跃时间参数
    (uint32 *)&(jump_time_num[0]),
    (uint32 *)&(jump_time_num[1]),
    (uint32 *)&(jump_time_num[2]),
    (uint32 *)&(jump_time_num[3]),
    
    //储存转向控制参数
    (uint32 *)&turn_control.kp,
    (uint32 *)&turn_control.ki,
    (uint32 *)&turn_control.kd,
    (uint32 *)&turn_control.turn_output_max,
    (uint32 *)&turn_control.turn_threshold,

};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除FLASH数据
// 返回参数     void
// 使用示例     flash_erase_data();
// 备注信息     清除FLASH中保存的参数数据
//-------------------------------------------------------------------------------------------------------------------
#define     PAGE_DISP_NUM             (7)                   //一页最多显示的操作函数个数
#define     FLASH_STORAGE_PAGE        (95)                  // 存储数据用的页码 (最后一页)

void flash_erase_data(void)
{
    uint32 sector_num = 0;
    uint32 page_num = FLASH_STORAGE_PAGE;
    
    if(flash_check(sector_num, page_num))                  // 判断是否有数据
    {
        flash_erase_page(sector_num, page_num);            // 擦除这一页
    }
    
    // 通过菜单显示清除完成信息
    tft180_clear();
    tft180_show_string(0, 0, "Clear Finish!");
    system_delay_ms(1000);
    tft180_clear();
    menu_show();                                         // 重新显示菜单
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向FLASH写入数据
//  备注信息      CYT4BB7 FLASH特性：
//                - 总大小：192KB
//                - 每页大小：2KB
//                - 总页数：96页
//                - 每页可存储512个uint32数据
// 返回参数     void
// 使用示例     flash_write_data();
// 备注信息     将参数数据写入FLASH保存
//-------------------------------------------------------------------------------------------------------------------
void flash_write_data(void)
{
    uint32 sector_num = 0;
    uint32 page_num = FLASH_STORAGE_PAGE;
    uint32 data_count = 0;
    
    uint32 FLASH_DATA_NUM = sizeof(FLASH_DATA) / sizeof(FLASH_DATA[0]); //需要写入的数据个数
    uint32 *data_ptr = NULL;      //创建指针，将数据作为指针传入写入函数

    // 通过菜单显示写入信息
    tft180_clear();
//    tft180_show_string(0, 0, "START WRITE!");
//    system_delay_ms(500);

    // 清空缓冲区
    flash_buffer_clear();
    
    tft180_show_string(0, 0, "START WRITE!");
    system_delay_ms(500);
    tft180_clear();
    // 准备数据到缓冲区
    for(data_count = 0; data_count < FLASH_DATA_NUM; ++data_count)
    {
      	tft180_show_string(0, 0, "reading...");
        data_ptr = (uint32 *)FLASH_DATA[data_count];
        if(data_ptr != NULL) //如果待写入数据非空，就写入缓冲页
        {
            flash_union_buffer[data_count].uint32_type = (uint32)*data_ptr;
        }
    }

    // 擦除目标页
    flash_erase_page(sector_num, page_num);
    
    // 向指定页写入缓冲区中的数据
    flash_write_page_from_buffer(sector_num, page_num, FLASH_DATA_NUM);
    
    // 清空缓冲区
    flash_buffer_clear();
    
    // 通过菜单显示写入完成信息
    tft180_clear();
    tft180_show_string(0, 0, "WRITE IS OK!");
    system_delay_ms(1000);
    tft180_clear();
    menu_show();                                         // 重新显示菜单
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从FLASH读取数据
// 返回参数     void
// 使用示例     flash_read_data();
// 备注信息     从FLASH读取保存的参数数据
//-------------------------------------------------------------------------------------------------------------------
void flash_read_data(void)
{
    uint32 sector_num = 0;
    uint32 page_num = FLASH_STORAGE_PAGE;
    uint32 data_count = 0;
    
    uint32 FLASH_DATA_NUM = sizeof(FLASH_DATA) / sizeof(FLASH_DATA[0]); //需要读取的数据个数
    uint32 *data_ptr = &data_start;

    // 通过菜单显示读取信息
    tft180_clear();
    tft180_show_string(0, 0, "Start Reading!");         //显示读取字样
    system_delay_ms(1000);
//    tft180_clear();

    // 清空缓冲区
    flash_buffer_clear();
    
    // 从FLASH读取数据到缓冲区
    flash_read_page_to_buffer(sector_num, page_num, FLASH_DATA_NUM);

    // 遍历每一个数据
    for (data_count = 0; data_count < FLASH_DATA_NUM; data_count ++ )
    {
        uint32 temp_value = flash_union_buffer[data_count].uint32_type;

        if(temp_value != 0xFFFFFFFF)  // FLASH 擦除后为全 1
        {
            data_ptr = (uint32 *)FLASH_DATA[data_count];
            if (data_ptr != NULL)
            {
                *data_ptr = (uint32)temp_value;
            }
        }
        else
        {
            data_ptr = (uint32 *)FLASH_DATA[data_count];
            if (data_ptr != NULL)
            {
                *data_ptr = 0;
            }
        }
    }
    
    // 保存平衡控制备份结构体到初始参数
	// 读取 FLASH 后，将主结构体复制到备份结构体
	
    // 通过菜单显示读取完成信息
    tft180_clear();
    tft180_show_string(0, 0, "Read Complete!");
    system_delay_ms(1000);
    tft180_clear();
}
