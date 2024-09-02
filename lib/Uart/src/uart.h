#ifndef _UART_H
#define _UART_H

#include <8051.h>
void UART_SendChar(char);
void UART_SendString(char *str);

#endif // !_UART_H