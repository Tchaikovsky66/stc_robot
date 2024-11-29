#include <stdio.h>
#include <8051.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>


float p = 3.1415926;
int a = 0;
__xdata char buf[100] = {0};
__xdata char buf1[10] = {0xff};
// __xdata char rx_buf[100] = {0x5A, 0xA5, 0x2E, 0x82, 0x00, 0x10,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x01, 0x2c, 0x01, 0x2c, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x03, 0x00, 0x0a, 0x00, 0x00
// };
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
        core_diameter = (CFGBUF[0x29*2] << 8) | CFGBUF[0x29*2+1];
        end_face_distance = (CFGBUF[0x2A*2] << 8) | CFGBUF[0x2A*2+1];
        up_down_speed = (CFGBUF[0x25*2] << 8) | CFGBUF[0x25*2+1];
        up_down_distance = (CFGBUF[0x23*2] << 8) | CFGBUF[0x23*2+1];
        left_right_speed = (CFGBUF[0x24*2] << 8) | CFGBUF[0x24*2+1];
        left_right_distance = (CFGBUF[0x22*2] << 8) | CFGBUF[0x22*2+1];
        all_data_flag = CFGBUF[0x16*2+1] & 0x01;
}

//err

// void InitData(void)
// {
//     int i = 0x00;
//     for(i;i<100;i++)
//     {
//         Uart1_SendByte(rx_buf[i]);
//     }
// }

void main(void)
{
    Uart1_Init();
    //Interrupt0_Init();
    WriteData(0x28,0x00,0x03);
    WriteData(0x10,0x00,0x01);
    WriteData(0x24,0x00,0xC8);
    WriteData(0x25,0x00,0xC8);
    WriteData(0x22,0x00,0x32);
    WriteData(0x23,0x00,0x32);
    DelayMs(10);
    GoToPage(0x03);
    DelayMs(10);
    GetAllData();
    DelayMs(10);

    while(1)
    {
        if(Button44_Pressed())
        {
            GetAllData();
            DelayMs(500);
            SendAllData();
            DelayMs(500);
            //update_parameters();
            //sprintf(buf,"model = %d\r\n",model);
            //Uart1_SendString(buf);
        }

        if(RCVOK == 0xff)
        {
            RCVOK = 0x00;
            DelayMs(10);
            update_parameters();
            DelayMs(10);
            // Uart1_SendByte(CFGBUF[48]);
            // Uart1_SendByte(CFGBUF[49]);
           // sprintf(buf,"model = %d,all_data_falg = %d\r\n",model,all_data_flag);
            //Uart1_SendString(buf);
        }

        if(all_data_flag)
        {
            WriteData(0x16,0x00,0x00);
            DelayMs(5);
            GetAllData();
            DelayMs(10);
            update_parameters();
            DelayMs(10);
            Uart1_SendString("update all data\r\n");
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
