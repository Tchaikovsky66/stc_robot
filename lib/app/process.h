#ifndef _PROCESS_H
#define _PROCESS_H

/*=== 运行流程（对应旧代码的 运行1() + 新修正后的流程） ===*/
void process_run(void);

/*=== 参数调试流程（对应旧代码的 调试()） ===*/
void process_debug(void);

/*=== 测试运行流程 ===*/
void process_test(void);

/*=== 保存参数到 EEPROM ===*/
void process_save(void);

/*=== 保存工件名称 ===*/
void process_save_name(void);

/*=== 恢复出厂设置 ===*/
void process_factory_reset(void);

/*=== 切换工件型号 ===*/
void process_switch_model(void);

#endif /* _PROCESS_H */
