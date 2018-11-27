#include "lcd.h"
#include "Tiky_LCD.h" 
#include "delay.h"

/*
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */

volatile void LCD_delay(char j)
{
volatile u16 i;	
volatile u16 k;	
	while(j--)
for(i=7200;i>0;i--)
	for(k=3;k>0;k--);
}

//写寄存器函数
//regval:寄存器值
void WriteComm(u16 regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void WriteData(u16 data)
{	 
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD->LCD_RAM=data;		 
}

/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
//LCD_GPIO_Config();
LCD_FSMC_Config();
//LCD_Rst();

//delay_ms(1000);	

WriteComm(0x11);
WriteData(0x00);   
LCD_delay(40);
WriteComm(0xB0);
WriteData(0x04);
WriteComm(0xB3);
WriteData(0x02);
WriteData(0x00);
WriteComm(0xB6);
WriteData(0x52);
WriteData(0x83);
WriteComm(0xB7);
WriteData(0x80);
WriteData(0x72);
WriteData(0x11);
WriteData(0x25);
WriteComm(0xB8);
WriteData(0x00);
WriteData(0x0F);
WriteData(0x0F);
WriteData(0xFF);
WriteData(0xFF);
WriteData(0xC8);
WriteData(0xC8);
WriteData(0x02);
WriteData(0x18);
WriteData(0x10);
WriteData(0x10);
WriteData(0x37);
WriteData(0x5A);
WriteData(0x87);
WriteData(0xBE);
WriteData(0xFF);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xB9);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xBD);
WriteData(0x00);
WriteComm(0xC0);
WriteData(0x02);
WriteData(0x76);
WriteComm(0xC1);
WriteData(0x63);
WriteData(0x31);
WriteData(0x00);
WriteData(0x27);
WriteData(0x27);
WriteData(0x32);
WriteData(0x12);
WriteData(0x28);
WriteData(0x4E);
WriteData(0x10);
WriteData(0xA5);
WriteData(0x0F);
WriteData(0x58);
WriteData(0x21);
WriteData(0x01);
WriteComm(0xC2);
WriteData(0x28);
WriteData(0x06);
WriteData(0x06);
WriteData(0x01);
WriteData(0x03);
WriteData(0x00);
WriteComm(0xC3);
WriteData(0x40);
WriteData(0x00);
WriteData(0x03);
WriteComm(0xC4);
WriteData(0x00);
WriteData(0x01);
WriteComm(0xC6);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xC7);
WriteData(0x11);
WriteData(0x8D);
WriteData(0xA0);
WriteData(0xF5);
WriteData(0x27);
WriteComm(0xC8);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xC9);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xCA);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xD0);
WriteData(0xA9);
WriteData(0x03);
WriteData(0xCC);
WriteData(0xA5);
WriteData(0x00);
WriteData(0x53);
WriteData(0x20);
WriteData(0x10);
WriteData(0x01);
WriteData(0x00);
WriteData(0x01);
WriteData(0x01);
WriteData(0x00);
WriteData(0x03);
WriteData(0x01);
WriteData(0x00);
WriteComm(0xD1);
WriteData(0x18);
WriteData(0x0C);
WriteData(0x23);
WriteData(0x03);
WriteData(0x75);
WriteData(0x02);
WriteData(0x50);
WriteComm(0xD3);
WriteData(0x33);
WriteComm(0xD5);
WriteData(0x2a);
WriteData(0x2a);
WriteComm(0xD6);
WriteData(0x28);//a8
WriteComm(0xD7);
WriteData(0x01);
WriteData(0x00);
WriteData(0xAA);
WriteData(0xC0);
WriteData(0x2A);
WriteData(0x2C);
WriteData(0x22);
WriteData(0x12);
WriteData(0x71);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x00);
WriteData(0xA0);
WriteData(0x00);
WriteData(0x03);
WriteComm(0xD8);
WriteData(0x44);
WriteData(0x44);
WriteData(0x22);
WriteData(0x44);
WriteData(0x21);
WriteData(0x46);
WriteData(0x42);
WriteData(0x40);
WriteComm(0xD9);
WriteData(0xCF);
WriteData(0x2D);
WriteData(0x51);
WriteComm(0xDA);
WriteData(0x01);
WriteComm(0xDE);
WriteData(0x01);
WriteData(0x51);//58
WriteComm(0xE1);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xE6);
WriteData(0x55);//58
WriteComm(0xF3);
WriteData(0x06);
WriteData(0x00);
WriteData(0x00);
WriteData(0x24);
WriteData(0x00);
WriteComm(0xF8);
WriteData(0x00);
WriteComm(0xFA);
WriteData(0x01);
WriteComm(0xFB);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFC);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFD);
WriteData(0x00);
WriteData(0x00);
WriteData(0x70);
WriteData(0x00);
WriteData(0x72);
WriteData(0x31);
WriteData(0x37);
WriteData(0x70);
WriteData(0x32);
WriteData(0x31);
WriteData(0x07);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFE);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x20);
WriteComm(0xB0);
WriteData(0x04); //04
LCD_delay(40);
WriteComm(0x35);
WriteData(0x00);
WriteComm(0x44);
WriteData(0x00);
WriteComm(0x36);
WriteData(0x00);
WriteComm(0x3A);
WriteData(0x55);
WriteComm(0x2A);
WriteData(0x00);
WriteData(0x00);
WriteData(0x01);
WriteData(0xDF);
WriteComm(0x2B);
WriteData(0x00);
WriteData(0x00);
WriteData(0x03);
WriteData(0x1F);
WriteComm(0x29);
WriteData(0x00);     
LCD_delay(180);
WriteComm(0x2C);
WriteData(0x00); 
LCD_delay(10); 
//Lcd_Light_ON;
LCD_LED=0;					//点亮背光
	
WriteComm(0x36);
WriteData(0x68);//设置为横屏显示,三原色的顺序是B，G，R
//	Lcd_ColorBox(0,0,800,480,YELLOW);
	
// 	LCD_Fill_Pic(80,160,320,480, gImage_MM_T035);
// 	BlockWrite(0,0,799,479);
}

/**
  * @breif  The lcd write reg function.
  * @param  reg -- the lcd register address.
  * @retval None
  */
static void lcd_write_reg(uint16_t reg)
{
	*(__IO u16*) (Bank1_LCD_R) = reg;
}

/**
  * @breif  The lcd write data function.
  * @param  data -- the lcd data value.
  * @retval None
  */
static void lcd_write_data(uint16_t data)
{
	*(__IO u16*) (Bank1_LCD_D) = data; 
}


/**
  * @breif  The lcd gpio init function.
  * @param  None
  * @retval return the lcd data.
  */
static uint16_t lcd_read_data(void)
{
    return *(__IO  u16 *) (Bank1_LCD_D);
}

void lcd_command_init(void)
{	
	lcd_write_reg(EXIT_SLEEP_MODE);
	delay_ms(20);
	
	lcd_write_reg(MANUFACTURER_COMMAND_PROTECT);
	lcd_write_data(0x04);
	
	lcd_write_reg(FRAME_MEMORY_SETTING);
	lcd_write_data(0x02);
	lcd_write_data(0x00);
	
	lcd_write_reg(PANEL_DRIVING_SETTING2);
	lcd_write_data(0x23); 
	/* ?? -- 800 */
	lcd_write_data(0x31);
	/* set output waveform -- mode2,pattern7 */
	lcd_write_data(0x99);
	/* set GSPF -- 51clk */
	lcd_write_data(0x21); 
    /* set GSPS -- 50clk */	
	lcd_write_data(0x20);
	/* none */
	lcd_write_data(0x00);
	/* set clk 2MHz */
	lcd_write_data(0x10);
	/* set RTN */
	lcd_write_data(0x28);
	/* set BP -- 12lines */
	lcd_write_data(0x0C);
	/* set FP -- 10lines */
	lcd_write_data(0x0A);
	/* none */
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	/* default */
	lcd_write_data(0x21);
	lcd_write_data(0x01);
	
	lcd_write_reg(DISPLAY_VTIME_SETTING);
	/* none */
	lcd_write_data(0x00);
	/* set GND per-change period -- 6clk */
	lcd_write_data(0x06);
	/* set VCI per-change period -- 6clk */
	lcd_write_data(0x06);
	/* set VSp per_change period -- 1clk */
	lcd_write_data(0x01);
	/* default */
	lcd_write_data(0x03);
	lcd_write_data(0x00);
	
	/* ???? */
	lcd_write_reg(GAMMA_SETTING_A);
	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);

	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);
	
	lcd_write_reg(GAMMA_SETTING_B);
	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);
	
	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);
	
    lcd_write_reg(GAMMA_SETTING_C);
	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);
	
	lcd_write_data(0x01);
	lcd_write_data(0x0A);
	lcd_write_data(0x12);
	lcd_write_data(0x1C);
	lcd_write_data(0x2B);
	lcd_write_data(0x45);
	lcd_write_data(0x3F);
	lcd_write_data(0x29);
	lcd_write_data(0x17);
	lcd_write_data(0x13);
	lcd_write_data(0x0F);
	lcd_write_data(0x04);
	
	/* charge pump setting */
	lcd_write_reg(POWER_SETTING1);
	/* set DC */
	lcd_write_data(0x99);
	/* default */
	lcd_write_data(0x03);
	/* set VCL VSP voltage */
	lcd_write_data(0xCE);
	/* clock cycle */
	lcd_write_data(0xA6);
	/* set a set-up circuit method */
	lcd_write_data(0x00);
	/* set VGH VGL voltage */
	lcd_write_data(0x43);
	/* default */
	lcd_write_data(0x20);
	lcd_write_data(0x10);
	lcd_write_data(0x01);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0x01);
	lcd_write_data(0x00);
	lcd_write_data(0x03);
	lcd_write_data(0x01);
	lcd_write_data(0x00);
	
	lcd_write_reg(POWER_SETTING3);
	/* set display quality -- 100%*/
	lcd_write_data(0x33);
	
	lcd_write_reg(VPLVLVNLVL_SETTING);
	lcd_write_data(0x2A);
	lcd_write_data(0x2A);
	
	/* test mode2 */
	lcd_write_reg(0xD6);
	lcd_write_data(0xA8);
	
	lcd_write_reg(0xD6);
	lcd_write_data(0x01);
	
	lcd_write_reg(VCOMDC_SETTING1);
	/* set VCCOM output voltage */
	lcd_write_data(0x01);
	lcd_write_data(0x4F);
	
	lcd_write_reg(VCOMDC_SETTING2);
	/* SET VCCOM output voltage */
	lcd_write_data(0x4F);
	
	lcd_write_reg(VDC_SEL_SETTING);
	/* set VCCOM ouput level */
	lcd_write_data(0x03);
	
	delay_ms(20);
	
	lcd_write_reg(0x2A);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0xDF);
	
	lcd_write_reg(0x2B);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x03);
	lcd_write_data(0x1F);	
	
	lcd_write_reg(SET_ADDRESS_MODE);
	/* set diaplay mode */
	lcd_write_data(0x60);
	lcd_write_reg(SET_PIXEL_FORMAT);
	/* set RGB colors */
	lcd_write_data(0x55);
	
	/* set display on */
	lcd_write_reg(SET_DISPLAY_ON);
	delay_ms(20);
	
	lcd_write_reg(WRITE_MEMORY_START);
}

/**
  * @breif  The function of set backcolor.
			????,???,??
  * @param  color is the sereen position.
  * @retval None
  */
void lcd_clear(uint16_t x_start,
 	           uint16_t x_end, 
               uint16_t y_start, 
               uint16_t y_end, 
			   uint16_t color)
{
	uint32_t index = 0;
//	x_start += 64;
//	x_end   += 64;
	
	LCD_REGX = lcddev.setxcmd;
	LCD_RAMX = x_start >> 8;
	LCD_RAMX = x_start & 0xFF;
	LCD_RAMX = x_end >> 8;
	LCD_RAMX = x_end & 0xFF;
	
	LCD_REGX = lcddev.setycmd;
	LCD_RAMX = y_start >> 8;
	LCD_RAMX = y_start & 0xFF;
	LCD_RAMX = y_end >> 8;
	LCD_RAMX = y_end & 0xFF;	
	
    LCD_REGX = lcddev.wramcmd;
	
	for(index = 0; index < 800*480; index++)
	{
	    LCD_RAMX = color;
	}
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
	LCD_REGX = SET_ADDRESS_MODE;
	/* set diaplay mode */
	LCD_RAMX = reg_val;
}

/**
  * @breif  The function of set lcd display direction.
			??????,?????????lcd????????????
			???,??
  * @param  display -- the direction mode.
  * @retval None
  */
void lcd_display_dir(uint8_t display_mode)
{
	switch(display_mode)
	{
	case HORIZON_DISPLAY:
		lcddev.dir = HORIZON_DISPLAY;
		lcddev.height  = 480;
		lcddev.width   = 800;
		lcddev.wramcmd = 0X2C;
		lcddev.setxcmd = 0X2A;
		lcddev.setycmd = 0X2B;  	
		break;
	
	case VERTICAL_DISPLAY:
		lcddev.dir = VERTICAL_DISPLAY;
		lcddev.height  = 800;
		lcddev.width   = 480;
		lcddev.wramcmd = 0x2C;
		lcddev.setxcmd = 0x2A;
		lcddev.setycmd = 0x2B;
		break;
	
	default:
		break;
	}
	/* ????????? */
	lcd_scan_dir(L2R_U2D);
}

void lcd_set_cursor(uint16_t x, uint16_t y)
{
//    x += 64;
	
	LCD_REGX = SET_COLUMN_ADDRESS;
	LCD_RAMX = x >> 8;
	LCD_RAMX = x & 0xFF;
	LCD_RAMX = x >> 8;
	LCD_RAMX = x & 0xFF;
	
	LCD_REGX = SET_PAGE_ADDRESS;
	LCD_RAMX = y >> 8;
	LCD_RAMX = y & 0xFF;
	LCD_RAMX = y >> 8;
	LCD_RAMX = y & 0xFF;
}

uint16_t lcd_read_point(uint16_t x, uint16_t y)
{
#if 1
	uint16_t a = 0;
	uint16_t b = 0;
	uint16_t data = 0;
	
	if(x > lcddev.width || y > lcddev.height)
	{
		return 0;
	}
	
//	x += 64;
	
	LCD_REGX = SET_COLUMN_ADDRESS;
	LCD_RAMX = x >> 8;
	LCD_RAMX = x & 0xFF;
	LCD_RAMX = x >> 8;
	LCD_RAMX = x & 0xFF;
	
	LCD_REGX = SET_PAGE_ADDRESS;
	LCD_RAMX = y >> 8;
	LCD_RAMX = y & 0xFF;
	LCD_RAMX = y >> 8;
	LCD_RAMX = y & 0xFF;
	
	LCD_REGX = READ_MEMORY_START;
	
	a = LCD_RAMX;
	a = 1;
	while(--a);
	a = LCD_RAMX;
	delay_us(1);	
	b = LCD_RAMX;
	data = (a&0xf800) | ((a&0x00fc)<<3) | (b>>11);
	return data;
#else
	WriteComm(0x2a);   
	WriteData(x>>8);
	WriteData(x);


	WriteComm(0x2b);   
	WriteData(y>>8);
	WriteData(y);


	WriteComm(0x2e);
	
	x = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
	x = *(__IO u16 *) (Bank1_LCD_D);
	y = *(__IO u16 *) (Bank1_LCD_D);

	//return (x&0xf800)|((x&0x00fc)<<3)|(y>>11);
	return 0xFFE0;
#endif
}

void lcd_data_bus_test(void)
{
	volatile uint16_t temp;
	lcd_set_cursor(1, 1);
	LCD_REGX = WRITE_MEMORY_START;
	LCD_RAMX = 0x5555;
	temp = lcd_read_point(1, 1);
	//printf("temp = %d\r\n", temp);
	if(temp == 0x5555)
	{
#ifdef DEBUG
		printf("lcd init success...\r\n");
#endif /* DEBUG */
	}
	else
	{
#ifdef DEBUG
		printf("lcd init failed...\r\n");
#endif /* DEBUG */		
	}
}

#if 0
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
#endif

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
//	WriteComm(0x2a);   
//	WriteData(x>>8);
//	WriteData(x);


//	WriteComm(0x2b);   
//	WriteData(y>>8);
//	WriteData(y);


//	WriteComm(0x2e);
//	
//	x = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
//	x = *(__IO u16 *) (Bank1_LCD_D);
//	y = *(__IO u16 *) (Bank1_LCD_D);

//	return (x&0xf800)|((x&0x00fc)<<3)|(y>>11);

	return 0;//故意注释掉正确值，返回一个错值，令其出现拖影
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

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xA0);
}

//在指定座标上打一个点
void DrawPixel(u16 x, u16 y, int Color)
{
	BlockWrite(x,x,y,y);

	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

/**
  * @breif  lcd display on function.
			?LCD,???,??
  * @param  None
  * @retval None
  */
void lcd_on(void)
{
	LCD_REGX = SET_DISPLAY_ON;
}



/**
  * @breif  lcd display off function.
			?LCD,???,??
  * @param  None
  * @retval None
  */
void lcd_off(void)
{
	LCD_REGX = SET_DISPLAY_OFF;
}





/**
  * @breif  The color convert function.
			RGB?BGR
  * @param  c is the color we need convert.
  * @retval return the color converted.
  */
uint16_t LCD_rgb2gbr(uint16_t c)
{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t rgb;   
	b=(c>>0) & 0x1f;
	g=(c>>5) & 0x3f;
	r=(c>>11) & 0x1f;	 
	rgb=(b<<11) + (g<<5) + (r<<0);		 
	return(rgb);
}

void lcd_draw_point(uint16_t x,
					uint16_t y,
					uint16_t color)
{
	lcd_set_cursor(x, y);
	LCD_REGX = lcddev.wramcmd;
	LCD_RAMX = color;
}





/**
  * @breif  The function of set the display block position and size.
            ????,?????,???,??
  * @param  x is the point's x_position
  * @param  y is the point's y_position
  * @param  color is the point's color.
  * @retval None
  */
void lcd_fast_draw_point(uint16_t x,
						 uint16_t y,
						 uint16_t color)
{
	//x += 64;
	LCD_REGX = SET_COLUMN_ADDRESS;
	LCD_RAMX = x >> 8;
	LCD_RAMX = x & 0xFF;
	
	LCD_REGX = SET_PAGE_ADDRESS;
	LCD_RAMX = y >> 8;
	LCD_RAMX = y & 0xFF;

	LCD_REGX = lcddev.wramcmd;
	LCD_RAMX = color;
}

/**
  * @breif  The function of set the display block position and size.
            ???????, ???,??
  * @param  x_start is the block x_start.
  * @param  x_end   is the block x_end.
  * @param  y_start is the block y_start.
  * @param  y_end   is the block y_end.
  * @retval None
  */
void lcd_set_block(uint16_t x_start, 
				   uint16_t y_start,
				   uint16_t x_end,
				   uint16_t y_end)
{
//	x_start += 64;
//	x_end += 64;
	LCD_REGX = lcddev.setxcmd;
	LCD_RAMX = x_start >> 8;
	LCD_RAMX = x_start & 0xFF;
	LCD_RAMX = x_end >> 8;
	LCD_RAMX = x_end & 0xFF;
	
	LCD_REGX = lcddev.setycmd;
	LCD_RAMX = y_start >> 8;
	LCD_RAMX = y_start & 0xFF;
	LCD_RAMX = y_end >> 8;
	LCD_RAMX = y_end & 0xFF;	
}



/**
  * @breif  The function of set the display block position and size.
            ???????,???,??
  * @param  x_start is the block x_start.
  * @param  x_end   is the block x_end.
  * @param  y_start is the block y_start.
  * @param  y_end   is the block y_end.
  * @param  color   is the pixel color.
  * @retval None
  */
void lcd_fill(uint16_t x_start,
			  uint16_t x_end,
			  uint16_t y_start,
			  uint16_t y_end,
			  uint16_t color)
{
	uint32_t i;
	
//	x_start += 64;
//	x_end += 64;
	
	LCD_REGX = lcddev.setxcmd;
	LCD_RAMX = x_start >> 8;
	LCD_RAMX = x_start & 0xFF;
	LCD_RAMX = x_end >> 8;
	LCD_RAMX = x_end & 0xFF;
	
	LCD_REGX = lcddev.setycmd;
	LCD_RAMX = y_start >> 8;
	LCD_RAMX = y_start & 0xFF;
	LCD_RAMX = y_end >> 8;
	LCD_RAMX = y_end & 0xFF;	
	
	LCD_REGX = lcddev.wramcmd;
	
	for(i = 0; i < (x_end - x_start + 1)*(y_end - y_start + 1); i++)
	{
		LCD_RAMX = color;
	}
}