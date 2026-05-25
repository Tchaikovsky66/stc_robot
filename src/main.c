#include "stc15.h"
#include "config.h"
#include "gpio.h"
#include "delay.h"
#include "uart.h"
#include "dwin.h"
#include "eeprom.h"
#include "motor.h"
#include "motion.h"
#include "isr.h"
#include "fsm.h"
#include "params.h"

/*=== 完整主循环测试 ===*/
void main(void)
{
    P4M0 |= 0x14;
    P4M1 &= ~0x14;

    fsm_init();

    /* 所有外设就绪后再开急停和低压检测中断 */
    Interrupt0_Init();
    LVD_Init();

    while (1)
    {         
        fsm_run();
    }
}
