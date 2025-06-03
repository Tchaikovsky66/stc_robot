
//*******机械手2025-010C版*10-C1*************************************************   
// 202503数据重新规划分配
//*****触摸屏数据分配****************
//1.10	发送提示字符,查表发送并判断00H停止发送 ,所以地址也不能有00H.
//1.1.1	触摸屏版本号显示0201H-0210H,(32个字节) 
//1.1.2	触摸屏报警&提示0211-0230H  (64个字节,32个中文字)
//1.1.3		触摸屏型号注解中文显示0100-014FH(9*16个字节)   >> 
//*****STC	EEPROM数据分配***************
//2.10	STC15W4K48S4<EEPROM(10K)地址IAP/0000H-27FFH(MOVC/C000H-E7FFH) 
//					20(512个字节)个扇区 1-0000H-01FFH	2-0200H-03FFH   3-0400H-05FFH  	
//2.1.1 第1扇区 存储当前使用参数，第2扇区 存储型号1使用参数,3-2,4-3 .....。  
//*****STC	SRAM数据分配***************
//3.20	15W4K48S2<SRAM地址常规00H-FFH扩展OOOOH-0EFFH总共4K,读指令MOVX	@DOTR, MOVX		@Ri
//3.2.1	接收缓冲SRAM首址	 0000H -009FH(160个字节）
//3.2.2	发送缓冲SRAM首址	 00A0H -00BF( 32个字节）
//3.2.3	串口2接收缓冲SRAM首址		00C0H - 00DFH(32个字节）
//3.2.4	串口2发送缓冲SRAM首址		00E0H - 00FFH(32个字节）
//3.3.1	型号注解SRAM首址	0100H-018FH(9*8*2个字节    
//3.3.2	型号0:0100H-010FH(16个字节)触摸屏0100H-0108H(8*2=16个字节)	  
//3.3.4	型号1:0110H-011FH(16个字节)触摸屏0108H-0110H(8*2=16个字节)  
//3.3.5	型号0	当前型号用	0100H-018F  统一发送，发送时STC数据首址H不除2需注意
//***********注意事项******************* 
//1.因速度655*100=FFFF下面使用函数三十二除十六除数R7,R6不能大于16位
//		>>>所以速度不能大于655
//2.因276320*100/X=FFFF下面使用函数三十二除十六商R3,R2不能大于16位       
//		>>>所以X=276320*100/FFFF=421=速度*6（正弦值最小值）				速度不能小于421/6=70
// 计算S型当前速度并赋值  速度不能小于421/6=70  	>>>所以最小总步数不能小于240 
//3.	P_SW1默认0，MOV	P_SW1	,#00H	选择DPTR0 MOV	P_SW1	,#01H	选择DPTR1 
   /*
//************IO口定义*************

	X轴方向	BIT	P0.0	                                          //DRV
	X轴脉冲	BIT	P0.1	                                          //PUL
	X轴使能	BIT	P0.2	                                          //ENB
	Y1轴右方向	BIT	P0.3	                                       //DRV
	Y1轴右脉冲	BIT	P0.4	                                       //PUL
	Y2轴左方向	BIT	P0.5	                                       //DRV
	Y2轴左脉冲	BIT	P0.6	                                       //PUL
	Y1Y2轴使能	BIT	P0.7
 ;*********P1口		本地放V工件有无P1.6需要单独有传感器端子，	*******
		***IO口共+24V,低电平有效 与本地隔离
串口2接收口	BIT	P1.0	                                      //RXD2
	串口2发送口	BIT	P1.1	                                      //TXD2
	通讯下一台输出串口信号							BIT	P1.2
	通讯上一台输入串口信号							BIT	P1.3
	通讯下一台输出可取信号	BIT	P1.4	                                    //DE
	通讯上一台输入可取信号		BIT	P1.5
	本地放V工件有无	BIT	P1.6	   ;通讯下一台输出取V工件有无信号                                  //	左边存放V型块上工件是否取走
	取V工件有无（上一台信号）	BIT	P1.7	   ;通讯上一台输入取V工件有无信号                                  //右边上一台	存放V型块上有无工件
							


 X左右限位信号  BIT	P2.0
	X中间传感器  		BIT	P2.1

	加工设备有无工件	BIT	P2.3                                 //加工设备Y3
	加工设备异常	BIT	P2.4	                                     //加工设备Y2
	加工设备原位置	BIT	P2.5	                                   //加工设备Y1
	加工设备复位	BIT	P2.6	                                     //加工设备X1
	加工设备运行	BIT	P2.7	  //加工设备X2
   

	串口1接收口	BIT	P3.0	                                      //RXD1
	串口1发送口	BIT	P3.1	                                      //TXD1
				
	急停开关					BIT	P3.2	                                         //中断INT0
	Y1右工件检测	BIT	P3.3	                                     //A2
	Y1右零位信号	BIT	P3.4	                                     //A1
	Y2左零位信号	BIT	P3.5	                                     //A3
	Y2左工件检测		BIT	P3.6	                                     //A4
				 	BIT	P3.7	                                      //A5
 


	Y刹车				BIT	0C1H	                                    						//P4.1	
	Y2轴左吸爪	BIT	0C2H	                                       //P4.2
	Y1轴右吸爪	BIT	0C4H	                                       //P4.4
	启动开关	指示灯			BIT	0C5H	                                         //P4.5
	选择开关1					BIT	0CCH	                                    //P5.4
	选择开关2					BIT	0CDH	                                    //P5.5


//*******IO口定义结束**************
     */
//************IO口定义*************

	X轴方向	BIT	P0.0	                                          //DRV
	X轴脉冲	BIT	P0.1	                                          //PUL
	X轴使能	BIT	P0.2	                                          //ENB
	Y1轴右方向	BIT	P0.3	                                       //DRV
	Y1轴右脉冲	BIT	P0.4	                                       //PUL
	Y2轴左方向	BIT	P0.5	                                       //DRV
	Y2轴左脉冲	BIT	P0.6	                                       //PUL
	Y1Y2轴使能	BIT	P0.7

	串口2接收口	BIT	P1.0	                                      //RXD2
	串口2发送口	BIT	P1.1	                                      //TXD2
	X左防撞信号	BIT	P1.2	                                    //DE
	X右限位信号	BIT	P1.3
	X左限位信号	BIT	P1.4
	X中间传感器	BIT	P1.5
	本地放V工件有无	BIT	P1.6	   ;通讯下一台输出取V工件有无信号                                  //	左边存放V型块上工件是否取走
	取V工件有无（上一台信号）	BIT	P1.7	   ;通讯上一台输入取V工件有无信号                                  //右边上一台	存放V型块上有无工件

	加工设备有无工件	BIT	P2.0	                                 //加工设备Y3
	加工设备异常	BIT	P2.1	                                     //加工设备Y2
	加工设备原位置	BIT	P2.2	                                   //加工设备Y1
	加工设备复位	BIT	P2.3	                                     //加工设备X1
	加工设备运行	BIT	P2.4	  
	//加工设备X2
	通讯下一台输出可取信号	BIT	P2.5    
 通讯上一台输入可取信号	BIT	P2.6
  
	串口1接收口	BIT	P3.0	                                      //RXD1
	串口1发送口	BIT	P3.1	                                      //TXD1
				
	急停开关					BIT	P3.2	                                         //中断INT0
	Y1右工件检测	BIT	P3.3	                                     //A2
	Y1右零位信号	BIT	P3.4	                                     //A1
	Y2左零位信号	BIT	P3.5	                                     //A3
	Y2左工件检测		BIT	P3.6	                                     //A4
	启动开关				 	BIT	P3.7	                                      //A5
  
	Y刹车				BIT	0C1H	                                    						//P4.1	
	Y2轴左吸爪	BIT	0C2H	                                       //P4.2
	Y1轴右吸爪	BIT	0C4H	                                       //P4.4
		指示灯			BIT	0C5H	                                         //P4.5
	选择开关1					BIT	0CCH	                                    //P5.4
	选择开关2					BIT	0CDH	                                    //P5.5


//*******IO口定义结束**************

;*****************标记定义开始***************************************

//*传感器信号标记0011H	EQU	22H*(10-1fh)*********************
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
	Y1轴右吸爪标记	BIT	18H	   																						 //0

//***临时标记12H	EQU	24H(20-2fh)***********************
	;标记BIT	27H	                                      //F15
	;标记	BIT	26H	                                      //E14
;标记	BIT	25H	                                //D13
	;标记	BIT	24H	                                //C12
	Y2脉冲输出标记	BIT	23H	                             //B11
	Y1脉冲输出标记	BIT	22H	                            //A10
	X脉冲输出标记	BIT	21H	                                      //9
	脉冲输出总开关标记	BIT	20H	                                     //8
//****临时标记14L	EQU	25H	****
	;标志	BIT	2FH	                                        //7
	;标记	BIT	2EH	                                  //6
	;标记	BIT	2DH	                                  //5
	;标记	BIT	2CH	                                 //4
;标记	BIT	2BH	                                 //3
	;标记	BIT	2AH	                                 //2
	;标记	BIT	29H	                                      //1
	;标记	BIT	28H	                                  //0

//*****临时标记13H	EQU	26H27H(30-3fh)************************
	数值正负标记	BIT	37H	                                      //F15
	数值为零标记	BIT	36H	                                      //E14
	数值小于极限值标记	BIT	35H	                                //D13
	串口有未读数据标记	BIT	34H	                                //C12
	串口1发送缓冲函数标记	BIT	33H	                             //B11
	缓存数据不自动读出标记	BIT	32H	                            //A10
	T1计时1S标记	BIT	31H	                                      //9
	T0中断4次标记	BIT	30H	                                     //8
//****临时标记14L	EQU	25H	****
	发送忙标志	BIT	3FH	                                        //7
	串口接收中断标记	BIT	3EH	                                  //6
	接收数据读完标记	BIT	3DH	                                  //5
	串口2TI发送忙标记	BIT	3CH	                                 //4
	串口2RI接收忙标记	BIT	3BH	                                 //3
	串口2接收中断标记	BIT	3AH	                                 //2
	长度减一标记	BIT	39H	                                      //1
	当前速度为零标记	BIT	38H	                                  //0
	当前步数为零标记	BIT	38H

//*****按扭标记	14H	EQU	28H29H(40-4FH)************************
	参数保存标记	BIT	47H	                                      //F15
	参数初始化标记	BIT	46H	                                    //E14
	调试按钮标记	BIT	45H	                                      //D13
	复位运行按钮标记	BIT	44H	                                  //C12
	单独型号改变标记	BIT	43H	                                  //B11
	调试下一项标记	BIT	42H	                                    //A10
	型号注解保存标记	BIT	41H	                                  //9
	测试按钮标记	BIT	40H	                                      //8
//*****按扭标记	14L	EQU	29H*******
	;调试1标记	BIT	4FH	                                         //7
	;调试2标记	BIT	4EH	                                         //6
	;调试3标记	BIT	4DH	                                         //5
	;调试4标记	BIT	4CH	                                         //4
	;信号标记	BIT	4BH																																							//3
	;信号标记	BIT	4AH																																							//2
	急停开关标记	BIT	49H	                                      //1
	启动按钮标记	BIT	48H	                                      //0

//*****保存标记15H	EQU	2AH2BH(50-5F)	**开机不复位***************************
	;标记	BIT	57H	                                      //F15
	;标记	BIT	56H	                                    //E14
	;标记	BIT	55H	                                      //D13
	;标记	BIT	54H	                                  //C12
	;标记	BIT	53H	                                  //B11
	;标记	BIT	52H	                                    //A10
	;标记	BIT	51H	                                  //9
	;标记	BIT	50H	                                      //8
//*****按扭标记	14L	EQU	2BH*******
	;标记	BIT	5FH	                                         //7
	;标记	BIT	5EH	                                         //6
	;标记	BIT	5DH	                                         //5
	;标记	BIT	5CH	                                         //4
	;标记	BIT	5BH																																							//3
	;号标记	BIT	5AH																																							//2
	;标记	BIT	59H	                                      //1
	位置检测使能标记	BIT	58H	                                      //0




//****以上是通讯标记***********标记定义END******************** 
 
//**开始***触摸屏0010H地址=单片机高位20H低位21H***    

   
;*********发送缓冲RAM区10h-1FH**数据10个*************
;发送缓冲SRAM首址 ,
;发送缓冲数据个数 ,
;STC数据首址L ,
;发送缓冲数据总个数, UART1串口中断,UART1发送计数用

;5A	A5	15(#3+X)	82	00	00	0001 0203 0405 0607	

	发送帧头5A	EQU	10H
	发送帧头A5	EQU	11H
	发送数据个数	EQU	12H
	发送指令82	EQU	13H
	发送数据首址H	EQU	14H
	发送数据首址L	EQU	15H
   ;  000BH
	T0脉冲数05H	EQU	16H
	T0脉冲数05L	EQU	17H
     ;000C          
	当前时间H	EQU	18H
	当前时间L	EQU	19H
		    ;000D   
	运行速度H	EQU	1AH
	运行速度L	EQU	1BH
  ;000E	                 		
	运行长度H	EQU    1CH
 运行长度L	EQU  	 1DH
	;000F    
	;	EQU	1EH
	;	EQU	1FH 
;	*************************************88
;0010
	;	EQU	20H
			;HL	EQU	21H
;0011
	传感器标记11H	EQU	22H
	传感器标记11L	EQU	23H



;0012
	临时标记12H	EQU	24H
	临时标记12L	EQU	25H
 ;**************************************
;0013
	临时标记13H		EQU	26H
	临时标记13L		EQU	27H
;0014
	按扭标记14H	EQU	28H
	按扭标记14L	EQU	29H      
	//	以下保存参数     
;0015
	保存标记15H	EQU	2AH
	保存标记15L	EQU	2BH      
	/*	----C260-<--------B320------<.....A180........,	/---
------------D580------- ----E80-->--F20-->...G70...> 
------C260-<--------B320--------<------H170--------<  */   

//	X1=A+B			X2=A+B+C		 

;0016
	X向右零位H	EQU	2CH	                                        //			开机不置1
	X向右零位L	EQU	2DH
;0017
	Y向取物料长距离H	EQU	2EH
	Y向取物料长距离L	EQU	2FH
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
	;	EQU	3CH
	步数十倍放大系数	EQU	3DH
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
	型号更改值	EQU	44H
	型号当前值	EQU	45H
	;0023
	备用23H	EQU	46H
	备用23L	EQU	47H
;0024
	左右X方向接收数值H	EQU	48H
	左右X方向接收数值L	EQU	49H
;0025
	上下Y方向接收数值H	EQU	4AH
	上下Y方向接收数值L	EQU	4BH
;0026
	左右X方向上一次接收数值H	EQU	4CH
	左右X方向上一次接收数值L	EQU	4DH
;0027
	上下Y方向上一次接收数值H	EQU	4EH
	上下Y方向上一次接收数值L	EQU	4FH

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
;002F
	端面检测距离H	EQU	5EH
	端面检测距离L	EQU	5FH
;0040
	        


//触摸屏0000H地址=单片机高位00H低位01H
	;运行长度H	EQU	60H
;	运行长度L	EQU	61H
	发送缓冲数据个数	EQU	62H
	发送缓冲数据总个数	EQU	63H
	发送缓冲计数	EQU	64H
	步数速度总系数	EQU	65H
	加减速总步数H	EQU	66H
	加减速总步数L	EQU	67H
	匀速运行长度H	EQU	68H
	匀速运行长度L	EQU	69H
	当前长度H	EQU	6AH
	当前长度L	EQU	6BH
	当前速度H	EQU	6CH
	当前速度L	EQU	6DH
	当前步数H	EQU	6EH
	当前步数L	EQU	6FH


;	运行速度H	EQU	70H
;	运行速度L	EQU	71H
	DY1	EQU	72H
	DY2	EQU	73H
	DY3	EQU	74H
	DGUS变量首址H	EQU	75H
	DGUS变量首址L	EQU	76H
	DGUS变量个数	EQU	77H
	STC数据首址H	EQU	78H
	STC数据首址L	EQU	79H
	STC数据个数	EQU	7AH
	接收计数	EQU	7BH
	发送个数	EQU	7CH
	串口2接收计数	EQU	7DH
	脉冲100个计数	EQU	7DH
	T0中断4次计数	EQU	7EH
	显示画面	EQU	7FH
	定时1秒计时	EQU	12H
 
	




//**开始****数值赋值*********
	掉电保存地址	DATA	030H
	掉电保存个数	DATA	02
	待存首址	DATA	2AH	                                         //028H	//	020H
	保存个数	DATA	64	                                          //		48	                                          //	56		//64      
//3.10	5W4K48S2<EEPROM地址IAP/0000H-27FFH(MOVC/C000H-E7FFH) 20个扇区 1-0000H-01FFH	2-0200H-03FFH....
//3.11 第1扇区 存储当前使用参数，第2扇区 存储型号1使用参数,2-2,3-3 .....。
	;型号0  	第1扇区   	 IAP/0000H      		MOVC 0C000H
	;型号1 	 第2扇区     IAP/0200H 	      MOVC 0C200H		
	;型号2   第3扇区     IAP/0400H        MOVC 0C400H 		
	;型号3   第4扇区     IAP/0600H        MOVC 0C600H 	
//  型号IAP地址 =   (型号*2)H ,00L
// 型号MOVC地址 =   <(型号*2)+C0>H ,00L
	MOVC保存首址	DATA	0C000H
	IAP保存首址	DATA	0000H
	IAP型号注解保存首址	DATA	0100H
//3.20	15W4K48S2<SRAM地址常规00H-FFH扩展OOOOH-0EFFH共4K, 读指令MOVX	@DOTR, MOVX		@Ri
//3.21	接收缓冲SRAM首址	 0000H -009FH(160个字节）
//3.21		发送缓冲SRAM首址	 00A0H -00BF( 32个字节）
//3.22		串口2接收缓冲SRAM首址		00C0H - 00DFH(32个字节）
//3.22		串口2发送缓冲SRAM首址		00E0H - 00FFH(32个字节）
//3.23  型号注解SRAM首址  	0100H-01FF    型号0:0100H-010FH(16个字节)	
	接收缓冲SRAM首址	DATA	0000H
	发送缓冲SRAM首址	DATA	00A0H
	发送缓冲RAM首址	DATA	10H

	串口2接收缓冲SRAM首址	DATA	00C0H
	串口2发送缓冲SRAM首址	DATA	00E0H
	
	型号触摸屏注解SRAM首址	DATA	0100H



	Y1Y2中心320距离	DATA	3200	                                    //#3200	  //固定320MM//	X零位至V型块距离H
	检测提前20距离	DATA	200	                                     //200/20MM
	精细归零速度	DATA	040
	快速归零速度	DATA	080
	
	无加减速赋值速度	DATA	0040
	调试点动速度	DATA	006
	调试有加减速速度	DATA	0500
;检测速度     DATA			50	
	调试速度T0定时值	DATA	60009	                               //50MM/S
	正弦值等分数	DATA	100
	T0中断脉冲次数	DATA	4	                                     ;4对应2000细分数，8/4000 
	脉冲计数个数	DATA	100

//2.因276320*100/X=FFFF下面使用函数三十二除十六商R3,R2不能大于16位       
//		>>>所以X=276320*100/FFFF=421=速度*6（正弦值最小值）				速度不能小于421/6=70
// 计算S型当前速度并赋值  速度不能小于421/6=70  	>>>所以最小步数不能小于240 
	最小步数计算极限值	DATA	240	                               ;不能小于240  		
	最大步数计算极限值	DATA	65535	                             ;FFFFH
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
	工件型号表2	DATA	10
	调试画面1K	DATA	010
	调试画面2K	DATA	011
	调试画面3K	DATA	012
	调试准备型号画面	DATA	013
	调试准备参数画面	DATA	014
	调试准备动作画面	DATA	015
	调试物料取画面	DATA	016
	调试送放工件画面	DATA	017
	调试取放工件画面	DATA	018
//工作单步画面
中间等待来料  DATA 020
右边向X快进E80  DATA 021
右边向X检测F20  DATA 022
右边向X快进G70  DATA 023
右边Y1取前上停  DATA 024
右边Y1取往下  DATA 025
右边Y1取下停  DATA 026
右边Y1取往上  DATA 027
右边Y1取后上停  DATA 028
中间向X快进H170  DATA 029
中间Y2取前上停  DATA 030
中间Y2取往下  DATA 031
中间Y2取下停  DATA 032
中间Y2取往上  DATA 033
中间Y2取后上停  DATA 034
中间向X快进B320  DATA 035
中间Y1放前上停  DATA 036
中间Y1放往下  DATA 037
中间Y1放下停  DATA 038
中间Y1放往上  DATA 039
中间Y1放后上停  DATA 040
左边向X260快进  DATA 041
左边Y2放前上停  DATA 042
左边Y2放往下  DATA 043
左边Y2放下停  DATA 044
左边Y2放往上  DATA 045
左边Y2放后上停  DATA 046
右边向X快进D580  DATA 047
中间零位等待  DATA 048



//报警画面 40-49
//	报警K1画面	DATA	040


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

	传感器图画面	DATA	066
	传感器表格1画面	DATA	067
	传感器表格2画面	DATA	068
	传感器表格3画面	DATA	069
	测试画面1	DATA	070









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
	LJMP	开机
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
	ORG	0100H

开机:	MOV	SP, #090H	                                        ;090H
	LCALL	设置为准双向口
	MOV	IP, #01	                                               //中断INT0优先                                         ;
	MOV	P0, #00000000B
	MOV	P1, #11111111B
	MOV	P2, #00000000B
	MOV	P3, #11111111B
	MOV	P4, #11111001B
	MOV	P5, #00000000B
	MOV	P_SW1	, #00H
	LCALL	DY1S
	LCALL	读EEPROM
	lcall	dy100ms
	LCALL	型号注解第10扇区读出到SRAM
//LCALL	上电读0200H掉电保存到2E2F
//	JNB	急停开关	,	不读掉电保存
	//LCALL	读0200H掉电保存到2E2F   
不读掉电保存:
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

	LCALL	串口1初始化
//	LCALL	串口2初始化
	LCALL	DY10MS
	LCALL	发送20H至5FH到触摸屏
	LCALL	发送版本号
	LCALL	急停开关检查
	LCALL	使能INT0中断
;	LCALL	低电压LVD中断使能
	//	LCALL	初始化CCPMPW
	LCALL	使能定时器T0
	LCALL	使能定时器T1
待机前准备:       

LCALL		每段长度计算


	CLR	Y1轴右吸爪
	CLR	Y2轴左吸爪
	SETB	Y刹车
	SETB	缓存数据不自动读出标记
;###

	LCALL	发送RAM10H缓冲区帧头初始化
;### 
	LCALL	发送型号表格注解
待机检测:

	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片

	LCALL	DY100MS
	LCALL	发送当前型号注解


	;LCALL	型号表格注解初始化	   
;LCALL	型号注解第10扇区读出保存测试 
;LCALL	 读8型号IAPEEPROM初始化测试
;LCALL	写型号IAPEEPROM初始化测试 
;	LCALL	型号注解SRAM保存到第10扇区   
	;LCALL	型号注解第10扇区读出到SRAM
                          
检测: 

型号注解更改:
	JB	型号注解保存标记, 型号更改
	SETB	型号注解保存标记
	LCALL	发送按扭标记到触摸屏
	LCALL	读变量地址0100h数据	                                 //2503  
	LCALL	型号注解SRAM保存到第10扇区
;LCALL	发送型号表格注解  
	LCALL	发送当前型号注解
;	MOV	显示画面, #等待画面
	;LCALL	DGUS切换图片
;LCALL			写型号IAPEEPROM
型号更改:
	MOV	A, 型号更改值
	SUBB	A	, 型号当前值
	JZ	开机参数复位	                                           ;JZ,A=0 ; PC=(PC)+2+REL
	
	MOV	型号当前值	, 型号更改值
	LCALL	读型号IAPEEPROM
;LCALL		发送型号值到触摸屏   
LCALL	每段长度计算
	LCALL	发送20H至5FH到触摸屏
	LCALL	发送当前型号注解
	LCALL	写EEPROM
 LCALL	  发送RAM10H缓冲区帧头初始化
开机参数复位:
	JB	参数初始化标记, 参数保存
	SETB	参数初始化标记
	LCALL	发送按扭标记到触摸屏
	LCALL	EEPROM初始化
	LCALL	写型号IAPEEPROM初始化
	LCALL	型号表格注解初始化	                                  ;SRAM0100H 初始化     
	LCALL	型号注解SRAM保存到第10扇区
	LCALL	发送型号表格注解
	LCALL	发送当前型号注解
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
LCALL	  发送RAM10H缓冲区帧头初始化

参数保存:
	JB	参数保存标记	, 调试
	SETB	参数保存标记
	MOV	2AH, #0FFH
	MOV	2BH, #0FFH
	LCALL	发送按扭标记到触摸屏
	MOV	显示画面, #数据保存画面
	LCALL	DGUS切换图片
	LCALL	读变量地址数据
	LCALL	DY500MS
	SETB	参数保存标记
	LCALL	写EEPROM
	LCALL	写型号IAPEEPROM	                                     //2503	
;	LCALL		读变量地址0100h数据  //2503    
;	LCALL		DY100MS
;	LCALL	型号注解SRAM保存到第10扇区
;	LCALL	DY1S   

	LCALL		每段长度计算  
LCALL	发送20H至5FH到触摸屏
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
LCALL	  发送RAM10H缓冲区帧头初始化
调试:
	JB	调试按钮标记	, 编写测试
	SETB	调试按钮标记
	LCALL	发送按扭标记到触摸屏 
		LCALL	机械手归零
	MOV	显示画面, #调试准备动作画面
	LCALL	DGUS切换图片
	LCALL	调试准备
编写测试:

	JB	测试按钮标记, 是否运行检测 
 LCALL	发送RAM10H缓冲区帧头初始化      
	LCALL	机械手归零
	LCALL	编写测试动作


是否运行检测:
	JB	复位运行按钮标记, 返回检测
	SETB	复位运行按钮标记
	LCALL	发送按扭标记到触摸屏
	LJMP	运行前准备

返回检测:
	LJMP	检测

 ;********************************** 



                        //
 ;*******************************************88888
//是否运行检测:
//	JB	复位运行按钮标记, 检测
//	SETB	复位运行按钮标记
//	LCALL	发送按扭标记到触摸屏
运行前准备:
	SETB	Y刹车
	LCALL	机械手归零
	LCALL	每段长度计算
;	LCALL	发送长度到触摸屏 
	MOV	显示画面, #中间等待来料
	LCALL	DGUS切换图片
	LCALL	X向右零位距离运行
	LCALL	X轴A180固定距离运行
运行程序:
  
机械手中间等待来料:
	MOV	显示画面, #中间等待来料
LCALL	DGUS延时切换图片   
;????????????????
	LCALL	DY1S
                            
机械手右边向X快进E80:
	MOV	显示画面, #右边向X快进E80
LCALL	DGUS延时切换图片
	LCALL	X轴E80运行   

机械手工件检测中:
	MOV	显示画面, #右边向X检测F20
LCALL	DGUS延时切换图片 

	LCALL	工件F20端面检测运行
	;LCALL	DY1S

机械手右边向X快进G70:
	MOV	显示画面, #右边向X快进G70
LCALL	DGUS延时切换图片
	LCALL	X轴F70抓铁芯右运行
 MOV	显示画面, #右边Y1取前上停   
 LCALL	DGUS清零切换图片 

机械手右边Y1取往下:
 MOV	显示画面, #右边Y1取往下
LCALL	DGUS延时切换图片
	SETB	Y1轴右方向
	LCALL	Y1轴二头运行

 MOV	显示画面, #右边Y1取下停
LCALL	DGUS清零切换图片  

机械手Y1轴右吸爪紧:
	SETB	Y1轴右吸爪   
	LCALL	DY200MS    

机械手右边Y1取往上:
MOV	显示画面,   #右边Y1取往上
LCALL	DGUS延时切换图片
	CLR	Y1轴右方向 
	LCALL	Y1轴二头运行
	MOV	显示画面, #右边Y1取后上停   
LCALL	DGUS清零切换图片  
 

机械手中间向X快进H170:

	MOV	显示画面, #中间向X快进H170 
LCALL	DGUS延时切换图片  

	LCALL	X轴H180计算长度运行 

	MOV	显示画面, #中间Y2取前上停   
LCALL	DGUS清零切换图片
; ????????????????????????///
等待加工完成:  
	LCALL	DY1S      

机械手中间Y2取往下:
	MOV	显示画面, #中间Y2取往下
LCALL	DGUS延时切换图片
	SETB	Y2轴左方向
	LCALL	Y2轴中间运行

	MOV	显示画面, #中间Y2取下停 
LCALL	DGUS清零切换图片  

机械手Y2轴左吸爪紧:
 	SETB	Y2轴左吸爪  ;5555555555   
 	LCALL	DY200MS   

机械手中间Y2取往上:
	MOV	显示画面, #中间Y2取往上
LCALL	DGUS延时切换图片
;LCALL	DY500MS
	CLR	Y2轴左方向
	LCALL	Y2轴中间运行

	MOV	显示画面, #中间Y2取后上停
LCALL	DGUS清零切换图片
机械手中间向X快进B320:
	MOV	显示画面, #中间向X快进B320
LCALL	DGUS延时切换图片
	LCALL	X轴B320运行 
	
	MOV	显示画面, #中间Y1放前上停
LCALL	DGUS清零切换图片 
机械手中间Y1放往下:
	MOV	显示画面, #中间Y1放往下
LCALL	DGUS延时切换图片
	SETB	Y1轴右方向
	LCALL	Y1轴中间运行 
		MOV	显示画面, #中间Y1放下停
LCALL	DGUS清零切换图片
机械手Y1轴右吸爪松:
	CLR	Y1轴右吸爪 ;5555555555 
  
    
 LCALL	DY200MS 


机械手中间Y1放往上:

		MOV	显示画面, #中间Y1放往上
LCALL	DGUS延时切换图片

	CLR	Y1轴右方向
	LCALL	Y1轴中间运行

	MOV	显示画面, #中间Y1放后上停 
	LCALL	DGUS清零切换图片
等待加工工件取走:
;???????????????????????????
	LCALL	DY1S   
           

机械手左边向X260快进:
	MOV	显示画面, #左边向X260快进 
LCALL	DGUS延时切换图片
	LCALL	X轴C260运行   

 MOV	显示画面, #左边Y2放前上停
LCALL	DGUS清零切换图片
机械手左边Y2放往下:
 MOV	显示画面, # 左边Y2放往下
LCALL	DGUS延时切换图片
	SETB	Y2轴左方向
	LCALL	Y2轴二头运行  
	MOV	显示画面, #左边Y2放下停 
LCALL	DGUS清零切换图片 
机械手Y2轴左吸爪松:
	CLR	Y2轴左吸爪      
  
 
LCALL	DY200MS  


机械手左边Y2放往下上:
MOV	显示画面, #左边Y2放往上 
LCALL	DGUS延时切换图片
CLR	Y2轴左方向
CALL	Y2轴二头运行
                   
MOV	显示画面, #左边Y2放后上停 
LCALL	DGUS清零切换图片
机械手回到零位:
	MOV	显示画面, #右边向X快进D580
LCALL	DGUS延时切换图片
	LCALL	X轴D580运行   
	MOV	显示画面, #中间零位等待
LCALL	DGUS清零切换图片


LCALL	发送长度到触摸屏
	LJMP		机械手中间等待来料


	LJMP	待机检测	                                             //运行程序

DGUS延时切换图片:
LCALL		DY5MS       
LCALL	DGUS切换图片   
  RET
DGUS清零切换图片:
LCALL		DY5MS       
 LCALL	DGUS切换图片 
 LCALL		DY20MS
 MOV		T0脉冲数05H,#0
 MOV		T0脉冲数05L,#0  
 LCALL	发送刷新8数据 
 ;LCALL	DY20MS  
  	RET


 







 ;****************************************8888888
/*
  型号更改值	EQU	44H
	型号当前值	EQU	45H
单独型号改变检测:
	JB	单独型号改变标记	, 无保存
	SETB	单独参数保存标记 	
	LCALL	读型号EEPROM         	
LCALL	发送20H至5FH到变量地址
	;LCALL	读变量地址数据
	LCALL	DY1S

		MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	LCALL	发送20H至5FH到触摸屏
无保存:	RET
//***ABCDEF************************  
        */
// ****************************************************88888*****************

;接收数据读完标记    SETB	接收数据读完标记
;下一项开关标记    
	//调试准备画面	DATA	015
	//调试物料取画面	DATA	016
	//调试送放工件画面	DATA	017
	//调试取放工件画面	DATA	01


调试准备:
   
接收调试数值初始化:
	MOV	左右X方向接收数值H	, #HIGH	00
	MOV	左右X方向接收数值L	, #LOW	00
	MOV	上下Y方向接收数值H	, #HIGH	00
	MOV	上下Y方向接收数值L	, #LOW	00
	MOV	左右X方向上一次接收数值H	, 左右X方向接收数值H
	MOV	左右X方向上一次接收数值L	, 左右X方向接收数值L
	MOV	上下Y方向上一次接收数值H	, 上下Y方向接收数值H
	MOV	上下Y方向上一次接收数值L	, 上下Y方向接收数值L
	LCALL	发送XY当前数值到触摸屏

	JB	调试下一项标记	, $
	SETB	调试下一项标记
	LCALL	发送按扭标记到触摸屏
;*******右边****************************************	
右边物料取位置调试A快速移动:
	MOV	DPTR, #调试物料取位置TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片
	MOV	左右X方向接收数值H	, #HIGH	400
	MOV	左右X方向接收数值L	, #LOW	400
	MOV	上下Y方向接收数值H	, #HIGH	800
	MOV	上下Y方向接收数值L	, #LOW	800
	LCALL	发送XY当前数值到触摸屏
	LCALL	左右X方向调试动作
	LCALL	上下Y1右方向调试动作
 ///////////////////////////////////////
	MOV	显示画面, #调试物料取画面
	LCALL	DGUS切换图片
手动右边物料取位置调试A:
	JB	接收数据读完标记	, $
	SETB	接收数据读完标记
	LCALL	左右X方向调试动作
	LCALL	上下Y1右方向调试动作
	JB	调试下一项标记	, 手动右边物料取位置调试A
	SETB	调试下一项标记
	LCALL	发送按扭标记到触摸屏
	MOV	Y向两头取放距离H, 上下Y方向接收数值H
	MOV	Y向两头取放距离L, 上下Y方向接收数值L
	MOV	X向右零位H, 左右X方向接收数值H
	MOV	X向右零位L, 左右X方向接收数值L
///////////////////////////////////////     

	SETB	Y1轴右吸爪
	LCALL	DY100MS
右边物料取位置调试A快速回归:
	MOV	DPTR, #调试物料取回归TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片
	;MOV	左右X方向接收数值H	, #HIGH	400
;	MOV	左右X方向接收数值L	, #LOW		400
	MOV	上下Y方向接收数值H	, #HIGH	000
	MOV	上下Y方向接收数值L	, #LOW	000
	LCALL	发送XY当前数值到触摸屏
	LCALL	上下Y1右方向调试动作
;	LCALL	左右X方向调试动作


;*******中间*1***************************************	
中间加工位置调试B快速移动:
	MOV	DPTR, #调试中间加工位置放TAB
	LCALL	发送提示字符
;MOV	显示画面, #提示语画面
;	LCALL	DGUS切换图片     
	

	//23MOV	R4,	X向右零位H 
//23	MOV	R3,	X向右零位L
 MOV	R2,	 #HIGH	5000     ;(B320+A180 )
 MOV	 R1,	 #LOW	5000
;R2,R1+R4R3=R3R2R1
//23LCALL	十六加十六
	MOV	左右X方向接收数值H	, R2
	MOV	左右X方向接收数值L	, R1
	MOV	上下Y方向接收数值H	, #HIGH	4000
	MOV	上下Y方向接收数值L	, #LOW	4000
	LCALL	发送XY当前数值到触摸屏
	LCALL	左右X方向调试动作
	LCALL	上下Y1右方向调试动作
//////////////////////////////////////////
	MOV	显示画面, #调试送放工件画面
	LCALL	DGUS切换图片
手动中间加工位置调试B:
	JB	接收数据读完标记	, $
	SETB	接收数据读完标记
	LCALL	左右X方向调试动作
	LCALL	上下Y1右方向调试动作
	JB	调试下一项标记	, 手动中间加工位置调试B
	SETB	调试下一项标记
	LCALL	发送按扭标记到触摸屏
	MOV	Y向中间抓V距离H, 上下Y方向接收数值H
	MOV	Y向中间抓V距离L, 上下Y方向接收数值L 
MOV	X向右零位至中心距离H, 左右X方向接收数值H
	MOV	X向右零位至中心距离L, 左右X方向接收数值L
/*23	MOV	R4,   左右X方向接收数值H   	
	MOV	R3,   左右X方向接收数值L	
	MOV	R2,    X向右零位H 
	MOV	R1,    X向右零位L
	 ;R4R3-R2R1=R3R2R1
	LCALL	十六减十六正负标记  
MOV	X向右零位至中心距离H, R2
MOV	X向右零位至中心距离L, R1
	                 */
	CLR	Y1轴右吸爪
	LCALL	DY100MS
/////////////////////////////////////////
中间加工位置调试B快速回归:
 
	MOV	DPTR, #调试中间加工位置放回归TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片

	;MOV	左右X方向接收数值H	, #HIGH	5000
	;MOV	左右X方向接收数值L	, #LOW	5000
	MOV	上下Y方向接收数值H	, #HIGH	000
	MOV	上下Y方向接收数值L	, #LOW	000
	LCALL	发送XY当前数值到触摸屏
	LCALL	上下Y1右方向调试动作
;	LCALL	左右X方向调试动作

;20250309*******中间*2***************************************	

中间加工位置调试BY2快速移动:
	MOV	DPTR, #调试中间加工位置取TAB
	LCALL	发送提示字符
	;MOV	显示画面, #提示语画面
	;LCALL	DGUS切换图片 
 

	;MOV	R4,X向右零位至中心距离H  
;	MOV	R3,X向右零位至中心距离L 
	MOV	R4,	左右X方向接收数值H 
	MOV	R3	,左右X方向接收数值L 

	MOV	R2,     #HIGH	Y1Y2中心320距离 
	MOV	R1,     #LOW	 Y1Y2中心320距离
	 ;R4R3-R2R1=R3R2R1
	LCALL	十六减十六正负标记

	MOV	左右X方向接收数值H	, R2
	MOV	左右X方向接收数值L	, R1
	MOV	上下Y方向接收数值H	, Y向中间抓V距离H                   ;HIGH	300
	MOV	上下Y方向接收数值L	, Y向中间抓V距离L                   ;#LOW	300
	LCALL	发送XY当前数值到触摸屏
	LCALL	左右X方向调试动作
	LCALL	上下Y2左方向调试动作
	SETB	Y2轴左吸爪
	LCALL	DY100MS


中间加工位置调试BY2快速回归:
	MOV	DPTR, #调试中间加工位置取回归TAB
	LCALL	发送提示字符
	;MOV	显示画面, #提示语画面
	;LCALL	DGUS切换图片
//	MOV	左右X方向接收数值H	, X向右零位至中心距离H
	//MOV	左右X方向接收数值L	, X向右零位至中心距离L  
	MOV	R4,	左右X方向接收数值H
	MOV	R3,	左右X方向接收数值L
	MOV	R2,     #HIGH	Y1Y2中心320距离 
	MOV	R1,     #LOW	 Y1Y2中心320距离
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	左右X方向接收数值H	, R2
	MOV	左右X方向接收数值L	, R1


	MOV	上下Y方向接收数值H	, #HIGH	000
	MOV	上下Y方向接收数值L	, #LOW	000
	LCALL	发送XY当前数值到触摸屏
	LCALL	上下Y2左方向调试动作                                 ;上下Y1右方向调试动作
	LCALL	左右X方向调试动作



;**********左边*****************************************
左边放位置调试C快速移动:
	MOV	DPTR, #调试左边位置放TAB
	LCALL	发送提示字符
	;MOV	显示画面, #提示语画面
	;LCALL	DGUS切换图片    
	MOV	R4,	左右X方向接收数值H
	MOV	R3,	左右X方向接收数值L
 MOV		R2,	 #HIGH	2600
 MOV	 R1,	 #LOW		2600
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	左右X方向接收数值H	, R2
	MOV	左右X方向接收数值L	, R1

	MOV	上下Y方向接收数值H	, Y向两头取放距离H	                 ;#HIGH	300
	MOV	上下Y方向接收数值L	, Y向两头取放距离L	                 ; #LOW	300
	LCALL	发送XY当前数值到触摸屏
	LCALL	左右X方向调试动作
	LCALL	上下Y2左方向调试动作
	MOV	显示画面, #调试取放工件画面
	LCALL	DGUS切换图片
  ////////////////////////////////////////////////
手动左边放位置调试C:
	JB	接收数据读完标记	, $
	SETB	接收数据读完标记
	LCALL	左右X方向调试动作
	LCALL	上下Y2左方向调试动作
	JB	调试下一项标记, 手动左边放位置调试C
	SETB	调试下一项标记
	LCALL	发送按扭标记到触摸屏
	;MOV	Y向两头取放距离H, 上下Y方向接收数值H
	;MOV	Y向两头取放距离L, 上下Y方向接收数值L
	MOV	X向右零位至左零位距离H, 左右X方向接收数值H
	MOV	X向右零位至左零位距离L, 左右X方向接收数值L
	/*23MOV	R4,   左右X方向接收数值H   	
	MOV	R3,   左右X方向接收数值L	
	MOV	R2,    X向右零位H 
	MOV	R1,    X向右零位L
	 ;R4R3-R2R1=R3R2R1
	LCALL	十六减十六正负标记  
MOV		X向右零位至左零位距离H, R2
MOV		X向右零位至左零位距离L ,R1   */
	CLR	Y2轴左吸爪
	LCALL	DY100MS
左边放位置调试C快速回归:
	MOV	DPTR, #调试左边位置放回归TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片
	MOV	左右X方向接收数值H	, X向右零位H
	MOV	左右X方向接收数值L	, X向右零位L
	MOV	上下Y方向接收数值H	, #HIGH	000
	MOV	上下Y方向接收数值L	, #LOW	000
	LCALL	发送XY当前数值到触摸屏
	LCALL	上下Y2左方向调试动作
	LCALL	左右X方向调试动作
	MOV	DPTR, #调试结束归零TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
                            LCALL		DY100MS
	;LCALL	机械手归零


;	LCALL	左右X方向调试动作    
/* MOV	DPTR, #调试物料取位置TAB
	MOV	DPTR, #调试物料取回归TAB
	MOV	DPTR, #调试中间加工位置放TAB  
	MOV	DPTR, #调试中间加工位置放回归TAB 
	MOV	DPTR, #调试中间加工位置取TAB  
	MOV	DPTR, #调试中间加工位置取回归TAB  
	MOV	DPTR, #调试左边位置放TAB  
	MOV	DPTR, #调试左边位置放回归TAB  
     */
 ////////////////////////////////////

手动位置调试结束:
	LCALL	每段长度计算
	LCALL	发送20H至5FH到触摸屏
	LCALL	写EEPROM
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET

//************右边调试**X****************************
左右X方向调试动作:
	MOV	R4, 左右X方向接收数值H
	MOV	R3, 左右X方向接收数值L
	MOV	R2, 左右X方向上一次接收数值H
	MOV	R1, 左右X方向上一次接收数值L
	LCALL	十六减十六正负标记
	JNB	数值为零标记	, 左右X方向调试退出
	MOV	运行长度H, R2
	MOV	运行长度L, R1
	JNB	数值正负标记	, 左右X方向调试右移
左右X方向调试左移:
	SETB	X轴方向
	CLR	X脉冲输出标记
	LCALL	调试慢速计数运行
	SETB	X脉冲输出标记
	MOV	左右X方向上一次接收数值H	, 左右X方向接收数值H
	MOV	左右X方向上一次接收数值L	, 左右X方向接收数值L
	RET
左右X方向调试右移:
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, 运行长度H
	MOV	R1, 运行长度L
 ;R4R3-R2R1=R3R2R1
	;LCALL	十六减十六正负标记
	;-R2R1=R2R1
	LCALL	十六位负数转换成正数

	MOV	运行长度H, R2
	MOV	运行长度L, R1
左右X方向调试右移2:
	CLR	X轴方向
	CLR	X脉冲输出标记
	LCALL	调试慢速计数运行
	SETB	X脉冲输出标记
	MOV	左右X方向上一次接收数值H	, 左右X方向接收数值H
	MOV	左右X方向上一次接收数值L	, 左右X方向接收数值L
	RET
左右X方向调试退出:
	RET

 ;-R2R1=R2R1



//************上下Y1右方向调试动作****************************
上下Y1右方向调试动作:
	MOV	R4, 上下Y方向接收数值H
	MOV	R3, 上下Y方向接收数值L
	MOV	R2, 上下Y方向上一次接收数值H
	MOV	R1, 上下Y方向上一次接收数值L
	LCALL	十六减十六正负标记
	JNB	数值为零标记	, 上下Y1右方向调试退出
	MOV	运行长度H, R2
	MOV	运行长度L, R1
	JNB	数值正负标记	, 上下Y1右方向调试上移
上下Y1右方向调试下移:
	SETB	Y1轴右方向
	CLR	Y1脉冲输出标记
	LCALL	调试慢速计数运行
	SETB	Y1脉冲输出标记
	MOV	上下Y方向上一次接收数值H	, 上下Y方向接收数值H
	MOV	上下Y方向上一次接收数值L	, 上下Y方向接收数值L
	RET
上下Y1右方向调试上移:
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, 运行长度H
	MOV	R1, 运行长度L
 ;R4R3-R2R1=R3R2R1
	;LCALL	十六减十六正负标记     
	;-R2R1=R2R1
	LCALL	十六位负数转换成正数
	MOV	运行长度H, R2
	MOV	运行长度L, R1
上下Y1右方向调试上移2:

	CLR	Y1轴右方向
	CLR	Y1脉冲输出标记
	LCALL	调试慢速计数运行
	SETB	Y1脉冲输出标记
	MOV	上下Y方向上一次接收数值H	, 上下Y方向接收数值H
	MOV	上下Y方向上一次接收数值L	, 上下Y方向接收数值L
	RET
上下Y1右方向调试退出:
	RET
//**************上下Y1右方向调试动作END******************  

//************上下Y2左方向调试动作*********************** 
上下Y2左方向调试动作:
	MOV	R4, 上下Y方向接收数值H
	MOV	R3, 上下Y方向接收数值L
	MOV	R2, 上下Y方向上一次接收数值H
	MOV	R1, 上下Y方向上一次接收数值L
	LCALL	十六减十六正负标记
	JNB	数值为零标记	, 上下Y2左方向调试退出
	MOV	运行长度H, R2
	MOV	运行长度L, R1
	JNB	数值正负标记	, 上下Y2左方向调试上移
上下Y2左方向调试下移:
	SETB	Y2轴左方向
	CLR	Y2脉冲输出标记
	LCALL	调试慢速计数运行
	SETB	Y2脉冲输出标记
	MOV	上下Y方向上一次接收数值H	, 上下Y方向接收数值H
	MOV	上下Y方向上一次接收数值L	, 上下Y方向接收数值L
	RET
上下Y2左方向调试上移:
	MOV	R4, #0FFH
	MOV	R3, #0FFH
	MOV	R2, 运行长度H
	MOV	R1, 运行长度L
 ;R4R3-R2R1=R3R2R1
	;	LCALL	十六减十六正负标记

	;-R2R1=R2R1
	LCALL	十六位负数转换成正数
	MOV	运行长度H, R2
	MOV	运行长度L, R1
上下Y2左方向调试上移2:

	CLR	Y2轴左方向
	CLR	Y2脉冲输出标记
	LCALL	调试慢速计数运行
	

	SETB	Y2脉冲输出标记
	MOV	上下Y方向上一次接收数值H	, 上下Y方向接收数值H
	MOV	上下Y方向上一次接收数值L	, 上下Y方向接收数值L
	RET
上下Y2左方向调试退出:
	RET
//**************上下Y2左方向调试动作END******************

//***调试慢速计数运行*XY方向共用*******
//CLR			X方向    
//CLR	X脉冲输出标记       
调试慢速计数运行:


;	MOV	运行速度H	, #HIGH	调试快速度
	;MOV	运行速度L	, #LOW		调试快速度   
;LCALL	电机加减速运行 
 ;  ret 

	MOV	当前速度h	, #HIGH	调试点动速度
	MOV	当前速度L	, #LOW	调试点动速度
	MOV	a, 运行长度H
	JNZ	调试有加速运行
	MOV	a	, 运行长度L
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #100, 调试比较L结果不相同
调试比较L结果不相同:
	jc	调试无加速运行
调试有加速运行:
	MOV	运行速度H	, #HIGH	调试有加减速速度
	MOV	运行速度L	, #low	调试有加减速速度


	LCALL	电机加减速运行
	ret
调试无加速运行:
	MOV	当前长度H	, 运行长度H
	MOV	当前长度l	, 运行长度L
;	MOV	当前速度h,运行速度H	 
;	MOV	当前速度L,运行速度L 
	LCALL	计算当前速度转T0赋值
	LCALL	中断脉冲计数清零
调试点动运行中:
	CLR	脉冲输出总开关标记
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 调试点动运行中
	SETB	脉冲输出总开关标记
	SETB	长度减一标记
	LCALL	中断脉冲计数结束发送
	RET

 //*******调试慢速计数运行*END*********************

 //*******  运行中XY函数*********************

X向右零位距离运行:
	MOV	运行长度H	, X向右零位H
	MOV	运行长度L	, X向右零位L
	SETB	X轴方向
	LCALL	X轴方向运行
	RET
X轴A180固定距离运行:
	MOV	运行长度H	, A180长度H
	MOV	运行长度L	, A180长度L
	SETB	X轴方向
	LCALL	X轴方向运行
	RET

X轴B320运行:
	MOV	运行长度H, #HIGH	Y1Y2中心320距离	                         ;#320
	MOV	运行长度L, #LOW	Y1Y2中心320距离
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

工件F20端面检测运行: 
	
;lcall	工件F20端面检测     


	;MOV	端面检测距离H	, #HIGH	158;检测提前20距离
	;MOV	端面检测距离L	, #LOW	158	;检测提前20距离

	MOV	运行长度H,	端面检测距离h
	MOV	运行长度L, 端面检测距离L
	CLR	X轴方向
	LCALL	X轴方向运行
	RET



工件F20端面检测直接运行:    
	MOV	运行长度H, #HIGH	检测提前20距离
	MOV	运行长度L, #LOW	检测提前20距离
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
lcall		H180长度计算
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



;<--C260--<--B320--<------A180-------<--右零位--  
; --------D580----->-E80->-F20->-G70->
//*****长度计算***** 
每段长度计算:     
	MOV	端面检测距离H	, #HIGH	198;检测提前20距离
	MOV	端面检测距离L	, #LOW	198	;检测提前20距离
B320长度计算:
	MOV	B320长度H, #HIGH	Y1Y2中心320距离
	MOV	B320长度L, #LOW	Y1Y2中心320距离
F20长度计算:
	MOV	F20H	, #HIGH	检测提前20距离
	MOV	F20L	, #LOW	检测提前20距离    

	LCALL	A180长度计算
	LCALL	C260长度计算
	LCALL	D580长度计算
	LCALL	E80长度计算
	LCALL	H180长度计算
	RET

A180长度计算:
	MOV	R4, X向右零位至中心距离H
	MOV	R3, X向右零位至中心距离L
	MOV	R2, #HIGH	Y1Y2中心320距离	                                ;#320
	MOV	R1, #LOW	Y1Y2中心320距离
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	A180长度H	, R2
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
	MOV	R2, #HIGH	Y1Y2中心320距离	                                ;#320
	MOV	R1, #LOW	Y1Y2中心320距离
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
	MOV	R2, #HIGH	检测提前20距离
	MOV	R1, #LOW	检测提前20距离
//R4R3-R2,R1=R3R2R1 判断-数值为零标记-数值正负标记
	LCALL	十六减十六正负标记
	MOV	E80长度H	, R2
	MOV	E80长度L	, R1
	RET

F20端面检测距离检测:
  ;?????????????????
	MOV	端面检测距离H	, #HIGH	检测提前20距离
	MOV	端面检测距离L	, #LOW	检测提前20距离
	RET

H180长度计算:  
	MOV	R4, 工件端面至中心距离H    ;700
	MOV	R3, 工件端面至中心距离L
	MOV	R2, 端面检测距离H            ;200 		
	MOV	R1, 端面检测距离L
;R2,R1+R4R3=R3R2R1
	LCALL	十六加十六
	MOV	R4, E80长度H              	;800
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
	MOV	运行速度H	, 检测速度H	                                 //检测速度H	                          
	MOV	运行速度L	, 检测速度L	                                 //检测速度L
	LCALL	计算当前速度转T0赋值
工件端面检测中:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
	INC	DPTR
	JB	Y1右工件检测, 工件端面检测中
	SETB	X脉冲输出标记
	MOV	端面检测距离H	, DPH
	MOV	端面检测距离L	, DPL
	RET

;*********X,Y1,Y2轴方向运行:**************
X轴方向运行:
	MOV	运行速度H	, 左右运行速度H
	MOV	运行速度L	, 左右运行速度L
	CLR	X脉冲输出标记
	LCALL	电机加减速运行
	SETB	X脉冲输出标记
	RET
    ;????????????
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
  ;*****************机械手归零**开始************* 
机械手归零:   
lcall	发送RAM10H缓冲区帧头初始化
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片

	MOV	DPTR, #垂直方向归零TAB
	LCALL	发送提示字符
	LCALL	Y1Y2方向快速归零

	MOV	DPTR, #水平方向归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	X方向快速归零

	MOV	DPTR, #方向归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	X方向快速归零

	LCALL	XY1Y2方向精细归零

	MOV	DPTR, #开机复位完成TAB
	LCALL	发送提示字符
	LCALL	DY100MS
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET

X方向快速归零:
	MOV	当前速度H	, #HIGH	快速归零速度
	MOV	当前速度L	, #LOW	快速归零速度
	LCALL	计算当前速度转T0赋值
	CLR	脉冲输出总开关标记
X归零到位检查:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
X归零动作:
	JNB	X右限位信号	, X归零到位
	CLR	脉冲输出总开关标记
	CLR	X轴方向
	CLR	X脉冲输出标记
	JB	X右限位信号	, X归零到位检查
X归零到位:
	SETB	X脉冲输出标记
	setb	脉冲输出总开关标记
	RET


Y1Y2方向快速归零:
	MOV	当前速度H	, #HIGH	快速归零速度
	MOV	当前速度L	, #LOW	快速归零速度
	LCALL	计算当前速度转T0赋值
	CLR	脉冲输出总开关标记
Y1Y2归零到位检查:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
Y1归零动作:
	JNB	Y1右零位信号	, Y1归零到位
	CLR	Y1轴右方向
	CLR	Y1脉冲输出标记
	JB	Y1右零位信号, Y2归零动作
Y1归零到位:
	SETB	Y1脉冲输出标记
Y2归零动作:
	JNB	Y2左零位信号	, Y2归零到位
	CLR	Y2轴左方向
	CLR	Y2脉冲输出标记
	JB	Y2左零位信号, Y1Y2归零到位检查
Y2归零到位:
	SETB	Y2脉冲输出标记
	JB	Y1右零位信号	, Y1Y2归零到位检查
	JB	Y2左零位信号	, Y1Y2归零到位检查
	SETB	脉冲输出总开关标记
	RET

XY1Y2方向精细归零:
	MOV	当前长度H, #HIGH	200
	MOV	当前长度L, #LOW	200
	MOV	当前速度H	, #HIGH	精细归零速度
	MOV	当前速度L	, #LOW	精细归零速度
	LCALL	计算当前速度转T0赋值
精细归零进20MM运行中:
	CLR	脉冲输出总开关标记
	SETB	X轴方向
	CLR	X脉冲输出标记
	SETB	Y1轴右方向
	CLR	Y1脉冲输出标记
	SETB	Y2轴左方向
	CLR	Y2脉冲输出标记
精细进20MM运行中:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 精细进20MM运行中
	SETB	长度减一标记
	SETB	脉冲输出总开关标记
	LCALL	DY500MS
重新找零位:
	CLR	X轴方向
	CLR	Y1轴右方向
	CLR	Y2轴左方向
	CLR	脉冲输出总开关标记

重新找零位中:
	JB	T0中断4次标记	, $
	SETB	T0中断4次标记
Y1重新找零检查:
	JB	Y1右零位信号, Y2重新找零检查
	SETB	Y1脉冲输出标记
Y2重新找零检查:
	JB	Y2左零位信号	, X重新找零检查
	SETB	Y2脉冲输出标记
X重新找零检查:
	JB	X右限位信号	, XY重新找零判断
	SETB	X脉冲输出标记
XY重新找零判断:
	JB	Y1右零位信号, 重新找零位中
	JB	Y2左零位信号	, 重新找零位中
	JB	X右限位信号	, 重新找零位中
	SETB	脉冲输出总开关标记
	SETB	X轴方向
	SETB	X脉冲输出标记
	SETB	Y1轴右方向
	SETB	Y1脉冲输出标记
	SETB	Y2轴左方向
	SETB	Y2脉冲输出标记
	RET
 ;*****************机械手归零**END*************
	
;********电机加减速运行***开始***********************
电机加减速运行:
	LCALL	中断脉冲计数清零
	LCALL	判断运行长度小于极限值
	JNB	数值小于极限值标记	, 无加减速运行
	LCALL	判断速度大于总步数
	LCALL	计算S型步数速度总系数
	LCALL	计算S型加减速总步数
	LCALL	计算S型匀速长度
	CLR	脉冲输出总开关标记
	LCALL	计数S型加速XMM
	LCALL	计数S型匀速运行
	LCALL	计数S型减速XMM
	SETB	脉冲输出总开关标记
	SETB	X脉冲输出标记
	SETB	Y1脉冲输出标记
	SETB	Y2脉冲输出标记   
	LCALL	中断脉冲计数结束发送
	RET
无加减速运行:
	MOV	当前长度H, 运行长度H
	MOV	当前长度L, 运行长度L
	MOV	运行速度H	, #HIGH	无加减速赋值速度
	MOV	运行速度L	, #LOW	无加减速赋值速度
	MOV	当前速度H	, 运行速度H
	MOV	当前速度L	, 运行速度L
	LCALL	计算当前速度转T0赋值
无加速运行中:
	CLR	脉冲输出总开关标记
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 无加速运行中
	SETB	脉冲输出总开关标记
	SETB	长度减一标记
	RET
 ;******************************************************8888


中断脉冲计数结束发送:
	LCALL	DY5MS
	MOV	P_SW1	, #01H
	MOV	A	, DPH	                                               //24H
	MOV	T0脉冲数05H	, A  
	MOV	A, DPL	                                                ;										25H    
	MOV	T0脉冲数05L	, A  
	MOV	P_SW1	, #00H	                                          ; 选择DPTR0	
	CLR	串口1发送缓冲函数标记  
;MOV	发送缓冲计数	, #发送缓冲RAM首址
	SETB	TI
	LCALL	DY5MS
	RET

中断脉冲计数清零:
	MOV	P_SW1	, #01H	                                          ; 选择DPTR0
	MOV	DPTR, #00H
	MOV	P_SW1	, #00H	                                          ; 选择DPTR1 
	MOV	当前时间H, #00H
	MOV	当前时间L, #00H
MOV		T0脉冲数05H,#0
MOV		T0脉冲数05L	,#0
	MOV	脉冲100个计数, #脉冲计数个数
	LCALL	使能定时器T1
	RET  

计数匀速运行:
;;	LCALL	匀速运行长度计数
	MOV	当前长度H, 匀速运行长度H
	MOV	当前长度L, 匀速运行长度L
	MOV	当前速度H	, 运行速度H
	MOV	当前速度L	, 运行速度L
	LCALL	计算当前速度转T0赋值
运行长度动作:	                                              ///
	CLR	P4.5
	JB	T0中断4次标记	, $
	SETB	P4.5
	SETB	T0中断4次标记
	LCALL	长度减一
	JB	长度减一标记	, 运行长度动作
	SETB	长度减一标记
	RET

 //*********S型加减速编写测试动作开始******************* 
编写测试动作:
	CLR	缓存数据不自动读出标记	                                ; 不自动读
	MOV	显示画面, #测试画面1
	LCALL	DGUS切换图片
编写测试动作1:
	MOV	运行长度H	, X向右零位至中心距离H	                      //100:2000
	MOV	运行长度L	, X向右零位至中心距离L
	MOV	运行速度H	, 左右运行速度H
	MOV	运行速度L	, 左右运行速度L
	setb	X轴方向
	CLR	X脉冲输出标记
	LCALL	电机加减速运行
	LCALL	DY100MS
	cpl	X轴方向
	CLR	X脉冲输出标记
	LCALL	电机加减速运行 

	LCALL	DY1S

	LCALL	检查串有新接收数据并读出
	JNB	测试按钮标记, 编写测试动作1
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	SETB	缓存数据不自动读出标记

	RET
 //*********S型加减速编写测试动作开始******************* 

  //*********S型加减速开始******************* 

计数S型加速XMM:
	MOV	当前步数H, #00
	MOV	当前步数L, #00	                                        ;20
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
	;LCALL	计算当前速度转T0赋值
运行S型长度动作中:	                                         ///
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

;********************最小步数计算极限值 ************************888888

判断运行长度小于极限值:
	MOV	R4, 运行长度H
	MOV	R3, 运行长度L
	MOV	R2, #HIGH	最小步数计算极限值
	MOV	R1, #LOW	最小步数计算极限值
//R4R3-R2,R1=R3R2R1
	LCALL	十六减十六正负标记
	JB	数值正负标记	, 数值正常极限内
	CLR	数值小于极限值标记	                                    ;数值小于极限	
	RET
数值正常极限内:
	SETB	数值小于极限值标记
	RET

;*************判断速度放大系数总步数*****************************************
判断速度大于总步数:
;先判断运行长度小于极限值
	MOV	R4, 运行速度H	                                         ;  最高速度
	MOV	R3, 运行速度L
	MOV	R2, 步数十倍放大系数	                                  //*2
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八
	MOV	R5, #0
	MOV	R7, #0
	MOV	R6, #5	                                                ;10 
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4 
	LCALL	三十二除十六
	MOV	A, R2
	MOV	R1, A
	MOV	A, R3
	MOV	R2, A

	MOV	R4, 运行长度H
	MOV	R3, 运行长度L
//R4R3-R2,R1=R3R2R1
	LCALL	十六减十六正负标记
	JNB	数值正负标记	, 临时更改参数
	JNB	数值为零标记	, 临时更改参数
	LJMP	运行速度写入
	RET
临时更改参数:
   ; 运行速度= 运行长度*10/ 步数十倍放大系数/2次（加减速）
			;									= 运行长度*9/ 步数十倍放大系数/2
	MOV	R4, 运行长度H
	MOV	R3, 运行长度L
	MOV	R2, #9	                                                ;当正数时无  	
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八
	MOV	R5, #0
	MOV	A, 步数十倍放大系数
	RL	A	                                                      ;A*2
	MOV	R7, #0
	MOV	R6, A
//MOV	当前步数十倍放大系数     ，步数十倍放大系数
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4 
	LCALL	三十二除十六
	MOV	运行速度H	, R3	                                        ;  最高速度
	MOV	运行速度L	, R2
运行速度写入:
	MOV	A	, 运行速度H	                                         //24H
	MOV	R0, #0ACH
	MOVX	@R0	, A
	MOV	A, 运行速度L	                                          ;										25H    
	MOV	R0, #0ADH
	MOVX	@R0	, A
	RET



  //*********S型加减速子函数*******************   

	//	3. 注意 ;  运行长度小于 加速步数.减速步数的和时  ,匀速运行步数为负数造成错误 
	//4.      运行速度设置为  运行长度的1/2 ,匀速运行长度 =  低速#2

计算S型当前速度并赋值:
;1.当前步数速度=当前步数/步数速度总系数<当前（最高）速度/100(正弦值等分数)* 步数十倍放大系数 >
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
 ;被除数R3.R2.	除数R1				商R3,R2	余数R1   
//MOV R4,   #0      ; 被除数高8位
//MOV	R1, 步数速度总系数
//LCALL	十六除八错误
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
;01A5A180  = 276320*100
	MOV	R5, #01H
	MOV	R4, #0A5H
	MOV	R3, #0A1H
	MOV	R2, #80H
 /*
 276320*50
	MOV	R5, #00H
	MOV	R4, #0D2H
	MOV	R3, #0D0H
	MOV	R2, #0C0H
   */
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
        

计算S型加减速总步数:
	MOV	R4, 运行速度H	                                         ;  最高速度
	MOV	R3, 运行速度L
	MOV	R2, 步数十倍放大系数
//	R4R3*R2	=R4R3R2<R7R6R5>
	LCALL	十六乘八
	MOV	R5, #00
	;R4R3R2
	MOV	R7, #0	                                                ; 除数
	MOV	R6, #10
;被除数	R5,R4,R3.R2.	除数R7,R6				商R3,R2	余数R5,R4   
   ;500/100*10
	LCALL	三十二除十六
	MOV	加减速总步数H, R3
	MOV	加减速总步数L, R2
	RET


	//	1.计数匀速运行 = 运行长度-  计数加速XMM- 计数减速XMM 
计算S型匀速长度:
	MOV	R4, 加减速总步数H
	MOV	R3, 加减速总步数L
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
	RET


计算S型步数速度总系数:
	;MOV	 运行速度H	, #HIGH		450;  最高速度
	;MOV		 运行速度L, #LOW			450  	
 ;MOV  步数十倍放大系数 , #20   

;步数速度总系数=当前（最高）速度/100(正弦值等分数)* 步数十倍放大系数   
	;= 400MM/S除100乘1.2     
	;= 400MM/S乘12 除100  = 48 (多10倍
	MOV	R4, 运行速度H	                                         ;  最高速度
	MOV	R3, 运行速度L
	MOV	R2, 步数十倍放大系数
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
	DB	0EH, 0EH, 0EH, 0EH, 0EH, 0FH, 0FH, 0FH, 0FH, 010H
	DB	010H, 011H, 011H, 012H, 012H, 013H, 013H, 014H, 015H, 015H
	DB	016H, 017H, 018H, 019H, 01AH, 01BH, 01CH, 01DH, 01EH, 01FH
	DB	020H, 021H, 022H, 023H, 024H, 025H, 027H, 028H, 029H, 02AH
	DB	02CH, 02DH, 02EH, 030H, 031H, 032H, 034H, 035H, 036H, 038H
	DB	039H, 03AH, 03CH, 03DH, 03EH, 040H, 041H, 042H, 044H, 045H
	DB	046H, 048H, 049H, 04AH, 04BH, 04DH, 04EH, 04FH, 050H, 051H
	DB	052H, 053H, 054H, 055H, 056H, 057H, 058H, 059H, 05AH, 05BH
	DB	05CH, 05DH, 05DH, 05EH, 05FH, 05FH, 060H, 060H, 061H, 061H
	DB	062H, 062H, 063H, 063H, 063H, 063H, 064H, 064H, 064H, 064H
	DB	064H, 064H, 064H, 064H, 064H, 063H, 063H, 063H, 063H, 062H



	DB	007H, 007H, 007H, 007H, 007H, 009H, 00AH, 00BH, 00CH, 00CH
	DB	00CH, 00DH, 00DH, 00EH, 00EH, 00FH, 010H, 010H, 011H, 012H
	DB	013H, 013H, 014H, 015H, 016H, 017H, 018H, 019H, 01AH, 01BH
	DB	01DH, 01EH, 01FH, 020H, 021H, 023H, 024H, 025H, 026H, 028H
	DB	029H, 02AH, 02CH, 02DH, 02FH, 030H, 031H, 033H, 034H, 036H
	DB	037H, 038H, 03AH, 03BH, 03DH, 03EH, 03FH, 041H, 042H, 044H
	DB	045H, 046H, 048H, 049H, 04AH, 04BH, 04DH, 04EH, 04FH, 050H
	DB	051H, 053H, 054H, 055H, 056H, 057H, 058H, 059H, 05AH, 05BH
	DB	05BH, 05CH, 05DH, 05EH, 05EH, 05FH, 060H, 060H, 061H, 061H
	DB	062H, 062H, 063H, 063H, 063H, 063H, 064H, 064H, 064H, 064H
	DB	064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H
	DB	064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H, 064H
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
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 加减速总步数H, 加速步数比较H结果不相同
加速步数比较H结果相同:
	MOV	A, 当前步数L
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
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
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #00, 减速步数比较H结果不相同
减速步数比较H结果相同:
	MOV	A, 当前步数L
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #00, 减速步数比较L结果不相同	                      //	20
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
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 运行速度H, 加速比较H结果不相同
加速比较H结果相同:
	MOV	A, 当前速度L
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, 运行速度L, 加速比较L结果不相同
加速比较L结果相同:
	CLR	当前速度为零标记
	RET
加速比较H结果不相同:
加速比较L结果不相同:
	RET

计算当前速度转T0赋值:	                                      ;无加减速运行时应用
	;细分2000/转,5M*20齿=100MM/转,	脉冲数=2000
	;运行速度10MM/S=>200脉冲数/S=>定时值10/20/2(高低电平）=2500US
	;定时器赋值				65536-2500*11.0592=65536-276480
	;计算运行速度30MM/S时	=>65536-(276480/30)
//A,MOV					当前速度	,运行速度		(300MM/S)
//B,赋值	65536-27648/速度(300MM/S)
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
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #0, 比较H结果不相同
比较H结果相同:
	MOV	A, 当前长度L
;A=(值)顺序执行<CY=0>	,A>(值)跳转<CY=0>,A<(值)	跳转<CY=1>
	CJNE	A, #0, 比较L结果不相同
比较L结果相同:
	CLR	长度减一标记
	RET
比较H结果不相同:
比较L结果不相同:
	RET

;********************************************8




EEPROM初始化:
	MOV	显示画面, #数据初始化画面
	LCALL	DGUS切换图片
	LCALL	DY500MS
;5A	A5	15(#3+X)	82	00	00	0001 0203 0405 0607	

	MOV	发送帧头5A	, #05AH                                     ;EQU	10H
	MOV	发送帧头A5	, #0A5H                                     ;EQU	11H
	MOV	发送数据个数	, #17;#13                                     ;EQU	12H
	MOV	发送指令82	, #82H                                      ;EQU	13H
	MOV	发送数据首址H, #HIGH	00H	                              ;	EQU	14H
	MOV	发送数据首址L, #LOW	0BH	                               ;EQU	15H
   ;  000BH
	MOV	T0脉冲数05H	, #0H                                      ;EQU	16H
	MOV	T0脉冲数05L	, #0H                                      ;EQU	17H
     ;000C  
	MOV	当前时间H	, #0H                                        ;EQU	18H
	MOV	当前时间L	, #0H                                        ;EQU	19H
		    ;000D   
	MOV	运行速度H	, #0H                                        ;EQU	1AH
	MOV	运行速度L, #0H                                         ;	EQU		1BH
  ;000E	                 		
	;通讯脉冲数H	EQU    1CH
 ;通讯脉冲数L	EQU  	 1DH
	;000F
	MOV	运行长度H	, #0H                                        ;EQU	1EH
	MOV	运行长度L	, #0H                                        ;EQU		1FH
;	*************************************88

;0010
	;	EQU	20H
	;	EQU	21H
;0011
	MOV	传感器标记11H	, #HIGH	0FFFFH	                        ;	22H
	MOV	传感器标记11L	, #LOW	0FFFFH	                         ;		23H	                                   //	主板P1口OUT标记
;0012
	MOV	临时标记12H	, #HIGH	0FFFFH	                              ;	24H
	MOV	临时标记12L	, #LOW	0FFFFH
 ;**************************************
;0013
	MOV	临时标记13H	, #HIGH	0FFFFH
	MOV	临时标记13L	, #LOW	0FFFFH
;0014
	MOV	按扭标记14H	, #HIGH	0FFFFH	                              ;	2CH	                                         //			开机不置1
	MOV	按扭标记14L, #LOW	0FFFFH
;0015
	MOV	保存标记15H	, #HIGH	0FFFFH	                            ;	2AH
	MOV	保存标记15L		, #LOW	0FFFFH
	//	以下保存参数     
;0016
	MOV	X向右零位H, #HIGH	300                                   ;	EQU	2CH	                                         //			开机不置1
	MOV	X向右零位L	, #LOW	300                                   ;		EQU	2DH
;0017
	MOV	Y向取物料长距离H	, #HIGH	800                           ;EQU	2EH
	MOV	Y向取物料长距离L	, #LOW	800                            ;	EQU	2FH
;0018     Y3
	MOV	Y向两头取放距离H	, #HIGH	800	                          ;	30H     	
	MOV	Y向两头取放距离L	, #LOW	800	                           ;	31H
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
	//MOV	低速H	, #HIGH	20	                                      ;	3CH
	MOV	步数十倍放大系数	, #15	                                ;	3DH
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
	MOV	型号更改值	, #2                                       ;	44H
	MOV	型号当前值	, #2                                  ;	45H
;0023
	MOV	备用23H	, #HIGH	00	                                    ;	46H
	MOV	备用23L	, #LOW	00	                                     ;	47H 

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
	MOV	脉冲100个计数	, #脉冲计数个数
	MOV	T0中断4次计数, #T0中断脉冲次数	                        //4   4对应2000细分数，8/4000
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
	ORL	PSW, #00001000	                                        ;R0-R7用第二组08H-0FH
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
	DJNZ	T0中断4次计数	, 中断T0结束
	CLR	T0中断4次标记	                                         //4 
	MOV	T0中断4次计数, #T0中断脉冲次数	                        //4   4对应2000细分数，8/4000
TO脉冲计数并写缓冲发送:
        
	MOV	P_SW1	, #01H	                                          ; 选择DPTR0
	INC	DPTR
	MOV	P_SW1	, #00H
	DJNZ	脉冲100个计数	, 中断T0结束
	MOV	脉冲100个计数	, #脉冲计数个数

	MOV	P_SW1	, #01H
	;MOV	A	, DPH	                                               //24H
	;MOV	R0, #0AAH
;	MOVX	@R0	, A   

;	MOV	A, DPL	                                                ;										25H    
	;MOV	R0, #0ABH
	;MOVX	@R0	, A 
	
	MOV	T0脉冲数05H	, DPH   
	MOV	T0脉冲数05L	, DPL
	MOV	P_SW1	, #00H	                                          ; 选择DPTR0	

	CLR	串口1发送缓冲函数标记    
MOV	发送缓冲计数	, #发送缓冲RAM首址
	SETB	TI

中断T0结束:
     

	POP	PSW
	POP	ACC


	RETI
//  	脉冲输出总开关标记(至1时X,Y1,Y2,都不输出脉冲）
//			X,Y1,Y2脉冲输出标记 ( 至0时对应单独输出脉冲）

	//***开始****算数运算*********

/*
十六加1: 样
INC 当前时间L             
MOV A, 当前时间L          ; 将R2的值加载到累加器A
JNZ NOCARRY16       			; 如果A不为0,跳转到NO_CARRY
INC 当前时间H            
NOCARRY16:
RET
   */
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
;A=(值)顺序执行<CY=0> ,A>(值)跳转<CY=0>,A<(值) 跳转<CY=1>
	CJNE	R2, #0, 比较R1R2结果不相同
;A=(值)顺序执行<CY=0> ,A>(值)跳转<CY=0>,A<(值) 跳转<CY=1>
	CJNE	R1, #0, 比较R1R2结果不相同
	CLR	数值为零标记
	RET
比较R1R2结果不相同:
	SETB	数值为零标记
	RET

;-R2R2=R2R1
十六位负数转换成正数:
	MOV	A, R2
	JNB	ACC.7, 换成正数结束
换成正数:
	MOV	A, R1	                                                 ; 加载负数的低字节
	CPL	A	                                                     ; 取反
	ADD	A, #01H	                                               ; 加1
	MOV	R1, A	                                                 ; 保存结果低字节

	MOV	A, R2	                                                 ; 加载负数的高字节
	CPL	A	                                                     ; 取反
	ADDC	A, #00H	                                              ; 加上低字节的进位
	MOV	R2, A	                                                 ; 保存结果高字节
换成正数结束:
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
十六除八:
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
	JC	NO_SUB	                                                 ; 如果借位（余数 < 除数）,跳过减法

	MOV	R4, A	                                                 ; 存回新的余数
	INC	R2	                                                    ; 商低8位加1
	MOV	A, R2	                                                 ; 检查低8位是否溢出
	JNZ	NO_CARRY	                                              ; 如果没有溢出,跳过
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
	MOV	R3, #0
	MOV	R2, #0
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


;写型号IAPEEPROM
;读型号IAPEEPROM
//3.015W4K48S2<EEPROM地址IAP/0000H-27FFH(MOVC/C000H-E7FFH) 20个扇区 1-0000H-02FFH	2-0300H-04FFH....
//3.11 第1扇区 存储;型号0当前使用参数，第2扇区 存储型号1使用参数,2-2,3-3 .....。
	;型号0  	第1扇区   	 IAP/0000H      		MOVC 0C000H
	;型号1 	 第2扇区     IAP/0200H 	      MOVC 0C200H		
	;型号2   第3扇区     IAP/0400H        MOVC 0C400H 		
	;型号3   第4扇区     IAP/0600H        MOVC 0C600H 	


// 型号IAP地址 =   (型号*2)H ,00L
// 型号MOVC地址 =  <(型号*2)+C0>H ,00L
读8型号IAPEEPROM初始化测试:
	MOV	型号当前值, #8
读8型号:
	LCALL	读型号IAPEEPROM
	LCALL	发送2CH至4FH到触摸屏
	LCALL	DY100MS
	LCALL	发送当前型号注解
	LCALL	DY1S
	DJNZ	型号当前值, 读8型号
	RET
写型号IAPEEPROM初始化测试:
写型号IAPEEPROM初始化:
	MOV	型号当前值, #0
	MOV	型号更改值, 型号当前值
写8型号:
	INC	工件端面至中心距离L
	INC	型号当前值
	MOV	型号更改值, 型号当前值
	LCALL	写型号IAPEEPROM
	MOV	A, 型号当前值
	CJNE	A, #8, 写8型号
	RET


读型号IAPEEPROM:
;	MOV	DPTR	, #IAP保存首址   
//  型号IAP首址 = 00H+(型号*2),00L=0(2X)00H
	LCALL	型号IAP首址转换
	MOV	R0, #待存首址
	MOV	R2, #保存个数
IAPEEPROM2:
	LCALL	读IAP数据
	MOV	@R0	, A
	INC	R0
	INC	DPTR
	DJNZ	R2, IAPEEPROM2
	RET



;读型号IAPEEPROM

写型号IAPEEPROM:
;	MOV	DPTR	, #IAP保存首址   
//  型号IAP首址 =   (型号*2)H+00H ,00L
	LCALL	型号IAP首址转换
	LCALL	扇区擦除	                                            //扇区擦除
;-------------------------------
;	MOV	DPTR	, #IAP保存首址   
//  型号IAP首址 =   (型号*2)H+00H ,00L
	LCALL	型号IAP首址转换
	MOV	R0, #保存个数	                                         //16																					//检测512字节																													//检测512字节
IAPCHECK1:	                                                 //检测是否擦除成功(全FF检测)
	LCALL	读IAP数据	                                           //读IAP数据
	CJNE	A, #0FFH, 写型号IAPEEPROM	                            //如果出错,则退出
	INC	DPTR	                                                  //IAP地址+1
	DJNZ	R0, IAPCHECK1
							;擦除成功
;-------------------------------
;	MOV	DPTR	, #IAP保存首址   
//  型号IAP首址 =   (型号*2)H+00H ,00L
	LCALL	型号IAP首址转换
	MOV	R0, #保存个数	                                         //16																							//编程512字节
	MOV	R1, #待存首址	                                         ;;;;;;;;---------------
IAPNEXTZ:	MOV	A, @R1	                                       //准备数据
	LCALL	字节编程	                                            //字节编程
	INC	DPTR	                                                  //IAP地址+1
	INC	R1	                                                    //修改保存数据
	DJNZ	R0, IAPNEXTZ
	RET

	;*******************************************************
//  型号IAP首址 =   (型号*2)H+00H ,00L
型号IAP首址转换:
	MOV	A, 型号当前值
	MOV	B	, #2
	MUL	AB
	MOV	DPH	, A	                                               ; #HIGH	IAP保存首址 
	MOV	DPL	, #LOW	IAP保存首址
	RET
;******************************************************  

   
型号注解第10扇区读出保存测试:
	LCALL	读变量地址0100h数据	                                 //2503   
	LCALL	DY100MS
	LCALL	型号注解SRAM保存到第10扇区
	LCALL	DY100MS
	LCALL	型号注解第10扇区读出到SRAM
	LCALL	DY100MS
	LCALL	发送型号表格注解
	LCALL	DY100MS
	LCALL	发送当前型号注解
	RET



;型号注解SRAM保存到第10扇区
型号注解第10扇区读出到SRAM:
	;型号注解IAP首址 =100+型号IAP首址<(型号*2)H+00H ,00L >
;读EEPORM		DPTR  	
	MOV	P_SW1	, #01H
	MOV	DPTR	, #1200H	                                         ;(EEPROM地址0100H)IAP型号注解保存首址
;保存SRAM	DPTR      
	MOV	P_SW1	, #00H
	MOV	DPTR, #100H	                                           ; #型号注解SRAM保存首址
	MOV	P_SW1	, #01H
	MOV	R0, #8FH	                                              ;#保存个数	
型号注解读到SRAM:
	LCALL	读IAP数据
	MOV	P_SW1	, #00H
	MOVX	@DPTR, A
	INC	DPTR
	MOV	P_SW1	, #01H
	INC	DPTR
	DJNZ	R0, 型号注解读到SRAM
	RET


型号注解SRAM保存到第10扇区:
	MOV	DPTR, #1200H
	LCALL	扇区擦除

;检测是否擦除成功(全FF检测)
	MOV	DPTR, #1200H
	MOV	R0, #8FH	                                              //16																					//检测512字节																													//检测512字节
IAPCHECK10:	                                                //检测是否擦除成功(全FF检测)
	LCALL	读IAP数据	                                           //读IAP数据
	CJNE	A, #0FFH, 型号注解SRAM保存到第10扇区	                 //如果出错,则退出
	INC	DPTR	                                                  //IAP地址+1
	DJNZ	R0, IAPCHECK10
     
	;型号注解IAP首址 =100+型号IAP首址<(型号*2)H+00H ,00L >
;保存EEPORM		DPTR  	 
	MOV	P_SW1	, #01H
	MOV	DPTR	, #1200H	                                         // #IAP型号注解保存首址
;读SRAM	DPTR      
	MOV	P_SW1	, #00H
	MOV	DPTR, #100H	                                           ;	 #型号注解SRAM保存首址
	MOV	P_SW1	, #01H
	MOV	R0, #8FH	                                              ;#保存个数	
型号注解写入EEPROM:
	MOV	P_SW1	, #00H
	MOVX	A	, @DPTR
	INC	DPTR
	MOV	P_SW1	, #01H
	LCALL	字节编程
	INC	DPTR
	DJNZ	R0, 型号注解写入EEPROM
	RET


	;************************************** 

//***开始*****写EEPROM读EEPROM	******
读EEPROM:
	MOV	DPTR	, #MOVC保存首址	                                  //#0F000H
	MOV	R0, #待存首址
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
	MOV	DPTR, #IAP保存首址	                                    //设置ISP/IAP/EEPROM地址
	LCALL	扇区擦除	                                            //扇区擦除
;-------------------------------
	MOV	DPTR, #IAP保存首址	                                    //设置ISP/IAP/EEPROM地址
	MOV	R0, #保存个数	                                         //16																					//检测512字节																													//检测512字节
CHECK1:	                                                    //检测是否擦除成功(全FF检测)
	LCALL	读IAP数据	                                           //读IAP数据
	CJNE	A, #0FFH, 写EEPROM	                                   //如果出错,则退出
	INC	DPTR	                                                  //IAP地址+1
	DJNZ	R0, CHECK1
							;擦除成功
;-------------------------------
	MOV	DPTR, #IAP保存首址	                                    //设置ISP/IAP/EEPROM地址
	MOV	R0, #保存个数	                                         //16																							//编程512字节
	MOV	R1, #待存首址	                                         ;;;;;;;;---------------
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
	JNB	P3.2, $
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


//***开始*****@11.0592MHZ串口参数*****
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
	   
SETB		串口1发送缓冲函数标记
	MOV	接收计数, #0
	;MOV	发送缓冲计数	, #发送缓冲SRAM首址  
	MOV	发送缓冲计数	, #发送缓冲RAM首址
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
	ORL	PSW, #00001000	                                        ;R0-R7用第二组08H-0FH
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
;每次CLR	串口1发送缓冲函数标记SETB	TI  启动发送  

;
	JB	串口1发送缓冲函数标记	, 结束UART1中断
缓存A0数据读出发送:
	MOV	R1, 发送缓冲计数	                                      ; #发送缓冲RAM首址++
	MOV	A, @R1
	MOV	SBUF, A	                                               ;发送一个字节                           
	INC	发送缓冲计数
	MOV	A, R1
	CJNE	A, 发送缓冲数据总个数	, 结束UART1中断
	MOV	发送缓冲计数, #发送缓冲RAM首址
	SETB	串口1发送缓冲函数标记

结束UART1中断:
	POP	ACC
	POP	PSW
	RETI
;**************发送缓冲地址数据****************
 
发送RAM10H缓冲区帧头初始化:
	MOV	发送帧头5A	, #5AH
	MOV	发送帧头A5	, #0A5H
	MOV	发送数据个数, #17;#13
	MOV	发送指令82	, #82H
	MOV	发送数据首址H	, #00H
	MOV	发送数据首址L	, #0BH                                   ;16H    
;		MOV	发送缓冲数据总个数	= #15+#发送缓冲RAM首址
	MOV	A	, #发送缓冲RAM首址
	MOV	R1, 发送数据个数
	ADD	A, R1
	ADD	A, #2	                                                 //(6-1)    
	MOV	发送缓冲数据总个数	, A  
	MOV	发送缓冲计数	, #发送缓冲RAM首址
	RET
;*****RAM10H/SRAMA0H***二选一********** 
;******UART1串口中断自动发送用   
/*  
	; 由于 发送缓冲计数从#发送缓冲RAM首址（值）开始加数
;	所以	发送缓冲数据总个数 =  发送缓冲RAM首址+2+发送数据个数
	JB	串口1发送缓冲函数标记	, 结束UART1中断
缓存A0数据读出发送:
	MOV	R1, 发送缓冲计数
	MOVX	A, @R1	   ;用SRAM地址时用 
	MOV	SBUF, A	                                               ;发送一个字节                           
	INC	发送缓冲计数
	MOV	A, R1
	CJNE	A, 发送缓冲数据总个数	, 结束UART1中断
	MOV	发送缓冲计数, #发送缓冲SRAM首址
	SETB	串口1发送缓冲函数标记
;结束UART1中断:
;	POP	ACC
;	POP	PSW
;	RETI
;******SRAMA0H--SRAMBF******** 
;5A	A5	15(#3+X)	82	00	00	0001 0203 0405 0607	 


//MOV	STC数据首址L	, #20H
//MOV	发送缓冲数据个数, #8H
发送SRAM0A0H缓冲区数据帧头初始化:
	;发送缓冲SRAM首址  DATA	 #0A0H  
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
	MOV	A, STC数据首址L
	RR	A	                                                      //40/2=0020
	MOVX	@R0, A	                                               ;变量首址																																									//#00H
	INC	R0
	MOV	R1, STC数据首址L	                                      ;发送地址20H
	MOV	A, 发送缓冲数据个数	                                   ;发送长度16
	ADD	A, #3
	MOV	R2, A
写XX变量发送缓冲:
	MOV	A, @R1
	MOVX	@R0, A
	INC	R1
	INC	R0
	DJNZ	R2, 写XX变量发送缓冲
	
发送缓冲数据总个数计算:
	MOV	A	, #发送缓冲SRAM首址
	MOV	R1, 发送缓冲数据个数
	ADD	A, R1
	ADD	A, #5	                                                 //(6-1)    
	MOV	发送缓冲数据总个数	, A
	RET  
	*
	*/
;******RAM10H/SRAMA0H***二选一********** 


;*********************	UART2中断函数************************ 

UART2串口中断:
	PUSH	PSW
	PUSH	ACC
	ORL	PSW, #00001000	                                        ;R0-R7用第二组08H-0FH
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

;定时器T1用于10毫秒 及串口接收中断判断
使能定时器T1:	                                              ;10毫秒@11.0592MHZ
	  ;10毫秒@11.0592MHz
	ANL	AUXR, #0BFH	                                           ;定时器时钟12T模式
	ANL	TMOD, #0FH	                                            ;设置定时器模式
	MOV	TL1, #000H	                                            ;设置定时初始值
	MOV	TH1, #0DCH	                                            ;设置定时初始值
	CLR	TF1	                                                   ;清除TF1标志
	SETB	TR1	                                                  ;定时器1开始计时
	SETB	ET1
	//MOV	定时1秒计时	, #20  
	RET
	     

; 串口接收开始计时函数当 每次串口接收中断时重装初始值开始计时
;	无 串口接收中断 时定时器产生中断
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

检查串有新接收数据并读出:
	JB	串口有未读数据标记	, 无新收数据
	LCALL	接收缓存数据读出
	SETB	串口有未读数据标记
			;	接收数据读完标记      
无新收数据:
	RET


中断T1:
	PUSH	PSW
	PUSH	ACC
	PUSH	B
	ORL	PSW, #00001000	                                        ;R0-R7用第二组08H-0FH
串口1接收结束检测:
	JB	串口接收中断标记	, 串口2接收结束检测
	CLR	TR1
	SETB	串口接收中断标记
	MOV	接收计数, #0
	JNB	缓存数据不自动读出标记, 缓存数据不自动读出
	LCALL	接收缓存数据读出
缓存数据不自动读出:
	SETB	串口接收中断标记
	CLR	串口有未读数据标记	                                    //空闲时  接收缓存数据读出    


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
	JB	脉冲输出总开关标记, 中断T1结束
	;CPL	    T1计时1S标记
;JB	  T1计时1S标记, 中断T1结束
;INC			定时1秒计时
//	JB	串口1发送缓冲函数标记	, 中断T1结束
//******************** 
;十六加一
;H.L 加1
	INC	当前时间L
	MOV	A, 当前时间L	                                          ; 将R2的值加载到累加器A
	JNZ	NOCARRY16	                                             ; 如果A不为0,跳转到NO_CARRY
	INC	当前时间H
NOCARRY16:

中断T1结束:
	SETB	TR1

	POP	B
	POP	ACC
	POP	PSW
	RETI



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
	MOVX	A, @R1	                                               ;DGUS变量首址H                                    //01变量地址H		(16位转8位
	MOV	STC数据首址H, A	                                       //数值为100
缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               ;DGUS变量首址L             				           //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	STC数据首址L, A
缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               ; DGUS变量个数                                          //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	STC数据个数, A
缓存07后数据读出:
	MOV	DPH	, STC数据首址H
	MOV	DPL	, STC数据首址L	                                    //	DGUS0010H=STC20H                                              														 //;接收地址20-2F
	MOV	R2, STC数据个数	                                       ;		接收数据长度

	MOV	A, STC数据首址H
	JNZ	有帧头16位接收数据
	MOV	A, STC数据首址L	                                       //	DGUS0010H=STC20H
	MOV	R0, A

有帧头接收数据:
	INC	R1
	MOVX	A, @R1
	MOV	@R0, A
	INC	R0
	DJNZ	R2, 有帧头接收数据
有帧头接收结束:
	CLR	接收数据读完标记
	RET
有帧头16位接收数据:
	INC	R1
	MOVX	A, @R1
	MOVX	@DPTR	, A
	INC	DPTR
	DJNZ	R2, 有帧头16位接收数据

缓存数据错误:
	RET
缓存数据非5A:
	RET



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
	MOV	DGUS变量首址H, A	                                      //数值为00
付板缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量首址L, A
付板缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量个数	, A
付板缓存07后数据读出:
	MOV	A, DGUS变量首址L	                                      //	DGUS0010H=STC20H
	MOV	R0, A	                                                 //;接收地址20-2F
	MOV	R2, DGUS变量个数	                                      ;		接收数据长度

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

//****************单片机之间通讯******************************


;A5	5A	06<4+(发送个数*2>	83	1001	01(发送个数)	0002
发送数据到付板:
				;	MOV			STC数据首址L	,#10H
					;MOV			STC数据个数,#1(16位）
				;LCALL		发送数据到触摸屏
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #05AH
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据个数	                                        ;发送个数
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
	MOV	SBUF, #00H	                                            ;	STC数据首址H
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据首址L
	RR	A	                                                      //40/2=0020
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据个数	                                        ;发送个数	/	16位个数
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	MOV	R0, STC数据首址L	                                      ;发送地址20H
	MOV	A, STC数据个数
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
	RET
			///////付板/////////////////////

;	5AA5		06=4+(发送个数*2>	83	1001		01(发送个数)	0002
发送0011H22H23H到付板:
;20-5FH=>0000-001FH
	MOV	STC数据首址L, #22H
	MOV	STC数据个数, #1
	LCALL	发送数据到付板
	RET
发送20H21H到主板:	                                          ;付板				用
;20-5FH=>0000-001FH
	MOV	STC数据首址L, #20H
	MOV	STC数据个数, #1
	LCALL	发送数据到付板
	RET
////////付板END//////////////////////////////

;5AA5	<013(#3+X)	82			00	00		>	0060-006F
发送数据到触摸屏:
				;	MOV			STC数据首址L	,#00H
					;MOV			发送个数,#32
				;LCALL		发送数据到触摸屏
	MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	A, #3
	ADD	A, STC数据个数	                                        ;发送个数
	MOV	SBUF, A	                                               //#19
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #82H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0	                                              ;STC数据首址H
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据首址L
	RR	A	                                                      //40/2=0020
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	R0, STC数据首址L	                                      ;发送地址20H
	MOV	R2, STC数据个数	                                       //#32										;发送长度16
写XX变量发送中:
	MOV	A, @R0
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	R0
	DJNZ	R2, 写XX变量发送中
	RET





	//***改*202409****@11.0592MHZ串口参数*****
;5AA5	013(#4+X)	<82			00	00			0060-006F 

发送20H至5FH到触摸屏:
发送32个数据:
;20-5FH=>0010-0030H
	MOV	STC数据首址L, #20H
	MOV	STC数据个数, #64
	LCALL	发送数据到触摸屏
	RET
发送2CH至4FH到触摸屏:
发送34个数据:
;20-5FH=>0010-0030H
	MOV	STC数据首址L, #2CH
	MOV	STC数据个数, #34
	LCALL	发送数据到触摸屏
	RET
发送30触摸屏:
	MOV	STC数据首址L, #30
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET

发送20H至24H到触摸屏:
;20-5FH=>0000-001FH
	MOV	STC数据首址L, #20H
	MOV	STC数据个数, #4
	LCALL	发送数据到触摸屏
	RET   



发送刷新8数据:
	MOV	STC数据首址L, #16H
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET

发送传感器信号标记触摸屏:
	MOV	STC数据首址L, #传感器标记11H
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET
发送按扭标记到触摸屏:
	MOV	STC数据首址L, #按扭标记14H
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET










发送型号值到触摸屏:
	MOV	STC数据首址L, #44H
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET
发送XY当前数值到触摸屏:
	MOV	STC数据首址L, #48H
	MOV	STC数据个数, #4
	LCALL	发送数据到触摸屏
	RET
发送长度到触摸屏:
	MOV	STC数据首址L, #50H
	MOV	STC数据个数, #18
	LCALL	发送数据到触摸屏
	RET
发送单个数据到触摸屏:
		//MOV	STC数据首址L, #3H 
	MOV	A, STC数据首址L
	MOV	B, #2
	DIV	AB
	JZ	地址位为高位正常发送
	MOV	A, STC数据首址L
	DEC	A
	MOV	STC数据首址L	, A
地址位为高位正常发送:
	MOV	STC数据个数, #2
	LCALL	发送数据到触摸屏
	RET



       	//***改*202503新增****@11.0592MHZ串口参数***** 
//3.20	15W4K48S2<SRAM地址常规00H-FFH扩展OOOOH-0EFFH总共4K,读指令MOVX	@DOTR, MOVX		@Ri
//3.3.1	型号注解SRAM首址	0100H-018FH(9*8*2个字节    
//3.3.2	型号0:0100H-010FH(16个字节)触摸屏0100H-0108H(8*2=16个字节)	  
//3.3.4	型号1:0110H-011FH(16个字节)触摸屏0108H-0110H(8*2=16个字节)  
//3.3.5	型号0	当前型号用,	0100H-018F  统一发送，发送时STC数据首址H不除2需注意 
;0100H-018F  统一发送，发送时STC数据首址H不除2需注意 

;************发送扩展数据到触摸屏***************************************88

	;5AA5	<013(#3+X)	82			00	00		>	0060-006F
发送扩展数据到触摸屏:
				;MOV			STC数据首址H	,#01H
				;MOV			STC数据首址L	,#00H
					;MOV		STC数据个数,#32    ;不能大于255-3
				;LCALL		发送扩展数据到触摸屏
		MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	A, #3
	ADD	A, STC数据个数	                                        ;发送个数
	MOV	SBUF, A	                                               //#19
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #82H
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据首址H
 ;MOV	B, #2
	;DIV	AB            					;0100H*2						
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	MOV	A, STC数据首址L
	MOV	B, #2
	DIV	AB	                                                    //40/2=0020
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	DPH, STC数据首址H
	MOV	DPL, STC数据首址L	                                     ;发送地址20H
	MOV	R2, STC数据个数	                                       //#32										;发送长度16
十六位变量发送中:
	MOVX	A, @DPTR
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	DPTR
	DJNZ	R2, 十六位变量发送中
	RET




;********发送当前型号0注解**转用***** 01X0到0100触摸屏
;5AA5	<013(#3+X)	82			00	00		>	0060-006F
发送当前型号注解到触摸屏:
				;MOV			STC数据首址H	,#01H
				;MOV			STC数据首址L	,#00H
					;MOV		STC数据个数,#32    ;不能大于255-3
				;LCALL		发送扩展数据到触摸屏
	MOV	SBUF, #5AH
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #0A5H
	JNB	TI, $
	CLR	TI
	MOV	A, #3
	ADD	A, STC数据个数	                                        ;发送个数
	MOV	SBUF, A	                                               //#19
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #82H
	JNB	TI, $
	CLR	TI
	MOV	A, #01H	                                               ; 触摸屏当前型号注解首址H	 ;     
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	MOV	A, #00H	                                               ; 触摸屏当前型号注解首址H;                                            
	MOV	SBUF, A	                                               ;变量首址																																									//#00H
	JNB	TI, $
	CLR	TI
	MOV	DPH, STC数据首址H
	MOV	DPL, STC数据首址L	                                     ;发送地址20H
	MOV	R2, STC数据个数	                                       //#32										;发送长度16
注解十六位变量发送中:
	MOVX	A, @DPTR
	MOV	SBUF, A
	JNB	TI, $
	CLR	TI
	INC	DPTR
	DJNZ	R2, 注解十六位变量发送中
	RET



;5AA5	013(#4+X)	<82	01	00			0100-018FF 
发送型号表格注解:
发送100H至18FH到100H触摸屏:
;发送143个数据:    ;
;0100-018F=>0100-0147H   
	MOV	STC数据首址H	, #01H
	MOV	STC数据首址L	, #00H
	MOV	STC数据个数, #8FH	                                     ;不能大于255-3(发送时还要加3,不能大于FF)   
	LCALL	发送扩展数据到触摸屏
	ret

发送当前型号注解:
;0100-011F=>0100-0107H  
	MOV	STC数据首址H	, #01H
	MOV	A	, 型号当前值
	MOV	B, #10H
	MUL	AB
	MOV	STC数据首址L	, A
	MOV	STC数据个数, #0FH	                                     ;不能大于255-3(发送时还要加3,不能大于FF)
	LCALL	发送当前型号注解到触摸屏
	ret


;******************************************
型号表格注解初始化:
型号0注解初始化:
	MOV	P_SW1	, #00H
	MOV	DPTR, #表格A注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #100H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格1注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #110H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格2注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #120H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格3注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #130H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格4注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #140H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格5注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #150H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格6注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #160H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格7注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #170H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	MOV	DPTR, #表格8注解TAB
	MOV	P_SW1	, #01H
	MOV	DPTR, #180H
	MOV	P_SW1	, #00H
	LCALL	型号16字节表格注解查表
	RET




型号16字节表格注解查表:
;MOV	DPTR, #表格注解TAB
;MOV	P_SW1	, #00H
;MOV	DPH,  	STC数据首址H	 
;MOV	DPL, 		STC数据首址L	 
;MOV	P_SW1	, #01H          
	MOV	R0, #00H
	MOV	R1, #0FH
表格注解写入:
	MOV	A, R0
	MOVC	A, @A+DPTR
	MOV	P_SW1	, #01H
	MOVX	@DPTR	, A
	INC	DPTR
	MOV	P_SW1	, #00H
	INC	R0
	DJNZ	R1, 表格注解写入
	RET

表格A注解TAB:
	DB	"-----------------"	                                    ;不用
表格1注解TAB:
	DB	"电磨-10 0000000000"
表格2注解TAB:
	DB	"吹风机-2000000000"
表格3注解TAB:
	DB	"手电钻-300000000"
表格4注解TAB:
	DB	"水钻-4000000000"
表格5注解TAB:
	DB	"磨光机-5000000000"
表格6注解TAB:
	DB	"电刨-6000000000"
表格7注解TAB:
	DB	"抛光机-7000000000"
表格8注解TAB:
	DB	"电镐-8000000000"
表格9注解TAB:
	DB	"冲击钻-9000000000"


;*************END*******改*202503新增****@11.0592MHZ串口参数*********************** 



/*
写DGUS屏寄存器指令（80）
				此处以切换图片为例,如从当前页面切换到第6幅图片,向屏发指令如下：
5A	A5	04	80	03	0005
5A	A5		表示：帧头
04					表示：数据长度
80					表示：写DGUS屏寄存器
03					表示：寄存器（存储DGUS	屏页面寄存器（03和04寄存器存放的页面））
0005			表示：向寄存器03和04分别写00和05,也就是切换到第5页
*/
写DGUS屏寄存器:
;MOV	显示画面,
DGUS切换图片:  
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
01	表示：从	1000	地址开始读	1	个字长度,数据指令最大容许长度	0X7C
	*/
      
读变量地址数据:
//5A	A5	04	83	0010	40
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
	MOV	SBUF, #15H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #32	                                             //X(	16	)	*2	=
	JNB	TI, $
	CLR	TI
	LCALL	DY100MS
	RET
//***结束*****串口读变量***********
    
读变量地址0100h数据:
//5A	A5	04	83	0010	40
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
	MOV	SBUF, #01H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #00H
	JNB	TI, $
	CLR	TI
	MOV	SBUF, #47H	                                            ;#8FH	                                         //X(	16	)	*2	=
	JNB	TI, $
	CLR	TI
	LCALL	DY200MS
	RET
//***结束*****串口读变量***********






                                                                  

	
//***开始*****发送提示符	**************************************8**** 
//**注意发送提示字符,查表发送并判断00H停止发送
//*****触摸屏数据分配****************
//1.10	发送提示字符,查表发送并判断00H停止发送 ,所以地址也不能有00H.
//1.1.1	触摸屏版本号显示0201H-0210H,(32个字节) 
//1.1.2	触摸屏报警&提示0211-0230H  (64个字节,32个中文字)
//1.1.3	触摸屏型号注解中文显示0100-014FH(9*16个字节)   >> 

调试物料取位置TAB:
;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" X,Y1机构快速移动到物料抓取位置，注意安全。"
	DB	00H, 0FFH
调试物料取回归TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" Y1机构物料抓取位置调试成功，快速回归，注意安全。"
	DB	00H, 0FFH
调试中间加工位置放TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" X,Y1机构快速移动到加工位置，物料抓取位置，注意安全。"
	DB	00H, 0FFH
调试中间加工位置放回归TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" Y1机构加工位置调试成功，物料松开，快速回归，注意安全。"
	DB	00H, 0FFH
调试中间加工位置取TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" X,Y2机构快速移动到加工位置并抓取物料，注意安全。"
	DB	00H, 0FFH
调试中间加工位置取回归TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" X,Y2机构快速移动到右边成品物料送出位置，注意安全。"
	DB	00H, 0FFH
调试左边位置放TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" Y2机构快速移动到成品物料送出位置，注意安全。"
	DB	00H, 0FFH
调试左边位置放回归TAB:
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	" Y2机构快成品物料送出位置调试成功，快速回归，注意安全。"
	DB	00H, 0FFH
调试结束归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	" 调试完成， X,Y1,Y2机构快速回归零位，注意安全。"
	;字符不能超12*4=48个
	DB	00H, 0FFH
;**********************************************************






 
急停开关TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"报警:急停开关触点接通。"
	;字符不能超12*4=48个
	DB	00H, 0FFH	                                              ;FFH,		空白
 
//MOV	DPTR,	#提示符TAB 
//LCALL		中文发送到0121
按急停按钮退出TAB:
;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	DB	"提示:请按急停按钮退出"
	//DB	"内部参数修改请联系0579-87280018 "
	DB	00H	, 0FFH
;FFH,		空白

方向归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"正在执行精准定位归零动作,请注意安全。"
	DB	00H	, 0FFH
	;FFH,		空白
垂直方向归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0100H接收地址
	DB	"正在执行垂直方向归零动作,请注意安全。"
	DB	00H	, 0FFH
	;FFH,		空白
水平方向归零TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0121H接收地址
	DB	"正在执行水平方向归零动作,请注意安全。"
	DB	00H	, 0FFH
;FFH,		空白

开机复位完成TAB:
	;帧头
	DB	05AH, 0A5H, 064H	, 082H	, 02H, 11H
	;82H发送;发送个数23,80H接收;0121H接收地址
	DB	"开机复位完成。"
	DB	00H	, 0FFH
;FFH,		空白

运行长度错误TAB:
	;帧头          接收地址 =0121H
	DB	05AH, 0A5H, 064H, 082H, 02H, 11H
	DB	" 注意:运行长度太短,运行速度太高,正在自动计算运行速度。"
	DB	00H, 0FFH	                                              ;FFH,		空白



发送版本号:	                                                //0101
	MOV	DPTR, #版本号TAB
	LCALL	发送提示字符
	RET
版本号TAB:
	;帧头          接收地址 =0101H
	DB	05AH, 0A5H, 023H, 082H, 02H, 01H
	DB	"机械手STC20251.0.C"
	DB	00H, 0FFH	                                              ;FFH,		空白

;MOV	DPTR, #提示符TAB
;LCALL		  发送提示字符
发送提示字符:
//发送个数 , 接收地址 ,在帧头里改   
//单字符为零时继续发送FFH填补
;MOV	DPTR, #TAB (查表地址写入DPTR)
	MOV	A, #2H
	MOVC	A, @A+DPTR
	ADD	A, #3H
	MOV	发送个数, A
	MOV	R0, #0H
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
//*****结束*****发送提示符	*************************************
//
;5AA5 <013(#3+X) 82   00 00  > 0060-006F
串口2发送数据:
    ; MOV   STC数据首址L ,#00H
     ;MOV   发送个数,#32
    ;LCALL  串口2发送数据
	MOV	S2BUF, #5AH
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #0A5H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	A, #3
	ADD	A, STC数据个数	                                        ;发送个数
	MOV	S2BUF, A	                                              //#19
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #82H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	S2BUF, #00H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	A, STC数据首址L
	RR	A	                                                      //40/2=0020
	MOV	S2BUF, A	                                              ;变量首址                                         //#00H
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	MOV	R0, STC数据首址L	                                      ;发送地址20H
	MOV	R2, STC数据个数	                                       //#32          ;发送长度16
串口2写XX变量发送中:
	MOV	A, @R0
	MOV	S2BUF, A
	JB	串口2TI发送忙标记, $	                                   //等待前面的数据发送完成
	SETB	串口2TI发送忙标记
	INC	R0
	DJNZ	R2, 串口2写XX变量发送中
	RET

	//***改*202409****@11.0592MHZ串口参数*****
;5AA5	013(#4+X)	<82			00	00			0060-006F
串口2发送20H至5FH到变量地址:
串口2发送32个数据:
;20-5FH=>0000-001FH
	MOV	STC数据首址L, #50H
	MOV	STC数据个数, #16
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
	MOV	DGUS变量首址H, A	                                      //数值为00
串口2缓存05数据读出:
	INC	R1
	MOVX	A, @R1	                                               //01变量地址L			(16位转8位
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量首址L, A
串口2缓存06数据读出:
	INC	R1	                                                    //01变量地址L			(16位转8位
	MOVX	A, @R1	                                               //01*2	数据长度
	MOV	B, #2
	MUL	AB
	MOV	DGUS变量个数	, A
串口2缓存07后数据读出:
	MOV	A, DGUS变量首址L	                                      //	DGUS0010H=STC20H
	MOV	R0, A	                                                 //;接收地址20-2F
	MOV	R2, DGUS变量个数	                                      ;		接收数据长度
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
	MOV	STCSTC数据首址H	, #02H
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


速度对应的定时值TAB:
;	0-1000mm/s对应T0值 （2000个脉冲=1转(20齿*M5）=100MM								
	DW	0281FH, 0281FH, 0281FH, 0281FH, 04C19H, 065CCH, 07913H, 08810H, 0940FH, 09DDFH
	DW	0A60CH, 0ACF7H, 0B2E5H, 0B809H, 0BC89H, 0C080H, 0C407H, 0C72FH, 0CA07H, 0CC98H
	dw	0CEEFH, 0D111H, 0D305H, 0D4D2H, 0D67BH, 0D804H, 0D972H, 0DAC6H, 0DC04H, 0DD2DH
	dw	0DE44H, 0DF49H, 0E03FH, 0E128H, 0E203H, 0E2D2H, 0E397H, 0E451H, 0E503H, 0E5ABH
	dw	0E64BH, 0E6E4H, 0E777H, 0E802H, 0E888H, 0E907H, 0E982H, 0E9F7H, 0EA68H, 0EAD4H
	dw	0EB3DH, 0EBA1H, 0EC01H, 0EC5FH, 0ECB8H, 0ED0FH, 0ED62H, 0EDB3H, 0EE01H, 0EE4DH
	dw	0EE96H, 0EEDCH, 0EF21H, 0EF63H, 0EFA4H, 0EFE2H, 0F01FH, 0F05AH, 0F093H, 0F0CBH
	dw	0F101H, 0F135H, 0F168H, 0F19AH, 0F1CBH, 0F1FAH, 0F228H, 0F255H, 0F281H, 0F2ABH
	dw	0F2D5H, 0F2FDH, 0F325H, 0F34CH, 0F371H, 0F396H, 0F3BBH, 0F3DEH, 0F400H, 0F422H
	dw	0F443H, 0F463H, 0F483H, 0F4A2H, 0F4C0H, 0F4DEH, 0F4FBH, 0F517H, 0F533H, 0F54FH
	dw	0F569H, 0F584H, 0F59EH, 0F5B7H, 0F5D0H, 0F5E8H, 0F600H, 0F617H, 0F62FH, 0F645H
	dw	0F65BH, 0F671H, 0F687H, 0F69CH, 0F6B0H, 0F6C5H, 0F6D9H, 0F6ECH, 0F700H, 0F713H
	dw	0F726H, 0F738H, 0F74AH, 0F75CH, 0F76DH, 0F77FH, 0F790H, 0F7A0H, 0F7B1H, 0F7C1H
	dw	0F7D1H, 0F7E1H, 0F7F0H, 0F800H, 0F80FH, 0F81EH, 0F82CH, 0F83BH, 0F849H, 0F857H
	dw	0F865H, 0F872H, 0F880H, 0F88DH, 0F89AH, 0F8A7H, 0F8B3H, 0F8C0H, 0F8CCH, 0F8D9H
	dw	0F8E5H, 0F8F0H, 0F8FCH, 0F908H, 0F913H, 0F91FH, 0F92AH, 0F935H, 0F940H, 0F94AH
	dw	0F955H, 0F95FH, 0F96AH, 0F974H, 0F97EH, 0F988H, 0F992H, 0F99BH, 0F9A5H, 0F9AFH
	dw	0F9B8H, 0F9C1H, 0F9CAH, 0F9D4H, 0F9DDH, 0F9E5H, 0F9EEH, 0F9F7H, 0F9FFH, 0FA08H
	dw	0FA10H, 0FA19H, 0FA21H, 0FA29H, 0FA31H, 0FA39H, 0FA41H, 0FA48H, 0FA50H, 0FA58H
	dw	0FA5FH, 0FA67H, 0FA6EH, 0FA75H, 0FA7DH, 0FA84H, 0FA8BH, 0FA92H, 0FA99H, 0FAA0H
	dw	0FAA7H, 0FAADH, 0FAB4H, 0FABBH, 0FAC1H, 0FAC8H, 0FACEH, 0FAD4H, 0FADBH, 0FAE1H
	dw	0FAE7H, 0FAEDH, 0FAF3H, 0FAF9H, 0FAFFH, 0FB05H, 0FB0BH, 0FB11H, 0FB17H, 0FB1CH
	dw	0FB22H, 0FB27H, 0FB2DH, 0FB32H, 0FB38H, 0FB3DH, 0FB43H, 0FB48H, 0FB4DH, 0FB52H
	dw	0FB57H, 0FB5DH, 0FB62H, 0FB67H, 0FB6CH, 0FB71H, 0FB75H, 0FB7AH, 0FB7FH, 0FB84H
	dw	0FB89H, 0FB8DH, 0FB92H, 0FB97H, 0FB9BH, 0FBA0H, 0FBA4H, 0FBA9H, 0FBADH, 0FBB2H
	dw	0FBB6H, 0FBBAH, 0FBBFH, 0FBC3H, 0FBC7H, 0FBCBH, 0FBCFH, 0FBD4H, 0FBD8H, 0FBDCH
	dw	0FBE0H, 0FBE4H, 0FBE8H, 0FBECH, 0FBF0H, 0FBF4H, 0FBF7H, 0FBFBH, 0FBFFH, 0FC03H
	dw	0FC07H, 0FC0AH, 0FC0EH, 0FC12H, 0FC15H, 0FC19H, 0FC1DH, 0FC20H, 0FC24H, 0FC27H
	dw	0FC2BH, 0FC2EH, 0FC32H, 0FC35H, 0FC38H, 0FC3CH, 0FC3FH, 0FC42H, 0FC46H, 0FC49H
	dw	0FC4CH, 0FC4FH, 0FC53H, 0FC56H, 0FC59H, 0FC5CH, 0FC5FH, 0FC62H, 0FC65H, 0FC68H
	dw	0FC6CH, 0FC6FH, 0FC72H, 0FC75H, 0FC77H, 0FC7AH, 0FC7DH, 0FC80H, 0FC83H, 0FC86H
	dw	0FC89H, 0FC8CH, 0FC8FH, 0FC91H, 0FC94H, 0FC97H, 0FC9AH, 0FC9CH, 0FC9FH, 0FCA2H
	dw	0FCA4H, 0FCA7H, 0FCAAH, 0FCACH, 0FCAFH, 0FCB1H, 0FCB4H, 0FCB7H, 0FCB9H, 0FCBCH
	dw	0FCBEH, 0FCC1H, 0FCC3H, 0FCC6H, 0FCC8H, 0FCCBH, 0FCCDH, 0FCCFH, 0FCD2H, 0FCD4H
	dw	0FCD7H, 0FCD9H, 0FCDBH, 0FCDEH, 0FCE0H, 0FCE2H, 0FCE4H, 0FCE7H, 0FCE9H, 0FCEBH
	dw	0FCEEH, 0FCF0H, 0FCF2H, 0FCF4H, 0FCF6H, 0FCF8H, 0FCFBH, 0FCFDH, 0FCFFH, 0FD01H
	dw	0FD03H, 0FD05H, 0FD07H, 0FD09H, 0FD0CH, 0FD0EH, 0FD10H, 0FD12H, 0FD14H, 0FD16H
	dw	0FD18H, 0FD1AH, 0FD1CH, 0FD1EH, 0FD20H, 0FD22H, 0FD23H, 0FD25H, 0FD27H, 0FD29H
	dw	0FD2BH, 0FD2DH, 0FD2FH, 0FD31H, 0FD33H, 0FD34H, 0FD36H, 0FD38H, 0FD3AH, 0FD3CH
	dw	0FD3EH, 0FD3FH, 0FD41H, 0FD43H, 0FD45H, 0FD46H, 0FD48H, 0FD4AH, 0FD4CH, 0FD4DH
	dw	0FD4FH, 0FD51H, 0FD53H, 0FD54H, 0FD56H, 0FD58H, 0FD59H, 0FD5BH, 0FD5DH, 0FD5EH
	dw	0FD60H, 0FD61H, 0FD63H, 0FD65H, 0FD66H, 0FD68H, 0FD69H, 0FD6BH, 0FD6DH, 0FD6EH
	dw	0FD70H, 0FD71H, 0FD73H, 0FD74H, 0FD76H, 0FD77H, 0FD79H, 0FD7AH, 0FD7CH, 0FD7DH
	dw	0FD7FH, 0FD80H, 0FD82H, 0FD83H, 0FD85H, 0FD86H, 0FD88H, 0FD89H, 0FD8BH, 0FD8CH
	dw	0FD8DH, 0FD8FH, 0FD90H, 0FD92H, 0FD93H, 0FD94H, 0FD96H, 0FD97H, 0FD98H, 0FD9AH
	dw	0FD9BH, 0FD9DH, 0FD9EH, 0FD9FH, 0FDA1H, 0FDA2H, 0FDA3H, 0FDA4H, 0FDA6H, 0FDA7H
	dw	0FDA8H, 0FDAAH, 0FDABH, 0FDACH, 0FDAEH, 0FDAFH, 0FDB0H, 0FDB1H, 0FDB3H, 0FDB4H
	dw	0FDB5H, 0FDB6H, 0FDB8H, 0FDB9H, 0FDBAH, 0FDBBH, 0FDBCH, 0FDBEH, 0FDBFH, 0FDC0H
	dw	0FDC1H, 0FDC2H, 0FDC4H, 0FDC5H, 0FDC6H, 0FDC7H, 0FDC8H, 0FDC9H, 0FDCBH, 0FDCCH
	dw	0FDCDH, 0FDCEH, 0FDCFH, 0FDD0H, 0FDD1H, 0FDD3H, 0FDD4H, 0FDD5H, 0FDD6H, 0FDD7H
	dw	0FDD8H, 0FDD9H, 0FDDAH, 0FDDBH, 0FDDCH, 0FDDDH, 0FDDFH, 0FDE0H, 0FDE1H, 0FDE2H
	dw	0FDE3H, 0FDE4H, 0FDE5H, 0FDE6H, 0FDE7H, 0FDE8H, 0FDE9H, 0FDEAH, 0FDEBH, 0FDECH
	dw	0FDEDH, 0FDEEH, 0FDEFH, 0FDF0H, 0FDF1H, 0FDF2H, 0FDF3H, 0FDF4H, 0FDF5H, 0FDF6H
	dw	0FDF7H, 0FDF8H, 0FDF9H, 0FDFAH, 0FDFBH, 0FDFCH, 0FDFDH, 0FDFEH, 0FDFFH, 0FE00H
	dw	0FE01H, 0FE02H, 0FE03H, 0FE03H, 0FE04H, 0FE05H, 0FE06H, 0FE07H, 0FE08H, 0FE09H
	dw	0FE0AH, 0FE0BH, 0FE0CH, 0FE0DH, 0FE0EH, 0FE0EH, 0FE0FH, 0FE10H, 0FE11H, 0FE12H
	dw	0FE13H, 0FE14H, 0FE15H, 0FE15H, 0FE16H, 0FE17H, 0FE18H, 0FE19H, 0FE1AH, 0FE1BH
	dw	0FE1BH, 0FE1CH, 0FE1DH, 0FE1EH, 0FE1FH, 0FE20H, 0FE20H, 0FE21H, 0FE22H, 0FE23H
	dw	0FE24H, 0FE25H, 0FE25H, 0FE26H, 0FE27H, 0FE28H, 0FE29H, 0FE29H, 0FE2AH, 0FE2BH
	dw	0FE2CH, 0FE2DH, 0FE2DH, 0FE2EH, 0FE2FH, 0FE30H, 0FE30H, 0FE31H, 0FE32H, 0FE33H
	dw	0FE33H, 0FE34H, 0FE35H, 0FE36H, 0FE37H, 0FE37H, 0FE38H, 0FE39H, 0FE3AH, 0FE3AH
	dw	0FE3BH, 0FE3CH, 0FE3CH, 0FE3DH, 0FE3EH, 0FE3FH, 0FE3FH, 0FE40H, 0FE41H, 0FE42H
	dw	0FE42H, 0FE43H, 0FE44H, 0FE44H, 0FE45H, 0FE46H, 0FE47H, 0FE47H, 0FE48H, 0FE49H
	dw	0FE49H, 0FE4AH, 0FE4BH, 0FE4BH, 0FE4CH, 0FE4DH, 0FE4DH, 0FE4EH, 0FE4FH, 0FE4FH
	dw	0FE50H, 0FE51H, 0FE51H, 0FE52H, 0FE53H, 0FE53H, 0FE54H, 0FE55H, 0FE55H, 0FE56H
	dw	0FE57H, 0FE57H, 0FE58H, 0FE59H, 0FE59H, 0FE5AH, 0FE5BH, 0FE5BH, 0FE5CH, 0FE5CH
	dw	0FE5DH, 0FE5EH, 0FE5EH, 0FE5FH, 0FE60H, 0FE60H, 0FE61H, 0FE61H, 0FE62H, 0FE63H
	dw	0FE63H, 0FE64H, 0FE65H, 0FE65H, 0FE66H, 0FE66H, 0FE67H, 0FE68H, 0FE68H, 0FE69H
	dw	0FE69H, 0FE6AH, 0FE6BH, 0FE6BH, 0FE6CH, 0FE6CH, 0FE6DH, 0FE6DH, 0FE6EH, 0FE6FH
	dw	0FE6FH, 0FE70H, 0FE70H, 0FE71H, 0FE71H, 0FE72H, 0FE73H, 0FE73H, 0FE74H, 0FE74H
	dw	0FE75H, 0FE75H, 0FE76H, 0FE77H, 0FE77H, 0FE78H, 0FE78H, 0FE79H, 0FE79H, 0FE7AH
	dw	0FE7AH, 0FE7BH, 0FE7BH, 0FE7CH, 0FE7DH, 0FE7DH, 0FE7EH, 0FE7EH, 0FE7FH, 0FE7FH
	dw	0FE80H, 0FE80H, 0FE81H, 0FE81H, 0FE82H, 0FE82H, 0FE83H, 0FE83H, 0FE84H, 0FE84H
	dw	0FE85H, 0FE86H, 0FE86H, 0FE87H, 0FE87H, 0FE88H, 0FE88H, 0FE89H, 0FE89H, 0FE8AH
	dw	0FE8AH, 0FE8BH, 0FE8BH, 0FE8CH, 0FE8CH, 0FE8DH, 0FE8DH, 0FE8EH, 0FE8EH, 0FE8FH
	dw	0FE8FH, 0FE90H, 0FE90H, 0FE91H, 0FE91H, 0FE91H, 0FE92H, 0FE92H, 0FE93H, 0FE93H
	dw	0FE94H, 0FE94H, 0FE95H, 0FE95H, 0FE96H, 0FE96H, 0FE97H, 0FE97H, 0FE98H, 0FE98H
	dw	0FE99H, 0FE99H, 0FE99H, 0FE9AH, 0FE9AH, 0FE9BH, 0FE9BH, 0FE9CH, 0FE9CH, 0FE9DH
	dw	0FE9DH, 0FE9EH, 0FE9EH, 0FE9FH, 0FE9FH, 0FE9FH, 0FEA0H, 0FEA0H, 0FEA1H, 0FEA1H
	dw	0FEA2H, 0FEA2H, 0FEA2H, 0FEA3H, 0FEA3H, 0FEA4H, 0FEA4H, 0FEA5H, 0FEA5H, 0FEA6H
	dw	0FEA6H, 0FEA6H, 0FEA7H, 0FEA7H, 0FEA8H, 0FEA8H, 0FEA9H, 0FEA9H, 0FEA9H, 0FEAAH
	dw	0FEAAH, 0FEABH, 0FEABH, 0FEABH, 0FEACH, 0FEACH, 0FEADH, 0FEADH, 0FEAEH, 0FEAEH
	dw	0FEAEH, 0FEAFH, 0FEAFH, 0FEB0H, 0FEB0H, 0FEB0H, 0FEB1H, 0FEB1H, 0FEB2H, 0FEB2H
	dw	0FEB2H, 0FEB3H, 0FEB3H, 0FEB4H, 0FEB4H, 0FEB4H, 0FEB5H, 0FEB5H, 0FEB6H, 0FEB6H
	dw	0FEB6H, 0FEB7H, 0FEB7H, 0FEB7H, 0FEB8H, 0FEB8H, 0FEB9H, 0FEB9H, 0FEB9H, 0FEBAH
	dw	0FEBAH, 0FEBBH, 0FEBBH, 0FEBBH, 0FEBCH, 0FEBCH, 0FEBCH, 0FEBDH, 0FEBDH, 0FEBEH
	dw	0FEBEH, 0FEBEH, 0FEBFH, 0FEBFH, 0FEBFH, 0FEC0H, 0FEC0H, 0FEC1H, 0FEC1H, 0FEC1H
	dw	0FEC2H, 0FEC2H, 0FEC2H, 0FEC3H, 0FEC3H, 0FEC3H, 0FEC4H, 0FEC4H, 0FEC5H, 0FEC5H
	dw	0FEC5H, 0FEC6H, 0FEC6H, 0FEC6H, 0FEC7H, 0FEC7H, 0FEC7H, 0FEC8H, 0FEC8H, 0FEC8H
	dw	0FEC9H, 0FEC9H, 0FEC9H, 0FECAH, 0FECAH, 0FECAH, 0FECBH, 0FECBH, 0FECBH, 0FECCH
	dw	0FECCH, 0FECCH, 0FECDH, 0FECDH, 0FECEH, 0FECEH, 0FECEH, 0FECFH, 0FECFH, 0FECFH
	dw	0FED0H, 0FED0H, 0FED0H, 0FED1H, 0FED1H, 0FED1H, 0FED1H, 0FED2H, 0FED2H, 0FED2H
	dw	0FED3H, 0FED3H, 0FED3H, 0FED4H, 0FED4H, 0FED4H, 0FED5H, 0FED5H, 0FED5H, 0FED6H
	dw	0FED6H, 0FED6H, 0FED7H, 0FED7H, 0FED7H, 0FED8H, 0FED8H, 0FED8H, 0FED9H, 0FED9H
	dw	0FED9H, 0FED9H, 0FEDAH, 0FEDAH, 0FEDAH, 0FEDBH, 0FEDBH, 0FEDBH, 0FEDCH, 0FEDCH
	dw	0FEDCH, 0FEDDH, 0FEDDH, 0FEDDH, 0FEDDH, 0FEDEH, 0FEDEH, 0FEDEH, 0FEDFH, 0FEDFH
	dw	0FEDFH, 0FEE0H, 0FEE0H, 0FEE0H, 0FEE0H, 0FEE1H, 0FEE1H, 0FEE1H, 0FEE2H, 0FEE2H
	dw	0FEE2H, 0FEE3H, 0FEE3H, 0FEE3H, 0FEE3H, 0FEE4H, 0FEE4H, 0FEE4H, 0FEE5H, 0FEE5H
	dw	0FEE5H, 0FEE5H, 0FEE6H, 0FEE6H, 0FEE6H, 0FEE7H, 0FEE7H, 0FEE7H, 0FEE7H, 0FEE8H
	dw	0FEE8H, 0FEE8H, 0FEE9H, 0FEE9H, 0FEE9H, 0FEE9H, 0FEEAH, 0FEEAH, 0FEEAH, 0FEEAH



/////X方向Y1Y2归零	//////
机械手归零:

	MOV	DPTR, #垂直方向右归零TAB
	LCALL	发送提示字符
	MOV	显示画面, #提示语画面
	LCALL	DGUS切换图片
	MOV	当前数值L	, #02
	MOV	STC数据首址L, #当前数值L
	LCALL	发送单个数据到触摸屏
	LCALL	DY500MS
	
	LCALL	Y1方向归零

	MOV	当前数值L	, #04
	MOV	STC数据首址L, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #垂直方向左归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	Y2方向归零

	MOV	当前数值L	, #06
	MOV	STC数据首址L, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #水平方向归零TAB
	LCALL	发送提示字符
	LCALL	DY500MS
	LCALL	X方向归零

	MOV	当前数值L	, #09
	MOV	STC数据首址L, #当前数值L
	LCALL	发送单个数据到触摸屏
	MOV	DPTR, #开机复位完成TAB
	LCALL	发送提示字符
	LCALL	DY100MS
	RET
	MOV	显示画面, #等待画面
	LCALL	DGUS切换图片
	RET

//////X方向Y1Y2归零	//////
X方向归零:
	
	JNB	X右限位	, X方向精细归零
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算当前速度转T0赋值
X方向继续归零:
	CLR	脉冲输出总开关标记
	CLR	X轴方向
	CLR	X脉冲输出标记
	JB	X右限位	, $
	SETB	X脉冲输出标记
	setb	脉冲输出总开关标记
	//RET
X方向精细归零:

	MOV	匀速运行长度H	, #HIGH	200
	MOV	匀速运行长度L	, #LOW	200
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	CLR	脉冲输出总开关标记
	SETB	X轴方向
	CLR	X脉冲输出标记
	LCALL	计数匀速运行
	CLR	X脉冲输出标记
	setb	脉冲输出总开关标记
	LCALL	DY100MS
	CLR	X轴方向
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算当前速度转T0赋值
X方向继续精细归零:
	CLR	脉冲输出总开关标记
	CLR	X脉冲输出标记
	JB	X右限位	, $
	SETB	X脉冲输出标记
	setb	脉冲输出总开关标记
	RET



Y1方向归零:
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算当前速度转T0赋值

Y1归零到位检查:
	CLR	脉冲输出总开关标记
	CLR	Y1轴右方向
	CLR	Y1脉冲输出标记
	JB	Y1右零位	, Y1归零到位检查
	SETB	Y1脉冲输出标记
	SETB	脉冲输出总开关标记
	RET


Y2方向归零:
	MOV	当前速度H	, #HIGH	归零速度
	MOV	当前速度L	, #LOW	归零速度
	LCALL	计算当前速度转T0赋值
	
Y2归零到位检查:
	CLR	脉冲输出总开关标记
	CLR	Y2轴左方向
	CLR	Y2脉冲输出标记
	JB	Y2左零位	, Y2归零到位检查
	SETB	Y2脉冲输出标记
	SETB	脉冲输出总开关标记
	RET

//////X方向Y1Y2归零	/////*/