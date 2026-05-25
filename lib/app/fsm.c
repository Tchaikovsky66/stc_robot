#include "fsm.h"
#include "process.h"
#include "stc15.h"
#include "gpio.h"
#include "config.h"
#include "delay.h"
#include "uart.h"
#include "dwin.h"
#include "motor.h"
#include "motion.h"
#include "eeprom.h"
#include "params.h"

static SystemState 当前状态 = STATE_INIT;
static __xdata int 上次工件序号 = 0;

SystemState fsm_state(void)
{
    return 当前状态;
}

/*=== 初始化：读 EEPROM、首次启动处理、上电同步到迪文屏 ===*/
void fsm_init(void)
{
    unsigned char i;

    Blick();
    Uart1_Init();
    DelayMs(20);

    if (是否首次启动())
    {
        /* 首次启动：写入默认参数 */
        恢复出厂设置();
        if (eeprom_write(IAP_ADDR_PARAMS, CFGBUF_SIZE, (unsigned char *)CFGBUF) != OK)
            Blick();

        /* 默认型号名称：全部初始化为"工件1" */
        for (i = 0; i < MODEL_BUF_SIZE; i++)
            型号数组[i] = 0x00;
        for (i = 0; i < 6; i++)
        {
            型号数组[i]              = 工件名称[i];
            型号数组[ADDR_MODEL_1 + i] = 型号1[i];
            型号数组[ADDR_MODEL_2 + i] = 型号2[i];
            型号数组[ADDR_MODEL_3 + i] = 型号3[i];
            型号数组[ADDR_MODEL_4 + i] = 型号4[i];
            型号数组[ADDR_MODEL_5 + i] = 型号5[i];
            型号数组[ADDR_MODEL_6 + i] = 型号6[i];
            型号数组[ADDR_MODEL_7 + i] = 型号7[i];
            型号数组[ADDR_MODEL_8 + i] = 型号8[i];
        }
        保存型号名称();
        标记已初始化();
        DelayMs(10);
    }
    else
    {
        /* 已初始化：从 EEPROM 读 */
        if (eeprom_read(IAP_ADDR_PARAMS, CFGBUF_SIZE, (unsigned char *)CFGBUF) != OK)
        {
            Blick();
            从缓存区发送数据给迪文();
        }
        读取型号名称();
    }

    DelayMs(200);
    发送型号(0x100, 0x00, MODEL_BUF_SIZE);
    从接收缓冲区更新数据();
    发送序号(工件序号);
    上次工件序号 = 工件序号;

    EN_485 = 1;
    motor_init();
    Timer1_10ms_Init();
    DelayMs(20);
    GoToPage(1);
    DelayMs(1000);
    GoToPage(2);
    DelayMs(20);
    给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
    DelayMs(20);
    从缓存区发送数据给迪文();

    当前状态 = STATE_IDLE;
}

/*=== 主循环：轮询迪文屏标志位 ===*/
void fsm_run(void)
{
    /* 恢复出厂设置（优先级最高） */
    if (恢复出厂设置标志位)
    {
        恢复出厂设置标志位 = 0;
        当前状态 = STATE_FACTORY;
        process_factory_reset();
        当前状态 = STATE_IDLE;
        return;
    }

    /* 保存参数 */
    if (保存)
    {
        保存 = 0;
        当前状态 = STATE_SAVE;
        process_save();
        当前状态 = STATE_IDLE;
        return;
    }

    /* 保存工件名称 */
    if (保存工件名称)
    {
        保存工件名称 = 0;
        当前状态 = STATE_SAVE_NAME;
        process_save_name();
        当前状态 = STATE_IDLE;
        return;
    }

    /* 接收到新数据：检测是否切换型号 */
    if (接收到数据)
    {
        DelayMs(20);
        上次工件序号 = 工件序号;
        从接收缓冲区更新数据();
        DelayMs(20);
        if (上次工件序号 != 工件序号)
            process_switch_model();
        接收到数据 = 0;
    }

    /* 参数调试 */
    if (参数调试)
    {
        参数调试 = 0;
        当前状态 = STATE_DEBUG;
        process_debug();
        保存 = 1;  /* 调试结束触发保存 */
        当前状态 = STATE_IDLE;
        return;
    }

    /* 复位并运行 */
    if (复位并运行)
    {
        给迪文上传数据(ADDR_DWIN_FLAG14, 0xFFFF);
        GoToPage(70);
        获取迪文所有数据();
        DelayMs(20);
        当前状态 = STATE_RUNNING;
        process_run();
        当前状态 = STATE_IDLE;
        return;
    }

    /* 测试运行 */
    if (测试运行)
    {
        当前状态 = STATE_TEST;
        process_test();
        当前状态 = STATE_IDLE;
        return;
    }

    /* 空闲 */
    DelayMs(10);
}
