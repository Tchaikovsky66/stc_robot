#include <delay.h>

// 实现一个软件延时函数，输入值为毫秒数
void DelayMs(unsigned int ms)
{
    unsigned char i, j;
    while (ms--) // 每次循环实现1ms的延时
    {
        i = 24;  // 外层循环计数器
        j = 85;  // 内层循环计数器
        do
        {
            while (--j); // 内层循环，消耗时间
        } while (--i);   // 外层循环，消耗时间
    }
}