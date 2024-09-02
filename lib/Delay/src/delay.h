#ifndef _DELAY_H
#define _DELAY_H

#include <8051.h>

void Timer0Init_ms(void);		//1毫秒@11.0592MHz
void Timer1Init_100us(void);
void delay_ms(unsigned int ms);
void delay_100us(unsigned int us);

#endif // !_DELAY_H