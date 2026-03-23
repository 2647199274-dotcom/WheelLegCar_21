#ifndef CODE_MENU_H_
#define CODE_MENU_H_

#include "zf_common_headfile.h"

typedef enum MENU_KIND{
        MENU_Folder = 0,
        int8_Box,                                               // �з���  8 bits
        int16_Box,                                              // �з��� 16 bits
        int32_Box,
        int_Box,                                              // �з��� 32 bits
        int64_Box,                                              // �з��� 64 bits
        uint8_Box,                                               // �ױ������� �޷���  8 bits
        uint16_Box,                                               // �ױ������� �޷��� 16 bits
        uint32_Box,                                               // �ױ������� �޷��� 32 bits
        uint64_Box,
        float_Box,
        double_Box,
        bool_Box,
        funtion_Box,
        max_number_of_box
}MENU_KIND;

typedef struct Menu_Item{

        const char*name;
        void* data;             //�������
        MENU_KIND kind;         //��¼�ڵ�����

        bool select;            //�Ƿ�ѡ��
        uint8 sons;             //��¼�ӽڵ�����
        uint8 number;               //��¼�Ǹ��ڵ�ڼ����ӽڵ�
        struct Menu_Item *father;
        struct Menu_Item *first_son;
        struct Menu_Item *next_brother;
        struct Menu_Item *last_brother;
}Menu_Item;

//void Creat_Menu_Item(Menu_Item *father,Menu_Item *me,const char name[],MENU_KIND)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     创建菜单文件夹
// 参数说明     father - 父菜单项
//              me - 当前菜单项
//              name - 菜单名称
// 返回参数     void
// 使用示例     Creat_Menu_Folder(&head, &folder, "Settings");
// 备注信息     创建一个菜单文件夹
//-------------------------------------------------------------------------------------------------------------------
void Creat_Menu_Folder(Menu_Item *father,Menu_Item *me,const char name[]);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动态创建菜单文件夹
// 参数说明     father - 父菜单项
//              name - 菜单名称
// 返回参数     Menu_Item* - 创建的菜单文件夹指针
// 使用示例     Menu_Item* folder = Creat_Menu_Dynamic_Folder(&head, "Settings");
// 备注信息     动态创建一个菜单文件夹
//-------------------------------------------------------------------------------------------------------------------
Menu_Item* Creat_Menu_Dynamic_Folder(Menu_Item *father,const char name[]);

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
void Creat_Menu_Dynamic_Data(Menu_Item *father,const char name[],void *data, MENU_KIND kind);

#endif /* CODE_MENU_H_ */
