#include "menu.h"

#define MENU_MAX_SIZE   (128)
static Menu_Item menu_item_arr[MENU_MAX_SIZE];
static uint16_t menu_arr_index = 0;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     创建菜单项
// 参数说明     father - 父菜单项
//              me - 当前菜单项
//              name - 菜单名称
//              data - 数据指针
//              kind - 数据类型
// 返回参数     void
// 使用示例     Creat_Menu_Item(&head, &item, "Settings", NULL, MENU_Folder);
// 备注信息     创建一个菜单项
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     创建菜单文件夹
// 参数说明     father - 父菜单项
//              me - 当前菜单项
//              name - 菜单名称
// 返回参数     void
// 使用示例     Creat_Menu_Folder(&head, &folder, "Settings");
// 备注信息     创建一个菜单文件夹
//-------------------------------------------------------------------------------------------------------------------
void Creat_Menu_Folder(Menu_Item *father,Menu_Item *me,const char name[])
{
    Creat_Menu_Item(father,me,name,NULL,MENU_Folder);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     创建菜单文件
// 参数说明     father - 父菜单项
//              me - 当前菜单项
//              name - 菜单名称
//              data - 数据指针
//              kind - 数据类型
// 返回参数     void
// 使用示例     Creat_Menu_File(&folder, &item, "KP:", &kp, float_Box);
// 备注信息     创建一个数据菜单项
//-------------------------------------------------------------------------------------------------------------------
void Creat_Menu_File(Menu_Item *father,Menu_Item *me,const char name[],void *data, MENU_KIND kind)
{
    Creat_Menu_Item(father,me,name,data,kind);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动态创建菜单文件夹
// 参数说明     father - 父菜单项
//              name - 菜单名称
// 返回参数     Menu_Item* - 创建的菜单文件夹指针
// 使用示例     Menu_Item* folder = Creat_Menu_Dynamic_Folder(&head, "Settings");
// 备注信息     动态创建一个菜单文件夹
//-------------------------------------------------------------------------------------------------------------------
Menu_Item* Creat_Menu_Dynamic_Folder(Menu_Item *father,const char name[])
{
    Menu_Item *me = &menu_item_arr[menu_arr_index++];
    Creat_Menu_Item(father,me,name,NULL,MENU_Folder);
    return me;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动态创建数据菜单项
// 参数说明     father - 父菜单项
//              name - 菜单名称
//              data - 数据指针
//              kind - 数据类型
// 返回参数     void
// 使用示例     Creat_Menu_Dynamic_Data(folder, "KP:", &kp, float_Box);
// 备注信息     动态创建一个数据菜单项
//-------------------------------------------------------------------------------------------------------------------
void Creat_Menu_Dynamic_Data(Menu_Item *father,const char name[],void *data, MENU_KIND kind)
{
    Menu_Item *me = &menu_item_arr[menu_arr_index++];
    Creat_Menu_Item(father,me,name,data,kind);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化所有菜单文件夹
// 参数说明     Menu - 菜单指针
// 返回参数     void
// 使用示例     All_Folder_Menu_Init(&head);
// 备注信息     初始化所有菜单文件夹，设置兄弟节点的循环链接
//-------------------------------------------------------------------------------------------------------------------
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
