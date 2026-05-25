#ifndef _FSM_H
#define _FSM_H

/*=== 系统状态 ===*/
typedef enum {
    STATE_INIT    = 0,
    STATE_IDLE,         /* 待机，轮询迪文屏标志位 */
    STATE_RUNNING,      /* 自动运行 */
    STATE_DEBUG,        /* 参数调试 */
    STATE_SAVE,         /* 保存参数到 EEPROM */
    STATE_FACTORY,      /* 恢复出厂设置 */
    STATE_TEST,         /* 测试运行 */
    STATE_SAVE_NAME,    /* 保存工件名称 */
    STATE_ERROR,
} SystemState;

/*=== API ===*/
void fsm_init(void);
void fsm_run(void);
SystemState fsm_state(void);

#endif /* _FSM_H */
