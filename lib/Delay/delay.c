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

