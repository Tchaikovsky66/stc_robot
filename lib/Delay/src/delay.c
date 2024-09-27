#include <delay.h>

void Timer0Init_ms(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初始值
	TH0 = 0xA2;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void delay_ms(unsigned int ms) {
    unsigned int i;
  for (i = 0; i < ms; i++) {
        Timer0Init_ms();
        while (!TF0);  // 等待定时器溢出
        TR0 = 0;       // 停止定时器
        TF0 = 0;       // 清除溢出标志
    }
}

void Timer1Init_10us(void)		//10微秒@24MHz
{
    AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x40;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
}

void delay_10us(unsigned int us) {
    unsigned int i;
    for (i = 0; i < us; i++) {
        Timer1Init_10us();
        while (!TF1);  // 等待定时器溢出
        TR1 = 0;       // 停止定时器
        TF1 = 0;       // 清除溢出标
    }
}


