#include "stm32f10x.h"
#include "Tiky_LCD.h" 

_lcd_dev lcddev;

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
#ifdef MCU_ZET6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF , ENABLE);
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
#endif    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    /* config tft Reset gpio */
#ifdef MCU_ZET6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
#endif
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* config tft BL gpio 背光开关控制引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  		   
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
#ifdef MCU_ZET6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
#endif
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
#ifdef MCU_ZET6
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOF, &GPIO_InitStructure);
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
#endif
    
    /* tft control gpio init */	 
#ifdef MCU_ZET6
	GPIO_SetBits(GPIOD, GPIO_Pin_2);		 // Reset
#else
    GPIO_SetBits(GPIOD, GPIO_Pin_13);		 // Reset				
#endif
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1 
#ifdef MCU_ZET6
	GPIO_SetBits(GPIOF, GPIO_Pin_0);		 // RS
#else
	GPIO_SetBits(GPIOD, GPIO_Pin_11);		 // RS
#endif
    GPIO_SetBits(GPIOD, GPIO_Pin_7);		 //	CS = 1

//		GPIO_SetBits(GPIOA, GPIO_Pin_1);	 	 //	BL = 1 
		
#if 0
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//按键 OK
    GPIO_Init(GPIOA, &GPIO_InitStructure);
																//  right         up           left        down
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6;		
    GPIO_Init(GPIOE, &GPIO_InitStructure);
#endif
}

/*
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */

static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD
    


    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

volatile void LCD_delay(char j)
{
volatile u16 i;	
	while(j--)
for(i=7200;i>0;i--);
}
static void LCD_Rst(void)
{			
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    LCD_delay(100);					   
    GPIO_SetBits(GPIOD, GPIO_Pin_13 );		 	 
    LCD_delay(100);	
}
 void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
 void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}


/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
LCD_GPIO_Config();
LCD_FSMC_Config();
LCD_Rst();
	
WriteComm(0xB0);
WriteData(0x04);
 
WriteComm(0x36);    
WriteData(0x00);    
 
WriteComm(0x3A);    
WriteData(0x55);   
 
WriteComm(0xB4);    
WriteData(0x00); 
 
WriteComm(0xC0);
WriteData(0x03);//0013 ori 03
WriteData(0xDF);//480
WriteData(0x40);
WriteData(0x12);
WriteData(0x00);
WriteData(0x01);
WriteData(0x00);
WriteData(0x43);
 
 
WriteComm(0xC1);//frame frequency
WriteData(0x05);//BCn,DIVn[1:0
WriteData(0x2f);//RTNn[4:0] 
WriteData(0x08);// BPn[7:0]
WriteData(0x08);// FPn[7:0]
WriteData(0x00);
 
 
 
WriteComm(0xC4);
WriteData(0x63);
WriteData(0x00);
WriteData(0x08);
WriteData(0x08);
 
WriteComm(0xC8);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
 
WriteComm(0xC9);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
 
WriteComm(0xCA);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
WriteComm(0xD0);
WriteData(0x95);
WriteData(0x06);
WriteData(0x08);
WriteData(0x10);
WriteData(0x3f);
 
 
WriteComm(0xD1);
WriteData(0x02);
WriteData(0x28);
WriteData(0x28);
WriteData(0x40);
 
WriteComm(0xE1);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);   
WriteData(0x00);   
 
WriteComm(0xE2);    
WriteData(0x80);    
 
WriteComm(0x2A);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x01);    
WriteData(0x3F);    
 
WriteComm(0x2B);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x01);    
WriteData(0xDF);    
 
WriteComm(0x11);
 
LCD_delay(120);
 
WriteComm(0x29);
 
WriteComm(0xC1);//frame frequency
WriteData(0x05);//BCn,DIVn[1:0
WriteData(0x2f);//RTNn[4:0] 
WriteData(0x08);// BPn[7:0]
WriteData(0x08);// FPn[7:0]
WriteData(0x00);
//Lcd_Light_ON;
WriteComm(0x20);
	
	
WriteComm(0x36);    
WriteData(0x68); 
// 	LCD_Fill_Pic(80,160,320,480, gImage_MM_T035);
// 	BlockWrite(0,0,799,479);
}
/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);

	WriteComm(0x2c);
}
 u16 GetPoint( u16 x, u16 y)
{
	u16 res = 0;
	BlockWrite(x,x,y,y);
	x = *(__IO u16 *) (Bank1_LCD_D);

	x = *(__IO u16 *) (Bank1_LCD_D);

	y = *(__IO u16 *) (Bank1_LCD_D);
	x = (x<<8)+y;
	y = *(__IO u16 *) (Bank1_LCD_D);
	res = (x&0xf800)|((x&0x00fc)<<3)|(y>>3);
	
	return res;
//	return (x&0xf800)|((x&0x00fc)<<3)|(y>>11);
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0x00); //横屏，从左下角开始，从左到右，从下到上
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0xA0);
}

//在指定座标上打一个点
void DrawPixel(u16 x, u16 y, int Color)
{
	BlockWrite(x,x,y,y);

	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	DrawPixel(x, y, color);
}


void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	Lcd_ColorBox(sx,sy,ex-sx+1,ey-sy+1,color);
}

u16 LCD_ReadPoint(u16 x,u16 y)
{
	return GetPoint(x, y);
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
	WriteComm(0x2a);   
	WriteData(Xpos>>8);
	WriteData(Xpos&0xff);
	WriteData(Xpos>>8);
	WriteData(Xpos&0xff);

	WriteComm(0x2b);   
	WriteData(Ypos>>8);
	WriteData(Ypos&0xff);
	WriteData(Ypos>>8);
	WriteData(Ypos&0xff);
}

void LCD_Clear(u16 color)
{
	Lcd_ColorBox(0,0,480,320,color);
	delay_ms(100);
	Lcd_Light_ON;
}

void lcd_scan_dir(uint8_t dir)
{
	uint16_t reg_val;
	
	switch(dir)
	{
	case L2R_U2D:
		reg_val = ( 0<<7 ) | ( 0<<6 ) | ( 0<<5 );
		break;
	
	case L2R_D2U:
		reg_val = ( 1<<7 ) | ( 0<<6 ) | ( 0<<5 );
		break;
	
	case R2L_U2D:
		reg_val = ( 0<<7 ) | ( 1<<6 ) | ( 0<<5 );
		break;

	case R2L_D2U:
		reg_val = ( 1<<7 ) | ( 1<<6 ) | ( 0<<5 );
		break;

	case U2D_L2R:
		reg_val = ( 0<<7 ) | ( 0<<6 ) | ( 1<<5 );
		break;
	
	case U2D_R2L:
		reg_val = ( 0<<7 ) | ( 1<<6 ) | ( 1<<5 );
		break;
	
	case D2U_L2R:
		reg_val = ( 1<<7 ) | ( 0<<6 ) | ( 1<<5 );
		break;
	
	case D2U_R2L:
		reg_val = ( 1<<7 ) | ( 1<<6 ) | ( 1<<5 );
		break;
	
	default:
		break;
	}
	
//	LCD_REGX = 0x36;
	/* set diaplay mode */
//	LCD_RAMX = reg_val;
	
	WriteComm(0x36);    
	WriteData(reg_val);
}

void LCD_Display_Dir(uint8_t display_mode)
{	
	switch(display_mode)
	{
	case HORIZON_DISPLAY:// 横
		lcddev.dir = HORIZON_DISPLAY;
		lcddev.height  = 320;
		lcddev.width   = 480;
		lcddev.wramcmd = 0X2C;
		lcddev.setxcmd = 0X2A;
		lcddev.setycmd = 0X2B;  	
		break;
	
	case VERTICAL_DISPLAY:// 竖
		lcddev.dir = VERTICAL_DISPLAY;
		lcddev.height  = 480;
		lcddev.width   = 320;
		lcddev.wramcmd = 0x2C;
		lcddev.setxcmd = 0x2A;
		lcddev.setycmd = 0x2B;
		break;
	
	default:
		break;
	}
	
//	lcd_scan_dir(D2U_L2R);
	lcd_scan_dir(U2D_R2L);
}

void lcd_draw_point(uint16_t x,
					uint16_t y,
					uint16_t color)
{
	LCD_SetCursor(x, y);
	WriteComm(0x2c);   
	WriteData(color);
}

void LCD_DrawPoint(u16 x,u16 y)
{
	lcd_draw_point(x, y, POINT_COLOR);
	return;
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	LCD_Fill(sx,ex,sy,ey,*color);
	return;
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
