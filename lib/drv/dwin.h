#ifndef _DWIN_H
#define _DWIN_H

#include "config.h"

/*=== 图标管理 ===*/
#define ICON_ADD    1
#define ICON_REMOVE 0

/*=== 函数声明 ===*/
void GoToPage(unsigned char page);
void WriteData(unsigned char address, unsigned char data1, unsigned char data2);
void InitValue(unsigned char address, unsigned int data);
void Blick(void);

void 给迪文上传数据(unsigned char address, unsigned int data);
void 获取迪文所有数据(void);
void 获取所有工件名称(void);
void 从缓存区发送数据给迪文(void);
void 从接收缓冲区更新数据(void);
void 从迪文更新数据(void);
void 更新图标L(__bit add, char icon);
void 更新图标H(__bit add, char icon);
void 恢复出厂设置(void);
void 从缓存更新型号(void);
void 将型号发送给迪文(void);
void 发送型号(unsigned int addr, unsigned char buf_offset, unsigned char len);
void 发送中文(unsigned int address, const char *text, unsigned char len);
void 发送序号(unsigned int seq);

#endif /* _DWIN_H */
