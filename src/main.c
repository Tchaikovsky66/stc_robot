#include <stdio.h>
#include <8051.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>


float p = 3.1415926;
int a = 0;
__xdata char buf[10] = {0};
__xdata char buf1[100] = {0};
volatile int model = 0;
volatile int core_diameter = 0;
volatile int end_face_distance = 0;
volatile int up_down_speed = 0;
volatile int up_down_distance = 0;
volatile int left_right_speed = 0;
volatile int left_right_distance = 0;
volatile __bit all_data_flag = 0;

void update_parameters(void) 
{
        model = (CFGBUF[0x28*2] << 8) | CFGBUF[0x28*2+1];
        core_diameter = (CFGBUF[2] << 8) | CFGBUF[3];
        end_face_distance = (CFGBUF[4] << 8) | CFGBUF[5];
        up_down_speed = (CFGBUF[6] << 8) | CFGBUF[7];
        up_down_distance = (CFGBUF[8] << 8) | CFGBUF[9];
        left_right_speed = (CFGBUF[10] << 8) | CFGBUF[11];
        left_right_distance = (CFGBUF[12] << 8) | CFGBUF[13];
        all_data_flag = CFGBUF[13] & 0x01;
}

void main(void)
{
    Uart1_Init();
    //Interrupt0_Init();

    //GoToPage(0x03);
    GetAllData();
    while(1)
    {
        if(Button44_Pressed())
        {
            GetAllData();
            DelayMs(500);
            SendAllData();
            DelayMs(500);
            update_parameters();
            sprintf(buf,"model = %d\r\n",model);
            Uart1_SendString(buf);
        }

        if(RCVOK == 0xff)
        {
            RCVOK = 0x00;
            DelayMs(10);
            update_parameters();
            DelayMs(10);
            // Uart1_SendByte(CFGBUF[48]);
            // Uart1_SendByte(CFGBUF[49]);
            sprintf(buf,"model = %d\r\n",model);
            Uart1_SendString(buf);
        }
        DelayMs(10);
        
        // if(Button44_Pressed())
        // {
        //     GetAllData();
        //     DelayMs(10);
        //     //update_parameters();

        //     SendAllData();
        // }
        // if(model == 0x01)
        // {
        //     P00 = 0;
        //     DelayMs(100);
        //     P00 = 1;
        //     DelayMs(100);
        //     P00 = 0;
        //     DelayMs(100);
        //     P00 = 1;
        // }
       

    }
}
