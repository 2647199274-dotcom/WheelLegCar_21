#include "INS.h"
#include "zf_device_tft180.h"

// 定义MIN宏
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

ins_control_struct ins_control = {
    .count = 0,
    .last_distance = 0,
    .start_x = 0,
    .start_y = 0,
};

ins_state_t ins_state = INS_STATE_IDLE;  //默认空闲状态

// 内存数组存储所有惯导点位
static ins_point_struct ins_points[MAX_INS_POINTS];

// 复现相关变量
static uint16 replay_index = 0;


/**
 * @brief 惯导初始化后启动
 */
void ins_start_recording(void)
{
    
    ins_control.count = 0;
    ins_control.last_distance = car_distance;
    ins_control.start_x = 0;
    ins_control.start_y = 0;
    replay_index = 0;
    
    ins_state = INS_STATE_RECORDING;
}

/**
 * @brief 停止记录惯导数据
 */
void ins_stop_recording(void)
{
    if (ins_state == INS_STATE_RECORDING)
    {
        ins_save();
        ins_state = INS_STATE_IDLE;
    }
}

/**
 * @brief 开始复现惯导路径
 */
void ins_start_replay(void)
{
    if (ins_control.count > 0)
    {
        replay_index = 0;
        ins_state = INS_STATE_REPLAYING;  
    }
}

/**
 * @brief 停止复现惯导路径
 */
void ins_stop_replay(void)
{
    ins_state = INS_STATE_IDLE;
    replay_index = 0;
}

/**
 * @brief 显示惯导路径
 */
void ins_display_path(void)
{
    if (ins_control.count < 2)
    {
        return; // 至少需要2个点才能显示路径
    }
    
    ins_state = INS_STATE_DISPLAYING;
    
    // 计算坐标范围
    float min_x = 0, max_x = 0;
    float min_y = 0, max_y = 0;
    
    float current_x = ins_control.start_x;
    float current_y = ins_control.start_y;
    
    // 计算所有点的坐标范围
    for (uint16 i = 0; i < ins_control.count; i++)
    {
        if (i == 0)
        {
            min_x = max_x = current_x;
            min_y = max_y = current_y;
        }
        else
        {
            if (current_x < min_x) min_x = current_x;
            if (current_x > max_x) max_x = current_x;
            if (current_y < min_y) min_y = current_y;
            if (current_y > max_y) max_y = current_y;
        }
        
        // 计算下一个点的坐标
        float yaw = ins_points[i].yaw;
        float delta_x = INS_INTERVAL_CM * sinf(yaw * PI / 180.0f);
        float delta_y = INS_INTERVAL_CM * cosf(yaw * PI / 180.0f);
        current_x += delta_x;
        current_y += delta_y;
    }
    
    // 计算坐标范围
    float x_range = max_x - min_x;
    float y_range = max_y - min_y;
    
    // 确保范围不为零
    if (x_range == 0) x_range = 1.0f;
    if (y_range == 0) y_range = 1.0f;
    
    // 屏幕尺寸
    const int SCREEN_WIDTH = 160;
    const int SCREEN_HEIGHT = 128;
    const int MARGIN = 10;
    
    // 计算缩放比例
    float scale_x = (SCREEN_WIDTH - 2 * MARGIN) / x_range;
    float scale_y = (SCREEN_HEIGHT - 2 * MARGIN) / y_range;
    float scale = MIN(scale_x, scale_y);
    
    // 清空屏幕
    tft180_clear();
    
    // 绘制路径
    current_x = ins_control.start_x;
    current_y = ins_control.start_y;
    int last_x = MARGIN + (current_x - min_x) * scale;
    int last_y = SCREEN_HEIGHT - MARGIN - (current_y - min_y) * scale;
    
    for (uint16 i = 0; i < ins_control.count; i++)
    {
        // 计算下一个点的坐标
        float yaw = ins_points[i].yaw;
        float delta_x = INS_INTERVAL_CM * sinf(yaw * PI / 180.0f);
        float delta_y = INS_INTERVAL_CM * cosf(yaw * PI / 180.0f);
        current_x += delta_x;
        current_y += delta_y;
        
        // 将坐标转换为屏幕坐标
        int x = MARGIN + (current_x - min_x) * scale;
        int y = SCREEN_HEIGHT - MARGIN - (current_y - min_y) * scale;
        
        // 绘制线段
        tft180_draw_line(last_x, last_y, x, y, RGB565_RED);
        
        // 更新上一个点的坐标
        last_x = x;
        last_y = y;
    }
    
    // 绘制起点和终点
    current_x = ins_control.start_x;
    current_y = ins_control.start_y;
    int start_x = MARGIN + (current_x - min_x) * scale;
    int start_y = SCREEN_HEIGHT - MARGIN - (current_y - min_y) * scale;
    
    // 绘制起点（绿色）
    tft180_draw_point(start_x-1, start_y-1, RGB565_GREEN);
    tft180_draw_point(start_x, start_y-1, RGB565_GREEN);
    tft180_draw_point(start_x+1, start_y-1, RGB565_GREEN);
    tft180_draw_point(start_x-1, start_y, RGB565_GREEN);
    tft180_draw_point(start_x, start_y, RGB565_GREEN);
    tft180_draw_point(start_x+1, start_y, RGB565_GREEN);
    tft180_draw_point(start_x-1, start_y+1, RGB565_GREEN);
    tft180_draw_point(start_x, start_y+1, RGB565_GREEN);
    tft180_draw_point(start_x+1, start_y+1, RGB565_GREEN);
    
    // 显示点位数量
    char buffer[32];
    sprintf(buffer, "INS Points: %d", ins_control.count);
    tft180_show_string(5, 5, buffer);
    
    ins_state = INS_STATE_IDLE; // 显示完成后回到空闲状态
}

/**
 * @brief 添加惯导点位
 * @return 是否添加成功
 */
bool ins_add_point(void)
{
    if (ins_control.count >= MAX_INS_POINTS)
    {
        return false;
    }
    
 
    ins_point_struct *point = &ins_points[ins_control.count];
    
    // 只保存偏航角
    point->yaw = angle.yaw; 
    
    ins_control.count++;
    ins_control.last_distance = car_distance;
    
    return true;
}

/**
 * @brief 保存惯导点位到flash
 * @return 是否保存成功
 */
bool ins_save(void)
{
    // 保存总点数到第21页
    flash_buffer_clear();
    flash_union_buffer[0].uint16_type = ins_control.count;
    
    if (flash_check(0, INS_FLASH_COUNT_PAGE))
    {
        flash_erase_page(0, INS_FLASH_COUNT_PAGE);
    }
    flash_write_page_from_buffer(0, INS_FLASH_COUNT_PAGE, 1); // 只保存一个uint16
    
    // 批量写入所有点位数据
    uint16 points_written = 0;
    uint8 current_page = 0;
    
    while (points_written < ins_control.count)
    {
        // 计算当前页的flash地址
        uint32 page_address = INS_FLASH_START_PAGE + current_page;
        if (page_address >= INS_FLASH_START_PAGE + INS_FLASH_PAGE_COUNT)
        {
            return false; // 超出预留的flash空间
        }
        
        // 计算本页可以写入的点数
        uint16 points_in_page = MIN(42, ins_control.count - points_written);
        
        // 准备页数据
        ins_page_struct page_data;
        page_data.point_count = points_in_page;
        
        // 复制点位数据到页数据中
        for (uint16 i = 0; i < points_in_page; i++)
        {
            page_data.points[i] = ins_points[points_written + i];
        }
        
        // 写入flash
        const uint16 PAGE_DATA_NUM = sizeof(page_data) / sizeof(uint32);
        flash_buffer_clear();
        uint32 *data_ptr = (uint32 *)&page_data;
        for (uint16 i = 0; i < PAGE_DATA_NUM; ++i)
        {
            flash_union_buffer[i].uint32_type = data_ptr[i];
        }
        
        if (flash_check(0, page_address))
        {
            flash_erase_page(0, page_address);
        }
        flash_write_page_from_buffer(0, page_address, PAGE_DATA_NUM);
        
        // 更新计数
        points_written += points_in_page;
        current_page++;
    }
    
    flash_buffer_clear();
    return true;
}

/**
 * @brief 从flash加载惯导点位
 * @return 是否加载成功
 */
bool ins_load(void)
{
    // 从第21页加载总点数
    flash_read_page_to_buffer(0, INS_FLASH_COUNT_PAGE, 1);
    ins_control.count = flash_union_buffer[0].uint16_type;
    
    // 验证数据有效性
    if (ins_control.count > MAX_INS_POINTS)
    {
        ins_control.count = 0;
        return false;
    }
    
    // 初始化其他控制信息
    ins_control.last_distance = 0;
    ins_control.start_x = 0;
    ins_control.start_y = 0;
    
    // 初始化复现相关变量
    replay_index = 0;
    
    // 从flash加载所有点位数据
    uint16 points_loaded = 0;
    uint8 current_page = 0;
    
    while (points_loaded < ins_control.count)
    {
        // 计算当前页的flash地址
        uint32 page_address = INS_FLASH_START_PAGE + current_page;
        if (page_address >= INS_FLASH_START_PAGE + INS_FLASH_PAGE_COUNT)
        {
            ins_control.count = 0;
            return false; // 超出预留的flash空间
        }
        
        // 读取页数据
        ins_page_struct page_data;
        const uint16 PAGE_DATA_NUM = sizeof(ins_page_struct) / sizeof(uint32);
        flash_read_page_to_buffer(0, page_address, PAGE_DATA_NUM);
        
        uint32 *data_ptr = (uint32 *)&page_data;
        for (uint16 i = 0; i < PAGE_DATA_NUM; ++i)
        {
            data_ptr[i] = flash_union_buffer[i].uint32_type;
        }
        
        // 复制点位数据到内存数组
        uint16 points_in_page = MIN(42, ins_control.count - points_loaded);
        for (uint16 i = 0; i < points_in_page; i++)
        {
            ins_points[points_loaded + i] = page_data.points[i];
        }
        
        // 更新计数
        points_loaded += points_in_page;
        current_page++;
    }
    
    return true;
}

/**
 * @brief 清空惯导点位
 * @return 是否清空成功
 */
bool ins_clear(void)
{
    ins_control.count = 0;
    ins_control.last_distance = 0;
    ins_control.start_x = 0;
    ins_control.start_y = 0;
    replay_index = 0;
    
    // 清空第21页的总点数
    flash_buffer_clear();
    flash_union_buffer[0].uint16_type = 0;
    if (flash_check(0, INS_FLASH_COUNT_PAGE))
    {
        flash_erase_page(0, INS_FLASH_COUNT_PAGE);
    }
    
    for (uint8 i = 0; i < INS_FLASH_PAGE_COUNT; i++) {
        uint32 page_address = INS_FLASH_START_PAGE + i;
        if (flash_check(0, page_address)) {
            flash_erase_page(0, page_address);
    }
}
    
    return true;
}

/**
 * @brief 获取惯导点位数量
 * @return 点位数量
 */
uint16 ins_get_count(void)
{
    return ins_control.count;
}



/**
 * @brief 更新惯导系统
 */
void ins_update(void)
{
    switch (ins_state)
    {
        case INS_STATE_RECORDING:
        {
            // 每2cm打点
            if (fabsf(car_distance - ins_control.last_distance) >= INS_INTERVAL_CM)
            {
                ins_add_point();
            }
            break;
        }
        case INS_STATE_REPLAYING:
        {
            // 运动复现逻辑
            if (replay_index < ins_control.count)
            {
                //float yaw = ins_points[replay_index].yaw;
                
                // 控制小车按照记录的偏航角运动
                // 这里需要根据实际的小车控制逻辑来实现
                // 使用恒定速度，并控制方向
                // 示例：car_set_speed(speed); car_set_direction(yaw);
                
                // 直接切换到下一个点
                replay_index++;
            }
            else
            {
                ins_stop_replay();
            }
            break;
        }
        case INS_STATE_IDLE:
        default:
            break;
    }
}