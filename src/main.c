#include <stdio.h>
#include <8051.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>
#include <isr.h>
#include <motor.h>
#include <position.h>
#include <config.h>
#include "../../include/stc15.h"

__xdata char buf[100] = {0};
__xdata char buf1[10] = {0xff};

void main(void)
{
    // 初始化代码
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1;
    DelayMs(500);
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1; // 使能485发送
    DelayMs(500);
    Motor_init();
    Uart1_Init();
    DelayMs(10);
    GoToPage(0x06);
    DelayMs(10);
    InitData();
    DelayMs(10);
    Uart1_SendString("start!\r\n");
    DelayMs(20);
    GetAllData();
    EN_485 = 0; // 禁用发送，可接收
    DelayMs(10);
    update_parameters();

    CFGBUF[0] = 1;
    CFGBUF[1] = 1;
    while (1)
    {
        if (all_data_flag)
        {
            EN_485 = 1;
            WriteData(0x16, 0x00, 0x00);
            GetAllData();
            EN_485 = 0;
            update_parameters();
            RCVOK = 0x00;
        }
        else if (RCVOK == 0xff)
        {
            EN_485 = 1;
            update_parameters();
            RCVOK = 0x00;

            // #ifdef DEBUG
            Uart1_SendString("receive data\r\n");
            // sprintf(buf, "up_flag = %d \r,down_flag = %d \r",
            //         up_flag, down_flag);
            // Uart1_SendString(buf);
            // #endif

            EN_485 = 0;
        }

        if (init_pos_flag)
        {

            EN_485 = 1;
            init_pos_flag = 0;
            current_pos_x = 0;
            current_pos_y = 0;
            Uart1_SendString("init pos\r\n");
            DelayMs(10);
            // MoveToPosition(0, 0, 100);
            SetReferencePoint(0, 0, 1, 100);
            SetReferencePoint(0, 0, 2, 100);
            SetReferencePoint(0, 0, 3, 100);
            // UploadData(0x13 * 2, 0x00);
            UploadData(0x16 * 2, 0x00);
            update_parameters();
            DelayMs(10);
            GoToPage(0x07);
            DelayMs(10);
            EN_485 = 0;
            while (1)
            {
                if (HandleReferencePointSetting(left_right_distance, 100) == FINISH)
                {
                    break;
                }
            }
            EN_485 = 1;
            DelayMs(3);
            MoveToPosition(0, 0, 100);
            UploadData(0x16 * 2, 0x00);
            UploadData(0x13 * 2, 0x00);
            update_parameters();
            DelayMs(3);
            EN_485 = 0;
        }
        DelayMs(50);
        EN_485 = 0;
    }
}
