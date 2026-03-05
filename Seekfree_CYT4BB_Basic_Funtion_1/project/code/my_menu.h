/*
 * my_menu.h
 *
 *  Created on: 2025年11月28日
 *      Author: ZhuanZ（无密码）
 */

#ifndef CODE_MY_MENU_H_
#define CODE_MY_MENU_H_

#include "zf_common_headfile.h"



void menu_init(void);
void menu_show(void);
void show_key(void);


void Read_EEPROM(void);
void Write_EEPROM(void);
void eeprom_erase_sector(void);

#endif /* CODE_MY_MENU_H_ */
