#include <stdio.h>
#include <8051.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>
#include <isr.h>
#include <motor.h>
#include "../../include/stc15.h"


float p = 3.1415926;
int a = 0;
__xdata char buf[100] = {0};
__xdata char buf1[10] = {0xff};
volatile int model = 0;
volatile int core_diameter = 0;
volatile int end_face_distance = 0;
volatile int up_down_speed = 0;
volatile int up_down_distance = 0;
volatile int left_right_speed = 0;
volatile int left_right_distance = 0;
volatile __bit all_data_flag = 0;
volatile __bit left_flag = 0;
volatile __bit right_flag = 0;

volatile int steps;
volatile int delay_10us;
//单片机更新值
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
        left_flag = CFGBUF[0x16*2+1] & 0x02;
        right_flag = CFGBUF[0x16*2+1] & 0x04;

}

//初始化值。地址(0x01),数据(300);
void InitValue(unsigned char address,unsigned int data)
{
    unsigned char low_byte = data & 0xFF;
    unsigned char high_byte = (data>>8) & 0xFF;
    CFGBUF[address] = high_byte;
    CFGBUF[address + 1] = low_byte;
}

//初始化数据
void InitData(void)
{
    //初始化
    // WriteData(0x28,0x00,0x03);       //写单个字节
    InitValue(UP_DOWN_DISTANCE,40);
    InitValue(UP_DOWN_SPEED,200);
    InitValue(LEFT_RIGHT_SPEED,200);
    InitValue(LEFT_RIGHT_DISTANCE,50);
    InitValue(MODEL,1);

    //发送
    Uart1_SendByte(0x5A);
    Uart1_SendByte(0xA5);
    Uart1_SendByte(0x59);
    Uart1_SendByte(0x82); 
    Uart1_SendByte(0x00);
    Uart1_SendByte(0x00);
    int i = 0;
    for(i;i<89;i++)
    {
        Uart1_SendByte(CFGBUF[i]);
    }
}

 
void main(void)
{
    P41 = 0;
    DelayMs(500);
    P41 = 1;
    DelayMs(500);
    P41 = 0;
    DelayMs(500);
    P41 = 1;
    DelayMs(500);
    Motor_init();
    Uart1_Init();
    Interrupt0_Init();
    DelayMs(10);
    GoToPage(0x03);
    DelayMs(10);
    InitData();         //数据初始化
    GetAllData();       //从串口屏获取全部数据
    P41 = 0;
    DelayMs(10);    
    while(1)
    {
        // if(X_R_LIMIT == LIMIT_RICHED)
        // {
        //     X_DIR = GO_LEFT;
        //     P0_2 = 0;
        //     MotorGo(1,100,100);
        // }
        if(left_flag)
        {
            P41 = 1;
            Uart1_SendString("go left\r\n");
            DelayMs(10);
            MotorGo(X_MOTOR,GO_LEFT,left_right_distance,left_right_speed);
            //恢复标志位
            WriteData(0x16,0x00,0x00);
            CFGBUF[0x16*2+1] = 0x00;
            DelayMs(10);

            left_flag = 0;
            P41 = 0;
        }
        if(right_flag)
        {
            P41 = 1;
            Uart1_SendString("go right\r\n");
            DelayMs(10);
            MotorGo(X_MOTOR,GO_RIGHT,left_right_distance,left_right_speed);
            //恢复标志位 n
            WriteData(0x16,0x00,0x00);
            CFGBUF[0x16*2+1] = 0x00;

            DelayMs(10);

            right_flag = 0;
            P41 = 0;
        }

        if(RCVOK == 0xff)
        {
            P41 = 1;
            RCVOK = 0x00;
            DelayMs(10);
            update_parameters();
            DelayMs(10);
            Uart1_SendString("receive some data\r\n");
            // Uart1_SendByte(CFGBUF[48]);
            // Uart1_SendByte(CFGBUF[49]);
            sprintf(buf,"model = %d,all_data_falg = %d\r\n,left_flag = %d,right_flag =%d \r\n",model,all_data_flag,left_flag,right_flag);
            Uart1_SendString(buf);
            DelayMs(10);
            P41 = 0;
        }

        if(all_data_flag)
        {
            P41 = 1;
            WriteData(0x16,0x00,0x00);
            DelayMs(5);
            
            DelayMs(10);
            update_parameters();
            DelayMs(10);
            Uart1_SendString("update all data\r\n");
            RCVOK = 0x00;
            GetAllData();
            P41 = 0;
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
