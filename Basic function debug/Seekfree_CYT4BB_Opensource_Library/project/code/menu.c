/*
 * menu.c
 *
 *  Created on: 2025年11月28日
 *      Author: ZhuanZ（无密码）
 */


#include "menu.h"

#define MENU_MAX_SIZE   (128)
static Menu_Item menu_item_arr[MENU_MAX_SIZE];
static uint16_t menu_arr_index = 0;
//初始化
static void Creat_Menu_Item(Menu_Item *father,Menu_Item *me,const char name[],void *data,MENU_KIND kind)
{
    if(father->kind != MENU_Folder) return;

    me->name = name;
    me->father = father;

    me->first_son = NULL;
    me->last_brother = NULL;
    me->next_brother = NULL;
    me->sons = 0;
    me->select = false;

    me->data = data;
    me->kind = kind;

    if(father->first_son == NULL)
    {
        father->first_son = me;
    }
    else
    {
        Menu_Item *p = father->first_son;
        while(p->next_brother != NULL)
        {
            p = p->next_brother;
        }
        p->next_brother = me;
        me->last_brother = p;
    }
    me->number = father->sons;
    father->sons++;
}
//创建文件夹
void Creat_Menu_Folder(Menu_Item *father,Menu_Item *me,const char name[])
{
    Creat_Menu_Item(father,me,name,NULL,MENU_Folder);
}
//创建文件
void Creat_Menu_File(Menu_Item *father,Menu_Item *me,const char name[],void *data, MENU_KIND kind)
{
    Creat_Menu_Item(father,me,name,data,kind);
}
Menu_Item* Creat_Menu_Dynamic_Folder(Menu_Item *father,const char name[])
{
    Menu_Item *me = &menu_item_arr[menu_arr_index++];
    Creat_Menu_Item(father,me,name,NULL,MENU_Folder);
    return me;
}
void Creat_Menu_Dynamic_Data(Menu_Item *father,const char name[],void *data, MENU_KIND kind)
{
    Menu_Item *me = &menu_item_arr[menu_arr_index++];
    Creat_Menu_Item(father,me,name,data,kind);
}
void All_Folder_Menu_Init(Menu_Item *Menu)
{
    if(Menu->first_son == NULL)
    {
        return ;
    }
    Menu_Item *hp = Menu->first_son;
    Menu_Item *p = Menu->first_son;

    if(hp->next_brother == NULL) {
        All_Folder_Menu_Init(p);
    }
    while(p->next_brother != NULL) {
        if(p->kind == MENU_Folder) {
            All_Folder_Menu_Init(p);
        }
        p = p->next_brother;
    }
    if(hp->next_brother != NULL) {
        All_Folder_Menu_Init(p);
    }

    p->next_brother = hp;
    hp->last_brother = p;
}
