#ifndef CODE_MENU_H_
#define CODE_MENU_H_

#include "zf_common_headfile.h"

typedef enum MENU_KIND{
        MENU_Folder = 0,
        int8_Box,                                               // 有符号  8 bits
        int16_Box,                                              // 有符号 16 bits
        int32_Box,
        int_Box,                                              // 有符号 32 bits
        int64_Box,                                              // 有符号 64 bits
        uint8_Box,                                               // 易变性修饰 无符号  8 bits
        uint16_Box,                                               // 易变性修饰 无符号 16 bits
        uint32_Box,                                               // 易变性修饰 无符号 32 bits
        uint64_Box,
        float_Box,
        double_Box,
        bool_Box,
        funtion_Box,
        max_number_of_box
}MENU_KIND;

typedef struct Menu_Item{

        const char*name;
        void* data;             //储存变量
        MENU_KIND kind;         //记录节点属性

        bool select;            //是否被选中
        uint8 sons;             //记录子节点数量
        uint8 number;               //记录是父节点第几个子节点
        struct Menu_Item *father;
        struct Menu_Item *first_son;
        struct Menu_Item *next_brother;
        struct Menu_Item *last_brother;
}Menu_Item;

//void Creat_Menu_Item(Menu_Item *father,Menu_Item *me,const char name[],MENU_KIND)

void Creat_Menu_Folder(Menu_Item *father,Menu_Item *me,const char name[]);
Menu_Item* Creat_Menu_Dynamic_Folder(Menu_Item *father,const char name[]);
void Creat_Menu_Dynamic_Data(Menu_Item *father,const char name[],void *data, MENU_KIND kind);

#endif /* CODE_MENU_H_ */
