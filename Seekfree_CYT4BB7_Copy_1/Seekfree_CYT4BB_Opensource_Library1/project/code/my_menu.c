/*
 * my_menu.c
 *
 *  Created on: 2025年11月28日
 *      Author: ZhuanZ（无密码）
 */


#include "my_menu.h"
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
#define DISPLAY_POSITION 90  //数据展示起始位置
//=============================== 菜单参数 =============================

////=============================== PID参数 =============================
//extern CascadePID balance_pid;

////=============================== PID参数 =============================

static void create_Menus(void)
{
    //主菜单00
        Menu_Item* Folder_Balance_PID = Creat_Menu_Dynamic_Folder(&head,"1.imu_blance_pid");
        Menu_Item* Folder_imu660ra_DATA = Creat_Menu_Dynamic_Folder(&head,"2.imu_Data");
        Menu_Item* Folder_SENSOR = Creat_Menu_Dynamic_Folder(&head,"3.motor_PID");
        Menu_Item* Folder_ENCODER = Creat_Menu_Dynamic_Folder(&head,"4.speed");
        Menu_Item* Folder_STEUP = Creat_Menu_Dynamic_Folder(&head,"5.SetupIndex");
        Menu_Item* Folder_BEEP_TIME = Creat_Menu_Dynamic_Folder(&head,"6.beep_time");

        //PID 菜单
        //陀螺仪平衡控制PID
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"o_kp:",&balance_pid.outer.kp,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"o_ki:",&balance_pid.outer.ki,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"o_kd:",&balance_pid.outer.kd,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"m_kp:",&balance_pid.middle.kp,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"m_ki:",&balance_pid.middle.ki,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"m_kd:",&balance_pid.middle.kd,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"i_kp:",&balance_pid.inner.kp,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"i_ki:",&balance_pid.inner.ki,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"i_kd:",&balance_pid.inner.kd,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"o_out_v:",&balance_pid.outer.output,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Balance_PID,"i_out_v:",&balance_pid.inner.output,float_Box);
        // 限幅菜单
        Menu_Item* Folder_Max_out = Creat_Menu_Dynamic_Folder(&Folder_Balance_PID,"Max_out");
        Creat_Menu_Dynamic_Data(Folder_Max_out,"omax_o:",&balance_pid.outer.maxOutput,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Max_out,"imax_o:",&balance_pid.inner.maxOutput,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Max_out,"omax_i:",&balance_pid.outer.maxIntegral,float_Box);
        Creat_Menu_Dynamic_Data(Folder_Max_out,"imax_i:",&balance_pid.inner.maxIntegral,float_Box);

        //陀螺仪参数菜单
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"mid:",&balance_target_angel,float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"g_x:",&eulerAngle[0],float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"g_y:",&eulerAngle[1],float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"g_z:",&eulerAngle[2],float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"a_x:",&acceleration[0],float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"a_y:",&acceleration[1],float_Box);
        Creat_Menu_Dynamic_Data(Folder_imu660ra_DATA,"a_z:",&acceleration[2],float_Box);

        //SETUP菜单控制步长
        Creat_Menu_Dynamic_Data(Folder_STEUP,"SetupIndex:",&SetupIndex,uint8_Box);

        //蜂鸣器时间
        Creat_Menu_Dynamic_Data(Folder_BEEP_TIME,"beep:",&beep_time_ms,uint32_Box);

//        Creat_Menu_Dynamic_Data(Folder_PID,"Kp:",&SpeedPid.kp,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_PID,"Ki:",&SpeedPid.ki,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_PID,"Kd:",&SpeedPid.kd,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_PID,"Output:",&SpeedPid.output,float_Box);
//
//        //MOTOR 菜单
//        Creat_Menu_Dynamic_Data(Folder_MOTOR,"L_SPD:",&L_Speed,int32_Box);
//        Creat_Menu_Dynamic_Data(Folder_MOTOR,"R_SPD:",&R_Speed,int32_Box);
//        Creat_Menu_Dynamic_Data(Folder_MOTOR,"M_SPD:",&Max_Speed,uint16_Box);
//
//        //SENSOR 菜单
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"S_1:",&sense_val[0],uint16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"S_2:",&sense_val[1],uint16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"S_3:",&sense_val[2],uint16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"S_4:",&sense_val[3],uint16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"W_1:",&sense_weight[0],int16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"W_2:",&sense_weight[1],int16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"W_3:",&sense_weight[2],int16_Box);
//        Creat_Menu_Dynamic_Data(Folder_SENSOR,"W_4:",&sense_weight[3],int16_Box);
//        //ENCODER编码器菜单
//        Creat_Menu_Dynamic_Data(Folder_ENCODER,"E_RSPD:",&encoder_data_right,int16_Box);
//        Creat_Menu_Dynamic_Data(Folder_ENCODER,"E_LSPD:",&encoder_data_left,int16_Box);
//
//        //舵机菜单
//        Creat_Menu_Dynamic_Data(Folder_STEER,"ANGLE:",&steer_angel,int32_Box);
////        Creat_Menu_Dynamic_Data(Folder_STEER,"SCALE:",&scale_factor,float_Box);
//        Creat_Menu_Dynamic_Data(Folder_STEER,"Output:",&SpeedPid.output,float_Box);
//        //RUN_MODE菜单，测试or巡线
//
//        Creat_Menu_Dynamic_Data(Folder_RUN_MOOD,"TEST MODE",&test_mode,bool_Box);
//        Creat_Menu_Dynamic_Data(Folder_RUN_MOOD,"LINE_MODE",&loop_mode,bool_Box);
//        Creat_Menu_Dynamic_Data(Folder_RUN_MOOD,"STOP_MODE",&stop_mode,bool_Box);
//
//        //步长调节
//        Creat_Menu_Dynamic_Data(Folder_STEP,"STEP",&SetupIndex,uint8_Box);

}
void menu_init(void)
{
    switch_init();                                      //拨码开关初始化
//    SetupIndex = 2;                                     //初始化加减步长
    my_key_init(10);                                    //按键初始化     PIT1
    while (!Switch_Get(SWITCH_1) && !Switch_Get(SWITCH_2))
    {
        tft180_show_string(0, 0, "Whether Read DATA?");
    }
    if(Switch_Get(SWITCH_1))
    {
        Read_EEPROM();
    }

    else
    {
        tft180_show_string(0, 0, "Skip to Read DATA!");
        system_delay_ms(500);
    }

    Write_EEPROM(); //将数据写入EEPROM保存
    tft180_clear();

    head.data = NULL;
    head.father = NULL;
    head.first_son = NULL;
    head.kind = MENU_Folder;
    head.last_brother = NULL;
    head.name = "Main_MENU";
    head.next_brother = NULL;
    head.number = 0;
    head.sons = 0;

//    SetupIndex= 1;
    create_Menus();
    if(head.first_son!= NULL)
        key = head.first_son;
//    key = head.first_son;
    All_Folder_Menu_Init(&head);
    system_delay_ms(beep_time_ms);

}

void menu_show_num(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;
    for(int i = 0;i < h->sons; i++)
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
                tft180_show_float(DISPLAY_POSITION,i*16,*(double *)s->data,4,2);
                break;
            case double_Box:
                tft180_show_float(DISPLAY_POSITION,i*16,*(double *)s->data,4,2);
                break;
            case funtion_Box:
                tft180_clear();
                void (*func)() = (void (*)())s->data;
                func();
                tft180_show_string(DISPLAY_POSITION,i*16,s->name);
                break;
            case bool_Box:
                if(*(bool *)s->data != false)
                    tft180_show_char(DISPLAY_POSITION+3,i*16,'T');
                else
                    tft180_show_char(DISPLAY_POSITION+3,i*16,'F');
                break;
            default:
                break;
        }
        s = s->next_brother;
    }
}

void menu_show(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;
    for(int i = 0;i < h->sons;i++)
    {
        tft180_show_string(16,i*16,s->name);
      s = s->next_brother;
    }
    show_key();
    menu_show_num();
}

void show_key(void)
{
    Menu_Item *h = key->father;
    Menu_Item *s = h->first_son;

    for(int i = 0;i < h->sons; i++)
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


// ==================== EEPROM DATA ====================


float *EEPROM_DATA[] = {
    (float *)&beep_time_ms,
    (float *)&SetupIndex,
    //储存平衡PID参数
    (float *)&balance_pid.outer.kp,
//    (float *)&balance_pid.outer.ki,
    (float *)&balance_pid.outer.kd,
    (float *)&balance_pid.middle.kp,
    (float *)&balance_pid.inner.kp,
    (float *)&balance_pid.inner.ki,
    (float *)&balance_pid.inner.kd,


    (float *)&balance_target_angel,
//    (float *)&SpeedPid.kp,
//    (float *)&SpeedPid.ki,
//    (float *)&SpeedPid.kd,

//    (float *)&Max_Speed,        //速度限幅
//    (float *)&scale_factor,     //舵机PID映射
//    (float *)&run_mode,
};
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      清除EEPROM扇区信息
//  参数说明      sector_num    仅可填写0  此处扇区编号并无实际作用，只是留出接口
//  参数说明      page_num      需要写入的页编号   参数范围0-11
//  备注信息      遍历所有数据页，找到有数据的并且擦除
//-------------------------------------------------------------------------------------------------------------------
#define     PAGE_DISP_NUM             (7)                   //一页最多显示的操作函数个数
#define     FLASH_SECTION_INDEX       (0)                   // 存储数据用的扇区
#define     FLASH_PAGE_INDEX          (8)                   // 存储数据用的页码 倒数第一个页码

void eeprom_erase_sector(void)
{
    float sectorNum = 0;
    for (uint16 pageNum = 0; pageNum < 11; ++pageNum)
    {
        if(flash_check(sectorNum, pageNum))                  // 判断是否有数据
        {
            flash_erase_page(sectorNum, pageNum);            // 擦除这一页
        }
    }
    tft180_show_string(0, 0, "Clear Finish!");
    system_delay_ms(1000);
}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      向EEPROM写入数据(一页存一个数据)
//  参数说明      sector_num    仅可填写0  此处扇区编号并无实际作用，只是留出接口
//  参数说明      page_num      需要写入的页编号   参数范围0-11
/*  备注信息      一共有12个扇区
                 每个扇区有1024页
                 一共有96KB
                 一页只有8个字节，放两个32位，4个16位*/
//-------------------------------------------------------------------------------------------------------------------
void Write_EEPROM(void) //
{

    float EEPROM_DATA_NUM = sizeof(EEPROM_DATA) / sizeof(EEPROM_DATA[0]); //需要写入的数据页数
    float pageNum = 0;
    float *data_ptr = NULL;      //创建指针，将eeprom中的数据作为指针传入写入函数

    tft180_show_int(0,0,data_start,3);
    system_delay_ms(500);
    //擦除扇区0的第11页，作为数据缓冲页
    flash_erase_page(0,11);

    tft180_show_string(0, 0, "START WRITE!");
    system_delay_ms(500);

    for(uint16 pageNum = 0; pageNum < EEPROM_DATA_NUM; ++pageNum)
    {
        data_ptr = (float *)EEPROM_DATA[pageNum];
        if(data_ptr != NULL)//如果待写入数据非空，就写入缓冲页
        {
            flash_union_buffer[pageNum].float_type=(float)*data_ptr;
        }
    }

    //再向指定页写入缓冲区中的数据
    flash_write_page_from_buffer(0, 11);
    flash_buffer_clear();
    tft180_show_string(0, 0, "WRITE IS OK!");
    system_delay_ms(1000);
}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      读入EEPROM数据
//  参数说明      sector_num    仅可填写0  此处扇区编号并无实际作用，只是留出接口
//  参数说明      page_num      需要写入的页编号   参数范围0-11
//  参数说明      data_temp     存储读入的数据
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void Read_EEPROM(void)
{
    float EEPROM_DATA_NUM = sizeof(EEPROM_DATA) / sizeof(EEPROM_DATA[0]); //需要写入的数据页数
    float sectorNum = 0;
    float *data_ptr = &data_start;

    tft180_clear();                                     //清屏
    tft180_show_string(0, 0, "Start Reading!");         //显示读取字样
    system_delay_ms(1000);
    tft180_clear();

    flash_buffer_clear();
    flash_read_page_to_buffer(0, 11);
    tft180_clear();

    //从第一页开始遍历每一页
    for (uint16 pageNum = 0; pageNum < EEPROM_DATA_NUM; pageNum ++ )
    {
        tft180_show_string(0, 0, "reading...");
        system_delay_ms(200);

        float temp_vaule = flash_union_buffer[pageNum].float_type;

        if((float)temp_vaule<5000)
        {
            data_ptr=(float *)EEPROM_DATA[pageNum];
            if (data_ptr != NULL)
            {
                *data_ptr = (float)temp_vaule;
            }
        }
        else
        {
            data_ptr=(float *)EEPROM_DATA[pageNum];
            if (data_ptr != NULL)
            {
                *data_ptr = 0;
            }
        }
    }
    tft180_clear();
}
