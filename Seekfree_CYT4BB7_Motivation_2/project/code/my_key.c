#include "my_key.h"

// 按键定义
#define KEY_UP KEY_4
#define KEY_DOWN KEY_3
#define KEY_ENTER KEY_2
#define KEY_QUIT KEY_1

key_select_enum my_key_select = KEY_SELECT_NULL;

extern Menu_Item* key;                                                          //???
extern Menu_Item head;

extern float SetupNumber[];//加减步长
extern uint8_t SetupIndex;

int count = 0;


void my_key_init(uint32 period)
{
//    gpio_init(P15_4,GPI,0,GPI_PULL_DOWN);
//    gpio_init(P15_6,GPI,0,GPI_PULL_DOWN);
//    switch_init();	//拨码开关初始化

    key_init(period);
    pit_ms_init(PIT_CH1,period);
}

void key_up(void)
{
    key = key->last_brother;
}
void key_down(void)
{
    key = key->next_brother;
}
void key_enter(void)
{
    if(key->first_son == NULL) return ;
    if(key->sons > 0)
    {
        tft180_clear();
        key = key->first_son;
    }
}

void key_quit(void)
{
    if(key->father->father != NULL)
    {
        tft180_clear();
        key = key->father;
    }
}

bool key_select(void)
{
    if(key->kind != MENU_Folder && Switch_Get(SWITCH_1) && Switch_Get(SWITCH_2))
    {
        key->select = true;
    }
    else
    {
        key->select = false;
    }
    return key-> select;
}

void key_add(void)
{
    switch(key->kind)
    {
        case int8_Box:
            (*(int8_t *)(key->data)) += (int8_t)SetupNumber[SetupIndex];
            break;
        case int16_Box:
            (*(int16_t *)(key->data)) += (int16_t)SetupNumber[SetupIndex];
            break;
        case int32_Box:
            (*(int32_t *)(key->data)) += (int32_t)SetupNumber[SetupIndex];
            break;
        case int64_Box:
            (*(int64_t *)(key->data)) += (int64_t)SetupNumber[SetupIndex];
            break;
        case uint8_Box:
            (*(uint8_t *)(key->data)) += 1;
            break;
        case uint16_Box:
            (*(uint16_t *)(key->data)) += (uint16_t)SetupNumber[SetupIndex];
            break;
        case uint32_Box:
            (*(uint32_t *)(key->data)) += (uint32_t)SetupNumber[SetupIndex];
            break;
        case uint64_Box:
            (*(uint64_t *)(key->data)) += (uint64_t)SetupNumber[SetupIndex];
            break;
        case float_Box:
            (*(float *)(key->data)) += (float)SetupNumber[SetupIndex];
            break;
        case double_Box:
            (*(double *)(key->data)) += (double)SetupNumber[SetupIndex];
            break;
        case bool_Box:
            if(*(bool *)(key->data) == false)
                (*(bool *)(key->data)) = true;
            else
                (*(bool *)(key->data)) = false;
            break;
        default:
            break;
    }
//    tft180_clear();
}

void key_sub(void)
{
    switch(key->kind)
    {
        case int8_Box:
            (*(int8_t *)(key->data)) -= (int8_t)SetupNumber[SetupIndex];
            break;
        case int16_Box:
            (*(int16_t *)(key->data)) -= (int16_t)SetupNumber[SetupIndex];
            break;
        case int32_Box:
            (*(int32_t *)(key->data)) -= (int32_t)SetupNumber[SetupIndex];
            break;
        case int64_Box:
            (*(int64_t *)(key->data)) -= (int64_t)SetupNumber[SetupIndex];
            break;
        case uint8_Box:
            (*(uint8_t *)(key->data)) -= 1;
            break;
        case uint16_Box:
            (*(uint16_t *)(key->data)) -= (uint16_t)SetupNumber[SetupIndex];
            break;
        case uint32_Box:
            (*(uint32_t *)(key->data))-= (uint32_t)SetupNumber[SetupIndex];
            break;
        case uint64_Box:
            (*(uint64_t *)(key->data))-= (uint64_t)SetupNumber[SetupIndex];
            break;
        case float_Box:
            (*(float *)(key->data)) -= (float)SetupNumber[SetupIndex];
            break;
        case double_Box:
            (*(double *)(key->data)) -= (double)SetupNumber[SetupIndex];
            break;
        case bool_Box:
            if(*(bool *)(key->data) == false)
                (*(bool *)(key->data)) = true;
            else
                (*(bool *)(key->data)) = false;
            break;
        default:
            break;
    }
}

void pit0_ch1_isr()                     // 10ms中断控制按键  
{
    pit_isr_flag_clear(PIT_CH1);
    key_scanner();

    if(my_key_select == KEY_SELECT_NULL)
    {
      //短按功能
        if(key_get_state(KEY_UP) == KEY_SHORT_PRESS)
        {
            if(key_select())
                my_key_select = KEY_SELECT_ADD;
            else
                my_key_select = KEY_SELECT_UP;
        }
        if(key_get_state(KEY_DOWN) == KEY_SHORT_PRESS)
        {
            if(key_select())
                my_key_select = KEY_SELECT_SUB;
            else
                my_key_select = KEY_SELECT_DOWM;
        }
        if(key_get_state(KEY_ENTER) == KEY_SHORT_PRESS)
        {
            my_key_select = KEY_SELECT_ENTER;
        }

        if(key_get_state(KEY_QUIT) == KEY_SHORT_PRESS)
        {
            my_key_select = KEY_SELECT_QUIT;
        }
        //长按功能
        if(key_get_state(KEY_ENTER) == KEY_LONG_PRESS)
        {
            my_key_select = KEY_SELECT_MEMORY;
        }
        if(key_get_state(KEY_UP) == KEY_LONG_PRESS)
        {
            my_key_select = KEY_SELECT_ADD;
        }
        if(key_get_state(KEY_DOWN) == KEY_LONG_PRESS)
        {
            my_key_select = KEY_SELECT_SUB;
        }
    }
    else
    {
        my_key_select = KEY_SELECT_NULL;
    }

    switch(my_key_select)
    {
        case KEY_SELECT_UP:
            key_up();
            break;
        case KEY_SELECT_DOWM:
            key_down();
            break;
        case KEY_SELECT_ENTER:
            key_enter();
            break;
        case KEY_SELECT_QUIT:
            key_quit();
            break;
        case KEY_SELECT_MEMORY:
            flash_write_data();
            break;
        case KEY_SELECT_ADD:
            key_add();
            break;
        case KEY_SELECT_SUB:
            key_sub();
            break;

    }

//    menu_show();

}
//=====================================????????=================================

//=====================================??????=================================

/* ????SWITCH????????? */
gpio_pin_enum SWITCH_PTxn[SWITCH_MAX] = {SWITCH1, SWITCH2};

/******************************************************************************
 *  @brief      ?????switch???
 *****************************************************************************/
void switch_init()
{
    uint8 i = SWITCH_MAX;

    //???????? ????
    while(i--)
    {
        gpio_init(SWITCH_PTxn[i], GPI, 0, GPO_PUSH_PULL);
    }
}



/******************************************************************************
 *  @brief      ???switch??
 *  @param      SWITCH_e         SWITCH???
 *  @return     SWITCH_STATUS_e    SWITCH??(SWITCH_ON,SWITCH_OFF)
 ******************************************************************************/
SWITCH_STATUS_e Switch_Get(SWITCH_e i)
{
    if(gpio_get_level(SWITCH_PTxn[i]) == SWITCH_OFF)
    {
        return SWITCH_OFF;
    }
    return SWITCH_ON;
}

//=====================================??????=================================
