#ifndef _ISR_H
#define _ISR_H

#include <8051.h>

extern volatile __bit interruptButtonFlag;
void Interrupt0_Init(void);
void my_ISR(void) __interrupt (0);

#endif // !_ISR_H