/*********************************************************************************************************************
* CYT4BB Opensourec Library
* Copyright (c) 2022 SEEKFREE
********************************************************************************************************************/

#include "zf_common_headfile.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_250M);  
    debug_init();
    
    key_init(5);                        
    tft180_init();                      
    //balance_pid_init();                 
    //servo_init();                      
    small_driver_uart_init();           
    Attitude_Init();                    
    pit_ms_init(PIT_CH0,5 );            
    pit_ms_init(PIT_CH1,1 );            
    motor_control(1000,1000);
    while(true)
    {
        printf("%d,%d\n",motor_value.receive_left_speed_data,motor_value.receive_right_speed_data);
        
        system_delay_ms(50);
        
        
    }
    
}