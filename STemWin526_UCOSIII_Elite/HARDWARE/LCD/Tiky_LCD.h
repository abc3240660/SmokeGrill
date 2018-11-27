#ifndef __Tiky_LCD_H
#define __Tiky_LCD_H
#include "stm32f10x.h"
#include "common.h"

#define   BLACK                0x0000                // 黑色：    0,   0,   0 //
#define   BLUE                 0x001F                // 蓝色：    0,   0, 255 //
#define   GREEN                0x07E0                // 绿色：    0, 255,   0 //
#define   CYAN                 0x07FF                // 青色：    0, 255, 255 //
#define   RED                  0xF800                // 红色：  255,   0,   0 //
#define   MAGENTA              0xF81F                // 品红：  255,   0, 255 //
#define   YELLOW               0xFFE0                // 黄色：  255, 255, 0   //
#define   WHITE                0xFFFF                // 白色：  255, 255, 255 //
#define   NAVY                 0x000F                // 深蓝色：  0,   0, 128 //
#define   DGREEN               0x03E0                // 深绿色：  0, 128,   0 //
#define   DCYAN                0x03EF                // 深青色：  0, 128, 128 //
#define   MAROON               0x7800                // 深红色：128,   0,   0 //
#define   PURPLE               0x780F                // 紫色：  128,   0, 128 //
#define   OLIVE                0x7BE0                // 橄榄绿：128, 128,   0 //
#define   LGRAY                0xC618                // 灰白色：192, 192, 192 //
#define   DGRAY                0x7BEF                // 深灰色：128, 128, 128 //

//LCD?????
typedef struct  
{										    
	u16 width;			//LCD ??
	u16 height;			//LCD ??
	u16 id;				//LCD ID
	u8  dir;			//????????:0,??;1,???	
	u16	wramcmd;		//???gram??
	u16  setxcmd;		//??x????
	u16  setycmd;		//??y???? 
}_lcd_dev; 	  

//LCD??
extern _lcd_dev lcddev;	//??LCD????

//硬件相关的子函数
#ifdef MCU_ZET6
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x60000002)    //Disp Data ADDR
#else
#define Bank1_LCD_C    ((u32)0x6001FFFE)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#endif

#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

#define VERTICAL_DISPLAY     0x00 //竖屏
#define HORIZON_DISPLAY			 0x01

//Lcd初始化及其低级控制函数
void Lcd_Configuration(void);
void Lcd_Initialize(void);
void LCD_WR_REG(u16 Index,u16 CongfigTemp);
//Lcd高级控制函数

void Lcd_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void DrawPixel(u16 x, u16 y, int Color);
 u16 GetPoint( u16 x, u16 y);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Display_Dir(uint8_t display_mode);
void LCD_Clear(u16 color);

#endif


