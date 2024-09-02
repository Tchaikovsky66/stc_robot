# ipa15w4k61s4

## 实现功能

1. 计时器延时函数 delay_ms();
2. 串口字符串发送 UART_SendString();
3. 串口中断接收字符串（/n结束）  UART_ISR() __interrupt (4)
4. 按键防抖（P2_0） Button_Pressed(); --8.22
5. 控制步进电机旋转步数、速度、方向。rotate_motor(unsigned int steps,__bit dir);
6. 有级变速功能
7. 通过输入加速距离、距离、减速距离、方向控制电机contorlMotor()

## bugs

+ 外部中断
