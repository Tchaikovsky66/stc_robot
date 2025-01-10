#include <delay.h>
#include <8051.h>
#include "../../include/stc15.h"

void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 921; j++)
            ; // 921次循环大约1毫秒@11.0592MHz
    }
}

// void Timer0_Init(void) // 10΢@24.000MHz
// {
//     AUXR |= 0x80; // 定时器时钟1T模式
//     TMOD &= 0xF0; // 设置定时器模式
//     TL0 = 0x10;   // 设置定时初始值
//     TH0 = 0xFF;   // 设置定时初始值
//     TF0 = 0;      // 清除TF0标志
//     TR0 = 1;      // 定时器0开始计时
// }

// void Timer0_Init(void) // 100微秒@11.0592MHz
// {
//     AUXR |= 0x80; // 定时器时钟1T模式
//     TMOD &= 0xF0; // 设置定时器模式
//     TL0 = 0xAE;   // 设置定时初始值
//     TH0 = 0xFB;   // 设置定时初始值
//     TF0 = 0;      // 清除TF0标志
//     TR0 = 1;      // 定时器0开始计时
//     //ET0 = 1;      // 使能定时器0中断
// }

// 修改Delay10Us函数使用新的定时器初始化函数
// void Delay10Us(unsigned int us)
// {
//     unsigned int i;
//     for (i = 0; i < us; i++)
//     {
//         Timer0_Init(); // 初始化为10us定时
//         while (!TF0)
//             ;    // 等待定时结束
//         TF0 = 0; // 清除TF0标志
//     }
// }

/**
 * @brief 设置步进电机脉冲间隔时间
 * @param us 脉冲间隔时间(微秒)
 * @note 系统时钟11.0592MHz，1T模式下
 * @range 1us ~ 5900us
 */
void SetPulseInterval(unsigned int reload)
{

    // 配置定时器
    AUXR |= 0x80;      // 定时器时钟1T模式
    TMOD &= 0xF0;      // 设置定时器模式
    TL0 = reload;      // 设置定时初始值低8位
    TH0 = reload >> 8; // 设置定时初始值高8位
    TF0 = 0;           // 清除TF0标志
    TR0 = 1;           // 定时器0开始计时

    while (!TF0) // 等待定时器溢出
        ;

    TR0 = 0; // 停止定时器
    TF0 = 0; // 清除溢出标志
}
