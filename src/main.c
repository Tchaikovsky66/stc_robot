#include "main.h"
#include "motor.h"
#include "delay.h"
#include "uart.h"
#include "position.h"
#include "dwin.h"
#include "stc15.h"
#include "common.h"
#include "eeprom.h"
#include "low_v.h"
#include "isr.h"
#include <string.h>
#include <button.h>
//-----------------------------------------
#define GBK_永康施拓 0xD3, 0xC0, 0xBF, 0xB5, 0xCA, 0xD0, 0xCA, 0xA9, 0xCD, 0xD8, 0xB5, 0xE7, 0xD7, 0xD3, 0xBF, 0xC6, 0xBC, 0xBC
__xdata int tmp工件序号;
static const unsigned char 永康施拓[] = {GBK_永康施拓};
static const unsigned char 工件1[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x00};
static const unsigned char 工件2[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x32, 0x00};
static const unsigned char 工件3[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x33, 0x00};
static const unsigned char 工件4[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x34, 0x00};
static const unsigned char 工件5[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x35, 0x00};
static const unsigned char 工件6[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x36, 0x00};
static const unsigned char 工件7[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x37, 0x00};
static const unsigned char 工件8[8] = {0xB9, 0xA4, 0xBC, 0xFE, 0x38, 0x00};
//static const unsigned char 版本号[] = "v1.0.0";

#define IAP型号 0x2600
#define IAP_ADDRESS 0x2400

// 定义初始型号数组
static const unsigned char 初始型号数组[] = {
    0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB9, 0xA4, 0xBC, 0xFE, 0x38, 0x00, 0x00, 0x00};

void LVD_ISR(void) __interrupt(6)
{
    GoToPage(60);
    while (1)
        ;
}
/*************  主函数  *****************/
// __sbit __at(0x90) 加工设备有无工件; // P20 Y3
// __sbit __at(0x91) 加工设备异常;     // P21 Y2
// __sbit __at(0x92) 加工设备原位置;   // P22  Y1
void main(void)
{
    //模拟加工设备输出
    // P1M0 = 0x00; P1M1 = 0x00;
    // 加工设备有无工件 = 1; // 模拟加工设备无工件
    // 加工设备原位置 = 1; // 模拟加工设备未到原位置
    // 加工设备异常 = 1; // 模拟加工设备无异常
    // while(1)
    // {
    //     if(Button20_Pressed())
    //     {
    //         加工设备有无工件 = 1; // 模拟加工设备有工件
    //         加工设备原位置 = 0; // 模拟加工设备原位置
    //         加工设备异常 = 0; // 模拟加工设备无异常
    //         DelayMs(100);
    //         加工设备有无工件 = 1; // 模拟加工设备无工件
    //         加工设备原位置 = 1; // 模拟加工设备未到原位置
    //         加工设备异常 = 1; // 模拟加工设备异常
    //     }
    //     if(Button21_Pressed())
    //     {
    //         加工设备有无工件 = 0; // 模拟加工设备无工件
    //         加工设备原位置 = 0; // 模拟加工设备未到原位置
    //         加工设备异常 = 0; // 模拟加工设备异常
    //         DelayMs(100);
    //         加工设备有无工件 = 1; // 模拟加工设备无工件
    //         加工设备原位置 = 1; // 模拟加工设备未到原位置
    //         加工设备异常 = 1; // 模拟加工设备异常
    //     }
    //     DelayMs(100);
    //     P41 = !P41;
    // }
    //模拟结束

    // P4M0 &= ~0x14;
    // P4M1 &= ~0x14;
    P4M0 |= 0x14;
    P4M1 &= ~0x14;
    Blick();
    LVD_Init();
    Uart1_Init();
    DelayMs(20);
    if (IapReadByte(0x0000) != 0xcc)
    {
        恢复出厂设置();
        DelayMs(20);
        if (sequential_write_flash_in_one_sector(IAP_ADDRESS, sizeof(CFGBUF), CFGBUF) == ERROR)
        {
            Blick();
        }

        //!!获取所有工件名称();
        DelayMs(20);
        // 初始化型号数组的值
        for (int i = 0; i < 0x88; i++)
        {
            型号数组[i] = 初始型号数组[i];
        }
        if (sequential_write_flash_in_one_sector(IAP型号, 0x88, 型号数组) == ERROR)
        {
            Blick();
        }
        DelayMs(10);
    }
    else
    {
        if (从扇区中读取(IAP_ADDRESS, sizeof(CFGBUF), CFGBUF) == ERROR)
        {
            Blick();
            从缓存区发送数据给迪文();
        }
        // DelayMs(20);
        if (从扇区中读取(IAP型号, sizeof(型号数组), 型号数组) == ERROR)
        {
            Blick();
        }
    }
    DelayMs(200);
    发送型号(0x100, 0x00, sizeof(型号数组));
    从接收缓冲区更新数据();
    发送序号(工件序号);

    EN_485 = 1; // 使能485发送
    Interrupt0_Init();
    Uart1_Init();
    motor_init(); // 初始化电机和定时器
    Timer1_10ms_Init();
    DelayMs(20);
    GoToPage(1);
    DelayMs(1000);
    GoToPage(2);
    DelayMs(20);
    给迪文上传数据(Addr迪文14标志, 0xffff);
    DelayMs(20);
    从缓存区发送数据给迪文();

    //发送中文(0x201,版本号, sizeof(版本号) - 1);
    while (1)
    {
        /***************************恢复出厂设置***********************************/
        if (恢复出厂设置标志位)
        {
            DelayMs(20);
            恢复出厂设置();
            InitValue(Addr工件序号, 工件序号);

            DelayMs(1000);
            // 从迪文更新数据();
            DelayMs(20);
            GoToPage(3);
            DelayMs(20);
            恢复出厂设置标志位 = 0;
            // 数据区
            if (sequential_write_flash_in_one_sector(IAP_ADDRESS, sizeof(CFGBUF), CFGBUF) == ERROR)
            {
                Blick();
            }
            if (保存数据(工件序号) == ERROR)
            {
                Blick();
            }
            DelayMs(20);
            从接收缓冲区更新数据();
            从缓存区发送数据给迪文();
            // 汉字区（型号） 先给迪文发本序号的初始化型号数据，再从迪文获取全部数据
            switch (工件序号)
            {
            case 0x100:
                /* code */
                发送中文(0x108, 工件1, 8);
                DelayMs(20);
                发送中文(0x100, 工件1, 8);
                break;
            case 0x200:
                发送中文(0x110, 工件2, 8);
                DelayMs(20);
                发送中文(0x100, 工件2, 8);
                break;
            case 0x300:
                发送中文(0x118, 工件3, 8);
                DelayMs(20);

                发送中文(0x100, 工件3, 8);
                break;
            case 0x400:
                发送中文(0x120, 工件4, 8);
                DelayMs(20);

                发送中文(0x100, 工件4, 8);
                break;
            case 0x500:
                发送中文(0x128, 工件5, 8);
                DelayMs(20);

                发送中文(0x100, 工件5, 8);
                break;
            case 0x600:
                发送中文(0x130, 工件6, 8);
                DelayMs(20);

                发送中文(0x100, 工件6, 8);
                break;
            case 0x700:
                发送中文(0x138, 工件7, 8);
                DelayMs(20);

                发送中文(0x100, 工件7, 8);
                break;
            case 0x800:
                发送中文(0x140, 工件8, 8);
                DelayMs(20);
                发送中文(0x100, 工件8, 8);
                break;

            default:
                break;
            }
            DelayMs(20);
            获取所有工件名称();
            DelayMs(40);
            if (sequential_write_flash_in_one_sector(IAP型号, 0x88, 型号数组) == ERROR)
            {
                Blick();
            }
            发送序号(工件序号);
            GoToPage(3);
        }
        /***************************保存***********************************/
        if (保存)
        {
            给迪文上传数据(Addr迪文14标志, 0xffff);
            DelayMs(20);
            GoToPage(57); // 保存中
            DelayMs(20);
            获取迪文所有数据();
            DelayMs(800);
            if (sequential_write_flash_in_one_sector(IAP_ADDRESS, sizeof(CFGBUF), CFGBUF) == ERROR)
            {
                Blick();
            }
            DelayMs(20);
            if (保存数据(工件序号) == ERROR)
            {
                Blick();
            }
            保存 = 0;
            从缓存区发送数据给迪文();
            if (从扇区中读取(IAP型号, 0x88, 型号数组) == ERROR)
            {
                Blick();
            }
            发送型号(0x100, 0x00, sizeof(型号数组));
            发送序号(工件序号);

            GoToPage(2);
        }

        /***************************保存工件名称***********************************/
        if (保存工件名称)
        {
            保存工件名称 = 0;
            给迪文上传数据(Addr迪文14标志, 0xffff);
            DelayMs(20);
            GoToPage(57); // 保存中
            DelayMs(20);
            // 保存工件名称
            获取所有工件名称();
            DelayMs(500);
            if (sequential_write_flash_in_one_sector(IAP型号, 0x88, 型号数组) == ERROR)
            {
                Blick();
            }
            DelayMs(500);
            发送序号(工件序号);
            DelayMs(200);
            GoToPage(2);
        }
        /***************************接收数据***********************************/
        if (接收到数据 && (!保存) && (!保存工件名称) && (!恢复出厂设置标志位))
        {
            DelayMs(20);
            tmp工件序号 = 工件序号;
            从接收缓冲区更新数据();
            DelayMs(20);
            if (tmp工件序号 != 工件序号)
            {
                // CFGBUF[Addr工件序号 + 1] = 0;
                // 发送型号(0x100, 0x00, sizeof(型号数组));

                if (切换数据(工件序号) == ERROR)
                {
                    Blick();
                }
                InitValue(Addr工件序号, 工件序号); // 下次重启能知道当前序号
                if (sequential_write_flash_in_one_sector(IAP_ADDRESS, sizeof(CFGBUF), CFGBUF) == ERROR)
                {
                    // Uart1_SendString("write err");
                    Blick();
                }

                DelayMs(10);
                // 发送型号(0x100, (工件序号 / 100) * 8 * 2, 8);
                发送序号(工件序号);
                // 发送序号(工件序号);
                DelayMs(10);
                获取所有工件名称();
                if (sequential_write_flash_in_one_sector(IAP型号, 0x88, 型号数组) == ERROR)
                {
                    Blick();
                }
            }
            接收到数据 = 0;
        }
        /***************************参数调试***********************************/
        if (参数调试)
        {
            参数调试 = 0;
            电机复位();
            给迪文上传数据(Addr迪文14标志, 0xffff);
            调试();
            保存 = 1;
        }
        /***************************复位并运行***********************************/
        if (复位并运行)
        {
            复位并运行 = 0;
            // DelayMs(20);
            给迪文上传数据(Addr迪文14标志, 0xffff);
            GoToPage(70);
            电机复位();
            获取迪文所有数据();
            DelayMs(20);
            运行1();
        }
        /***************************测试运行***********************************/
        if (测试运行)
        {
            给迪文上传数据(Addr迪文14标志, 0xffff);
            DelayMs(20);
            GoToPage(70);
            while (1)
            {
                获取迪文所有数据();
                DelayMs(20);
                从接收缓冲区更新数据();

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
                MoveToPosition(未加工到加工距离, 0, 0, 左右速度);
                // motor_go(1, 1, 未加工到加工距离, 左右速度);

                DelayMs(1000);
                // InitValue(0, 未加工到加工距离);
                // Uart1_SendBuffer(CFGBUF, 2);
                MoveToPosition(0, 0, 0, 左右速度);
                // motor_go(1, 0, 未加工到加工距离, 左右速度);
            }

            // DelayMs(100);
            // Uart1_SendBuffer(CFGBUF, 0x15 * 2 + 1);
        }
        /***************************主函数循环***********************************/
        DelayMs(10);
        // Uart1_SendString("num\r\n");
        // InitValue(0, x轴脉冲总值);
        // Uart1_SendBuffer(CFGBUF, 0x15 * 2 + 1);
    }
}