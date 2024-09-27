#ifndef _DELAY_H
#define _DELAY_H

#include <8051.h>

void Timer0Init_ms(void);		//1毫秒@11.0592MHz
void Timer1Init_10us(void);
void delay_ms(unsigned int ms);
void delay_10us(unsigned int us);

#endif // !_DELAY_H