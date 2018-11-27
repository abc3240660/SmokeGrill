#ifndef __Tiky_LCD_H
#define __Tiky_LCD_H
#include "stm32f10x.h"
#include "common.h"

#define   BLACK                0x0000                // ��ɫ��    0,   0,   0 //
#define   BLUE                 0x001F                // ��ɫ��    0,   0, 255 //
#define   GREEN                0x07E0                // ��ɫ��    0, 255,   0 //
#define   CYAN                 0x07FF                // ��ɫ��    0, 255, 255 //
#define   RED                  0xF800                // ��ɫ��  255,   0,   0 //
#define   MAGENTA              0xF81F                // Ʒ�죺  255,   0, 255 //
#define   YELLOW               0xFFE0                // ��ɫ��  255, 255, 0   //
#define   WHITE                0xFFFF                // ��ɫ��  255, 255, 255 //
#define   NAVY                 0x000F                // ����ɫ��  0,   0, 128 //
#define   DGREEN               0x03E0                // ����ɫ��  0, 128,   0 //
#define   DCYAN                0x03EF                // ����ɫ��  0, 128, 128 //
#define   MAROON               0x7800                // ���ɫ��128,   0,   0 //
#define   PURPLE               0x780F                // ��ɫ��  128,   0, 128 //
#define   OLIVE                0x7BE0                // ����̣�128, 128,   0 //
#define   LGRAY                0xC618                // �Ұ�ɫ��192, 192, 192 //
#define   DGRAY                0x7BEF                // ���ɫ��128, 128, 128 //

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

//Ӳ����ص��Ӻ���
#ifdef MCU_ZET6
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x60000002)    //Disp Data ADDR
#else
#define Bank1_LCD_C    ((u32)0x6001FFFE)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#endif

#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

#define VERTICAL_DISPLAY     0x00 //����
#define HORIZON_DISPLAY			 0x01

//Lcd��ʼ������ͼ����ƺ���
void Lcd_Configuration(void);
void Lcd_Initialize(void);
void LCD_WR_REG(u16 Index,u16 CongfigTemp);
//Lcd�߼����ƺ���

void Lcd_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void DrawPixel(u16 x, u16 y, int Color);
 u16 GetPoint( u16 x, u16 y);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Display_Dir(uint8_t display_mode);
void LCD_Clear(u16 color);

#endif


