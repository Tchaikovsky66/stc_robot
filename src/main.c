#include <stdio.h>
#include <stc15.h>
#include <main.h>
#include <uart.h>
#include <delay.h>
#include <button.h>

void main(void)
{
    float p = 3.1415926;
    int a = 0;
    __xdata char buf[10] = {0};
    __xdata char buf1[100] = {0};
    
    Uart1_Init();
    //Interrupt0_Init();

    GoToPage(0x03);
    GetAllData();
    while(1)
    {
        if(Button44_Pressed())
        {
            GetAllData();
            DelayMs(10);
            update_parameters();

            SendAllData();
        }
        if(model == 0x01)
        {
            P00 = 0;
            DelayMs(100);
            P00 = 1;
            DelayMs(100);
            P00 = 0;
            DelayMs(100);
            P00 = 1;
        }
        DelayMs(100);
    }
}
