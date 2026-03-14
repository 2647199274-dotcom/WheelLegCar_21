/*********************************************************************************************************************
* CYT4BB Opensourec Library 即（ CYT4BB 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 CYT4BB 开源库的一部分
*
* CYT4BB 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cm7_0_isr
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-9      pudding            first version
* 2024-5-14     pudding            新增12个pit周期中断 增加部分注释说明
* 2025-2-4      pudding            优化串口中断逻辑，防止意外干扰导致的卡死问题，优化串口波特率计算逻辑
* 2025-2-4      pudding            新增两个串口接口
********************************************************************************************************************/

#include "zf_common_headfile.h"

//====================================================外部变量声明====================================================
extern small_device_value_struct motor_value;          

//====================================================回调函数声明====================================================
void uart_control_callback(void);                      

// **************************** PIT中断函数 ****************************
void pit0_ch0_isr()                                     
{
    pit_isr_flag_clear(PIT_CH0);
    imu660ra_get_gyro();                                
    imu660ra_get_acc();                                 
    
    quaternion_module_calculate(&roll_balance_cascade); 
    
    pit_callback();                                     
}

void pit0_ch1_isr()                                     
{
    pit_isr_flag_clear(PIT_CH1);
    key_scanner();                                      
}

void pit0_ch2_isr()                                     
{
    pit_isr_flag_clear(PIT_CH2);
    
}

void pit0_ch10_isr()                                    
{
    pit_isr_flag_clear(PIT_CH10);
    
}

void pit0_ch11_isr()                                    
{
    pit_isr_flag_clear(PIT_CH11);
    
}

void pit0_ch12_isr()                                    
{
    pit_isr_flag_clear(PIT_CH12);
    
}

void pit0_ch13_isr()                                    
{
    pit_isr_flag_clear(PIT_CH13);
    
}

void pit0_ch14_isr()                                    
{
    pit_isr_flag_clear(PIT_CH14);
    
}

void pit0_ch15_isr()                                    
{
    pit_isr_flag_clear(PIT_CH15);
    
}

void pit0_ch16_isr()                                    
{
    pit_isr_flag_clear(PIT_CH16);
    
}

void pit0_ch17_isr()                                    
{
    pit_isr_flag_clear(PIT_CH17);
    
}

void pit0_ch18_isr()                                    
{
    pit_isr_flag_clear(PIT_CH18);
    
}

void pit0_ch19_isr()                                    
{
    pit_isr_flag_clear(PIT_CH19);
    
}

void pit0_ch20_isr()                                    
{
    pit_isr_flag_clear(PIT_CH20);
    
}

void pit0_ch21_isr()                                    
{
    pit_isr_flag_clear(PIT_CH21);
    tsl1401_collect_pit_handler();                      
}
// **************************** PIT中断函数 ****************************


// **************************** 串口中断函数 ****************************
// 串口0默认作为调试串口
void uart0_isr (void)
{
    if(uart_isr_mask(UART_0))                            
    {
        
#if DEBUG_UART_USE_INTERRUPT                             
        debug_interrupr_handler();                       
#endif                                                    
      
    }
    else                                                 
    {           
        
        
        
    }
}

void uart1_isr (void)
{
    if(uart_isr_mask(UART_1))                            
    {
        
        wireless_module_uart_handler();                  
      
    }
    else                                                
    {
      
        
        
    }
}

void uart2_isr (void)
{
    if(uart_isr_mask(UART_2))                            
    {
        
        gnss_uart_callback();                            
        
    }
    else                                                
    {
        
        
       
    }
}

void uart3_isr (void)
{
    if(uart_isr_mask(UART_3))                            
    {
        
        
        
    }
    else                                                
    {
      
        
        
    }
}

void uart4_isr (void)
{
    if(uart_isr_mask(UART_4))                            
    {

        uart_receiver_handler();                        
       
    }
    else                                                
    {
      
        
        
    }
}

void uart5_isr (void)
{
    if(uart_isr_mask(UART_5))                            
    {
        
        
       
    }
    else                                                
    {
      
        
        
    }
}

void uart6_isr (void)
{
    if(uart_isr_mask(UART_6))                            
    {

        
       
    }
    else                                                
    {
      
        
        
    }
}
// **************************** 串口中断函数 ****************************

// **************************** 外部中断函数 ****************************
void gpio_0_exti_isr()                                  
{
    
  
  
}

void gpio_1_exti_isr()                                  
{
    if(exti_flag_get(P01_0))		                      
    {

      
      
            
    }
    if(exti_flag_get(P01_1))
    {

            
            
    }
}

void gpio_2_exti_isr()                                  
{
    if(exti_flag_get(P02_0))
    {
            
            
    }
    if(exti_flag_get(P02_4))
    {
            
            
    }

}

void gpio_3_exti_isr()                                  
{



}

void gpio_4_exti_isr()                                  
{



}

void gpio_5_exti_isr()                                  
{



}


void gpio_6_exti_isr()                                  
{



}

void gpio_7_exti_isr()                                  
{



}

void gpio_8_exti_isr()                                  
{



}

void gpio_9_exti_isr()                                  
{



}

void gpio_10_exti_isr()                                 
{



}

void gpio_11_exti_isr()                                 
{



}

void gpio_12_exti_isr()                                 
{



}

void gpio_13_exti_isr()                                 
{



}

void gpio_14_exti_isr()                                 
{



}

void gpio_15_exti_isr()                                 
{



}

void gpio_16_exti_isr()                                 
{



}

void gpio_17_exti_isr()                                 
{



}

void gpio_18_exti_isr()                                 
{



}

void gpio_19_exti_isr()                                 
{



}

void gpio_20_exti_isr()                                 
{



}

void gpio_21_exti_isr()                                 
{



}

void gpio_22_exti_isr()                                 
{



}

void gpio_23_exti_isr()                                 
{



}
// **************************** 外部中断函数 ****************************
