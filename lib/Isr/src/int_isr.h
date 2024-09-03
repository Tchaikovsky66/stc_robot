#ifndef _INT_ISR_H
#define _INT_ISR_H

#include <8051.h>
extern volatile __bit interruptButtonFlag; //按键中断

void Interrupt0_Init(void);
void my_ISR(void) __interrupt (0) ;


#endif // !_INT_ISR_H