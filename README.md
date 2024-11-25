# stc15

## 问题

1.sprinf无法处理浮点数
解决：整数，小数部分分别处理
sprintf(buf1,"p = %d.%03d\n",(unsigned int)p,(unsigned int)((p-(unsigned int)p)*1000));
