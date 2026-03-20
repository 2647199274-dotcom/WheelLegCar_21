#include "GPS.h"
#include "zf_device_tft180.h"

// 定义MIN宏
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

gnss_info_struct info;
gps_control_struct gps_control = {.count = 0};

// 内存数组存储所有GPS点位
static gps_point_struct gps_points[MAX_GPS_POINTS];

double azimuth = 0;       
double distance = 0;       
double degree_error = 0;  
uint16 point_now = 0; 

gps_point_struct gps_start_point = {0, 0.0, 0.0}; 
gps_point_struct gps_current_error = {0, 0.0, 0.0};
double gps_yaw_correction = 0.0; 
bool gps_error_flag = false;



/**
 * @brief 初始化并读取flash
 */
void gps_init(void)  
{
    flash_init();
    gnss_init(TAU1201);
    gps_load();
}

/**
 * @brief 获取GNSS信息
 * @param info GNSS信息结构体指针
 * @return 是否获取成功
 */
bool gps_get_info(gnss_info_struct *info)
{
    if (gnss_flag)       // 更新GNSS数据，确保能读到最新准确的数据
    {
        gnss_flag = 0;
        gnss_data_parse();
    }
   
    if (info == NULL || gnss.state != 1)  // 检查GNSS状态，确保数据有效
    {
      return false;
    }
    
    *info = gnss;   
    return true;
}

/**
 * @brief 添加GPS点位
 * @return 是否添加成功
 */
bool gps_add_point(void)
{
    if (gnss_flag)       
    {
        gnss_flag = 0;
        gnss_data_parse();
    }
   
    if (gnss.state != 1)  
    {
      return false;
    }
     
    // 检查是否达到最大点数
    if (gps_control.count >= MAX_GPS_POINTS)
    {
        return false;
    }
    
    // 添加点位到内存数组
    gps_point_struct *point = &gps_points[gps_control.count]; 
    point->index = gps_control.count;
    point->latitude = gnss.latitude;
    point->longitude = gnss.longitude;
    
    gps_control.count++;
    
    return true;
}

/**
 * @brief 保存GPS点位到flash
 * @return 是否保存成功
 */
bool gps_save(void)
{
    // 保存总点数到第0页
    flash_buffer_clear();
    flash_union_buffer[0].uint16_type = gps_control.count;
    
    if (flash_check(0, GPS_FLASH_COUNT_PAGE))
    {
        flash_erase_page(0, GPS_FLASH_COUNT_PAGE);
    }
    flash_write_page_from_buffer(0, GPS_FLASH_COUNT_PAGE, 1); // 只保存一个uint16
    
    // 批量写入所有点位数据
    uint16 points_written = 0;
    uint8 current_page = 0;
    
    while (points_written < gps_control.count)
    {
        // 计算当前页的flash地址
        uint32 page_address = GPS_FLASH_START_PAGE + current_page;
        if (page_address >= GPS_FLASH_START_PAGE + GPS_FLASH_PAGE_COUNT)
        {
            return false; // 超出预留的flash空间
        }
        
        // 计算本页可以写入的点数
        uint16 points_in_page = MIN(113, gps_control.count - points_written);
        
        // 准备页数据
        gps_page_struct page_data;
        page_data.point_count = points_in_page;
        
        // 复制点位数据到页数据中
        for (uint16 i = 0; i < points_in_page; i++)
        {
            page_data.points[i] = gps_points[points_written + i];
        }
        
        // 写入flash
        const uint16 PAGE_DATA_NUM = sizeof(gps_page_struct) / sizeof(uint32);
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
 * @brief 从flash加载GPS点位
 * @return 是否加载成功
 */
bool gps_load(void)
{
    // 从第0页加载总点数
    flash_read_page_to_buffer(0, GPS_FLASH_COUNT_PAGE, 1);
    gps_control.count = flash_union_buffer[0].uint16_type;
    
    // 验证数据有效性
    if (gps_control.count > MAX_GPS_POINTS)
    {
        gps_control.count = 0;
        return false;
    }
    
    // 从flash加载所有点位数据
    uint16 points_loaded = 0;
    uint8 current_page = 0;
    
    while (points_loaded < gps_control.count)
    {
        // 计算当前页的flash地址
        uint32 page_address = GPS_FLASH_START_PAGE + current_page;
        if (page_address >= GPS_FLASH_START_PAGE + GPS_FLASH_PAGE_COUNT)
        {
            gps_control.count = 0;
            return false; // 超出预留的flash空间
        }
        
        // 读取页数据
        gps_page_struct page_data;
        const uint16 PAGE_DATA_NUM = sizeof(gps_page_struct) / sizeof(uint32);
        flash_read_page_to_buffer(0, page_address, PAGE_DATA_NUM);
        
        uint32 *data_ptr = (uint32 *)&page_data;
        for (uint16 i = 0; i < PAGE_DATA_NUM; ++i)
        {
            data_ptr[i] = flash_union_buffer[i].uint32_type;
        }
        
        // 复制点位数据到内存数组
        uint16 points_in_page = MIN(113, gps_control.count - points_loaded);
        for (uint16 i = 0; i < points_in_page; i++)
        {
            gps_points[points_loaded + i] = page_data.points[i];
        }
        
        // 更新计数
        points_loaded += points_in_page;
        current_page++;
    }
    
    return true;
}

/**
 * @brief 清空GPS点位
 * @return 是否清空成功
 */
bool gps_clear(void)
{
    // 清空内存中的点位数据
    gps_control.count = 0;
    point_now = 0;
    
    // 清空第0页的总点数
    flash_buffer_clear();
    flash_union_buffer[0].uint16_type = 0;
    if (flash_check(0, GPS_FLASH_COUNT_PAGE))
    {
        flash_erase_page(0, GPS_FLASH_COUNT_PAGE);
    }
    flash_write_page_from_buffer(0, GPS_FLASH_COUNT_PAGE, 1);
    
    return true;
}

/**
 * @brief 显示GPS路径
 */
void gps_display_path(void)
{
    if (gps_control.count < 2)
    {
        return; // 至少需要2个点才能显示路径
    }
    
    // 计算经纬度范围
    double min_lat = gps_points[0].latitude;
    double max_lat = gps_points[0].latitude;
    double min_lon = gps_points[0].longitude;
    double max_lon = gps_points[0].longitude;
    
    for (uint16 i = 1; i < gps_control.count; i++)
    {
        if (gps_points[i].latitude < min_lat) min_lat = gps_points[i].latitude;
        if (gps_points[i].latitude > max_lat) max_lat = gps_points[i].latitude;
        if (gps_points[i].longitude < min_lon) min_lon = gps_points[i].longitude;
        if (gps_points[i].longitude > max_lon) max_lon = gps_points[i].longitude;
    }
    
    // 计算经纬度范围
    double lat_range = max_lat - min_lat;
    double lon_range = max_lon - min_lon;
    
    // 确保范围不为零
    if (lat_range == 0) lat_range = 0.0001;
    if (lon_range == 0) lon_range = 0.0001;
    
    // 屏幕尺寸
    const int SCREEN_WIDTH = 160;
    const int SCREEN_HEIGHT = 128;
    const int MARGIN = 10;
    
    // 计算缩放比例
    float scale_x = (SCREEN_WIDTH - 2 * MARGIN) / lon_range;
    float scale_y = (SCREEN_HEIGHT - 2 * MARGIN) / lat_range;
    float scale = MIN(scale_x, scale_y);
    
    // 清空屏幕
    tft180_clear();
    
    // 绘制路径
    for (uint16 i = 0; i < gps_control.count - 1; i++)
    {
        // 将经纬度转换为屏幕坐标
        int x1 = MARGIN + (gps_points[i].longitude - min_lon) * scale;
        int y1 = SCREEN_HEIGHT - MARGIN - (gps_points[i].latitude - min_lat) * scale;
        int x2 = MARGIN + (gps_points[i+1].longitude - min_lon) * scale;
        int y2 = SCREEN_HEIGHT - MARGIN - (gps_points[i+1].latitude - min_lat) * scale;
        
        // 绘制线段
        tft180_draw_line(x1, y1, x2, y2, RGB565_RED);
    }
    
    // 绘制所有点位
    for (uint16 i = 0; i < gps_control.count; i++)
    {
        int x = MARGIN + (gps_points[i].longitude - min_lon) * scale;
        int y = SCREEN_HEIGHT - MARGIN - (gps_points[i].latitude - min_lat) * scale;
        
        // 绘制点位
        tft180_draw_point(x, y, RGB565_BLUE);
    }
    
    // 绘制当前目标点位
    if (point_now < gps_control.count)
    {
        int x = MARGIN + (gps_points[point_now].longitude - min_lon) * scale;
        int y = SCREEN_HEIGHT - MARGIN - (gps_points[point_now].latitude - min_lat) * scale;
        
        // 绘制目标点位标记（更大的点）
        tft180_draw_point(x-1, y-1, RGB565_GREEN);
        tft180_draw_point(x, y-1, RGB565_GREEN);
        tft180_draw_point(x+1, y-1, RGB565_GREEN);
        tft180_draw_point(x-1, y, RGB565_GREEN);
        tft180_draw_point(x, y, RGB565_GREEN);
        tft180_draw_point(x+1, y, RGB565_GREEN);
        tft180_draw_point(x-1, y+1, RGB565_GREEN);
        tft180_draw_point(x, y+1, RGB565_GREEN);
        tft180_draw_point(x+1, y+1, RGB565_GREEN);
    }
    
    // 显示点位数量
    char buffer[32];
    sprintf(buffer, "GPS Points: %d", gps_control.count);
    tft180_show_string(5, 5, buffer);
}

/**
 * @brief 获取GPS点位数量
 * @return 点位数量
 */
uint16 gps_get_count(void)
{
    return gps_control.count;
}

/**
 * @brief 获取指定索引的GPS点位
 * @param index 点位索引
 * @param point 点位结构体指针
 * @return 是否获取成功
 */
bool gps_get_point(uint16 index, gps_point_struct *point)
{
    if (point == NULL)
    {
        return false;
    }
    
    if (index >= gps_control.count)
    {
        return false;
    }
    
    *point = gps_points[index];
    
    return true;
}

/**
 * @brief 删除指定索引的GPS点位
 * @param index 点位索引
 * @return 是否删除成功
 */
bool gps_delete_point(uint16 index)
{
    if (index >= gps_control.count)
    {
        return false;
    }
    
    // 从内存数组中删除点位
    for (uint16 i = index; i < gps_control.count - 1; i++)
    {
        gps_points[i] = gps_points[i + 1];
        gps_points[i].index = i;
    }
    
    gps_control.count--;
    
    // 保存到flash
    return gps_save();
}

/**
 * @brief 插入GPS点位到指定索引
 * @param index 插入位置的索引
 * @param point 要插入的点位
 * @return 是否插入成功
 */
bool gps_insert_point(uint16 index, gps_point_struct *point)
{
    if (gps_control.count >= MAX_GPS_POINTS || index > gps_control.count || point == NULL)
    {
        return false;
    }
    
    // 从内存数组中插入点位
    for (uint16 i = gps_control.count; i > index; i--)
    {
        gps_points[i] = gps_points[i - 1];
        gps_points[i].index = i;
    }
    
    gps_points[index] = *point;
    gps_points[index].index = index;
    
    gps_control.count++;
    
    // 保存到flash
    return gps_save();
}

/**
 * @brief 修改指定索引的GPS点位
 * @param index 要修改的点位索引
 * @param point 新的点位数据
 * @return 是否修改成功
 */
bool gps_modify_point(uint16 index, gps_point_struct *point)
{
    if (index >= gps_control.count || point == NULL)
    {
        return false;
    }
    
    // 修改内存数组中的点位
    gps_points[index].latitude = point->latitude;
    gps_points[index].longitude = point->longitude;
    
    // 保存到flash
    return gps_save();
}

/**
 * @brief 检测是否到达指定点位
 * @param lat 目标纬度
 * @param lon 目标经度
 * @param radius 半径（米）
 * @return 是否到达
 */
bool gps_check(double lat, double lon, float radius)
{
    if (gnss.state != 1)
    {
        return false;
    }
    
    double distance = get_two_points_distance(gnss.latitude, gnss.longitude, lat, lon);
    return distance <= radius;
}

/**
 * @brief 设置目标点位索引
 * @param index 点位索引
 * @return 是否设置成功
 */
bool gps_set_current_point(uint16 index)
{
    if (index >= gps_control.count)
    {
        return false;
    }
    
    point_now = index;
    
    return true;
}

/**
 * @brief 获取当前起点与存点的误差
 * @return 是否获取成功
 */
bool gps_start_geterror(void)
{
    if (gnss.state != 1 || gps_control.count == 0)
    {
        return false;
    }
    
    gps_start_point.latitude = gnss.latitude;
    gps_start_point.longitude = gnss.longitude;
    
    // 从内存数组中获取第一个点位
    gps_current_error.latitude = gps_start_point.latitude - gps_points[0].latitude;
    gps_current_error.longitude = gps_start_point.longitude - gps_points[0].longitude;
    
    return true;
}

/**
 * @brief 获取航向角偏差
 */
void gps_yaw_geterror(void)
{
    if (car_distance >= 2.0)   // 出发距离超过2-3米，计算航向角修正值
    {
        double gps_yaw_correction = get_two_points_azimuth(gps_start_point.latitude, gps_start_point.longitude,info.latitude,info.longitude);
        gps_yaw_correction -= angle.yaw;
        
        gps_error_flag = true;
        
        if (gps_yaw_correction > 180)
        {
            gps_yaw_correction -= 360;
        }
    }
}

/**
 * @brief 切换点位函数
 */
void gps_change_point(void)
{
    // 使用Flash存的点循迹
    if (gnss.state == 1 && distance < 2 && distance != 0 || azimuth < 0)
    {
        // 开始接收数据 && 距离最近的点位小于2m || gps的方位角<0表示目标点在车后，车已经走过这个点，那就切换成下一个点
        point_now++;
        
        // 检查是否到达最后一个点位  到达最后一个点位后停止
        if (point_now >= gps_control.count)
        {
            point_now = gps_control.count - 1; // 停留在最后一个点位
            //point_now = 0; 循环切换点位
        }
    }
    
    //gps的定位在亚米级（应该是分米）
    // 在到具体序号的点的时候就要经过相应的元素，可以以此实现精细控制车模姿态和速度
    // 比如到第三个点的时候，就设置为distance < 1，再切换点位，可以控制的更精细
}

/**
 * @brief 核心循迹模块
 * @remark 得到方位角和点与点之间的距离，方位角与航向角计算出角度差，用来循迹。放在定时器
 */
void gps_trace(void)
{
    gps_get_info(&info);
    
    if(!gps_error_flag){gps_yaw_geterror();}//获取航向角偏差
    
    gps_change_point(); 
    
    gps_point_struct target_point;
    
    // 检查是否有有效点位
    if (gps_control.count == 0 || point_now >= gps_control.count)
    {
        return;
    }
    
    // 从内存数组中获取目标点位
    target_point = gps_points[point_now];

    // 经纬度偏差纠正
    target_point.latitude -= gps_current_error.latitude;
    target_point.longitude -= gps_current_error.longitude;
    
    azimuth = get_two_points_azimuth(info.latitude, info.longitude, target_point.latitude, target_point.longitude);
    distance = get_two_points_distance(info.latitude, info.longitude, target_point.latitude, target_point.longitude);

    if (azimuth > 180) // 保证GPS和imu的规划一致
    {
        azimuth -= 360; 
    }
    
    if(gps_error_flag)
    {
        degree_error = azimuth - (angle.yaw - gps_yaw_correction); // 考虑航向角修正值
    }
    else
    {
        degree_error=0;
    }
    
    // 这里规划error是为了让小车在打角的时候选择劣角进行打角
    if (degree_error > 180)
    {
        degree_error -= 360; 
    } else if (degree_error < -180)
    {
        degree_error += 360; 
    }
    else
    {
        degree_error = -degree_error; // 根据pid情况改正负
    } 
}









