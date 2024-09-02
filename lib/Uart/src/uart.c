# include <uart.h>
void UART_SendChar(char c)
{
    SBUF = c; // Load data into UART buffer
    while (!TI); // Wait for transmission to complete
    TI = 0; // Clear transmission interrupt flag
}

void UART_SendString(char *str)
{
    while (*str)
    {
        UART_SendChar(*str++);
    }
}