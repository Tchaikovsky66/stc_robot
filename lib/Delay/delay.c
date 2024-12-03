#include <delay.h>
#include <8051.h>
void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 1826; j++);  // 1826娆″惊鐜?澶х害涓?1姣?绉扏24MHz
    }
}

void Timer0_Init(void)		//10微秒@24.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x10;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}




void Delay10Us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i < us; i++)
    {
        Timer0_Init();  // 初始化定时器0
        while (!TF0);   // 等待定时器溢出
        TF0 = 0;        // 清除TF0标志
    }
}
