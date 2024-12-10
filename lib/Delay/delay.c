#include <delay.h>
#include <8051.h>
#include "../../include/stc15.h"

void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 1826; j++);  // 1826次循�?大约�?1�?秒@24MHz
    }
}

void Timer0_Init(void)		//10΢��@24.000MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x10;				//���ö�ʱ��ʼֵ
	TH0 = 0xFF;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}




void Delay10Us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i < us; i++)
    {
        Timer0_Init();  // ��ʼ����ʱ��0
        while (!TF0);   // �ȴ���ʱ�����
        TF0 = 0;        // ���TF0��־
    }
}
