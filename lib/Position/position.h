#ifndef _POSITION_H
#define _POSITION_H

// 删除Point结构体定义
// 修改函数声明
char MoveToPosition(int target_x, int target_y, int target_y2, int speed);
void UploadPosition(char num, int x, int y, int y2);

extern volatile int sign0_x; // 目前坐标
extern volatile int sign0_y;
extern volatile int sign0_y2;

// 临时目标位置不需要volatile，因为只在函数内部使用
extern __xdata int target_x;
extern __xdata int target_y;
extern __xdata int target_y2;

// 参考点位置放在片外RAM，因为空间较大且访问频率低
extern __xdata int sign1_x;
extern __xdata int sign1_y;
extern __xdata int sign1_y2;
extern __xdata int sign2_x;
extern __xdata int sign2_y;
extern __xdata int sign2_y2;
extern __xdata int sign3_x;
extern __xdata int sign3_y;
extern __xdata int sign3_y2;

// 标志位使用bit类型节省内存
extern volatile __bit sign1_flag;
extern volatile __bit sign2_flag;
extern volatile __bit sign3_flag;

#endif