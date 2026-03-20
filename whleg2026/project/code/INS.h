#ifndef _INS_h_
#define _INS_h_

#include "zf_common_headfile.h"

//flash共96页 每页2kb
//页面分配：0-20GPS  21-41偏航角惯导  42-82坐标惯导

// 惯导打点配置
#define MAX_INS_POINTS 3000            // 最大惯导点数（每2cm一个点，3000个点可覆盖60米）
#define INS_FLASH_COUNT_PAGE 21        // 总点数存储的Flash页编号
#define INS_FLASH_START_PAGE 22        // 惯导点位存储的Flash起始页编号
#define INS_FLASH_PAGE_COUNT 20        // 预留20页Flash空间（约40KB）
#define INS_INTERVAL_CM 2.0f           // 打点间隔（厘米）

// 惯导点(方案1仅偏航角，理论最多10200点)
typedef struct {
    float yaw;                         // 偏航角
} ins_point_struct;

// 每页存储的点位结构体
typedef struct {
    uint16 point_count;                // 本页点数
    ins_point_struct points[510];      // 每页存储510个点（510*4=2040字节，留有余量）
} ins_page_struct;


typedef struct {
    uint16 count;                      // 点数
    float last_distance;               // 上次打点时的距离
    float start_x;                     // 起点X坐标
    float start_y;                     // 起点Y坐标
} ins_control_struct;


// 惯导系统状态
typedef enum {
    INS_STATE_IDLE,                    // 空闲状态
    INS_STATE_RECORDING,               // 记录状态
    INS_STATE_REPLAYING,               // 复现状态
    INS_STATE_DISPLAYING               // 显示状态
} ins_state_t;

// 全局变量
extern ins_control_struct ins_control;
extern ins_state_t ins_state;

// 惯导系统控制函数
void ins_start_recording(void);
void ins_stop_recording(void);
void ins_start_replay(void);
void ins_stop_replay(void);
void ins_display_path(void);
void ins_update(void);

// 惯导点位操作相关函数
bool ins_add_point(void);
bool ins_save(void);
bool ins_load(void);
bool ins_clear(void);
uint16 ins_get_count(void);




#endif