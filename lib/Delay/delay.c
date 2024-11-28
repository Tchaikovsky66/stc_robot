#include <delay.h>

void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 1826; j++);  // 1826次循环大约为1毫秒@24MHz
    }
}