/*
 * my_key.h
 *
 *  Created on: 2025年12月25日
 *      Author: ZhuanZ（无密码）
 */

#ifndef CODE_MY_KEY_H_
#define CODE_MY_KEY_H_

#include "zf_common_headfile.h"

//===========================按键================================
//定义按键功能
typedef enum
{
    KEY_SELECT_UP,                                                                // 向上
    KEY_SELECT_DOWM,                                                              // 向下
    KEY_SELECT_QUIT,                                                              // 退出
    KEY_SELECT_ENTER,                                                             // 确认
    KEY_SELECT_MEMORY,                                                            // 写入内存
    KEY_SELECT_ADD,                                                             // 增加数字
    KEY_SELECT_SUB,                                                            // 减少
    KEY_SELECT_NULL
}key_select_enum;

void my_key_init(uint32 period);
void key_up(void);
void key_enter(void);
void key_down(void);
void key_quit(void);
void key_add(void);
void key_sub(void);
bool key_select(void);

//===========================按键================================

//==========================拨码开关===============================
#include "zf_driver_gpio.h"

#define SWITCH1         (P21_5)
#define SWITCH2         (P21_6)

//#define  GET_SWITCH1()  !gpio_get_level(SWITCH1)
//#define  GET_SWITCH2()  !gpio_get_level(SWITCH2)

//#define  GET_SWITCH1()  !gpio_get_level(SWITCH1)
//#define  GET_SWITCH2()  !gpio_get_level(SWITCH2)
//开关端口的枚举
typedef enum
{
    SWITCH_1,
    SWITCH_2,
    SWITCH_MAX,
} SWITCH_e;

typedef enum
{
    SWITCH_ON = 0,         //拨码开启时对应电平
    SWITCH_OFF = 1,        //拨码关闭时对应电平
} SWITCH_STATUS_e;

typedef enum
{
  VHIGH = 0,
  VMEDIUM,
  VLOW,
}VOICE_TYPE_e;

/*
 *  供外部调用的函数接口声明
 */
void              switch_init();
SWITCH_STATUS_e   Switch_Get(SWITCH_e key);

//==========================拨码开关===============================

#endif /* CODE_MY_KEY_H_ */
