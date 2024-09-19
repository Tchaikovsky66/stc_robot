# ipa15w4k61s4

## 已实现功能

1. 计时器延时函数 delay_ms();
2. 串口字符串发送 UART_SendString();
3. 串口中断接收字符串（/n结束）  UART_ISR() __interrupt (4)
4. 按键防抖（P2_0） Button_Pressed(); --8.22
5. 控制步进电机旋转步数、速度、方向。rotate_motor(unsigned int steps,__bit dir);
6. 有级变速功能
7. 通过输入距离方向控制电机,s型加速 contorlMotor()
8. calculate_s_curve_periods(speed) 计算速度变化曲线，将速度转换为周期

## 待实现

1. omron 限位传感器 ☑️
2. 上下零位传感器
3. 机械手间通信
4. 触摸屏输入
5. 参数记忆 eeprom
6. 初始化
7. 霍尔传感器ss49e

## bugs

+ movetotarget y为0等待时间3.6s
+ 循环中，检测按键后发送uart失败
  + 发现有时串口软件问题
  + 电机控制和串口发送一起时，电机控制也不工作
  + 电机控制过后，串口接受到消息不返回

## 急需

+ 串口控制位置(bug)
+ 右omron检测后不暂停，左检测后不重新启动；

## 迪文

+ 发送信息
  + hex模式
  + 5A A5 05 80 03 00 00 当前画面
  + 5A A5 05 80 03 00 01 第一张画面
  