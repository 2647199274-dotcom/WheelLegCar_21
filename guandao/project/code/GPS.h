#ifndef _GPS_h_
#define _GPS_h_

#include "zf_common_headfile.h"

// Flash页面分配：0页保存总点数，1-20页保存GPS点位
#define MAX_GPS_POINTS 500              // 最大GPS点数
#define GPS_FLASH_COUNT_PAGE 0          // 总点数存储的Flash页编号
#define GPS_FLASH_START_PAGE 1          // GPS点位存储的Flash起始页编号
#define GPS_FLASH_PAGE_COUNT 20         // 预留20页Flash空间

// GPS经纬度点
typedef struct {
    uint16 index;                      // 点位索引0开始
    double latitude;                   // 纬度
    double longitude;                  // 经度
} gps_point_struct;

// 每页存储的点位结构体
typedef struct {
    uint16 point_count;                // 本页点数
    gps_point_struct points[113];      // 每页存储113个点（113*18+2=2036字节，留有余量）
} gps_page_struct;

// GPS控制结构体
typedef struct {
    uint16 count;                      // 总点数
} gps_control_struct;
extern gps_control_struct gps_control;


void gps_init(void);
bool gps_get_info(gnss_info_struct *info);
extern gnss_info_struct info;

//点位操作
bool gps_add_point(void);                                 // 添加点位
bool gps_save(void);                                      // 保存点位到Flash
bool gps_load(void);                                      // 从Flash加载点位
bool gps_clear(void);                                     // 清除所有点位
uint16 gps_get_count(void);                               // 获取点位数量
bool gps_get_point(uint16 index, gps_point_struct *point); // 获取指定索引的点位
bool gps_delete_point(uint16 index);                        // 删除指定索引的点位
bool gps_insert_point(uint16 index, gps_point_struct *point); // 在指定索引插入点位
bool gps_modify_point(uint16 index, gps_point_struct *point); // 修改指定索引的点位

// 导航相关变量
extern double azimuth;         // 方向角
extern double distance;        // 距离
extern double degree_error;    // 方向角和航向角的差值
extern uint16 point_now;        // 当前目标点位

// 导航相关函数
bool gps_check(double lat, double lon, float radius);       // 检查是否到点
bool gps_set_current_point(uint16 index);                   // 设置目标点位
void gps_change_point(void);                                // 自动切换下一点位
void gps_trace(void);                                       // 跟踪导航
void gps_display_path(void);                                // 显示GPS路径

// 发车纠正相关变量（!!起点位置固定，朝向目标点位发车!!）
extern gps_point_struct gps_start_point;                    // 起点坐标
extern gps_point_struct gps_current_error;                   // 当前起点与保存起点的误差
extern double gps_yaw_correction;                             // 航向角修正值
extern bool gps_error_flag;                                   // 误差解算完毕后再巡点

//发车纠正相关函数
bool gps_start_geterror(void);
void gps_yaw_geterror(void);



#endif