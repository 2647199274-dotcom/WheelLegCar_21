/*
 * my_key.c
 *
 *  Created on: 2025??12??25??
 *      Author: ZhuanZ????????
 */
#include "my_key.h"

//=====================================????????=================================
// ???ÞÏ??????
#define KEY_UP KEY_1
#define KEY_DOWN KEY_2
#define KEY_ENTER KEY_3
#define KEY_QUIT KEY_4

key_select_enum my_key_select = KEY_SELECT_NULL;

extern Menu_Item* key;                                                          //???
extern Menu_Item head;

extern float SetupNumber[];//????????§³
extern uint8_t SetupIndex;

int count = 0;


void my_key_init(uint32 period)
{
//    gpio_init(P15_4,GPI,0,GPI_PULL_DOWN);
//    gpio_init(P15_6,GPI,0,GPI_PULL_DOWN);
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
            (*(double *)(key->data)) += (double)SetupNumber[SetupIndex];
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
            (*(double *)(key->data)) -= (double)SetupNumber[SetupIndex];
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
