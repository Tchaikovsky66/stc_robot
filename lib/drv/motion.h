#ifndef _MOTION_H
#define _MOTION_H

#include "config.h"

/*=== 当前坐标 ===*/
extern __data int 当前x坐标;
extern __data int 当前y1坐标;
extern __data int 当前y2坐标;

/*=== 函数声明 ===*/
char MoveToPosition(int target_x, int target_y1, int target_y2, int speed);
void 运行前复位(void);
void 夹取(char side);
void 放工件(char side);
void 通知加工设备复位(void);
void 通知加工设备运行(void);
void 等待工件取走(void);
void 等待加工完成(void);

#endif /* _MOTION_H */
