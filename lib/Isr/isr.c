#include <isr.h>
#include <delay.h>
extern volatile __bit interruptButtonFlag = 0; //�����ж�

//�ж�0��ʼ�� ���������ж� INT0	P3^2
void Interrupt0_Init(void)
{
	IT0 = 0;  // 0�½��ش����ж� 1����
    EX0 = 1;  // ʹ���ⲿ�ж� 0
    EA = 1;   // ʹ��ȫ���ж�
}

/*�����ж� INT0	P3^2*/
void my_ISR(void) __interrupt (0) 
{
    interruptButtonFlag = 1;  // ���ñ�־����ʾ�����Ѱ���
}