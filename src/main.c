#include "main.h"
#include "motor.h"
#include "delay.h"
#include "uart.h"
#include "position.h"
#include "dwin.h"
#include "stc15.h"
#include "common.h"

// void InitValue(unsigned char address, unsigned int data)
// {
//     unsigned char low_byte = data & 0xFF;
//     unsigned char high_byte = (data >> 8) & 0xFF;
//     CFGBUF[address] = high_byte;
//     CFGBUF[address + 1] = low_byte;
// }

/*************  主函数  *****************/
void main(void)
{
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1;
    DelayMs(500);
    EN_485 = 0;
    DelayMs(500);
    EN_485 = 1; // 使能485发送
    Uart1_Init();
    motor_init(); // 初始化电机和定时器
    Timer1_10ms_Init();
    初始化迪文数据();
    DelayMs(10);
    GoToPage(1);
    DelayMs(1000);
    GoToPage(2);

    while (1)
    {
        if (接收到数据)
        {
            从迪文更新数据();
            接收到数据 = 0;
        }
        if (参数调试)
        {
            参数调试 = 0;
            电机复位();
            给迪文上传数据(Addr迪文14标志, 0xffff);
            调试();
        }
        if (复位并运行)
        {
            复位并运行 = 0;
            DelayMs(20);
            给迪文上传数据(Addr迪文14标志, 0xffff);
            // GoToPage(70);
            电机复位();
            获取迪文所有数据();
            DelayMs(20);
            运行();
        }
        if (测试运行)
        {
            给迪文上传数据(Addr迪文14标志, 0xffff);
            DelayMs(20);
            GoToPage(70);
            while (1)
            {
                获取迪文所有数据();
                DelayMs(20);
                从迪文更新数据();
                if (测试运行)
                {
                    给迪文上传数据(Addr迪文14标志, 0xffff);
                    从迪文更新数据();
                    GoToPage(2);
                    break;
                }
                给迪文上传数据(Addr计算速度, 计算速度);
                DelayMs(1000);
                // InitValue(0, 左右速度);
                // Uart1_SendBuffer(CFGBUF, 2);
                MoveToPosition(未加工到加工距离, 0, 0, 500);
                // motor_go(1, 1, 未加工到加工距离, 左右速度);

                DelayMs(1000);
                // InitValue(0, 未加工到加工距离);
                // Uart1_SendBuffer(CFGBUF, 2);
                MoveToPosition(0, 0, 0, 500);
                // motor_go(1, 0, 未加工到加工距离, 左右速度);
            }

            // DelayMs(100);
            // Uart1_SendBuffer(CFGBUF, 0x15 * 2 + 1);
        }
        DelayMs(10);
        // Uart1_SendString("num\r\n");
        // InitValue(0, x轴脉冲总值);
        // Uart1_SendBuffer(CFGBUF, 0x15 * 2 + 1);
    }
}
