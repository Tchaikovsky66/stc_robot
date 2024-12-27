#include <isr.h>
#include <delay.h>
#include <../../include/stc15.h> 
extern volatile __bit interruptButtonFlag = 0; //�����ж�

//�ж�0��ʼ�� ���������ж� INT0	P3^2
void Interrupt0_Init(void)
{
	IT0 = 0;  // 0�½��ش����ж� 1����
    EX0 = 1;  // ʹ���ⲿ�ж� 0
    EA = 1;   // ʹ��ȫ���ж�
}
void int0_restart(void)
{
    if (P32 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(2);   // Debounce delay
        if (P32 == 0) // Check again to confirm
        {
            IAP_CONTR = 0x20; // 重启
        }
    }
}

/*INT0	P3^2*/
void my_ISR(void) __interrupt (0) 
{
    int0_restart();
}
