#include "Attitude.h"
#include "Quaternion.h"
#include "filter.h"


float gyroscopeOffset[3] = {0.0f, 0.0f, 0.0f};    // 陀螺仪零偏值
float accelerometerOffset[3] = {0.0f, 0.0f, 0.0f};    // 加速度计零偏值
Imu_data imudata = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float last_gyroscope[3] = {0.0f, 0.0f, 0.0f};   //一阶低通滤波保留数据

/*确保小车停稳*/
static void imu660ra_check_stable(void)
{

    const uint16_t stable_target_count = 500;  // 连续稳定500次即判定为停稳
    const uint16_t gyro_diff_threshold = 100; // 相邻两次陀螺仪数据差值阈值
    uint16_t current_stable_count = 0;        
    
    int32_t last_gx, last_gy, last_gz;
    int32_t cur_gx, cur_gy, cur_gz;

    imu660ra_get_gyro();
    last_gx = imu660ra_gyro_x;
    last_gy = imu660ra_gyro_y;
    last_gz = imu660ra_gyro_z;

    system_delay_ms(2);

    while (current_stable_count < stable_target_count)
    {
        imu660ra_get_gyro();
        cur_gx = imu660ra_gyro_x;
        cur_gy = imu660ra_gyro_y;
        cur_gz = imu660ra_gyro_z;
   
        if (abs(cur_gx - last_gx) < gyro_diff_threshold &&
            abs(cur_gy - last_gy) < gyro_diff_threshold &&
            abs(cur_gz - last_gz) < gyro_diff_threshold)
        {
            current_stable_count++;
        }
        else
        {
            current_stable_count = 0;
        }

        last_gx = cur_gx;
        last_gy = cur_gy;
        last_gz = cur_gz;

        system_delay_ms(2);
    }
}

/*获取imu零偏*/
void imu_get_bias(void)
{
    const uint16 sample_count = 1000;
    int32 gyro_sum[3] = {0};
    int32 acc_sum[3] = {0};
    
    imu660ra_check_stable();
    
    for(uint16 i = 0; i < sample_count; i++)
    {
        imu660ra_get_gyro();
        imu660ra_get_acc();
        if(abs(imu660ra_gyro_x) + abs(imu660ra_gyro_y) + abs(imu660ra_gyro_z) < 150) 
        {
            gyro_sum[0] += imu660ra_gyro_x;
            gyro_sum[1] += imu660ra_gyro_y;
            gyro_sum[2] += imu660ra_gyro_z;

            acc_sum[0] += imu660ra_acc_x;
            acc_sum[1] += imu660ra_acc_y;
            acc_sum[2] += imu660ra_acc_z;
        }
        else
        {
            i--;
        }   

        system_delay_ms(2);
    }
    
    gyroscopeOffset[0] = (float)gyro_sum[0] / (float)sample_count;
    gyroscopeOffset[1] = (float)gyro_sum[1] / (float)sample_count;
    gyroscopeOffset[2] = (float)gyro_sum[2] / (float)sample_count;

    accelerometerOffset[0] = (float)acc_sum[0]/ (float)sample_count;
    accelerometerOffset[1] = (float)acc_sum[1]/ (float)sample_count;
    accelerometerOffset[2] = (float)acc_sum[2]/ (float)sample_count - 4096.0f; // 减去1g偏置

    //printf("\r\n Gyro Bias: %.2f , %.2f , %.2f", gyroscopeOffset[0], gyroscopeOffset[1], gyroscopeOffset[2]);
    //printf("\r\n Acc Bias: %.2f , %.2f , %.2f", accelerometerOffset[0], accelerometerOffset[1], accelerometerOffset[2]);
}

/*imu初始化*/
void Attitude_Init(void)
{
    imu660ra_init();
      
    system_delay_ms(50);

    imu_get_bias();
    
    Attitude_calibration(); 
    last_gyroscope[0] = imudata.gx;
    last_gyroscope[1] = imudata.gy;
    last_gyroscope[2] = imudata.gz;

    Quaternion_Init();
}

/*去除零偏转化单位*/
void Attitude_calibration(void)
{
    
    imu660ra_get_gyro();
    imu660ra_get_acc();

    imudata.gx = ((float)imu660ra_gyro_x - gyroscopeOffset[0])/16.384*PI/180.0; // 转为弧度制
    imudata.gy = ((float)imu660ra_gyro_y - gyroscopeOffset[1])/16.384*PI/180.0;
    imudata.gz = ((float)imu660ra_gyro_z - gyroscopeOffset[2])/16.384*PI/180.0;

    imudata.ax = ((float)imu660ra_acc_x - accelerometerOffset[0]);
    imudata.ay = ((float)imu660ra_acc_y - accelerometerOffset[1]);
    imudata.az = ((float)imu660ra_acc_z - accelerometerOffset[2]);

}

/*对角速度进行一阶低通滤波，对加速度进行卡尔曼滤波,平滑数据*/
void Attitude_filter(void)
{
 
    imudata.gx = filter_FirstOrderLowPassFilter(imudata.gx, last_gyroscope[0]);
    imudata.gy = filter_FirstOrderLowPassFilter(imudata.gy, last_gyroscope[1]);
    imudata.gz = filter_FirstOrderLowPassFilter(imudata.gz, last_gyroscope[2]);
    last_gyroscope[0] = imudata.gx;
    last_gyroscope[1] = imudata.gy;
    last_gyroscope[2] = imudata.gz;
    
    imudata.ax = (float)filter_KalmanFilter(&kfs1[0], imudata.ax); 
    imudata.ay = (float)filter_KalmanFilter(&kfs1[1], imudata.ay);
    imudata.az = (float)filter_KalmanFilter(&kfs1[2], imudata.az);
    
}


/*获取姿态角*/
void Attitude_get(void)//pitch，roll，yaw前右右 正数
{
  Attitude_calibration();
  Attitude_filter();
  Quaternion_Update();
  Quaternion_GetEuler(&angle);
  //printf("angle:%f,%f,%f\n",angle.pitch,angle.roll,angle.yaw);
}