# ipa15w4k61s4

## 已实现功能

1. 计时器延时函数 delay_ms();
2. 串口字符串发送 UART_SendString();
3. 串口中断接收字符串（/n结束）  UART_ISR() __interrupt (4)
4. 按键防抖（P2_0） Button_Pressed(); --8.22
5. 控制步进电机旋转步数、速度、方向。rotate_motor(unsigned int steps,__bit dir);
6. 有级变速功能
7. 通过输入距离方向控制电机,s型加速 contorlMotor()

## 待实现

1. omron 限位传感器
2. 上下零位传感器
3. 机械手间通信
4. 触摸屏输入
5. 参数记忆
6. 初始化

## bugs

+ variable_speed_motor() 中： for循环计算时间  int currentperiod = speed_to_period(s_curve_speed) / 2;

## 急需

+ uart功能模块化 转移到其他文件