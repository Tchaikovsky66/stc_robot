#ifndef _POSITION_H
#define _POSITION_H

// 删除Point结构体定义
// 修改函数声明
void MoveToPosition(int target_x, int target_y, int target_y2, int speed);
char SetReferencePoint(int point_x, int point_y, int point_y2, unsigned char point_num, int speed);
unsigned char HandleReferencePointSetting(int left_right_distance, int speed);
void UploadPosition(char num, int x, int y, int y2);

// 当前位置放在片内RAM，因为需要频繁访问
extern __xdata volatile int current_pos_x;
extern __xdata volatile int current_pos_y;
extern __xdata volatile int current_pos_y2;

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
extern __xdata int sign0_x; //目前坐标
extern __xdata int sign0_y;
extern __xdata int sign0_y2;

// 标志位使用bit类型节省内存
extern volatile __bit sign1_flag;
extern volatile __bit sign2_flag;
extern volatile __bit sign3_flag;

#endif