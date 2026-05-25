#include "delay.h"

void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 921; j++)
            ; // 921次循环 ≈ 1ms @11.0592MHz
    }
}
