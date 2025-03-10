
//*******机械手2025-01版**
//	因速度655*100=FFFF使用函数三十二除十六除数不能大于16位,所以速度不能大于655
//发送提示字符，查表发送并判断00H停止发送
//触摸屏版本号显示0101H-0120H,  
//报警&提示0121-0153H  (64个字节，32个中文字)
//型号中文显示 0200-0200FH >0201-021F  >> 0209-029F
//************IO口定义*************

	X轴方向	BIT	P0.0	                                          //DRV
	X轴脉冲	BIT	P0.1	                                          //PUL
	X轴使能	BIT	P0.2	                                          //ENB
	Y1轴右方向	BIT	P0.3	                                       //DRV
	Y1轴右脉冲	BIT	P0.4	                                       //PUL
	Y2轴左方向	BIT	P0.5	                                       //DRV
	Y2轴左脉冲	BIT	P0.6	                                       //PUL
	备用OUT3	BIT	P0.7

	串口2接收口	BIT	P1.0	                                      //RXD2
	串口2发送口	BIT	P1.1	                                      //TXD2
	串口2发送使能	BIT	P1.2	                                    //DE
	X右限位	BIT	P1.3
	X左限位	BIT	P1.4
	X中间传感器	BIT	P1.5
	放V工件有无	BIT	P1.6	                                      //	左边存放V型块上工件是否取走
	取V工件有无	BIT	P1.7	                                      //右边上一台	存放V型块上有无工件

	加工设备有无工件	BIT	P2.0	                                 //加工设备Y3
	加工设备异常	BIT	P2.1	                                     //加工设备Y2
	加工设备原位置	BIT	P2.2	                                   //加工设备Y1
	加工设备复位	BIT	P2.3	                                     //加工设备X1
	加工设备运行	BIT	P2.4	                                     //加工设备X2
	备用OUT1	BIT	P2.5
	指示灯	BIT	P2.5
	备用OUT2	BIT	P2.6
	备用INC2	BIT	P2.7

	串口1接收口	BIT	P3.0	                                      //RXD1
	串口1发送口	BIT	P3.1	                                      //TXD1
	//串口1发送使能			BIT		0C1H	//P4.1	//DE
	急停开关	BIT	P3.2	                                         //中断INT0
	Y1右工件检测	BIT	P3.3	                                     //A2
	Y1右零位	BIT	P3.4	                                         //A1
	Y2左零位	BIT	P3.5	                                         //A3
	Y2左工件检测	BIT	P3.6	                                     //A4
	X左防撞	BIT	P3.7	                                          //A5
	Y刹车	BIT	P3.7	                                            //A5
	串口1发送使能	BIT	0C1H	                                    //P4.1	//DE
	Y2轴左吸爪	BIT	0C2H	                                       //P4.2
	Y1轴右吸爪	BIT	0C4H	                                       //P4.4
	备用C1IN	BIT	0C5H	                                         //P4.5
	下一台通讯口1	BIT	0CCH	                                    //P5.4
;下一台通讯口2输出	P1.6		放V工件有无传感器直接信号输出
	上一台通讯口1	BIT	0CDH	                                    //P5.5
;上一台通讯口2//取V工件有无	BIT	P1.7

//*******IO口定义结束**************



//付板主板通讯标记0010H	EQU	20H****付板发送给主板控制用
	//标记	BIT	07H																																												//F15
	//	BIT	06H																																																//E14
	//	BIT	05H																																																//D13
	//	BIT	04H																																																//C12
	//	BIT	03H																																																//B11
	//	BIT	02H																																																//A10
	//	BIT	01H																																																//9
	//付板更新标记	BIT	00H																																				//8

	//付板主板通讯0010L	EQU	21H	***	付板发送给主板控制用
	//X左防撞标记	BIT	0FH	                                       //7
	//Y2左工件检测标记	BIT	0EH	                                  //6
	//Y2左零位标记	BIT	0DH	                                      //5
	//Y1右零位标记	BIT	0CH	                                      //4
	//Y1右工件检测标记	BIT	0BH	                                  //3
	//标记		BIT	0AH																																														//2
	//标记		BIT	09H																																														//1
	//	BIT	08H																																																			//0

//*传感器信号标记0011H	EQU	22H*** 
	X右V工件检测标记	BIT	17H	                                  //P1.7																	//F15
	X左V工件检测标记	BIT	16H	                                  //P1.6																	//E14
	//X中间传感器标记	BIT	15H	                                   //P1.5																	//D13
	X左限位标记	BIT	14H	                                       //P1.4																	//C12
	X右限位标记	BIT	13H	                                       //B11
	加工设备原位置标记	BIT	12H	                                //P2.3																	//A10
	加工设备异常标记	BIT	11H	                                  //P2.1																//9
	加工设备V有无工件标记	BIT	10H	                             //P2.0																	//8

//*传感器信号标记0011L	EQU	23H	***	 
	X左防撞标记	BIT	1FH	                                       //7
	Y2左工件检测标记	BIT	1EH	                                  //6
	Y2左零位标记	BIT	1DH	                                      //5
	Y1右零位标记	BIT	1CH	                                      //4
	Y1右工件检测标记	BIT	1BH	                                  //3
	左右电机刹车标记	BIT	1AH	                                  //2
	Y2轴左吸爪标记	BIT	19H	                                    //1
	Y1轴右吸爪标记	BIT	18H	                                    //0

	//当前数值12H	EQU	24H

	//当前数值12L	EQU	25H
	//****调试H标记13H	EQU	26H****
	//标记	BIT	37H																																									//F15
	//标记	BIT	36H																																									//E14
	//	BIT	35H																																													//D13
	//	BIT		34H																																												//C12
	//	BIT		33H																																												//B11
	//	标记	BIT	32H																																								//A10
	//	标记	BIT	31H																																								//9
	下一项开关标记	BIT	30H	                                    //8
//****脉冲L输出标记13L	EQU	27H	***
	//错误X标记	BIT	3FH																																								//7
	//错误Y标记	BIT3EH																																								//6
	//标记	BIT	3DH																																													//5
	//标记	BIT	3CH																																													//4
	Y2脉冲输出标记	BIT	3BH	                                    //3
	Y1脉冲输出标记	BIT	3AH	                                    //2
	X脉冲输出标记	BIT	39H	                                     //1
	脉冲输出总开关标记	BIT	38H	                                //0
//***临时标记14H	EQU	28H****
	数值正负标记	BIT	47H	                                      //F15
	数值为零标记	BIT	46H	                                      //E14
	//标记							BIT	45H																																							//D13
	T1计时1S标记	BIT	44H	                                      //C12
	串口1发送缓冲函数标记	BIT	43H	                             //B11
	T0Y中断标记	BIT	42H	                                       //A10
	T0X中断标记	BIT	41H	                                       //9
	T0中断4次标记	BIT	40H	                                     //8
	//****临时标记13L	EQU	29H	****
	发送忙标志	BIT	4FH	                                        //7
	串口接收中断标记	BIT	4EH	                                  //6
	接收数据读完标记	BIT	4DH	                                  //5
	串口2TI发送忙标记	BIT	4CH	                                 //4
	串口2RI接收忙标记	BIT	4BH	                                 //3
	串口2接收中断标记	BIT	4AH	                                 //2
	长度减一标记	BIT	49H	                                      //1
	当前速度为零标记	BIT	48H	                                  //0
	当前步数为零标记	BIT	48H

//*****************
//	发收按扭标记15H	EQU	2AH
	参数保存标记	BIT	57H	                                      //F15
	参数初始化标记	BIT	56H	                                    //E14
	调试按钮标记	BIT	55H	                                      //D13
	复位运行按钮标记	BIT	54H	                                  //C12
	单独参数保存标记	BIT	53H	                                  //B11
	;标记	BIT	52H																																														//A10
	;标记	BIT	51H																																										//9
	;标记	BIT	50H																																							//8
//发收按扭标记15L	EQU	2BH
	;报警标记	BIT	6FH																																					//7
	;光栅标记	BIT	5EH																																											//6
	;信号标记	BIT	5DH																																			//5
	;信号标记	BIT	5CH																																			//4
	;信号标记	BIT	5BH																																							//3
	;信号标记	BIT	5AH																																							//2
	急停开关标记	BIT	59H	                                      //1
	启动按钮标记	BIT	58H	                                      //0
	//*****************
;保存标记16H	EQU	2CH	**开机不复位
	;使能1标记	BIT	67H																																					//F15
	;使能2标记	BIT	66H																																	//E14
	;使能3标记	BIT	65H																																							//D13
	;使能4标记	BIT	64H																																										//C12
	;标记	BIT	63H																																							//B11
	;标记	BIT	62H																																							//A10
	;标记	BIT	61H																																							//9
	;使能2标记	BIT	60H																																							//8
;保存标记16L	EQU	2DH	**开机不复位
	//标记	BIT	6FH																																			//7
	//标记	BIT	6EH																																							//6
	//标记	BIT	6DH																																									//5
	//标记	BIT	6CH																																							//4
	//标记	BIT	6BH																																						//3
	//标记	BIT	6AH																																						//2
	//标记	BIT	69H																																						//1
	//标记BIT	68H																																						//0


//****以上是通讯标记****//

//**开始***触摸屏0010H地址=单片机高位20H低位21H***
;0010
	//付板返回通讯数据0010H	EQU	20H
	//付板返回信号标记0010L	EQU	21H	                             //付板P1口OUT标记
;0011
	传感器信号标记H	EQU	22H
	传感器信号标记L	EQU	23H	                                   //	主板P1口OUT标记
;0012
	当前数值H	EQU	24H
	当前数值L	EQU	25H
;0013
	调试H标记13H	EQU	26H
	脉冲L输出标记13L	EQU	27H
;0014
	临时标记14H	EQU	28H
	临时标记14L	EQU	29H
;0015
	按扭标记15H	EQU	2AH
	按扭标记15L	EQU	2BH
	//	以下保存参数
;0016
	保存标记H	EQU	2CH	                                         //			开机不置1
	保存标记L	EQU	2DH

/*	----C260-(--------B320------<.....A180........
------------D580------- -------E100-->...F70...>   */
//	X1=A+B			X2=A+B+C		 

;0017
	X向右零位H	EQU	2EH
	X向右零位L	EQU	2FH
;0018     Y3
	Y向两头取放距离H	EQU	30H
	Y向两头取放距离L	EQU	31H
;0019     Y4	
	Y向中间抓V距离H	EQU	32H
	Y向中间抓V距离L	EQU	33H
;001A     X1   	
	X向右零位至中心距离H	EQU	34H	                              //	固定A180MM	(可输入微调）                                       
	X向右零位至中心距离L	EQU	35H
;001B     X2 	
	X向右零位至左零位距离H	EQU	36H	                            //	固定1A80+C260MM	(可输入微调）	
	X向右零位至左零位距离L	EQU	37H
;001C
	左右运行速度H	EQU	38H
	左右运行速度L	EQU	39H
;001D
	上下运行速度H	EQU	3AH
	上下运行速度L	EQU	3BH
;001E
	低速H	EQU	3CH
	低速L	EQU	3DH
	步数放大系数	EQU	3DH
;001F
	检测速度H	EQU	3EH
	检测速度L	EQU	3FH
;0020
	工件端面至中心距离H	EQU	40H
	工件端面至中心距离L	EQU	41H
;0021
	工件铁芯直径H	EQU	42H
	工件铁芯直径L	EQU	43H
;0022
	型号H	EQU	44H
	型号L	EQU	45H
	;0023
	备用24H	EQU	46H
	备用24L	EQU	47H
;0024
	左右X方向检测距离H	EQU	48H
	左右X方向检测距离L	EQU	49H
;0025
	上下Y方向检测距离H	EQU	4AH
	上下Y方向检测距离L	EQU	4BH
;0026
	左右X方向上一次检测距离H	EQU	4CH
	左右X方向上一次检测距离L	EQU	4DH
;0027
	上下Y方向上一次检测距离H	EQU	4EH
	上下Y方向上一次检测距离L	EQU	4FH

;0028
	A180长度H	EQU	50H
	A180长度L	EQU	51H
;0029
	B320长度H	EQU	52H
	B320长度L	EQU	53H
;002A
	C260长度H	EQU	54H
	C260长度L	EQU	55H
;002B
	D580长度H	EQU	56H
	D580长度L	EQU	57H
;002C
	E80长度H	EQU	58H
	E80长度L	EQU	59H
;002D
	F20H	EQU	5AH
	F20L	EQU	5BH
;002E
	H180长度H	EQU	5CH
	H180长度L	EQU	5DH
;003F
	端面检测距离H	EQU	5EH
	端面检测距离L	EQU	5FH
;0040
	        


//触摸屏0000H地址=单片机高位00H低位01H

		;EQU	60H
	; EQU	61H
	 ; 	EQU	62H
  ;	EQU	63H
	当前时间H	EQU	64H
	当前时间L	EQU	65H
	;运行速度TH0值	EQU	66H
	;运行速度TL0值	EQU	67H
	匀速运行长度H	EQU	68H
	匀速运行长度L	EQU	69H
当前长度H	 	EQU	6AH
当前长度L	 	EQU	6BH
当前速度H		EQU	6CH
当前速度L		EQU	6DH
	运行长度H	EQU	6EH
	运行长度L	EQU	6FH
	运行速度H	EQU	70H
	运行速度L	EQU	71H
	DY1	EQU	72H
	DY2	EQU	73H
	DY3	EQU	74H
	DGUS变量地址H	EQU	75H
	DGUS变量地址L	EQU	76H
	DGUS数据长度	EQU	77H
	发送首址	EQU	78H
	变量首址	EQU	79H
	发送变量个数	EQU	7AH
	发送个数	EQU	7BH
	显示画面	EQU	7CH
	接收计数	EQU	7DH
	串口2接收计数	EQU	7EH
	T0中断4次计数	EQU	7FH

	步数速度总系数	EQU	12H
;步数放大系数   	EQU	13H            
	当前步数H	EQU	14H
	当前步数L	EQU	15H
	加减速总步数H	EQU	16H
	加减速总步数L	EQU	17H
//   EQU	18H
 //   EQU	19H


	重复当前速度次数	EQU	1AH
	脉冲100个计数	EQU	1BH
	发送缓冲数据总数比较值	EQU	1CH
	发送缓冲数据个数	EQU	1DH
	发送缓冲计数	EQU	1EH
	定时1秒计时	EQU	1FH




//**开始****数值赋值*********
	掉电保存地址	DATA	030H
	掉电保存个数	DATA	02
	待存地址	DATA	2CH	                                         //028H	//	020H
	保存个数	DATA	64	                                          //		48	                                          //	56		//64      
//<15F2K60S2-0F000>
//	MOVC保存地址	DATA	0F000H
//15W4K48S4-0C000H	
	MOVC保存地址	DATA	0C000H
	IAP保存地址	DATA	0000H
	接收缓冲SRAM首址	DATA	0000H
	串口2接收缓冲SRAM首址	DATA	0050H
	发送缓冲SRAM首址	DATA	70H
	
	Y1Y2中心距离	DATA	3200	                                    //#3200	  //固定320MM//	X零位至V型块距离H
	检测提前距离	DATA	200	                                     //200/20MM
	归零速度	DATA	50
 ;检测速度     DATA			50	
	调试速度T0定时值	DATA	60009	                               //50MM/S
	正弦值等分数	DATA	100

	//**结束****地址数值赋值*********

//*****************************************************
	公司名称	DATA	00
	通讯画面	DATA	01
	等待画面	DATA	02
	参数修改1	DATA	03
	参数修改2	DATA	04
	参数修改3	DATA	05
	参数修改4	DATA	06
	参数修改5	DATA	07
	内部参数修改画面	DATA	08
	工件型号表	DATA	09
	调试画面1K	DATA	010
	调试画面2K	DATA	011
	调试画面3K	DATA	012
	调试准备型号画面	DATA	013
	调试准备参数画面	DATA	014
	调试准备画面	DATA	015
	调试物料取画面	DATA	016
	调试送放工件画面	DATA	017
	调试取放工件画面	DATA	018
//工作单步画面
	等待来料画面	DATA	020
	工件检测中画面	DATA	021
	未加工工件抓取画面	DATA	022
	等待加工完成画面	DATA	023
	加工工件抓取画面	DATA	024
	未加工工件放画面	DATA	025
	等待加工工件取走画面	DATA	026
	放加工工件画面	DATA	027
	回到零位画面	DATA	028    
 测试画面      DATA	030

//报警画面 40-49
	报警K1画面	DATA	040
	传感器图画面	DATA	041
	传感器表格1画面	DATA	042
	传感器表格2画面	DATA	043
	传感器表格3画面	DATA	044

//固定画面50-69
	版本号画面	DATA	050
	键盘背景画面	DATA	051
	键盘画面	DATA	052
	中文背景键盘	DATA	053
	中文键盘	DATA	054
	确定恢复成出厂画面	DATA	055
	数据初始化画面	DATA	056
	数据保存画面	DATA	057
	设备开机初始化	DATA	058
	低电压画面	DATA	060
	内部参数1	DATA	061
	内部参数2	DATA	062
	传感器报警画面	DATA	063
	传感器报警画面红	DATA	064
	提示语画面	DATA	65


////////////////////


	P4	DATA	0C0H
	P5	DATA	0C8H
	P6	DATA	0E8H
	P7	DATA	0F8H

	INT_CLKO	DATA	08FH	                                        ;	//外部中断与时钟输出控制寄存器
	数据寄存器	EQU	0C2H
	地址寄存器高字	EQU	0C3H
	地址寄存器低字	EQU	0C4H
	命令寄存器	EQU	0C5H
	命令触发寄存器	EQU	0C6H
	控制寄存器	EQU	0C7H	                                       //	控制寄存器		MOV20H复位



	ADC控制寄存器	EQU	0BCH	                                    ;ADC控制寄存器
	ADC高8位结果	EQU	0BDH	                                     ;ADC高8位结果
	ADC低2位结果	EQU	0BEH	                                     ;ADC低2位结果
	P1口第2功	EQU	09DH	                                        ;P1口第2功能控制寄存器

	ADC电源控制位	EQU	80H	                                     ;ADC电源控制位
	ADC完成标志	EQU	10H	                                       ;ADC完成标志
	ADC起始控制位	EQU	08H	                                     ;ADC起始控制位
	ADC时钟	EQU	00H	                                           ;00H/540个时钟;20H	/360个;40H	/180个;60H	/90个
	AUXR	DATA	08EH	                                            //辅助特殊功能寄存器
	S2CON	EQU	09AH	                                            //UART2	控制寄存器
	S2BUF	EQU	09BH	                                            //UART2	数据寄存器
	S2RI	EQU	01H	                                              //S2CON.0
	S2TI	EQU	02H	                                              //S2CON.1
	S2RB8	EQU	04H	                                             //S2CON.2
	S2TB8	EQU	08H	                                             //S2CON.3
	IE2	EQU	0AFH	                                              //中断控制寄存器2
	P_SW2	EQU	0BAH	                                            //外设功能切换寄存器2
	S2_S0	EQU	01H	                                             //P_SW2.0

	T2H	DATA	0D6H	                                             //定时器2高8位
	T2L	DATA	0D7H	                                             //定时器2低8位

	WAKECLKO	EQU	8FH
	P4SW	EQU	0BBH
	P_SW1	DATA	0A2H	                                           ;外设功能切换寄存器1
		;电源中断
	ELVD	BIT	IE.6	                                             //低压检测中断使能位
	LVDF	EQU	020H	                                             //PCON.5,低压检测中断标志位
	AUXR1	DATA	0A2H

;	WDT_CONTR	DATA	0C1H;看门狗
;	MOV			WDT_CONTR,	#23H				;使能看门狗,溢出时间约为0.5S
;	MOV	WDT_CONTR,	#24H					;使能看门狗,溢出时间约为1S
;	MOV	WDT_CONTR,	#25H		;使能看门狗,溢出时间约为2S
;	MOV	WDT_CONTR,	#26H					;使能看门狗,溢出时间约为4S
;	MOV	WDT_CONTR,	#27H			;使能看门狗,溢出时间约为8S
;	ORL	WDT_CONTR,	#10H					;清看门狗

/////////////////////////////////////////////////
	CCP_S0	EQU	10H	                                            ;P_SW1.3
	CCP_S1	EQU	20H	                                            ;P_SW1.5
	CCON	EQU	0D8H	                                             ;PCA控制寄存器
	CCF0	BIT	CCON.0	                                           ;PCA模块0中断标志
	CCF1	BIT	CCON.1	                                           ;PCA模块1中断标志
	CR	BIT	CCON.6	                                             ;PCA定时器运行控制位
	CF	BIT	CCON.7	                                             ;PCA定时器溢出标志
	CMOD	EQU	0D9H	                                             ;PCA模式寄存器
	CL	EQU	0E9H	                                               ;PCA定时器低字节
	CH	EQU	0F9H	                                               ;PCA定时器高字节
	CCAPM0	EQU	0DAH	                                           ;PCA模块0模式寄存器
	CCAP0L	EQU	0EAH	                                           ;PCA模块0捕获寄存器	LOW
	CCAP0H	EQU	0FAH	                                           ;PCA模块0捕获寄存器	HIGH
	CCAPM1	EQU	0DBH	                                           ;PCA模块1模式寄存器
	CCAP1L	EQU	0EBH	                                           ;PCA模块1捕获寄存器	LOW
	CCAP1H	EQU	0FBH	                                           ;PCA模块1捕获寄存器	HIGH
	CCAPM2	EQU	0DCH	                                           ;PCA模块2模式寄存器
	CCAP2L	EQU	0ECH	                                           ;PCA模块2捕获寄存器	LOW
	CCAP2H	EQU	0FCH	                                           ;PCA模块2捕获寄存器	HIGH
	PCA_PWM0	EQU	0F2H	                                         ;PCA模块0的PWM寄存器
	PCA_PWM1	EQU	0F3H	                                         ;PCA模块1的PWM寄存器
	PCA_PWM2	EQU	0F4H	                                         ;PCA模块2的PWM寄存器




	T4T3M	DATA	0D1H	                                           //定时器3和定时器4控制寄存器
	T4H	DATA	0D2H	                                             //定时器4高8位
	T4L	DATA	0D3H	                                             //定时器4低8位
	T3H	DATA	0D4H	                                             //定时器3高8位
	T3L	DATA	0D5H	                                             //定时器3低8位
///////////////////////////////////////

	ORG	0000H
	AJMP	开机
	ORG	0003H	                                                 //3.2急停开关
	LJMP	中断INT0
	ORG	000BH	                                                 //3.4
	LJMP	中断T0	                                               //步进电机脉冲用
;		ORG					0013H//3.3光栅信号
;		LJMP				中断INT1
	ORG	001BH	                                                 //3.5
	LJMP	中断T1
	ORG	023H
	LJMP	UART1串口中断
	ORG	0033H	                                                 //LVD中断入口
	LJMP	低电压LVD中断
	ORG	0043H
	LJMP	UART2串口中断
	;ORG	0053H
;	LJMP	中断INT2									//3.7
;	ORG	005BH
;	LJMP	中断INT3
	//ORG	063H
	//LJMP	中断T2								/串口中断用  57600
;	ORG	0083H
;	LJMP	中断INT4
	;ORG	009BH
;		LJMP	中断T3
;	ORG	00A3H
;		LJMP	中断T4
	ORG	0200H


开机:	MOV	SP, #090H
	LCALL	设置为准双向口
	MOV	IP, #01	                                               //中断INT0优先                                         ;
	MOV	P0, #00000000B
	MOV	P1, #11111111B
	MOV	P2, #00000000B
	MOV	P4, #11111001B
	MOV	P5, #00000000B
	LCALL	DY1S
	LCALL	读EEPROM
//LCALL	上电读0200H掉电保存到2E2F
//	JNB	急停开关	,	不读掉电保存
	//LCALL	读0200H掉电保存到2E2F   
 不读掉电保存:
	MOV	T0中断4次计数, #4
	MOV	20H, #0FFH
	MOV	21H, #0FFH
	MOV	22H, #0FFH
	MOV	23H, #0FFH
	MOV	24H, #0FFH
	MOV	25H, #0FFH
	MOV	26H, #0FFH
	MOV	27H, #0FFH
	MOV	28H, #0FFH
	MOV	29H, #0FFH
	MOV	2AH, #0FFH
	MOV	2BH, #0FFH
	//MOV	2CH,	#0FFH			保存标记06H	EQU	2CH
	//MOV	2DH,	#0FFH				保存标记06L
	MOV	2EH, #0FFH
	MOV	2FH, #0FFH
	LCALL	串口1初始化
//	LCALL	串口2初始化
	LCALL	DY10MS
	LCALL	发送20H至5FH到触摸屏
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	LCALL	发送版本号

	LCALL	急停开关检查
	LCALL	使能INT0中断
;	LCALL	低电压LVD中断使能
	//	LCALL	初始化CCPMPW
	LCALL	使能定时器T0
	LCALL	使能定时器T1

	CPL	指示灯
待机前准备:    
	MOV	脉冲100个计数	, #100
	MOV	DPTR, #型号中文显示TAB0
	LCALL	发送提示字符
	SETB	Y刹车
	MOV	发送首址	, #20H
	MOV	发送缓冲数据个数, #8H
	LCALL	指令数据串写入发送缓冲
待机检测:
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
检测:
开机参数复位:
	JB	参数初始化标记, 参数保存
	SETB	参数初始化标记
	LCALL	发送按扭标记到触摸屏
	LCALL	EEPROM初始化

参数保存:
	JB	参数保存标记	, 调试
	SETB	参数保存标记
	MOV	2AH, #0FFH
	MOV	2BH, #0FFH
	LCALL	发送按扭标记到触摸屏
	MOV	显示画面, #	数据保存画面
	LCALL	DGUS切换图片
	LCALL	读变量地址数据
	SETB	参数保存标记
	LCALL	写EEPROM
	LCALL	DY1S
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
调试:
	JB	调试H标记13H	, 是否运行检测
	LCALL	发送2627调试标记触摸屏
	MOV	显示画面, #调试准备型号画面	                           //传感器图
	LCALL	DGUS切换图片
	LCALL	调试准备

是否运行检测:
	JB	复位运行按钮标记, 检测
	SETB	复位运行按钮标记
	LCALL	发送按扭标记到触摸屏
 ;********************************** 
  	MOV	显示画面, # 测试画面    
	LCALL	DGUS切换图片     
编写测试1:
	//CPL	X轴方向  
MOV 当前时间H, #00H        
MOV 当前时间L, #00H  
LCALL	使能定时器T1
	MOV	运行长度H	, Y向中间抓V距离H	                          //100:2000
	MOV	运行长度L	, Y向中间抓V距离L
	MOV	运行速度H	, 上下运行速度H	                        
	MOV	运行速度L	, 上下运行速度L
	LCALL	计算步数速度总系数  
	LCALL	计算加减速总步数
	LCALL	匀速长度速度计算赋值  
MOV			P_SW1   ,#00H 		 ; 选择DPTR0
MOV	DPTR,#00H
MOV			P_SW1    ,#01H 		  ; 选择DPTR1  
	CLR	脉冲输出总开关标记 
	CPL	X轴方向
	CLR	X脉冲输出标记
LCALL	计数S型加速XMM 
LCALL	计数S型匀速运行
LCALL	计数S型减速XMM
	SETB	X脉冲输出标记
	SETB	脉冲输出总开关标记  
	LCALL	DY1S
	JB	复位运行按钮标记, 编写测试1
	SETB	复位运行按钮标记
	LCALL	发送按扭标记到触摸屏
	LJMP	待机检测	                                         //
 ;*******************************************88888
//是否运行检测:
//	JB	复位运行按钮标记, 检测
//	SETB	复位运行按钮标记
//	LCALL	发送按扭标记到触摸屏
运行前准备:
	SETB	Y刹车
	LCALL	每段长度计算
	LCALL	发送长度到触摸屏
	LCALL			机械手归零 
	CLR	脉冲输出总开关标记
	LCALL	X轴A180固定距离运行
运行程序:

等待来料:
	MOV	显示画面, #等待来料画面
	LCALL	DGUS切换图片
	LCALL	DY1S
     
工件检测中:
	MOV	显示画面, #工件检测中画面
	LCALL	DGUS切换图片
	LCALL	X轴E80运行
	LCALL	DY1S
	LCALL	工件F20端面检测直接运行
	LCALL	DY1S
未加工工件抓取:
	MOV	显示画面, #未加工工件抓取画面
	LCALL	DGUS切换图片
	LCALL	X轴F70抓铁芯右运行
	LCALL	DY1S
 
	SETB	Y1轴右方向
	LCALL	Y1轴二头运行
	SETB	Y1轴右吸爪
	LCALL	DY100MS
	CLR	Y1轴右方向
	LCALL	Y1轴二头运行
 

等待加工完成:
	MOV	显示画面, #等待加工完成画面
	LCALL	DGUS切换图片
	LCALL	X轴H180计算长度运行
	LCALL	DY1S


加工工件抓取:
	MOV	显示画面, #加工工件抓取画面
	LCALL	DGUS切换图片
 
	LCALL	DY1S
  
	SETB	Y2轴左方向
	LCALL	Y2轴中间运行
	SETB	Y2轴左吸爪
	LCALL	DY100MS
	CLR	Y2轴左方向
	LCALL	Y2轴中间运行

 

未加工工件放:
	MOV	显示画面, #未加工工件放画面
	LCALL	DGUS切换图片
	LCALL	X轴B320运行
	LCALL	DY1S
      
	SETB	Y1轴右方向
	LCALL	Y1轴中间运行
	CLR	Y1轴右吸爪
	LCALL	DY100MS
	CLR	Y1轴右方向
	LCALL	Y1轴中间运行
	   

等待加工工件取走:
	MOV	显示画面, #等待加工工件取走画面
	LCALL	DGUS切换图片
	LCALL	DY1S


放加工工件:
	MOV	显示画面, #放加工工件画面
	LCALL	DGUS切换图片

	LCALL	X轴C260运行
	LCALL	DY1S
 
	SETB	Y2轴左方向
	LCALL	Y2轴二头运行
	CLR	Y2轴左吸爪
	LCALL	DY100MS
	CLR	Y2轴左方向
	LCALL	Y2轴二头运行
 
回到零位:
	MOV	显示画面, #回到零位画面
	LCALL	DGUS切换图片
	LCALL	X轴D580运行
	LCALL	DY1S
	LJMP	待机检测	                                             //运行程序

 ;****************************************8888888

// ****************************************************88888*****************



调试准备:
手动右边抓取位置调试A:
	JB	接收数据读完标记, 手动右边抓取位置调试A
	SETB	接收数据读完标记
	LCALL	右边X抓取位置调试
	LCALL	右边Y1抓取位置调试
	JNB	下一项开关标记	, 手动右边抓取位置调试A
	MOV	Y向两头取放距离H, 上下Y方向检测距离H
	MOV	Y向两头取放距离L, 上下Y方向检测距离L
	MOV	X向右零位H, 左右X方向检测距离H
	MOV	X向右零位L, 左右X方向检测距离L
	MOV	显示画面, #调试物料取画面
	LCALL	DGUS切换图片

手动中间加工位置调试B:
	;调试送放工件画面 
	
手动左边放位置调试C:
;调试取放工件画面 


//************右边调试**X****************************
右边X抓取位置调试:
	MOV	R4, 左右X方向检测距离H
	MOV	R3, 左右X方向检测距离L
	MOV	R2, 左右X方向上一次检测距离H
	MOV	R1, 左右X方向上一次检测距离L
	LCALL	十六减十六正负标记
	JNB	数值为零标记	, 左右X方向调试退出
	MOV	当前长度H, R2
	MOV	当前长度L, R1
	JNB	数值正负标记	, 左右X方向调试右移
左右X方向调试左移:
	CLR	X轴方向
	CLR	X脉冲输出标记
	LCALL	调试慢速计数运行
	MOV	左右X方向上一次检测距离H	, 左右X方向检测距离H
	MOV	左右X方向上一次检测距离L	, 左右X方向检测距离L
	RET
左右X方向调试右移:
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, 当前长度H
	MOV	R1, 当前长度L
 ;R4R3-R2R1=R3R2R1
	LCALL	十六减十六正负标记
	MOV	当前长度H, R2
	MOV	当前长度L, R1
左右X方向调试右移2:
	SETB	X轴方向
	CLR	X脉冲输出标记
	LCALL	调试慢速计数运行
	MOV	左右X方向上一次检测距离H	, 左右X方向检测距离H
	MOV	左右X方向上一次检测距离L	, 左右X方向检测距离L
	RET
左右X方向调试退出:
	RET

//************右边调试**Y1****************************
右边Y1抓取位置调试:
	MOV	R4, 上下Y方向检测距离H
	MOV	R3, 上下Y方向检测距离L
	MOV	R2, 上下Y方向上一次检测距离H
	MOV	R1, 上下Y方向上一次检测距离L
	LCALL	十六减十六正负标记
	JNB	数值为零标记	, 上下Y方向调试退出
	MOV	当前长度H, R2
	MOV	当前长度L, R1
	JNB	数值正负标记	, 上下Y方向调试上移
上下Y方向调试下移:
	SETB	Y1轴右方向
	CLR	Y1脉冲输出标记
	LCALL	调试慢速计数运行
	MOV	上下Y方向上一次检测距离H	, 上下Y方向检测距离H
	MOV	上下Y方向上一次检测距离L	, 上下Y方向检测距离L
	RET
上下Y方向调试上移:
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, 当前长度H
	MOV	R1, 当前长度L
 ;R4R3-R2R1=R3R2R1
	LCALL	十六减十六正负标记
	MOV	当前长度H, R2
	MOV	当前长度L, R1
上下Y方向调试上移2:

	CLR	Y1轴右方向
	CLR	Y1脉冲输出标记
	LCALL	调试慢速计数运行
	MOV	上下Y方向上一次检测距离H	, 上下Y方向检测距离H
	MOV	上下Y方向上一次检测距离L	, 上下Y方向检测距离L
	RET
上下Y方向调试退出:
	RET
//*****************************************

//*****************************************

   

//***调试慢速计数运行*XY方向共用*******************8888
//CLR			X方向    
//CLR	X脉冲输出标记       
调试慢速计数运行:
	//MOV	当前长度H, 匀速运行长度H
	//MOV	当前长度L, 匀速运行长度L
//	MOV	当前速度H	, 低速H
//	MOV	当前速度L	, 低速L
	//LCALL	计算速度转定时器并赋值     
	MOV	TH0, #调试速度T0定时值	                                ;设置定时初值
	MOV	TL0, #调试速度T0定时值	                                ;设置定时初值
	CLR	脉冲输出总开关标记
调试慢速计数运行动作:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 调试慢速计数运行动作
	SETB	长度减一标记
//	SETB	脉冲输出总开关标记 
	RET
  //*****************************************************   


单独参数保存检测:
	JB	单独参数保存标记	, 无保存
	SETB	单独参数保存标记
//LCALL	发送20H至5FH到变量地址
	LCALL	读变量地址数据
	LCALL	DY1S
	LCALL	写EEPROM
	SETB	参数保存标记
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	LCALL	发送20H至5FH到触摸屏
无保存:	RET
//***ABCDEF************************  



//////////////////////////////

X轴A180固定距离运行:
	MOV	运行长度H	, A180长度H
	MOV	运行长度L	, A180长度H
	SETB	X轴方向
	LCALL	X轴方向运行
	RET




X轴B320运行:
	MOV	运行长度H, #HIGH	Y1Y2中心距离	                         ;#320
	MOV	运行长度L, #LOW	Y1Y2中心距离
	SETB	X轴方向
	LCALL	X轴方向运行
	RET
  

X轴C260运行:
	MOV	运行长度H	, C260长度H
	MOV	运行长度L	, C260长度L
	SETB	X轴方向
	LCALL	X轴方向运行
	RET
 
X轴D580运行:
	MOV	运行长度H	, D580长度H
	MOV	运行长度L	, D580长度L
	CLR	X轴方向
	LCALL	X轴方向运行
	RET
  
X轴E80运行:
	MOV	运行长度H	, E80长度H
	MOV	运行长度L	, E80长度L
	CLR	X轴方向
	LCALL	X轴方向运行
	RET


工件F20端面检测直接运行:
	MOV	运行长度H, #HIGH	检测提前距离
	MOV	运行长度L, #LOW	检测提前距离
	MOV	端面检测距离H	, #HIGH	检测提前距离
	MOV	端面检测距离L	, #LOW	检测提前距离
	CLR	X轴方向
	LCALL	X轴方向运行
	RET

 
X轴F70抓铁芯右运行:
	MOV	运行长度H, 工件端面至中心距离H
	MOV	运行长度L, 工件端面至中心距离L
	CLR	X轴方向
	LCALL	X轴方向运行
	RET

X轴H180计算长度运行:
	MOV	运行长度H, H180长度H
	MOV	运行长度L, H180长度L
	SETB	X轴方向
	LCALL	X轴方向运行
	RET
  /*
		A180长度H 
	B320长度H
	C260长度H	 
	D580长度H 
	E80长度H  
H180长度计算

 */


X全程左运行:
	SETB	X轴方向
	LCALL	右零位至左零位运行
	RET
X全程右运行:
	CLR	X轴方向
	LCALL	右零位至左零位运行
	RET
右零位至左零位运行:
	MOV	运行长度H, X向右零位至左零位距离H
	MOV	运行长度L	, X向右零位至左零位距离H
	LCALL	X轴方向运行
	RET


//*****长度计算***** 
每段长度计算:
	LCALL	A180长度计算
	LCALL	C260长度计算
	LCALL	D580长度计算
	LCALL	E80长度计算
	LCALL	H180长度计算

    ;   RET

B320长度计算:
	MOV	B320长度H, #HIGH	Y1Y2中心距离
	MOV	B320长度L, #LOW	Y1Y2中心距离
F20长度计算:
	MOV	F20H	, #HIGH	检测提前距离
	MOV	F20L	, #LOW	检测提前距离
G7长度0计算:
    // 工件端面至中心距离H
			//  工件端面至中心距离H
	RET

A180长度计算:
	MOV	R4, X向右零位至中心距离H
	MOV	R3, X向右零位至中心距离L
	MOV	R2, #HIGH	Y1Y2中心距离	                                ;#320
	MOV	R1, #LOW	Y1Y2中心距离
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	A180长度H, R2
	MOV	A180长度L	, R1
	RET

C260长度计算:
	MOV	R4, X向右零位至左零位距离H
	MOV	R3, X向右零位至左零位距离L
	MOV	R2, X向右零位至中心距离H
	MOV	R1, X向右零位至中心距离L
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	C260长度H	, R2
	MOV	C260长度L	, R1
	RET

D580长度计算:
	MOV	R4, C260长度H
	MOV	R3, C260长度L
	MOV	R2, #HIGH	Y1Y2中心距离	                                ;#320
	MOV	R1, #LOW	Y1Y2中心距离
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	D580长度H	, R2
	MOV	D580长度L	, R1
	RET

E80长度计算:
	MOV	R4, A180长度H
	MOV	R3, A180长度L
	MOV	R2, 工件端面至中心距离H
	MOV	R1, 工件端面至中心距离L
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	A, R2
	MOV	R4	, A
	MOV	A, R1
	MOV	R3	, A
	MOV	R2, #HIGH	检测提前距离
	MOV	R1, #LOW	检测提前距离
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	E80长度H	, R2
	MOV	E80长度L	, R1
	RET

H180长度计算:

	MOV	端面检测距离H	, #HIGH	检测提前距离
	MOV	端面检测距离L	, #LOW	检测提前距离

	MOV	R4, 工件端面至中心距离H
	MOV	R3, 工件端面至中心距离L
	MOV	R2, 端面检测距离H
	MOV	R1, 端面检测距离L
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	R4, E80长度H
	MOV	R3, E80长度L
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	H180长度H	, R2
	MOV	H180长度L	, R1
	RET



工件F20端面检测:
	SETB	X轴方向
	CLR	X脉冲输出标记
	MOV	DPTR, #00H
//用	检测速度  《X轴D580加右运行》运行长度H需重新计算 编写
	MOV	运行速度H	, 低速H	                                     //检测速度H	                          
	MOV	运行速度L	, 低速L	                                     //检测速度L
	LCALL	计算速度转定时器并赋值
工件端面检测中:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
	INC	DPTR
	JB	Y1右工件检测, 工件端面检测中
	SETB	X脉冲输出标记
	MOV	端面检测距离H	, DPH
	MOV	端面检测距离L	, DPL
	RET


X轴方向运行:
	MOV	运行速度H	, 左右运行速度H
	MOV	运行速度L	, 左右运行速度L
	CLR	X脉冲输出标记
	LCALL	电机加减速运行
	SETB	X脉冲输出标记
	RET
	//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 //* **********Y************************************
//	工件铁芯直径H	 
	//工件铁芯直径L	 
 
Y1轴二头运行:
	MOV	运行长度H	, Y向两头取放距离H	                          //100:2000
	MOV	运行长度L	, Y向两头取放距离L
	MOV	运行速度H	, 上下运行速度H	                             //#HIGH	400
	MOV	运行速度L	, 上下运行速度L	                             //#LOW	400
	CLR	Y1脉冲输出标记
	LCALL	电机加减速运行
	SETB	Y1脉冲输出标记
	RET

Y2轴二头运行:
	MOV	运行长度H	, Y向两头取放距离H	                          //100:2000
	MOV	运行长度L	, Y向两头取放距离L
	MOV	运行速度H	, 上下运行速度H	                             //#HIGH	400
	MOV	运行速度L	, 上下运行速度L	                             //#LOW	400
	CLR	Y2脉冲输出标记
	LCALL	电机加减速运行
	SETB	Y2脉冲输出标记
	RET


Y1轴中间运行:
	MOV	运行长度H	, Y向中间抓V距离H	                           //100:2000
	MOV	运行长度L	, Y向中间抓V距离L
	MOV	运行速度H	, 上下运行速度H	                             //#HIGH	400
	MOV	运行速度L	, 上下运行速度L	                             //#LOW	400
	CLR	Y1脉冲输出标记
	LCALL	电机加减速运行
	SETB	Y1脉冲输出标记
	RET

Y2轴中间运行:
	MOV	运行长度H	, Y向中间抓V距离H	                           //100:2000
	MOV	运行长度L	, Y向中间抓V距离L
	MOV	运行速度H	, 上下运行速度H	                             //#HIGH	400
	MOV	运行速度L	, 上下运行速度L	                             //#LOW	400
	
	CLR	Y2脉冲输出标记
	LCALL	电机加减速运行
	SETB	Y2脉冲输出标记
	RET

            

//////X方向Y1Y2归零	//////
机械手归零:

	MOV	DPTR, #垂直方向右归零TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片
	MOV	当前数值L	, #02
	MOV	发送首址, #当前数值L
	LCALL	发送单个数据到触摸屏
	LCALL	DY500MS
	
	LCALL	Y1方向归零

	MOV	当前数值L	, #04
	MOV	发送首址, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #垂直方向左归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	Y2方向归零

	MOV	当前数值L	, #06
	MOV	发送首址, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #水平方向归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	X方向归零

	MOV	当前数值L	, #09
	MOV	发送首址, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #开机复位完成TAB
	LCALL	发送提示字符
	LCALL	DY1S

	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET

//////X方向Y1Y2归零	//////
X方向归零:
	CLR	脉冲输出总开关标记
	JNB	X右限位	, X方向精细归零
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算速度转定时器并赋值 
X方向继续归零:      
	CLR	X轴方向
	CLR	X脉冲输出标记
	JB	X右限位	, $
	SETB	X脉冲输出标记
	//RET
X方向精细归零:
	SETB	X轴方向
	CLR	X脉冲输出标记
	MOV	匀速运行长度H	, #HIGH	200
	MOV	匀速运行长度L	, #LOW	200
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计数匀速运行
	CLR	X脉冲输出标记
	LCALL	DY100MS
	CLR	X轴方向
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算速度转定时器并赋值
X方向继续精细归零:
	CLR	X脉冲输出标记
	JB	X右限位	, $
	SETB	X脉冲输出标记
	CLR	脉冲输出总开关标记
	RET



Y1方向归零:
	SETB	脉冲输出总开关标记
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算速度转定时器并赋值
	CLR	脉冲输出总开关标记

Y1归零到位检查:
	CLR	Y1轴右方向
	CLR	Y1脉冲输出标记
	JB	Y1右零位	, Y1归零到位检查
	SETB	Y1脉冲输出标记
	RET


Y2方向归零:
	SETB 	脉冲输出总开关标记
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算速度转定时器并赋值
	CLR	脉冲输出总开关标记
Y2归零到位检查:
	CLR	Y2轴左方向
	CLR	Y2脉冲输出标记
	JB	Y2左零位	, Y2归零到位检查
	SETB	Y2脉冲输出标记
	SETB	脉冲输出总开关标记
	RET

//////X方向Y1Y2归零	//////

   
电机加减速运行:    
	LCALL	计算步数速度总系数
	LCALL	计算加减速总步数 
	LCALL	匀速长度速度计算赋值
	CLR	脉冲输出总开关标记
	LCALL	计数S型加速XMM
	LCALL	计数匀速运行
	LCALL	计数S型减速XMM
	SETB	脉冲输出总开关标记
	RET
 
 

	//	1.计数匀速运行 = 运行长度-  计数加速XMM- 计数减速XMM 
	//	2.计数加速XMM = 计数减速XMM= 运行速度-低速     
	//	3. 注意 ;  运行长度小于 加速步数.减速步数的和时  ，匀速运行步数为负数造成错误 
	//4.      运行速度设置为  运行长度的1/2 ,匀速运行长度 =  低速#2
匀速长度速度计算赋值:
;	MOV	R4, 运行速度H
	;MOV	R3, 运行速度L
	;MOV	R2, 低速H
	;MOV	R1, 低速L    
;R4R3-R2,R1=R3R2R1
	;LCALL	十六减十六
	;MOV	A	, R2
	;MOV	R4, A
	;MOV	A	, R1
	;MOV	R3, A   

	MOV	R4,加减速总步数H 
	MOV	R3,加减速总步数L 
	MOV	R2	, #2
;；				R4R3*R2	=R7R6R5
	LCALL	十六乘八
	MOV	R4, 运行长度H
	MOV	R3, 运行长度L
	MOV	A	, R6
	MOV	R2, A
	MOV	A	, R5
	MOV	R1, A
//R4R3-R2,R1=R3R2R1
	LCALL	十六减十六正负标记
	MOV	匀速运行长度H, R2
	MOV	匀速运行长度L, R1
	JNB	数值正负标记	, 匀速运行步数为负数
	JNB	数值为零标记	, 匀速运行步数为负数
	RET
匀速运行步数为负数:
//运行长度太小错误显示    
	;MOV	DPTR, #运行长度错误TAB
;	LCALL	发送提示字符
;	MOV	显示画面, #提示语画面
	;LCALL	DGUS切换图片    
//运行速度设置为  运行长度的1/2 ,匀速运行长度 =  低速#2
运行速度自动计算赋值:
	MOV	R5, #00H
	MOV	R4, #00H
	MOV	R3, 运行长度H
	MOV	R2, 运行长度L
//除数R7,R6
	MOV	R7, #00
	MOV	R6, #3                                               //  	速度设置为  运行长度的1/2
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4
	LCALL	三十二除十六
	MOV	运行速度H	, R3
	MOV	运行速度L	, R2
		//LCALL	DY1S
	//MOV	显示画面, #等待画面
	//LCALL	DGUS切换图片 
	MOV	   步数放大系数  ,#10
	LCALL	计算步数速度总系数
	LCALL	计算加减速总步数 
	LJMP	匀速长度速度计算赋值
	RET
计数匀速运行:
;;	LCALL	匀速运行长度计数
	MOV	当前长度H, 匀速运行长度H
	MOV	当前长度L, 匀速运行长度L
	MOV	当前速度H	, 运行速度H
	MOV	当前速度L	, 运行速度L
	LCALL	计算速度转定时器并赋值
运行长度动作:	                                              ///
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 运行长度动作
	SETB	长度减一标记
	RET




  //*********S型加减速开始******************* 
	MOV	步数放大系数	, #30
	MOV	运行速度H	, #HIGH	500
	MOV	运行速度L	, #LOW	500
	LCALL	计算步数速度总系数
	LCALL	计算加减速总步数     

计数S型加速XMM:
	MOV	当前步数H, #00
	MOV	当前步数L, #00   ;20
计数S型加速XMM中:
	LCALL	当前加速步数加一
	LCALL	计算S型当前速度并赋值
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记
	JB	当前速度为零标记	, 计数S型加速XMM中
	SETB	当前速度为零标记
	RET
计数S型匀速运行:
;;	LCALL	匀速运行长度计数
	MOV	当前长度H, 匀速运行长度H
	MOV	当前长度L, 匀速运行长度L
;	MOV	当前速度H	, 运行速度H
;	MOV	当前速度L	, 运行速度L
	;LCALL	计算速度转定时器并赋值
运行S型长度动作中:	                                              ///
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 运行S型长度动作中
	SETB	长度减一标记
	RET

计数S型减速XMM:
	MOV	当前步数H, 加减速总步数H
	MOV	当前步数L, 加减速总步数L
计数S型减速XMM中:
	LCALL	计算S型当前速度并赋值
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记 
	LCALL	当前减速步数减一
	JB	当前速度为零标记	, 计数S型减速XMM中
	SETB	当前速度为零标记
	RET



  //*********S型加减速子函数******************* 
计算S型当前速度并赋值:
;1.当前步数速度=当前步数/步数速度总系数<当前（最高）速度/100(正弦值等分数)* 步数放大系数 >
	 //21*10	/(400/100)*20
	
	MOV	R4, 当前步数H
	MOV	R3, 当前步数L
	MOV	R2, #10
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八

	MOV	R5, #0
//MOV R4,         ; 被除数高8位 
//MOV R3,         ; 被除数中8位 
//MOV R2,         ; 被除数低8位  

MOV	R7, #0
MOV	R6, 步数速度总系数
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4 
LCALL	三十二除十六   
	 //////////////////////////////////////////////////////////////////
;被除数R3.R2.	除数R1				商R3,R2	余数R1   
//MOV R4,   #0      ; 被除数高8位
//MOV	R1, 步数速度总系数
//LCALL	十六除八错误

 //////////////////////////////////////////

	MOV	A	, R2
	;查表   

	MOV	DPTR	, #正弦值TAB
	MOVC	A, @A+DPTR
  ;276320*100/运行速度/正弦值   
;=276320*100/(运行速度*正弦值)
	MOV	R4, 运行速度H
	MOV	R3, 运行速度L
	MOV	R2, A
//	R4R3*R2	=R4R3R2<R7R6R5> 
;	因速度655*100=FFFF下面使用函数三十二除十六除数不能大于16位
// 所以速度不能大于655
		LCALL	十六乘八
;276320*100/(运行速度*正弦值)
//除数R7,R6
	MOV	A, R3
	MOV	R7, A

	MOV	A, R2
	MOV	R6, A

;1A5 A180  = 276320*100
	MOV	R5, #01H
	MOV	R4, #0A5H
	MOV	R3, #0A1H
	MOV	R2, #80H
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4 
	LCALL	三十二除十六

	MOV	A	, R3
	MOV	B	, R2
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, A
	MOV	R1, B
;R4R3-R2,R1=R3R2R1
	LCALL	十六减十六
	MOV	TH0, R2	                                               ;设置定时初值
	MOV	TL0, R1	                                               ;设置定时初值
	RET


计算加减速总步数:
	MOV	R4, 运行速度H	                                         ;  最高速度
	MOV	R3, 运行速度L
	MOV	R2, 步数放大系数
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八
	MOV	R5, #00	      
	;R4R3R2
	MOV	R7, #0      ; 除数
	MOV	R6, #10
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4   
   ;500/100*10
	LCALL	三十二除十六
	MOV	加减速总步数H, R3
	MOV	加减速总步数L, R2
	RET



计算步数速度总系数:
	;MOV	 运行速度H	, #HIGH		450;  最高速度
	;MOV		 运行速度L, #LOW			450  	
 ;MOV  步数放大系数 , #20   

;步数速度总系数=当前（最高）速度/100(正弦值等分数)* 步数放大系数   
	;= 400MM/S除100乘1.2     
	;= 400MM/S乘12 除100  = 48 (多10倍
	MOV	R4, 运行速度H	                                         ;  最高速度
	MOV	R3, 运行速度L
	MOV	R2, 步数放大系数
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八
	MOV	R5, #00H
	;MOV	R4, #00H
	;MOV	R3,  
	;MOV	R2,  
	MOV	R7, #HIGH	正弦值等分数
	MOV	R6, #LOW	正弦值等分数
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4   
   ;500/100*10
	LCALL	三十二除十六
;	MOV	 		X,R3
	MOV	步数速度总系数, R2
	RET   


正弦值TAB:

				DB		05H, 05H, 06H, 07H, 08H, 09H, 0AH, 0BH, 0CH,	0CH
    DB  0CH,	0DH,	0DH,	0EH,	0EH,	0FH,	010H,010H,011H,012H
    DB  013H,013H,014H,015H,016H,017H,018H,019H,01AH,01BH
    DB  01DH,01EH,01FH,020H,021H,023H,024H,025H,026H,028H
    DB  029H,02AH,02CH,02DH,02FH,030H,031H,033H,034H,036H
    DB  037H,038H,03AH,03BH,03DH,03EH,03FH,041H,042H,044H
    DB  045H,046H,048H,049H,04AH,04BH,04DH,04EH,04FH,050H
    DB  051H,053H,054H,055H,056H,057H,058H,059H,05AH,05BH
    DB  05BH,05CH,05DH,05EH,05EH,05FH,060H,060H,061H,061H
    DB  062H,062H,063H,063H,063H,063H,064H,064H,064H,064H
    DB  064H,064H,064H,064H,064H,063H,063H,063H,063H,062H 	  

当前加速步数加一:
//*****加一*****
	MOV	R1, 当前步数L
	MOV	R2, 当前步数H
;R2,R1+1=R3R2R1
	LCALL	十六加一
	MOV	当前步数L, R1
	MOV	当前步数H, R2
//*****比较****
	MOV	A, 当前步数H
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 加减速总步数H, 加速步数比较H结果不相同
加速步数比较H结果相同:
	MOV	A, 当前步数L
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 加减速总步数L, 加速步数比较L结果不相同
加速步数比较L结果相同:
	CLR	当前步数为零标记
	RET
加速步数比较H结果不相同:
加速步数比较L结果不相同:
	RET


当前减速步数减一:
//*****减一*****
	MOV	R1, 当前步数L
	MOV	R2, 当前步数H
	LCALL	十六减一
	MOV	当前步数L, R1
	MOV	当前步数H, R2
//*****比较****
	MOV	A, 当前步数H
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #00, 减速步数比较H结果不相同
减速步数比较H结果相同:
	MOV	A, 当前步数L
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #00, 减速步数比较L结果不相同       //	20
减速步数比较L结果相同:
	CLR	当前步数为零标记
	CLR	当前速度为零标记
	RET
减速步数比较H结果不相同:
减速步数比较L结果不相同:
//SETB		当前速度为零标记
	RET

//*********S型加减速结束******************* 
 


当前加速速度加一:
//*****加一*****
	MOV	R1, 当前速度L
	MOV	R2, 当前速度H
;R2,R1+1=R3R2R1
	LCALL	十六加一
	MOV	当前速度L, R1
	MOV	当前速度H, R2
//*****比较****
	MOV	A, 当前速度H
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 运行速度H, 加速比较H结果不相同
加速比较H结果相同:
	MOV	A, 当前速度L
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 运行速度L, 加速比较L结果不相同
加速比较L结果相同:
	CLR	当前速度为零标记
	RET
加速比较H结果不相同:
加速比较L结果不相同:
	RET



当前减速速度减一:
//*****减一*****
	MOV	R1, 当前速度L
	MOV	R2, 当前速度H
	LCALL	十六减一
	MOV	当前速度L, R1
	MOV	当前速度H, R2
//*****比较****
	MOV	A, 当前速度H
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 低速H, 减速比较H结果不相同
减速比较H结果相同:
	MOV	A, 当前速度L
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 低速L, 减速比较L结果不相同
减速比较L结果相同:
	CLR	当前速度为零标记
	RET
减速比较H结果不相同:
减速比较L结果不相同:
//SETB		当前速度为零标记
	RET





计算速度转定时器并赋值:
	;细分2000/转,5M*20齿=100MM/转,	脉冲数=2000
	;运行速度10MM/S=>200脉冲数/S=>定时值10/20/2(高低电平）=2500US
	;定时器赋值				65536-2500*11.0592=65536-276480
	;计算运行速度30MM/S时	=>65536-(276480/30)
//A,MOV					当前速度	,运行速度		(3CM/S)
//B,赋值	65536-27648/速度(1CM/S)
//C,赋值	FFFF-	(276480(43800H)/速度)
	MOV	R5, #00H
	MOV	R4, #04H
	MOV	R3, #38H
	MOV	R2, #00H
//除数R7,R6
	MOV	R7, 当前速度H
	MOV	R6, 当前速度L
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4
	LCALL	三十二除十六
	MOV	A	, R3	                                                ;设置定时初值
	MOV	B, R2	                                                 ;设置定时初值
	
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, A
	MOV	R1, B
;R4R3-R2,R1=R3R2R1
	LCALL	十六减十六
	MOV	TH0, R2	                                               ;设置定时初值
	MOV	TL0, R1	                                               ;设置定时初值

	RET



长度减一:
//*****减一*****
	MOV	R1, 当前长度L
	MOV	R2, 当前长度H
	LCALL	十六减一
	MOV	当前长度L, R1
	MOV	当前长度H, R2
//*****比较****
	MOV	A, 当前长度H
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #0, 比较H结果不相同
比较H结果相同:
	MOV	A, 当前长度L
;A=(值)顺序执行<CY=0>	，A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #0, 比较L结果不相同
比较L结果相同:
	CLR	长度减一标记
	RET
比较H结果不相同:
比较L结果不相同:
	RET



/*	X轴方向	BIT	P0.0																																											//DRV
	X轴脉冲	BIT	P0.1																																											//PUL
	X轴使能	BIT	P0.2																																											//ENB
	Y轴右方向	BIT	P0.3																																									//DRV
	Y轴右脉冲	BIT	P0.4																																									//PUL
	Y轴左方向	BIT	P0.5																																									//DRV
	Y轴左脉冲	BIT	P0.6
	*/

EEPROM初始化:
	MOV	显示画面, #数据初始化画面
	LCALL	DGUS切换图片
	LCALL	DY500MS
;	0010
	//MOV	付板返回通讯数据0010H	 ,#HIGH	00 ;	20H
//	MOV	付板返回信号标记0010L	 ,#HIGH	00 ;	21H	                             //付板P1口OUT标记
;0011
	MOV	传感器信号标记H	, #HIGH	0FFFFH	                        ;	22H
	MOV	传感器信号标记L	, #LOW	0FFFFH	                         ;		23H	                                   //	主板P1口OUT标记
;0012
	MOV	当前数值H	, #HIGH	0FFFFH	                              ;	24H
	MOV	当前数值L	, #LOW	0FFFFH	                               ;	25H
;0013
	MOV	调试H标记13H	, #HIGH	0FFFFH	                           ;	26H
	MOV	脉冲L输出标记13L	, #LOW	0FFFFH	                        ;	27H
;0014
	MOV	临时标记14H	, #HIGH	0FFFFH	                            ;	28H
	MOV	临时标记14L	, #LOW	0FFFFH	                             ;	29H
;0015
	MOV	按扭标记15H	, #HIGH	0FFFFH	                            ;	2AH
	MOV	按扭标记15L	, #LOW	0FFFFH	                             ;	2BH   

;0016
	MOV	保存标记H	, #HIGH	0FFFFH	                              ;	2CH	                                         //			开机不置1
	MOV	保存标记L	, #LOW	0FFFFH	                               ;	2DH
;0017
	//	 ,#HIGH	00 ;	2EH
	//	 ,#LOW	00 ;	2FH

/*	----C260-(--------B320------<.....A180........
------------D580------- -------E100-->...F70...>   */
//	X1=A+B			X2=A+B+C		 
    
	//	以下保存参数
;0018     Y3
	MOV	Y向两头取放距离H	, #HIGH	1000	                          ;	30H     	
	MOV	Y向两头取放距离L	, #LOW	1000	                           ;	31H
;0019     Y4	
	MOV	Y向中间抓V距离H	, #HIGH	4000	                          ;	32H
	MOV	Y向中间抓V距离L	, #LOW	4000	                           ;	33H
;001A     X1   	
	MOV	X向右零位至中心距离H	, #HIGH	5000	                     ;	34H	                                            
	MOV	X向右零位至中心距离L	, #LOW	5000	                      ;	35H	                                        
;001B     X2 	
	MOV	X向右零位至左零位距离H	, #HIGH	7600	                   ;	36H       
	MOV	X向右零位至左零位距离L	, #LOW	7600	                    ;	37H         	
;001C
	MOV	左右运行速度H	, #HIGH	400	                             ;	38H
	MOV	左右运行速度L	, #LOW	400	                              ;	39H
;001D
	MOV	上下运行速度H	, #HIGH	450	                             ;	3AH
	MOV	上下运行速度L	, #LOW	450	                              ;	3BH
;001E
	MOV	低速H	, #HIGH	20	                                      ;	3CH
	MOV	低速L	, #LOW	20	                                       ;	3DH
;001F
	MOV	检测速度H	, #HIGH	40	                                  ;	3EH
	MOV	检测速度L	, #LOW	40	                                   ;	3FH
;0020
	MOV	工件端面至中心距离H	, #HIGH	700	                       ;	40H
	MOV	工件端面至中心距离L	, #LOW	700	                        ;		41H
;0021
	MOV	工件铁芯直径H	, #HIGH	400	                             ;	42H
	MOV	工件铁芯直径L	, #LOW	400	                              ;	43H
;0022
	MOV	型号H	, #2	                                            ;	44H
	MOV	型号L	, #2	                                            ;	45H
	;0023
	MOV	备用24H	, #HIGH	00	                                    ;	46H
	MOV	备用24L	, #LOW	00	                                     ;	47H 
;0024	
/*
MOV		左右X方向检测距离H	 ,#HIGH	00 ;	48H
MOV		左右X方向检测距离L	 ,#LOW	00 ;	49H
;0025
MOV		上下Y方向检测距离H	 ,#HIGH	00 ;	4AH
MOV		上下Y方向检测距离L	 ,#LOW	00 ;	4BH
;0026
MOV		左右X方向上一次检测距离H	 ,#HIGH	00 ;	4CH
MOV		左右X方向上一次检测距离	 ,#LOW	00 ;	4DH
;0027
MOV	上下Y方向上一次检测距离H		 ,#HIGH	00 ;	4EH
MOV		上下Y方向上一次检测距离L	 ,#LOW	00 ;	4FH     

;0028
	A180H 	 ,#HIGH	00 ;	50H
	A180L 	 ,#LOW	00 ;	51H
;0029
	B320H 	 ,#HIGH	00 ;	52H
	B320L 	 ,#LOW	00 ;	53H
;002A
	C260H 	 ,#HIGH	00 ;	54H	                                          
	C260L 	 ,#LOW	00 ;	55H	                                        
;002B
	D580H 	 ,#HIGH	00 ;	56H
	D580L 	 ,#LOW	00 ;	57H
;002C
	E100H 	 ,#HIGH	00 ;	58H
	E100L 	 ,#LOW	00 ;	59H
;002D
	F80H 		 ,#HIGH	00 ;	5AH
	F80L 		 ,#LOW	00 ;	5BH
;002E
 ;		 ,#HIGH	00 ;	5CH
	; 	 ,#LOW	00 ;	5DH
;003F
	; 	 ,#HIGH	00 ;	5EH
	; 	 ,#LOW	00 ;	5FH
;0040
	     
       */

//***********************
	LCALL	写EEPROM
	LCALL	DY500MS
	//LCALL	掉电保存扇区擦除200H
//	LCALL	DY100MS
	//LCALL	写EEPROM掉电保存0200H
	LCALL	发送20H至5FH到触摸屏
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET
//****结束********EEPROM初始化**********











//****开始****中断0**脉冲******

使能定时器T0:	                                              ;500微秒@11.0592MHZ
	ORL	AUXR, #80H	                                            ;定时器时钟1T模式
	ANL	TMOD, #0F0H	                                           ;设置定时器模式
	MOV	TL0, #066H	                                            ;设置定时初值
	MOV	TH0, #0EAH	                                            ;设置定时初值
	CLR	TF0	                                                   ;清除TF0标志
	SETB	TR0	                                                  ;定时器0开始计时
	SETB	ET0	                                                  ;使能定时器0中断
	SETB	EA
	MOV	脉冲100个计数	, #100
	RET


中断T0开:	SETB	ET0
	RET
中断T0关:	CLR	TR0	                                          ;定时器0开始计时
	CLR	ET0
	RET


//  	脉冲输出总开关标记(至1时X,Y1,Y2,都不输出脉冲）
//			X,Y1,Y2脉冲输出标记 ( 至0时对应单独输出脉冲）
中断T0:
	PUSH	ACC
	PUSH	PSW    
 ORL		PSW,		#00001000 ;R0-R7用第二组08H-0FH
	JB	脉冲输出总开关标记	, 中断T0结束
TOMCX:
	JB	X脉冲输出标记	, TOMCY1
	CPL	X轴脉冲
TOMCY1:
	JB	Y1脉冲输出标记	, TOMCY2
	CPL	Y1轴右脉冲
TOMCY2:
	JB	Y2脉冲输出标记	, TO计数4次
	CPL	Y2轴左脉冲
TO计数4次:
	DJNZ	T0中断4次计数	, 中断T0结束	                           //4   
	CLR	T0中断4次标记
	MOV	T0中断4次计数, #4
TO脉冲计数并写缓冲发送: 

	MOV				P_SW1    ,#00H 		 ; 选择DPTR0
	INC	DPTR
	MOV			P_SW1    ,#01H
	DJNZ	脉冲100个计数 ,中断T0结束		
	MOV	脉冲100个计数	, #100

	MOV				P_SW1    ,#00H 
	MOV	A	, DPH                                                //24H
	MOV	R0, #7AH
	MOVX	@R0	, A
	MOV	A, DPL                                                 ;										25H    
	MOV	R0, #7BH
	MOVX	@R0	, A  
	MOV			P_SW1    ,#01H 		  ; 选择DPTR0	     
	CLR	串口1发送缓冲函数标记
	SETB	TI
  
中断T0结束:
	POP	PSW
	POP	ACC
	RETI
//  	脉冲输出总开关标记(至1时X,Y1,Y2,都不输出脉冲）
//			X,Y1,Y2脉冲输出标记 ( 至0时对应单独输出脉冲）

	//***开始****算数运算*********

;R2,R1+1=R3R2R1
十六加一:
	MOV	A, R1
	ADD	A, #1
	MOV	R1, A
	MOV	A, #00H
	ADDC	A, R2
	MOV	R2, A
	MOV	A, #00H
	ADDC	A, #00H
	MOV	R3, A
	RET

	;R2,R1+R3=R3R2R1
十六加八:
	MOV	A, R1
	ADD	A, R3
	MOV	R1, A
	MOV	A, #00H
	ADDC	A, R2
	MOV	R2, A
	MOV	A, #00H
	ADDC	A, #00H
	MOV	R3, A
	RET
;R2,R1+R4R3=R3R2R1
十六加十六:
	MOV	A, R1
	ADD	A, R3
	MOV	R1, A
	MOV	A, R2
	ADDC	A, R4
	MOV	R2, A
	MOV	A, #00H
	ADDC	A, #00H
	MOV	R3, A
	RET

;；				R4R3*R2	=R7R6R5 
	//	R4R3*R2	=R4R3R2<R7R6R5>
十六乘八:
	MOV	A, R3
	MOV	B, R2
	MUL	AB
	MOV	R5, A	                                                 ;低8位
	MOV	R6, B	                                                 ;高8位Z暂时
	MOV	A, R4
	MOV	B, R2
	MUL	AB
	ADD	A, R6
	MOV	R6, A	                                                 ;高8位
	MOV	R3, A
	MOV	A, B	                                                  ;高8位Z暂时
	ADDC	A, #0
	MOV	R7, A
	MOV	R4, A
	MOV	A, R5
	MOV	R2, A
	RET


 
//R4R3-R2,R1=R3R2R1
十六减十六:
	ADD	A, #00H
	MOV	A, R3
	SUBB	A, R1
	MOV	R1, A
	MOV	A, R4
	SUBB	A, R2
	MOV	R2, A
	MOV	A, #00H
	ADDC	A, #00H
	MOV	R3, A
	RET
//*/   
	//数值正负标记
	//数值为零标记
//R4R3-R2,R1=R3R2R1
十六减十六正负标记:
	ADD	A, #00H
	MOV	A, R3
	SUBB	A, R1
	MOV	R1, A
	MOV	A, R4
	SUBB	A, R2
	MOV	R2, A
	MOV	A, #00H
	ADDC	A, #00H
	MOV	R3, A
	JB	ACC.0, 负标记
	SETB	数值正负标记
	LCALL	数值为零比较
	RET
负标记:
	CLR	数值正负标记
	LCALL	数值为零比较
	RET

数值为零比较:
;A=(值)顺序执行<CY=0> ，A>(值)跳转<CY=0>,A<(值) 跳转<CY=1>
	CJNE	R2, #0, 比较R1R2结果不相同
;A=(值)顺序执行<CY=0> ，A>(值)跳转<CY=0>,A<(值) 跳转<CY=1>
	CJNE	R1, #0, 比较R1R2结果不相同
	CLR	数值为零标记
	RET
比较R1R2结果不相同:
	SETB	数值为零标记
	RET

	;R2,R1-01=R2R1
十六减一:
;	MOV	R1,	L
;	MOV	R2,	H																														;
	ADD	A, #00H
	MOV	A, R1
	SUBB	A, #01
	MOV	R1, A
	MOV	A, R2
	SUBB	A, #0
	MOV	R2, A
;	MOV	L	,	R1
;	MOV	H	,	R2
	RET

;MOV R3,         ; 被除数高8位 
;MOV R2,         ; 被除数低8位  
;MOV R1,         		; 除数  
	;被除数R3.R2.	除数R1				商R3,R2	余数R1
十六除八错误:
	CLR	A	                                                     ; 清零累加器A
	MOV	R4, A	                                                 ; 清零余数寄存器R4
	MOV	R5, A	                                                 ; 清零临时寄存器R5
	MOV	R6, A	                                                 ; 清零临时寄存器R6
	MOV	R0, #16	                                               ; 设置循环次数为16（16位除法）
DIV168LOOP:
	CLR	C	                                                     ; 清除进位标志
	MOV	A, R2	                                                 ; 将被除数低8位加载到累加器
	RLC	A	                                                     ; 带进位左移
	MOV	R2, A	                                                 ; 存回被除数低8位
	MOV	A, R3	                                                 ; 将被除数高8位加载到累加器
	RLC	A	                                                     ; 带进位左移
	MOV	R3, A	                                                 ; 存回被除数高8位
	MOV	A, R4	                                                 ; 将余数加载到累加器
	RLC	A	                                                     ; 带进位左移
	MOV	R4, A	                                                 ; 存回余数

	MOV	A, R4	                                                 ; 将余数加载到累加器
	SUBB	A, R1	                                                ; 余数减去除数
	JC	NO_SUB	                                                 ; 如果借位（余数 < 除数），跳过减法

	MOV	R4, A	                                                 ; 存回新的余数
	INC	R2	                                                    ; 商低8位加1
	MOV	A, R2	                                                 ; 检查低8位是否溢出
	JNZ	NO_CARRY	                                              ; 如果没有溢出，跳过
	INC	R3	                                                    ; 商高8位加1（处理低8位溢出）
NO_CARRY:
NO_SUB:
	DJNZ	R0, DIV168LOOP	                                       ; 循环16次
; 将余数存回R1
	MOV	A, R4	                                                 ; 将余数加载到累加器
	MOV	R1, A	                                                 ; 存回R1
END_LOOP:
	RET








								;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4
三十二除十六:
	MOV	A, R6
	JNZ	START
	MOV	A, R7
	JZ	ERR
START:	MOV	A, R4
	CLR	C
	SUBB	A, R6
	MOV	A, R5
	SUBB	A, R7
	JNC	LOOP4
	MOV	B, #16
LOOP1:	CLR	C
	MOV	A, R2
	RLC	A
	MOV	R2, A
	MOV	A, R3
	RLC	A
	MOV	R3, A
	MOV	A, R4
	RLC	A
	MOV	R4, A
	MOV	A, R5
	RLC	A
	MOV	R5, A
	MOV	PSW.5, C
	CLR	C
	MOV	A, R4
	SUBB	A, R6
	MOV	R1, A
	MOV	A, R5
	SUBB	A, R7
	JB	PSW.5, LOOP2
	JC	LOOP3
LOOP2:	MOV	R5, A
	MOV	A, R1
	MOV	R4, A
	INC	R2
LOOP3:	DJNZ	B, LOOP1
	CLR	PSW.5
								;MOV					DPH			,R3
								;MOV					DPL			,R2
DONE:	RET
LOOP4:	SETB	PSW.5
	SJMP	DONE
ERR:	
	MOV		R3,#0
	MOV		R2,#0
	//	LJMP	$																																																	;报警声
//	LCALL	EEPROM初始化
	RET

//***结束****算数运算*********



//*****开始******软件延时**********
//@11.0592MHZ				无分频

DY1MS:	                                                     ;@11.0592MHZ
	NOP
	MOV	DY1, #9
	MOV	DY2, #150
NEXT001:
	DJNZ	DY2, NEXT001
	DJNZ	DY1, NEXT001
	RET

DY2MS:	                                                     ;@11.0592MHZ
	MOV	DY1, #18
	MOV	DY2, #44
NEXT002:
	DJNZ	DY2, NEXT002
	DJNZ	DY1, NEXT002
	RET

DY5MS:	                                                     ;@11.0592MHZ
	MOV	DY1, #44
	MOV	DY2, #2
NEXT005:
	DJNZ	DY2, NEXT005
	DJNZ	DY1, NEXT005
	RET



DY10MS:	                                                    ;@11.0592MHZ
	MOV	DY1, #87
	MOV	DY2, #10
NEXT01:
	DJNZ	DY2, NEXT01
	DJNZ	DY1, NEXT01
	RET

DY20MS:	                                                    ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #173
	MOV	DY2, #27
NEXT02:
	DJNZ	DY2, NEXT02
	DJNZ	DY1, NEXT02
	RET

DY50MS:	                                                    ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #2
	MOV	DY2, #175
	MOV	DY3, #72
NEXT05:
	DJNZ	DY3, NEXT05
	DJNZ	DY2, NEXT05
	DJNZ	DY1, NEXT05
	RET

DY100MS:	                                                   ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #4
	MOV	DY2, #93
	MOV	DY3, #152
NEXT1:
	DJNZ	DY3, NEXT1
	DJNZ	DY2, NEXT1
	DJNZ	DY1, NEXT1
	RET

DY200MS:	                                                   ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #2
	MOV	DY2, #175
	MOV	DY3, #72
NEXT2:
	DJNZ	DY3, NEXT2
	DJNZ	DY2, NEXT2
	DJNZ	DY1, NEXT2
	RET


DY500MS:	                                                   ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #17
	MOV	DY2, #208
	MOV	DY3, #24
NEXT5:
	DJNZ	DY3, NEXT5
	DJNZ	DY2, NEXT5
	DJNZ	DY1, NEXT5
	RET
DY1S:	                                                      ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #34
	MOV	DY2, #159
	MOV	DY3, #56
NEXT10:
	DJNZ	DY3, NEXT10
	DJNZ	DY2, NEXT10
	DJNZ	DY1, NEXT10
	RET

DY2S:	                                                      ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #68
	MOV	DY2, #61
	MOV	DY3, #120
NEXT20:
	DJNZ	DY3, NEXT20
	DJNZ	DY2, NEXT20
	DJNZ	DY1, NEXT20
	RET

DY4S:	                                                      ;@11.0592MHZ
	NOP
	NOP
	NOP
	MOV	DY1, #135
	MOV	DY2, #121
	MOV	DY3, #240
NEXT40:
	DJNZ	DY3, NEXT40
	DJNZ	DY2, NEXT40
	DJNZ	DY1, NEXT40
	RET



//*****结束******软件延时**********




//***开始*****写EEPROM读EEPROM	******
读EEPROM:
	MOV	DPTR	, #MOVC保存地址	                                  //#0F000H
	MOV	R0, #待存地址
	MOV	R1, #0
	MOV	R2, #保存个数
TEEPROM2:	MOV	A, R1
	MOVC	A, @A+DPTR
	MOV	@R0	, A
	INC	R0
	INC	R1
	DJNZ	R2, TEEPROM2
	RET

;---------------------
写EEPROM:
	MOV	DPTR, #IAP保存地址	                                    //设置ISP/IAP/EEPROM地址
	LCALL	扇区擦除	                                            //扇区擦除
;-------------------------------
	MOV	DPTR, #IAP保存地址	                                    //设置ISP/IAP/EEPROM地址
	MOV	R0, #保存个数	                                         //16																					//检测512字节																													//检测512字节
CHECK1:	                                                    //检测是否擦除成功(全FF检测)
	LCALL	读IAP数据	                                           //读IAP数据
	CJNE	A, #0FFH, 写EEPROM	                                   //如果出错,则退出
	INC	DPTR	                                                  //IAP地址+1
	DJNZ	R0, CHECK1
							;擦除成功
;-------------------------------
	MOV	DPTR, #IAP保存地址	                                    //设置ISP/IAP/EEPROM地址
	MOV	R0, #保存个数	                                         //16																							//编程512字节
	MOV	R1, #待存地址	                                         ;;;;;;;;---------------
NEXTZ:	MOV	A, @R1	                                          //准备数据
	LCALL	字节编程	                                            //字节编程
	INC	DPTR	                                                  //IAP地址+1
	INC	R1	                                                    //修改保存数据
	DJNZ	R0, NEXTZ
	RET
//***开始***EEPROM掉电保存0200H*******
//读EEPROM掉电保存0200H
//写EEPROM掉电保存0200H:
掉电保存扇区擦除200H:
	MOV	DPTR, #0200H	                                          //保存地址//设置ISP/IAP/EEPROM地址
	LCALL	扇区擦除	                                            //扇区擦除
;-------------------------------
	MOV	DPTR, #0200H	                                          //保存地址//设置ISP/IAP/EEPROM地址
	MOV	R0, #4	                                                //检测512字节
CHECK11:	                                                   //检测是否擦除成功(全FF检测)
	LCALL	读IAP数据	                                           //读IAP数据
	CJNE	A, #0FFH, 掉电保存扇区擦除200H	                       //如果出错,则退出
	INC	DPTR	                                                  //IAP地址+1
	DJNZ	R0, CHECK11
	RET
;-------------------------------
	;;掉电保存扇区擦除200H:
写EEPROM掉电保存0200H:
	MOV	DPTR, #0200H	                                          //保存地址//设置ISP/IAP/EEPROM地址
	MOV	R0, #掉电保存个数	                                     //编程512字节
	MOV	R1, #掉电保存地址
NEXTZ1:	MOV	A, @R1	                                         //准备数据
	LCALL	字节编程	                                            //字节编程
	INC	DPTR	                                                  //IAP地址+1
	INC	R1	                                                    //修改保存数据
	DJNZ	R0, NEXTZ1
	RET










/*-----*/
关闭IAP:
	MOV	控制寄存器, #0	                                        //关闭IAP功能
	MOV	命令寄存器, #0	                                        //清除命令寄存器
	MOV	命令触发寄存器, #0	                                    //清除触发寄存器
	MOV	地址寄存器高字, #80H	                                  //将地址设置到非IAP区域
	MOV	地址寄存器低字, #0
	RET

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
读IAP数据:
	MOV	控制寄存器, #82H	                                      //使能IAP
	MOV	命令寄存器, #01B	                                      ;字节读命令									//设置IAP命令
	MOV	地址寄存器低字, DPL	                                   //设置IAP低地址
	MOV	地址寄存器高字, DPH	                                   //设置IAP高地址
	MOV	命令触发寄存器, #5AH	                                  //写触发命令(0X5A)
	MOV	命令触发寄存器, #0A5H	                                 //写触发命令(0XA5)
	NOP	                                                       //等待ISP/IAP/EEPROM操作完成
	MOV	A, 数据寄存器	                                         //度IAP数据
	LCALL	关闭IAP	                                             //关闭IAP功能
	RET

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
字节编程:
	MOV	控制寄存器, #82H	                                      //使能IAP
	MOV	命令寄存器, #10B	                                      ;字节编程命令					//设置IAP命令
	MOV	地址寄存器低字, DPL	                                   //设置IAP低地址
	MOV	地址寄存器高字, DPH	                                   //设置IAP高地址
	MOV	数据寄存器, A	                                         //写IAP数据
	MOV	命令触发寄存器, #5AH	                                  //写触发命令(0X5A)
	MOV	命令触发寄存器, #0A5H	                                 //写触发命令(0XA5)
	NOP	                                                       //等待ISP/IAP/EEPROM操作完成
	LCALL	关闭IAP	                                             //关闭IAP功能
	RET

/*----------------------------
扇区擦除
----------------------------*/
扇区擦除:
	MOV	控制寄存器, #82H	                                      //使能IAP
	MOV	命令寄存器, #11B	                                      ;扇区擦除命令							//设置IAP命令
	MOV	地址寄存器低字, DPL	                                   //设置IAP低地址
	MOV	地址寄存器高字, DPH	                                   //设置IAP高地址
	MOV	命令触发寄存器, #5AH	                                  //写触发命令(0X5A)
	MOV	命令触发寄存器, #0A5H	                                 //写触发命令(0XA5)
	NOP	                                                       //等待ISP/IAP/EEPROM操作完成
	LCALL	关闭IAP	                                             //关闭IAP功能
	RET






//****开始	***中断INT0,P3.急停****

使能INT0中断:	SETB	IT0	                                     //设置INT0的中断类型	(1:仅下降沿	0:上升沿和下降沿)
	SETB	EX0	                                                  //使能INT0中断
	SETB	EA	                                                   ;	允许全局中断
	SETB	ES	                                                   ;	允许中断
	SETB	REN	                                                  ;	串口允许接收
	RET


中断INT0:

	LCALL	DY50MS
	;SETB	尾座气缸退
;	LCALL	DY200MS
;	CLR	尾座气缸退
	//JNB	P3.2,	$
	//LCALL	写EEPROM掉电保存0200H
	MOV	控制寄存器, #20H
	RETI

				//****结束***中断INT0,P3.急停****








//****开始	***中断INT1,P3.3光栅信号****
光栅使能:
;光栅关
使能INT1:
	CLR	IT1
	;SETB	IT1	//设置INT1的中断类型	(1:仅下降沿	0:上升沿和下降沿)
	SETB	EX1	                                                  //使能INT1中断
	SETB	EA
	RET

光栅关:
INT1关:
	CLR	EX1	                                                   ;光栅关
	RET
;光栅
中断INT1:	                                                  //原位置指示//运行中指示//
//	LCALL	写EEPROM掉电保存0200H
	MOV	控制寄存器, #20H
//	MOV	显示画面,	#光栅报警画面
//	LCALL	DGUS切换图片
TUE:	RETI
//****开始	***中断INT1,P3.3光栅信号****



//******
低电压LVD中断使能:
	LCALL	掉电保存扇区擦除200H
	ANL	PCON, #NOT	LVDF	                                       //上电后需要清LVD中断标记位
	SETB	ELVD	                                                 //使能LVD中断
	SETB	EA	                                                   //打开总中断开关
	MOV	IP, #00100000B	                                        //						PLVD			LVD中断高优先级
	RET

低电压LVD中断:
//LCALL	掉电保存扇区擦除200H
	LCALL	写EEPROM掉电保存0200H
	ANL	PCON, #NOT	LVDF	                                       //向PCON.5写0清LVD中断
	MOV	显示画面, #低电压画面
	LCALL	DGUS切换图片
	LJMP	$

	MOV	控制寄存器, #20H
	RETI
//		ORL					PCON,#02H											//进入掉电模式

//****结束****中断********









//***开始****设置IO口**202409*****
设置为准双向口:
;	P0M1.N,P0M0.N		=00->弱上拉200UA,	01->强上拉	20MA	=10->高阻,		11->开漏
	MOV	0X93, #00000000B
	MOV	0X94, #11111111B	                                      ;
;	P1M1.N,P1M0.N		=00->STANDARD,				01->PUSH-PULL		10->PURE	INPUT,		11->OPEN	DRAIN
	MOV	0X91, #00000000B
	MOV	0X92, #00000000B
;	P2M1.N,P2M0.N
	MOV	0X95, #00000000B
	MOV	0X96, #00011111B
;	P3M1.N,P3M0.N
	MOV	0XB1, #00000000B
	MOV	0XB2, #10000000B
;	P4M1.N,P4M0.N
	MOV	0XB3, #00000000B
	MOV	0XB4, #00011100B
;	P5M1.N,P5M0.N
	MOV	0XC9, #00000000B
	MOV	0XCA, #00000000B
	RET
//***结束****设置IO口	*******


急停开关检查:
	JB	急停开关, 急停开关正常
	MOV	DPTR, #急停开关TAB
	LCALL	发送提示字符
	MOV	显示画面, #传感器报警画面
	LCALL	DGUS切换图片
	LCALL	DY1S
	JB	急停开关, 急停开关正常
	MOV	显示画面, #传感器报警画面红
	LCALL	DGUS切换图片
	LCALL	DY1S
	LJMP	急停开关检查
急停开关正常:
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET




/***开始*****@11.0592MHZ串口参数*****
串口15F初始化:
	ANL	P_SW1,	#NOT	0C0H																																							;UART1/USART1:	RXD(P3.0),	TXD(P3.1)
	MOV	SCON,	#50H																																													;8位数据,可变波特率
	ORL	AUXR,	#40H																																													;定时器1时钟为FOSC,即1T
	ANL	AUXR,	#0FEH																																												;串口1选择定时器1为波特率发生器
	ANL	TMOD,	#0FH																																													;设定定时器1为16位自动重装方式
	MOV	TL1,	#0D0H																																													//设置波特率重装值(65536-11059200/4/115200)
	MOV	TH1,	#0FFH
	CLR	ET1																																																				;禁止定时器1中断
	SETB	TR1																																																			;启动定时器1
	SETB	REN																																																			;	允许接收
	SETB	ES																																																				;	允许中断
	SETB	EA																																																				;	允许全局中断
	RET
								*/      





串口1初始化:
UART1_INIT:	                                                ;57600BPS@11.0592MHZ
	MOV	SCON, #50H	                                            ;8位数据,可变波特率
	ORL	AUXR, #01H	                                            ;串口1选择定时器2为波特率发生器
	ORL	AUXR, #04H	                                            ;定时器时钟1T模式
	MOV	T2L, #0D0H	                                            ;设置定时初始值
	MOV	T2H, #0FFH	                                            ;设置定时初始值
	ORL	AUXR, #10H	                                            ;定时器2开始计时
	SETB	REN	                                                  ;	允许接收
	SETB	ES	                                                   ;	允许中断
	SETB	EA
	
	MOV	接收计数, #0
	MOV	发送缓冲计数	, #发送缓冲SRAM首址
	MOV	串口2接收计数	, #0
	RET
/*
																																												;9600BPS@11.0592MHZ
		MOV	T2L,	#0E0H																																													;设置定时初始值
	MOV	T2H,	#0FEH																																													;设置定时初始值
   */
串口2初始化:
	ORL	AUXR, #10H	                                            //T2为1T模式, 并启动定时器2
	SETB	EA
	ORL	IE2, #01H	                                             //使能串口2中断
	CLR	串口2TI发送忙标记
	RET

  ;*********************	UART1中断函数************************
UART1串口中断:
	PUSH	PSW
	PUSH	ACC 
 ORL		PSW,		#00001000 ;R0-R7用第二组08H-0FH
UART1接收忙检查:
	JNB	RI, UART1发送忙检查
	CLR	RI
	MOV	A, #接收缓冲SRAM首址
	ADD	A, 接收计数
	MOV	R0, A
	MOV	A, SBUF	                                               ;保存一个字节
	MOVX	@R0, A
	INC	接收计数
	LCALL	串口接收开始计时
UART1发送忙检查:
	JNB	TI, 结束UART1中断
	CLR	TI
	CLR	发送忙标志	                                            ;	清除发送忙标志
	JB	串口1发送缓冲函数标记	, 结束UART1中断
缓存70数据读出发送:
	MOV	R1, 发送缓冲计数
	MOVX	A, @R1                                               ;GET CURRENT CHAR
	MOV	SBUF, A	                                               ;发送一个字节                           
	INC	发送缓冲计数
	MOV	A, R1
	CJNE	A, 发送缓冲数据总数比较值	, 结束UART1中断
	MOV	发送缓冲计数, #发送缓冲SRAM首址
	SETB	串口1发送缓冲函数标记

结束UART1中断: 
	POP	ACC
	POP	PSW
	RETI

  
;5AA5	<013(#3+X)	82			00	00		>	0060-006F
指令数据串写入发送缓冲:
				;	MOV			发送首址	,#00H
					;MOV			发送缓冲数据个数,#8
				;LCALL		数据写入发送缓冲
	MOV	R0, #发送缓冲SRAM首址
	MOV	A	, #5AH
	MOVX	@R0, A
	INC	R0
	MOV	A, #0A5H
	MOVX	@R0, A
	INC	R0
	MOV	A	, 发送缓冲数据个数
	ADD	A, #3
	MOVX	@R0, A
	INC	R0
	MOV	A, #82H
	MOVX	@R0, A
	INC	R0
	MOV	A, #00H
	MOVX	@R0, A
	INC	R0
	MOV	A, 发送首址
	RR	A	                                                      //40/2=0020
	MOVX	@R0, A	                                               ;变量首址																																									//#00H
	INC	R0
	MOV	R1, 发送首址	                                          ;发送地址20H
	MOV	A, 发送缓冲数据个数	                                   ;发送长度16
	ADD	A, #3
	MOV	R2, A
写XX变量发送缓冲:
	MOV	A, @R1
	MOVX	@R0, A
	INC	R1
	INC	R0
	DJNZ	R2, 写XX变量发送缓冲
	
发送缓冲数据总数比较值计算:
	MOV	A	, #发送缓冲SRAM首址
	MOV	R1, 发送缓冲数据个数
	ADD	A, R1
	ADD	A, #5	                                                 //(6-1)    
	MOV	发送缓冲数据总数比较值	, A
	RET
	









/*;*********************	UART1中断函数************************
UART1串口中断:
	PUSH	PSW
	PUSH	ACC      
 ORL		PSW,		#00001000 ;R0-R7用第二组08H-0FH
UART1接收忙检查:
	JNB	RI, UART1发送忙检查
	CLR	RI
	MOV	A, #接收缓冲SRAM首址
	ADD	A, 接收计数
	MOV	R0, A
	MOV	A, SBUF	                                               ;保存一个字节
	MOVX	@R0, A
	INC	接收计数
	LCALL	串口接收开始计时
UART1发送忙检查:
	JNB	TI, 结束UART1中断
	CLR	TI
	CLR	发送忙标志	                                            ;	清除发送忙标志
结束UART1中断:
	POP	ACC
	POP	PSW
	RETI
     */
;*********************	UART2中断函数************************
UART2串口中断:
	PUSH	PSW
	PUSH	ACC 
 ORL		PSW,		#00001000 ;R0-R7用第二组08H-0FH
UART2接收忙检查:
	MOV	A, S2CON	                                              ;读取UART2控制寄存器
	JNB	ACC.0, UART2发送忙检查	                                ;检测S2RI位
	ANL	S2CON, #NOT	S2RI	                                      ;清除S2RI位
	CLR	串口2RI接收忙标记
	MOV	A, #串口2接收缓冲SRAM首址
	ADD	A, 串口2接收计数
	MOV	R0, A
	MOV	A, S2BUF	                                              ;串口数据保存
	MOVX	@R0, A
	INC	串口2接收计数
	LCALL	串口2接收开始计时
UART2发送忙检查:
	MOV	A, S2CON	                                              ;读取UART2控制寄存器
	JNB	ACC.1, 结束UART2中断	                                  ;检测S2TI位
	ANL	S2CON, #NOT	S2TI	                                      ;清除S2TI位
	CLR	串口2TI发送忙标记	                                     ;清忙标志
结束UART2中断:
	POP	ACC
	POP	PSW
	RETI


使能定时器T1:	                                              ;50毫秒@11.0592MHZ
	  ;10毫秒@11.0592MHz
	ANL		AUXR,#0BFH		;定时器时钟12T模式
	ANL		TMOD,#0FH		;设置定时器模式
	MOV		TL1,#000H		;设置定时初始值
	MOV		TH1,#0DCH		;设置定时初始值
	
	
	
	
	CLR	TF1	                                                   ;清除TF1标志
	SETB	TR1	                                                  ;定时器1开始计时
	SETB	ET1
	//MOV	定时1秒计时	, #20
	   
	RET
	
串口接收开始计时:
	ORL	AUXR, #40H	                                            ;定时器时钟1T模式
	ANL	TMOD, #0FH	                                            ;设置定时器模式
	CLR	TR1	                                                   ;定时器1停止计时
	MOV	TL1, #09AH	                                            ;设置定时初始值
	MOV	TH1, #0A9H	                                            ;设置定时初始值
	CLR	串口接收中断标记	                                      //开始T2计时2MS无接收中断
	SETB	TR1
	SETB	ET1	                                                  //允许T1中断
	RET

串口2接收开始计时:
	ORL	AUXR, #40H	                                            ;定时器时钟1T模式
	ANL	TMOD, #0FH	                                            ;设置定时器模式
	CLR	TR1	                                                   ;定时器1停止计时																																					;定时器1开始计时
	MOV	TL1, #09AH	                                            ;设置定时初始值
	MOV	TH1, #0A9H	                                            ;设置定时初始值
	CLR	串口2接收中断标记	                                     //开始T2计时2MS无接收中断
	SETB	TR1
	SETB	ET1	                                                  //允许T1中断
	RET

中断T1:
	PUSH	PSW
	PUSH	ACC 
	PUSH	B
 ORL		PSW,		#00001000 ;R0-R7用第二组08H-0FH
串口1接收结束检测:
	JB	串口接收中断标记	, 串口2接收结束检测
	CLR	TR1
	SETB	串口接收中断标记
	MOV	接收计数, #0
	LCALL	接收缓存数据读出
	SETB	串口接收中断标记

串口2接收结束检测:
	JB	串口2接收中断标记	, 接收中断计时结束
	CLR	TR1
	SETB	串口2接收中断标记
	MOV	串口2接收计数, #0
	LCALL	串口2接收缓存数据读出
	SETB	串口2接收中断标记


接收中断计时结束:	                                         
//	DJNZ	定时1秒计时	, 中断T1结束
//	MOV		定时1秒计时	, #10 //50MS*20=1S  
	JB			脉冲输出总开关标记, 中断T1结束  	
	;CPL	    T1计时1S标记
;JB	  T1计时1S标记, 中断T1结束
;INC			定时1秒计时
//	JB	串口1发送缓冲函数标记	, 中断T1结束
//******************** 
;十六加一
;H.L 加1
INC 当前时间L             
MOV A, 当前时间L          ; 将R2的值加载到累加器A
JNZ NOCARRY16       			; 如果A不为0，跳转到NO_CARRY
INC 当前时间H             
NOCARRY16:
//******************** 
	MOV	A,当前时间H 
	MOV	R0, #78H
	MOVX	@R0	, A
	MOV	A,当前时间L
	MOV	R0, #79H
	MOVX	@R0	, A
//	SETB	TI
   CPL	P2.6
中断T1结束: 

	SETB	TR1      
	POP	B
	POP	ACC
	POP	PSW
	RETI



	/*
触摸按键返回到串口数据（0X83）
5AA5	06	83	1001	01	0002
5AA5	表示：帧头
06	表示：数据长度
83	表示：读变量存储器指令指令
1001	表示：变量地址（两个字节）
01	表示：1	个字长度数据	（16位
00	02	表示：键值	0002
按键返回（非基本触控）在系统
	*/

//触摸屏数据返回


接收缓存数据读出:
;5A	A5	06	83	00	06	01	01	FE
	MOV	R1, #接收缓冲SRAM首址
缓存00数据读出:
	MOVX	A, @R1	                                               //5A
	CJNE	A, #5AH, 缓存数据非5A	                                //无接帧头数据读出
缓存01数据读出:
	INC	R1
	MOVX	A, @R1	                                               //A5
	CJNE	A, #0A5H, 缓存数据非5A
缓存02数据读出:
	INC	R1
	MOVX	A, @R1	                                               //06	下面接收数据个数
	//MOV	接收数据个数				,A																																			//06-4字节=2
缓存03数据读出:
	INC	R1
	MOVX	A, @R1	                                               //83读变量存储器指令
//	MOV			DGUS指令					,A
	CJNE	A, #83H, 缓存数据错误
缓存04数据读出:
	INC	R1
	MOVX	A, @R1	                                               //10变量地址H		(16位转8位
	MOV	DGUS变量地址H, A	                                      //数值为00
缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量地址L, A
缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	DGUS数据长度	, A
缓存07后数据读出:
	MOV	A, DGUS变量地址L	                                      //	DGUS0010H=STC20H
	MOV	R0, A	                                                 //;接收地址20-2F
	MOV	R2, DGUS数据长度	                                      ;		接收数据长度

有帧头接收数据:
	INC	R1
	MOVX	A, @R1
	MOV	@R0, A
	INC	R0
	DJNZ	R2, 有帧头接收数据
有帧头接收结束:
	CLR	接收数据读完标记
	RET
缓存数据错误:
	RET

//////////////单片机之间通讯////////
缓存数据非5A:
付板接收缓存数据读出:
;A5	5A	06	83	00	06	01	01	FE
	MOV	R1, #接收缓冲SRAM首址
付板缓存00数据读出:
	MOVX	A, @R1	                                               //5A
	CJNE	A, #0A5H, 付板缓存数据错误	                           //无接帧头数据读出
付板缓存01数据读出:
	INC	R1
	MOVX	A, @R1	                                               //A5
	CJNE	A, #05AH, 付板缓存数据错误
付板缓存02数据读出:
	INC	R1
	MOVX	A, @R1	                                               //06	下面接收数据个数
	//MOV	接收数据个数				,A																																			//06-4字节=2
付板缓存03数据读出:
	INC	R1
	MOVX	A, @R1	                                               //83读变量存储器指令
//	MOV			DGUS指令					,A
	CJNE	A, #83H, 付板缓存数据错误
付板缓存04数据读出:
	INC	R1
	MOVX	A, @R1	                                               //10变量地址H		(16位转8位
	MOV	DGUS变量地址H, A	                                      //数值为00
付板缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量地址L, A
付板缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	DGUS数据长度	, A
付板缓存07后数据读出:
	MOV	A, DGUS变量地址L	                                      //	DGUS0010H=STC20H
	MOV	R0, A	                                                 //;接收地址20-2F
	MOV	R2, DGUS数据长度	                                      ;		接收数据长度

付板有帧头接收数据:
	INC	R1
	MOVX	A, @R1
	MOV	@R0, A
	INC	R0
	DJNZ	R2, 付板有帧头接收数据
付板有帧头接收结束:
	RET
付板缓存数据错误:
	RET
	///////////////////////////////////////////



;5AA5	<013(#3+X)	82			00	00		>	0060-006F
发送数据到触摸屏:
				;
				;	MOV			发送首址	,#00H
					;MOV			发送个数,#32
				;LCALL		发送数据到触摸屏
	SETB	串口1发送使能
	MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	A, #3
	ADD	A, 发送变量个数	                                       ;发送个数
	MOV	SBUF, A	                                               //#19
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #82H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #00H
	JNB	TI, $
	CLR	TI
	MOV	A, 发送首址
	RR	A	                                                      //40/2=0020
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	R0, 发送首址	                                          ;发送地址20H
	MOV	R2, 发送变量个数	                                      //#32										;发送长度16
写XX变量发送中:	MOV	A, @R0
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	R0
	DJNZ	R2, 写XX变量发送中
	CLR	串口1发送使能
	RET



















	//***改*202409****@11.0592MHZ串口参数*****
;5AA5	013(#4+X)	<82			00	00			0060-006F
发送20H至5FH到触摸屏:
发送32个数据:
;20-5FH=>0010-0030H
	MOV	发送首址, #20H
	MOV	发送变量个数, #64
	LCALL	发送数据到触摸屏
	RET



发送20H至24H到触摸屏:
;20-5FH=>0000-001FH
	MOV	发送首址, #20H
	MOV	发送变量个数, #4
	LCALL	发送数据到触摸屏
	RET
发送传感器信号标记触摸屏:
	MOV	发送首址, #22H
	MOV	发送变量个数, #2
	LCALL	发送数据到触摸屏
	RET
发送按扭标记到触摸屏:
	MOV	发送首址, #2AH
	MOV	发送变量个数, #2
	LCALL	发送数据到触摸屏
	RET
发送当前数值到触摸屏:
	MOV	发送首址, #24H
	MOV	发送变量个数, #2
	LCALL	发送数据到触摸屏
	RET
发送2627调试标记触摸屏:
	MOV	发送首址, #26H
	MOV	发送变量个数, #2
	LCALL	发送数据到触摸屏
	RET
发送长度到触摸屏:
	MOV	发送首址, #50H
	MOV	发送变量个数, #18
	LCALL	发送数据到触摸屏
	RET

			///////付板/////////////////////

;	5AA5		06=4+(发送个数*2>	83	1001		01(发送个数)	0002
发送0011H22H23H到付板:
;20-5FH=>0000-001FH
	MOV	发送首址, #22H
	MOV	发送变量个数, #1
	LCALL	发送数据到付板
	RET
发送20H21H到主板:	                                          ;付板				用
;20-5FH=>0000-001FH
	MOV	发送首址, #20H
	MOV	发送变量个数, #1
	LCALL	发送数据到付板
	RET


发送单个数据到触摸屏:
		//MOV	发送首址, #3H 
	MOV	A, 发送首址
	MOV	B, #2
	DIV	AB
	JZ	地址位为高位正常发送
	MOV	A, 发送首址
	DEC	A
	MOV	发送首址	, A
地址位为高位正常发送:
	MOV	发送变量个数, #2
	LCALL	发送数据到触摸屏
	RET





;A5	5A	06<4+(发送个数*2>	83	1001	01(发送个数)	0002
发送数据到付板:
				;	MOV			发送首址	,#10H
					;MOV			发送变量个数,#1(16位）
				;LCALL		发送数据到触摸屏
	SETB	串口1发送使能
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #05AH
	JNB	TI, $
	CLR	TI
	MOV	A, 发送变量个数	                                       ;发送个数
	MOV	B, #2
	MUL	AB	                                                    ;A.B=BA'
	MOV	B, #4
	ADD	A, B	                                                  ;发送个数
	MOV	SBUF, A	                                               //#19
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #83H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #00H	                                            ;	发送首址H
	JNB	TI, $
	CLR	TI
	MOV	A, 发送首址
	RR	A	                                                      //40/2=0020
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	A, 发送变量个数	                                       ;发送个数	/	16位个数
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	MOV	R0, 发送首址	                                          ;发送地址20H
	MOV	A, 发送变量个数
	MOV	B, #2
	MUL	AB
	MOV	R2, A	                                                 //#1*2										;发送长度16
写X变量发送中:
	MOV	A, @R0
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	R0
	DJNZ	R2, 写X变量发送中
	CLR	串口1发送使能
	RET

////////付板END//////////////////////////////


/*
写DGUS屏寄存器指令（80）
				此处以切换图片为例，如从当前页面切换到第6幅图片，向屏发指令如下：
5A	A5	04	80	03	0005
5A	A5		表示：帧头
04					表示：数据长度
80					表示：写DGUS屏寄存器
03					表示：寄存器（存储DGUS	屏页面寄存器（03和04寄存器存放的页面））
0005			表示：向寄存器03和04分别写00和05，也就是切换到第5页
*/
写DGUS屏寄存器:
;MOV	显示画面,
DGUS切换图片:
	SETB	串口1发送使能
	MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #04
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #80H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #03H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #00H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, 显示画面	                                        ;报警代号;#05H
	JNB	TI, $
	CLR	TI
	CLR	串口1发送使能
	RET



	/*
	//***开始*****串口读变量***********
读变量存储器指令指令（0X83）
此处以读	1000	变量地址里的数值（假设当前数值为	2）为例：
5A	A5	04	83	0010	01
5A	A5	表示：帧头
04	表示：数据长度
83	表示：读数据存储区指令
0010	表示：变量地址（两个字节）
01	表示：从	1000	地址开始读	1	个字长度，数据指令最大容许长度	0X7C
	*/

读变量地址数据:
//5A	A5	04	83	0010	40
	SETB	串口1发送使能
	MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #04
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #83H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #00H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #18H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #32	                                             //X(	16	)	*2	=
	JNB	TI, $
	CLR	TI
	CLR	串口1发送使能
	LCALL	DY100MS
	RET
//***结束*****串口读变量***********



读0200H掉电保存到2E2F:
;	JB	开机标记	,	不读掉电保存
	MOV	DPTR	, #0F200H
	MOV	R0, #掉电保存地址
	MOV	R1, #0
	MOV	R2, #掉电保存个数
读TEEPROM200:	MOV	A, R1
	MOVC	A, @A+DPTR
	MOV	@R0	, A
	INC	R0
	INC	R1
	DJNZ	R2, 读TEEPROM200

;不读掉电保存:
	RET


//发送提示字符，查表发送并判断00H停止发送
//触摸屏版本号显示0101H-0120H,  
//报警&提示0121-0153H  (64个字节，32个中文字)
//型号中文显示 0200-020FH (2090-209F)    


型号中文显示TAB0:
;帧头
	DB	05AH, 0A5H, 12H	, 082H	, 02H, 01H
	DB	"施拓-001"
	DB	00H	, 0FFH
;FFH,		空白


 
急停开关TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"报警:急停开关触点接通。"
	;字符不能超12*4=48个
	DB	00H, 0FFH	                                              ;FFH,		空白
 
//MOV	DPTR,	#提示符TAB 
//LCALL		中文发送到0121
按急停按钮退出TAB:
;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	DB	"提示:请按急停按钮退出"
	//DB	"内部参数修改请联系0579-87280018 "
	DB	00H	, 0FFH
;FFH,		空白

垂直方向右归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"正在执行垂直方向（右）归零动作，请注意安全。"
	DB	00H	, 0FFH
	;FFH,		空白
垂直方向左归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"正在执行垂直方向（左）归零动作，请注意安全。"
	DB	00H	, 0FFH
	;FFH,		空白
水平方向归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"正在执行水平方向归零动作，请注意安全。"
	DB	00H	, 0FFH
;FFH,		空白

开机复位完成TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 01H, 21H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"开机复位完成。"
	DB	00H	, 0FFH
;FFH,		空白

运行长度错误TAB:
	;帧头          接收地址 =0120H
	DB	05AH, 0A5H, 064H, 082H, 01H, 21H
	DB	" 注意:运行长度太短，运行速度太高，正在自动计算运行速度。"
	DB	00H, 0FFH	                                              ;FFH,		空白



发送版本号:	                                                //0101
	MOV	DPTR, #版本号TAB
	LCALL	发送提示字符
	RET
版本号TAB:
	;帧头          接收地址 =0101H
	DB	05AH, 0A5H, 023H, 082H, 01H, 01H
	DB	"机械手STC20251.0.C"
	DB	00H, 0FFH	                                              ;FFH,		空白




//***开始*****发送提示符	************ 
//触摸屏版本号显示0101H-0120H,  
//报警&提示0121-0153H  (64个字节，32个中文字)

;MOV	DPTR, #提示符TAB
;LCALL		  发送提示字符
发送提示字符:
//发送个数 , 接收地址 ,在帧头里改   
//单字符为零时继续发送FFH填补
;MOV	DPTR, #TAB (查表地址写入DPTR)
	MOV	R0, #0H
	MOV	A, #2H
	MOVC	A, @A+DPTR
	ADD	A, #3H
	MOV	发送个数, A
提示符继续发送:
	MOV	A, R0
	MOVC	A, @A+DPTR
	JZ	继续空白符发送	                                         //判断字符为零时跳转
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	R0
	MOV	A, R0
	CJNE	A, 发送个数	, 提示符继续发送	                         ;     #29H     ;发送个数+6                                //  #2FH
//发送结束:
	RET
继续空白符发送:
	MOV	SBUF, #0FFH
	JNB	TI, $
	CLR	TI
	INC	R0
	MOV	A, R0
	CJNE	A, 发送个数	, 继续空白符发送
	RET


	
//*****结束*****发送提示符	************
//
;5AA5 <013(#3+X) 82   00 00  > 0060-006F
串口2发送数据:
    ; MOV   发送首址 ,#00H
     ;MOV   发送个数,#32
    ;LCALL  串口2发送数据
	SETB	串口2发送使能
	MOV	S2BUF, #5AH
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #0A5H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	A, #3
	ADD	A, 发送变量个数	                                       ;发送个数
	MOV	S2BUF, A	                                              //#19
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #82H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #00H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	A, 发送首址
	RR	A	                                                      //40/2=0020
	MOV	S2BUF, A	                                              ;变量首址                                         //#00H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	R0, 发送首址	                                          ;发送地址20H
	MOV	R2, 发送变量个数	                                      //#32          ;发送长度16
串口2写XX变量发送中:
	MOV	A, @R0
	MOV	S2BUF, A
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	INC	R0
	DJNZ	R2, 串口2写XX变量发送中
	CLR	串口2发送使能
	RET

	//***改*202409****@11.0592MHZ串口参数*****
;5AA5	013(#4+X)	<82			00	00			0060-006F
串口2发送20H至5FH到变量地址:
串口2发送32个数据:
;20-5FH=>0000-001FH
	MOV	发送首址, #50H
	MOV	发送变量个数, #16
	LCALL	串口2发送数据
	RET




串口2接收缓存数据读出:
;5A	A5	06	83	00	06	01	01	FE
	MOV	R1, #串口2接收缓冲SRAM首址
串口2缓存00数据读出:
	MOVX	A, @R1	                                               //5A
	CJNE	A, #5AH, 串口2缓存数据错误	                           //无接帧头数据读出
串口2缓存01数据读出:
	INC	R1
	MOVX	A, @R1	                                               //A5
	CJNE	A, #0A5H, 串口2缓存数据错误
串口2缓存02数据读出:
	INC	R1
	MOVX	A, @R1	                                               //06	下面接收数据个数
	//MOV	接收数据个数				,A																																			//06-4字节=2
串口2缓存03数据读出:
	INC	R1
	MOVX	A, @R1	                                               //83读变量存储器指令
//	MOV			DGUS指令					,A
串口2缓存04数据读出:
	INC	R1
	MOVX	A, @R1	                                               //10变量地址H		(16位转8位
	MOV	DGUS变量地址H, A	                                      //数值为00
串口2缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量地址L, A
串口2缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	DGUS数据长度	, A
串口2缓存07后数据读出:
	MOV	A, DGUS变量地址L	                                      //	DGUS0010H=STC20H
	MOV	R0, A	                                                 //;接收地址20-2F
	MOV	R2, DGUS数据长度	                                      ;		接收数据长度
串口2有帧头接收数据:
	INC	R1
	MOVX	A, @R1
	MOV	@R0, A
	INC	R0
	DJNZ	R2, 串口2有帧头接收数据
串口2有帧头接收结束:
	RET
串口2缓存数据错误:
	RET
 

	END


///////////SMC数据校验计算及发送。开始/////////// 
//SMC数据校验计算及发送  
/*   ////
	MOV	ID号, #01
	MOV	SMC通讯命令, #10
	MOV	SMC通讯地址H, #00
	MOV	SMC通讯地址L, #1E
	MOV	SMC通讯个数H, #00
	MOV	SMC通讯个数L, #02
	MOV	SMC通讯数据字节个数, #04
//MOV		SMC通讯数据1H,#01 
//MOV		SMC通讯数据1L,#01   
//MOV		SMC通讯数据2H,#02 
//MOV		SMC通讯数据2L,#02   
   */
SMC数据校验计算及发送:
	MOV	A, SMC通讯数据字节个数
	ADD	A, #9
	MOV	CRC校验数据个数	, A
	MOV	CRC校验数据首址	, #	ID号
	MOV	R0, CRC校验数据首址
	MOV	R1, CRC校验数据个数
	LCALL	十六位CRC校验码计算
	MOV	A	, CRC校验数据首址	                                   //数据长度    
	ADD	A, CRC校验数据个数
	MOV	R0	, A
	MOV	@R0	, CRC校验码L
	INC	R0
	MOV	@R0, CRC校验码H
// 	RET
SMC带校验数据发送:
	MOV	R0, CRC校验数据首址	                                   //  数据首址
	MOV	A	, #4	                                                //数据长度    
	ADD	A, CRC校验数据个数
	MOV	R2, A
	LCALL	发送读SMC命令
继续发送SMC数据:
	MOV	A, @R0
	MOV	SBUF, A
	JB	发送TI标记, $	                                          //等待前面的数据发送完成             
	SETB	发送TI标记
	INC	R0
	DJNZ	R2, 继续发送SMC数据
	RET

// CRC校验数据首址   EQU	7DH
//	CRC校验数据个数   EQU	7CH
//	CRC校验码H	EQU	7BH
//	CRC校验码L	EQU	7AH 
//  	MOV CRC校验数据首址  ,
//   MOV	CRC校验数据个数  ,
十六位CRC校验码计算:
	MOV	R0, CRC校验数据首址
	MOV	R1, CRC校验数据个数
第1步:	MOV	CRC校验码H, #0FFH
	MOV	CRC校验码L, #0FFH
第2步:	MOV	R2, #8	                                          ;校验次数 		
	MOV	A, @R0
	XRL	A, CRC校验码L
	MOV	CRC校验码L, A
第3步:	CLR	CY
CRC左移一位:
	MOV	A, CRC校验码H
	RRC	A
	MOV	CRC校验码H, A
	MOV	B, A
	MOV	A, CRC校验码L
	RRC	A
	MOV	CRC校验码L, A
判断CRC0位是1否:
	JNB	CY, 判断左移8次
CRC0位是1与A001异或:
	CLR	CY
	MOV	A, CRC校验码H
	XRL	A, #0A0H
	MOV	CRC校验码H, A
	MOV	B, A
	MOV	A, CRC校验码L
	XRL	A, #01H
	MOV	CRC校验码L, A
判断左移8次:
	DJNZ	R2, 第3步
	INC	R0
	DJNZ	R1, 第2步
	RET


/*
	MOV	STC发送首址H	, #02H
	MOV	STC发送首址L	, #00H
	MOV	DW屏变量地址H	, #1H
	MOV	DW屏变量地址L, #80H
	MOV	DW屏数据长度, #70H
	LCALL		发送读SMC命令
  */
发送读SMC命令:
	//MOV	DPH, STC发送首址H
	//MOV	DPL, STC发送首址L
;	MOV	DPTR, #100H                                             //发送首址        ;发送地址20H 
	//MOV	R2, DW屏数据长度	                                      //#32	         ;发送长度16
	SETB	控制口485DERE15
	SETB	控制口485DERE34
	SETB	控制口485DERE35
	MOV	R0, #0H
继续发送SMC命令A:
	MOV	A, R0
	MOVC	A, @A+DPTR
	MOV	SBUF, A
	JB	发送TI标记, $	                                          //等待前面的数据发送完成             
	SETB	发送TI标记
	INC	R0
	DJNZ	R2, 继续发送SMC命令A
	CLR	控制口485DERE15
	CLR	控制口485DERE34
	CLR	控制口485DERE35
	//LCALL	DY2MS
	RET

接收SMC反馈数据:
	LCALL	DW屏通讯地址加100H选择
	MOV	T2X100MS计时数值	, #6	                                 //#T2X100MS计时基数
	CLR	T2计时X100MS标记
等待ID1SMC反馈X:
	JB	T2计时X100MS标记, 等待ID1SMC反馈错误X	                  //需增加延时检测  
	JB	接收RI标记, 等待ID1SMC反馈X
继续接收SMC数据反馈X:
	JB	接收RI标记, $
	SETB	接收RI标记
	MOV	A, SBUF
	MOVX	@DPTR, A
	INC	DPTR
	DJNZ	R2, 继续接收SMC数据反馈X
	LCALL	DY10MS
;	LCALL	SMC40X写入23H25H标记
	SETB	SMC无中断标记
	SETB	T2计时X100MS标记
	RET
等待ID1SMC反馈错误X:
	CLR	SMC无中断标记
	SETB	T2计时X100MS标记
	LCALL	DY20MS
	RET
