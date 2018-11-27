/**************************************************************************************
*File Name: PPG-09.c
*CPU:STC12C5A60S2	Frequency: 11.0592MHZ
*Time: July 3, 2018
***************************************************************************************

Definition of new fuction register of Microcontroller:
P4SW----Port-4 switch 				Address:BBH   Reset Value: x000,xxxxB 
Reset Value is used to set: (NA/P4.4,ALE/P4.5,EX_LVD/P4.6) Second fuction of the three ports
===================================================================
AUXR1----Auxiliary Register1		Address: A2H   Reset Value: 0xxx,0xx0B
Reset Value is used to set: (PCA/PWM/SPI/UART2) At P1 or P4?
===================================================================
AUXR----Auxiliary Register			Address: 8EH   Reset Value: 0000,x000B
For setting T0/T1 speed,Serial mode 0 speed, independent baud rate generator, UART2 Baud Rate
Internal and External RAM access, serial port 1(UART1) baud rate generator, CLKOUT clock output
===================================================================
WAKE_CLK0----Clock output and Power-down Wakeup Control register
Address: 8FH	Reset Value: 0000,0000B
===================================================================
P4----P4 Port	          														Address: C0H  Reset Value: 1111,1111B
BRT----Independent buad rate genderator register		Address: 9CH	Reset Value: 0000,0000B
SADDR----Slave address control register							Address: A9H	Reset Value: 0000,0000B
SADEN----Slave address mask register								Address: B9H	Reset Value: 0000,0000B
WDT_CONTR----Watchdog control register							Address: C1H	Reset Value: 0x00,0000B
**************************************************************************************/  
#if 0
#include<REG51.h>
#include<intrins.h>
#include<stdio.h>
#include<stdlib.h>

/*************************** STC12C5A60S2--Extensions ***************************/
sfr			P4SW = 0xbb;			//See above
sfr			AUXR1 = 0xa2;			//See above
sfr			AUXR = 0x8e;			//See above
sfr			SADDR = 0xa9;			//See above
sfr			SADEN = 0xb9;			//See above
sfr			WDT_CONTR = 0xc1;	//Watchdog control register

sfr			P1ASF = 0x9d;			//P1 Analog Function Configure register
sfr			P1M1 = 0x91;			//P1 Port mode configuration register 1
sfr			P1M0 = 0x92;			//P1 Port mode configuration register 0
sfr			P0M1 = 0x93;			//P0 Port mode configuration register 1
sfr			P0M0 = 0x94;			//P0 Port mode configuration register 0
sfr			P2M1 = 0x95;			//P2 Port mode configuration register 1
sfr			P2M0 = 0x96;			//P2 Port mode configuration register 0
sfr			P3M1 = 0xb1;			//P3 Port mode configuration register 1
sfr			P3M0 = 0xb2;			//P3 Port mode configuration register 0
sfr			P4M1 = 0xb3;			//P4 Port mode configuration register 1
sfr			P4M0 = 0xb4;			//P4 Port mode configuration register 0
sfr			P4 = 0xc0;				//Port 4

sfr			CLK_DIV = 0x97;		//Clock divider register
sfr			S2CON = 0x9a;			//Serial port 2 control register
sfr			S2BUF = 0x9b;			//Serial port 2 data register
sfr			BRT = 0x9c;				//Independent buad rate genderator register

sfr			IP2 = 0xb5;				//Second interrupt priority low byte register
sfr			IP2H = 0xb6;			//Second interrupt priority high byte register
sfr			IPH = 0xb7;				//Interrupt priority high byte register

sfr			ADC_CONTR = 0xbc;	//A/D Conversion control register
sfr			ADC_RES = 0xbd;		//A/D Conversion result register high
sfr			ADC_RESL = 0xbe;	//A/D Conversion result register is low 

sfr			SPSTAT = 0xcd;		//SPI Status register
sfr			SPCTL = 0xce;			//SPI Control register
sfr			SPDAT = 0xcf;			//SPI Data register

/*PCA*/
sfr			CCON = 0xd8;			//PCA Control register
sbit		CCF0 = CCON^0;		//PCA Module 0 interrupt flag
sbit		CCF1 = CCON^1;		//PCA Module 1 interrupt flag
sbit		CR = CCON^6;			//PCA Counter array run control bit
sbit		CF = CCON^7;			//PCA Counter array overflow flag
sfr			CMOD = 0xd9;			//PCA Working mode register
sfr			CL = 0xe9;				//PCA 16-bit counter low 8 bits
sfr			CH = 0xf9;				//PCA 16-bit counter high 8 bits
sfr			CCAPM0 = 0xda;		//PCA Module 0 comparison / capture mode of operation
sfr			CCAP0L = 0xea;		//PCA Module 0 compare/capture register
sfr			CCAP0H = 0xfa;		//PCA Module 0 compare/capture register
sfr			CCAPM1 = 0xdb;		//PCA Module 1 comparison / capture mode of operation
sfr			CCAP1L = 0xeb;		//PCA Module 1 compare/capture register
sfr			CCAP1H = 0xfb;		//PCA Module 1 compare/capture register
sfr			PCAPWM0 = 0xf2;		//PCA Module 0 PWM Register
sfr			PCAPWM1 = 0xf3;		//PCA Module 1 PWM Register

/*EEPROM Define*/
sfr		IAP_DATA = 0xC2;		//Flash Data Register
sfr		IAP_ADDRH = 0xC3;		//Flash Adrr High
sfr		IAP_ADDRL = 0xC4;		//Flash Adrr Low
sfr		IAP_CMD = 0xC5;			//Flash Command Register
sfr		IAP_TRIG = 0xC6;		//Flash Command Register
sfr		IAP_CONTR = 0xC7;		//Flash Control Register

/* IE*/
sbit	ELVD = 0xae;
sbit	EADC = 0xad;

/********************************** Data Type Definition ***************************************/

#define		uchar	unsigned	char
#define		uint	unsigned	int
#define		ulon	unsigned	long
/*********************************** I/O Port Definition *****************************************/

sbit	DPY_1 = P0^0;					//Display Define
sbit	DPY_2 = P0^1;
sbit	DPY_3 = P0^2;
sbit	DPY_4 = P0^3;
sbit	DPY_5 = P0^4;
sbit	DPY_6 = P2^7;

sbit	SCLK = P0^5;
sbit	LCLK = P0^6;
sbit	S_DATA = P0^7;

sbit	Temp_Oven1 = P1^0;		//Temperature of Oven
sbit	Temp_Oven2 = P1^1;
sbit	Temp_Probe1 = P1^2;		//Temperature of Probe
sbit	Temp_Probe2 = P1^3;

sbit	ADC_Switch = P1^4;

sbit	Ctr_Mot = P2^0;
sbit	Ctr_Hot = P2^1;
sbit	Ctr_Fan = P2^2;
 
sbit	P_Temp = P3^2;
sbit	StartUp = P3^3;

/************************************ Bit Definition *******************************************/
uchar		bdata Flag1;
uchar		bdata Flag2;
uchar		bdata Flag3;
uchar		bdata Flag4; 

sbit		Sdown_F = Flag1^0;							// Entering the Shutdown mode flag for the first time Sdown_F = 1 means entering the mode for the first time
sbit		Feed_F = Flag1^1;								// Entering the Feed_F mode flag for the first time Feed_F = 1 means entering the mode for the first time
sbit		Others_F = Flag1^2;							// Entering the Others_F mode flag for the first time. Others_F = 1 means to enter this mode for the first time 
sbit		SEN_F = Flag1^3;								// Entering the SEN mode flag for the first time SEN_F = 1 means entering the mode for the first time
sbit		Out_T_F = Flag1^4;							// Barbecue temperature sampling end flag Out_T_F = 1 indicates the end of sampling
sbit		In_T_F = Flag1^5;								// In-furnace temperature sampling end flag In_T_F = 1 indicates the end of sampling
sbit		Feed_Mode_Start_F = Flag1^6;		// Feed mode execution, only bypass for three seconds to execute
sbit		Shutdown_off_F = Flag1^7;				// Shutdown is completed, you must turn off the power

sbit		Wait_F1 = Flag2^0;							// Waiting flag "1" is valid	
sbit		Key_F1 = Flag2^1;								// Key cycle scan flag 10Ms once "1" is valid
sbit		M10_F = Flag2^2;								// 10 minute timekeeping time mark "1" is valid
sbit		M7_F = Flag2^3;									// 7 minute timekeeping time mark "1" is valid
sbit		P_Temp_F = Flag2^4;							// P_Temp button press flag "1" is valid
sbit		S_Up_F = Flag2^5;								// StartUp button press mark "1" is valid
sbit		Wait_Feed = Flag2^6;						// Feed waiting flag "1" is valid
sbit		ADC_F = Flag2^7;								// KEY ADC sampling flag 0: end of sampling, 1: sampling

sbit		PWNON_F = Flag3^0;							//Power On flag "1" is valid
sbit		Safe_F = Flag3^1;								//Safe mode flag "1" is valid
sbit		Flame_F = Flag3^2;							//Flame flag "1" is valid
sbit		PRERR_F = Flag3^3;							//PRERR flag "1" is valid
sbit		SENSR_F = Flag3^4;							//SENSR flag "1" is valid
sbit		T_ADC_F = Flag3^5;							//Temp ADC sampling flag 0: end of sampling, 1: sampling
sbit		S1_DIS_F = Flag3^6;							//1S timing flag 1: timing time to
sbit		T_ERR_F = Flag3^7;							//A flag "1" with a temperature below a certain value indicates that an error has occurred.

sbit		RUN_F = Flag4^0;								//Whether "1" is valid in the running mode

/************************************ RAM Definition ********************************************/
uchar		Ms10_Counter;				//10Ms timer Ms10_Counter=100
uint		Time_Counter;				//Timer		
uint		Time_Sum;						//The total number of timers is set according to actual needs
uint		S1_Counter_10;			//1S timer S1_Counter_10=10000
uint		M10_Counter;				//10 minute timer M10_Counter=600
uint		S1_Counter_7;				//1S timer S1_Counter_7=10000
uint		M7_Counter;					//7 minute timer M7_Counter=420
uchar		S3_Counter;					//StartUp button press count
uint		S1_Counter_F;				//Feed 1S timer
uint		Feed_Counter;				//Feed timing, was uchar, changed for 1/10s feed timers
uint		S1_Counter_Flame;		//Flame 1S timer
uint		Flame_Counter;			//Flame timer
uint		Time_Feed;					//Feeding XS timer, was uchar, changed for 1/10s feed timers
uchar		Feed_Num;						//FEED order
uint		S1_Counter_DIS;			//1S timer S1_Counter_DIS=10000

uint		Other_Counter;			// 4, 6, 8 minute timer

uchar		Display_HData;			//Value to be displayed high 8 digits
uchar		Display_LData;			//Value to be displayed lower 8 digits
uchar		Display_Bit;				//Number of digits of the digital tube to be displayed 1 -- 5
uchar		Data_Bit;						//The bit value of the number to be displayed 1 -- 14

uchar		Channel;						//ADC input channel selection 0 -- 4
uint		Switch_Counter;			//Rotary switch gear 1 -- 15, was uchar, changed for 1/10s feed timers
uint		Switch_Code;				//Used to determine if the current position is the same as the label in the code, was uchar, changed for 1/10s feed timers

uchar		Start_Status;				//Start button status register 1--3
uchar		Probe_Status;				//Probe button status register 1--3

uint		ADC_Buff;						//AD sample value register

uint		In_Temp;						//Furnace temperature
ulon		In_Temp1;				
ulon		In_Temp2;
uint		Out_Temp;						//Outside furnace temperature
ulon		Out_Temp1;				
ulon		Out_Temp2;

ulon		Temp_DIS1;					//Temperature register for display
ulon		Temp_DIS2;		
ulon		Temp_DIS3;					//Temperature register for display

ulon		Temp_Buff1;					//Temperature register
ulon		Temp_Buff2;
ulon		Temp_Buff3;
ulon		Temp_Buff4;

ulon		R_Value;						//Store the calculated resistance value
uchar		ADC_Counter;				//Sample count register
uchar		T_Channel;					//Temperature sampling channel

uchar		Probe_Add=0;				//Each time the probe button is pressed, one is added, which is used to judge the number of times.
uchar		P1_Temp_F=0;				//Probe1 presses the flag bit, presses 1 for
uint		Out1_Temp=0;				//1 outside the furnace
ulon		Out1_Temp1=0;				
ulon		Out1_Temp2=0;
ulon		Temp_Buff5=0;
ulon		Temp_Buff6=0;
uchar		PRERR1_F = 0;				//PRERR flag "1" is valid
uchar		Out1_T_F = 0;				//Barbecue temperature sampling end flag Out_T_F = 1 indicates the end of sampling


uchar		Dis_C;							//Select C or F for display values, 0 is Fahrenheit and 1 is Celsius


ulon		Temp_Ini;   				//----------------------------------------------------------------------------------  MOD
int			Temp_Delta;  
uint    Temp_Set;   
int			Temp_Err;
ulon		S1_Counter_PID;     // 1 second counter for "I" in PID
uint		Ms_Counter_Fan;			// PWM counter for fan
uint		S1_Counter_TempAvg;	// Counter for averaging temperature
uint		Counter_Disp_Timer;
uint		Temp_Sum;
uint		Temp_Avg;
uint		Temp_Err_Sample;		// toggle sample for PID
long		I_Err_Sum;					// sum of error for "I" in PID
long		I_Err_Sum_Max;
int			Temp_Err_Prev;	
float		D_Err_Slope;
int			Auger_On_Ini;
int			Auger_On;
int			Auger_Off;
float		P_out;
float		I_out;
float		D_out;
uint		Toggle_Temp_Ini;		// delete???
float		Smoke_Mult;
int			Auger_On_Max;
float		Auger_Ratio_Offset;
float		Auger_Calc_Slope;
float		Auger_Ratio;
int			Auger_On_Min;					
int			Auger_Off_Min;
uint		PID_dt;							// toggle sample time for PID
float 	Fan_Off_Low, Fan_Off_High, m_slope, b_offset, Fan_Off_Ratio;
float		Auger_Ratio_Low, Auger_Ratio_High, m_slope_auger, b_offset_auger, Auger_On_Ratio;
uint		Temp_Sum;
uint		Temp_Avg;
uint		Temp_Prev;
uint		Num_Temp_Samples;
uint		MODEL;
uchar		ProbeNum2;
uint		Flame_Temp;
uint		Temp_Sample_Trigger;
uint		Temp_Sample;
uint		Feed_Rate;  				// TEST DISPLAY ONLY  - Remove after testing
uchar		OvrTmp_Err;					// Over Temperature Error  
uchar		Sensor_Fault;				// Chamber temp probe faulted
uint		S1_Shutdown_Counter;
uint		S1_Counter_SD;
uint		Switch_Counter_Ini; //-------------------------------------------------------------------------  MOD



/************************************ Constant Definition *****************************************/
#define		ADC_Power			0x80	//ADC Power Control Bit
#define		ADC_SpeedLL		0x00	//420 Clocks
#define		ADC_SpeedL		0x20	//280 Clocks
#define		ADC_SpeedH		0x40	//140 Clocks
#define		ADC_SpeedHH		0x60	//70  Clocks
#define		ADC_Flag			0x10	//ADC Complete Flag
#define		ADC_Start			0x08	//ADC Start Control Bit

#define		Temp_Offset		0x07	//Temp offset	

/*********************************** Function Declaration ******************************************/
void		Init_IOData(void);					//Initialize port

void		Init_Display(void);					//Digital Display initialization subroutine
void		Shift_595(void);						//595 Shift
void		Sdown_Dis(void);						//Sdown Display subroutine
void		Feed_Dis(void);							//Feed Display subroutine
void		Dash_Dis(void);							//---- Display subroutine
void		Start_Dis(void);						//Start Display subroutine
void		Bpass_Dis(void);						//Bpass Display subroutine
void		Sen_Dis(void);							//Sen Display subroutine
void		Preer_Dis(void);						//Preer Display subroutine
void		Flame_Dis(void);						//Flame Display subroutine
void		Temp_Dis(uint Temp);				//Furnace temperature Display subroutine
void		PTemp_Dis(uint PTemp);			//Barbecue probe temperature display subroutine
void		Bit_Data(void);							//Judging whether each digit of the data is "1" or "0"
void		OvrTmp_Dis(void);						//OVer-Temp display																			*** NEW ***

void		Init_ADC(void);							//ADC initialization
void		ADC_Key(void);							//Rotary switch AD conversion subroutine
void		ADC_Temp(uchar a);					//Temperature AD conversion subroutine
void		ADC_Avg(uchar x, uchar y);	//Average the AD sample value x: channel 0-3; y: number of cycles
void		Temp_Value(void);						//Calculate the furnace temperature value
void		PTemp_Value(void);					//Calculate the temperature of the barbecue

void		Switch_Display(void);				//Rotary switch gear position confirmation subroutine
void		Switch_Ctr(void);						//Rotary switch
void		Start_Mode(void);						//Start mode
void		Sdown_Mode(void);						//Shutdown mode
void		Feed_Mode(void);						//Feed mode
void		Run_Dis(void);							//Display mode subroutine
void		Run_Ctr(void);							//Control subroutine of operating mode with sensor


void		Feed_XS_YS(uint x, uint y);	//Send X seconds to stop Y seconds,      								*** changed from uchar ***
void		Safty_Ctr1(void);						//Security protection subroutine 7
void		Safty_Ctr2(void);						//Security protection subroutine 15
void		Safty_Ctr3(void);						//Feed stop, Fan 10, Display "FLAME"
void		Safty_Ctr4(void);						//Feed stop, Fan 10, Display "SENSR"
void		Safty_Ctr5(void);						//Security protection subroutine 6, 15
void		Safty_Ctr6(void);						//Over temp shutdown  																	*** NEW ***

void		Start_Scan(void);						//Start_Bypass Key scan subroutine
void		Probe_Scan(void);						//Probe Key scan subroutine
uint		RtoT_Scan(ulon i);					//In-furnace resistance and temperature one-to-one correspondence table
uint		PRtoT_Scan(ulon i);					//Barbecue resistance and temperature one-to-one correspondence table	
/********************************************************************/
uchar		code	Section_Data[10]={0x3f,0x06,0xdb,0xcf,0xe6,0xed,
								      0xfd,0x07,0xff,0xef};
/*
uchar		code	Section_HData[37]=	{
																	0x00,0x00,0x00,0x00,0x00,0x00,			// 0, 1, 2, 3, 4, 5
																	0x00,0x00,0x00,0x00,0x00						// 6, 7, 8, 9, BLANK
																	0x00,0x12,0x00,0x12,0x00,0x00,			// A, B, C, D, E, F
																	0x00,0x00,0x12,0x00,0x24,0x00,			// G, H, I, J, K, L
																	0x05,0x21,0x00,0x00,0x20,0x20,			// M, N, O, P, Q, R
																	0x00,0x12,0x00,0x0C,0x28,0x2D,			// S, T, U, V, W, X
																	0x00,0x0C														// Y, Z
																	};
uchar		code	Section_LData[37]=	{
																	0x3f,0x06,0xdb,0xcf,0xe6,0xed,			// 0, 1, 2, 3, 4, 5
																	0xfd,0x07,0xff,0xef,0x00						// 6, 7, 8, 9, BLANK
																	0xF7,0x8F,0x39,0x0F,0x79,0x71,			// A, B, C, D, E, F
																	0xBD,0xF6,0x09,0x1E,0x70,0x38,			// G, H, I, J, K, L
																	0x36,0x36,0x3F,0xF3,0x3F,0xF3,			// M, N, O, P, Q, R
																	0xED,0x01,0x3E,0x30,0x36,0x00,			// S, T, U, V, W, X
																	0xEE,0x09														// Y, Z
																	};
*/											
uint		code	T_Data[] = {-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,
							  	-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,
							  	11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
							  	31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
							  	51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,
							  	71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
							  	91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,
							  	111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,
							  	131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,
							  	151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,
							  	171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,
							  	191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,
							  	211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,
							  	231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,
							  	251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,
								271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,
								291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,
								311,312,313,314,315,316,317,318,319};

/************************************************* Furnace thermistor ************************************************/
//ulon		code	R1_Data[] = {10357516,9756348,9193882,8667393,8174363,7712469,7279557,6873636,6492865,6135535,
//								 5800070,5484999,5188967,4910712,4649064,4402935,4171317,3953271,3747924,3554465};		// -20 -- -1

ulon		code	R2_Data[] = {3372138,3200240,3038116,2885156,2740790,2604488,2475755,2354129,2239178,2130500,
								 2027718,1930480,1838457,1751342,1668845,1590698,1516648,1446459,1379908,1316788};		// 0 -- 19

ulon		code	R3_Data[] = {1256904,1200073,1146123,1094894,1046233,1000000,956060,914288,874565,836781,
								 800832,766618,734047,703033,673492,645348,618528,592962,568586,545339};				// 20 -- 39

ulon		code	R4_Data[] = {523162,502002,481806,462526,444116,426532,409734,393683,378341,363675,
								 349650,336236,323404,311126,299374,288124,277352,267036,257154,247686};				// 40 -- 59

ulon		code	R5_Data[] = {238613,229916,221578,213583,205915,198558,191500,184726,178224,171982,
								 165988,160231,154701,149387,144281,139373,134655,130119,125756,121560};				// 60 -- 79

ulon		code	R6_Data[] = {117523,113639,109900,106302,102838,99503,96290,93196,90216,87344,
								 84576,81909,79337,76858,74467,72161,69936,67790,65719,63721};							// 80 -- 99

uint		code	R7_Data[] = {61792,59930,58132,56395,54718,53099,51534,50022,48560,47148,
								 45783,44463,43187,41952,40759,39604,38487,37406,36360,35347};							// 100 -- 119

uint		code	R8_Data[] = {34367,33418,32500,31610,30748,29914,29105,28322,27563,26827,
								 26114,25423,24753,24103,23473,22862,22269,21695,21137,20596};							// 120 -- 139

uint		code	R9_Data[] = {20070,19561,19066,18586,18119,17667,17227,16800,16385,15982,
								 15591,15211,14841,14482,14133,13794,13464,13143,12832,12528};							// 140 -- 159

uint		code	R10_Data[] = {12233,11947,11668,11396,11132,10875,10625,10382,10145,9915,
								  9690,9472,9259,9052,8850,8653,8462,8275,8094,7917};									// 160 -- 179

uint		code	R11_Data[] = {7744,7576,7412,7253,7097,6945,6797,6653,6512,6375,
								  6241,6111,5984,5859,5738,5620,5505,5392,5282,5175};									// 180 -- 199

uint		code	R12_Data[] = {5071,4968,4869,4771,4676,4583,4493,4404,4318,4233,
								  4151,4070,3991,3914,3839,3765,3694,3623,3555,3488};									// 200 -- 219

uint		code	R13_Data[] = {3422,3358,3295,3234,3174,3115,3058,3002,2947,2893,
								  2841,2789,2739,2690,2642,2594,2548,2503,2459,2416};									// 220 -- 239

uint		code	R14_Data[] = {2373,2332,2291,2252,2213,2175,2137,2101,2065,2030,
								  1995,1962,1929,1896,1865,1834,1803,1774,1744,1716};									// 240 -- 259

uint		code	R15_Data[] = {1688,1660,1633,1607,1581,1555,1530,1506,1482,1459,
								  1435,1413,1391,1369,1348,1327,1306,1286,1266,1247};									// 260 -- 279

uint		code	R16_Data[] = {1228,1209,1191,1173,1155,1138,1121,1104,1087,1071,
								  1055,1040,1025,1010,995,980,966,952,939,925};											// 280 -- 299

uint		code	R17_Data[] = {912,899,886,874,861,849,837,826,814,803,
								  792,781,770,760,749,739,729,719,710,700};												// 300 -- 319
/************************************************* Barbecue thermistor ************************************************/
//ulon		code	PR1_Data[] = {9758038,9205962,8688615,8203603,7748710,7321889,6921238,6544999,6191540,5859346,
//								  5547016,5253245,4976821,4716621,4471599,4240781,4023264,3818204,3624818,3442375};		// -20 -- -1

ulon		code	PR2_Data[] = {3270195,3107640,2954121,2809084,2672014,2542428,2419877,2303940,2194224,2090361,
								  1992007,1898841,1810559,1726881,1647540,1572290,1500898,1433144,1368825,1307749};		// 0 -- 19

ulon		code	PR3_Data[] = {1249734,1194612,1142223,1092417,1045053,1000000,957132,916333,877492,840505,
								  805274,771707,739717,709222,680144,652411,625954,600707,576610,553604};				// 20 -- 39

ulon		code	PR4_Data[] = {531635,510651,490602,471443,453130,435621,418878,402862,387539,372876,
								  358842,345405,332538,320214,308408,297096,286253,275860,265895,256338};				// 40 -- 59

ulon		code	PR5_Data[] = {247171,238376,229937,221836,214061,206594,199424,192537,185920,179562,
								  173452,167578,161930,156499,151276,146251,141417,136764,132286,127976};				// 60 -- 79

ulon		code	PR6_Data[] = {123825,119828,115978,112270,108697,105254,101935,98736,95652,92678,
								  89809,87042,84373,81797,79312,76912,74596,72360,70201,68115};							// 80 -- 99

ulon		code	PR7_Data[] = {66101,64155,62274,60457,58701,57003,55362,53775,52240,50755,
								  49319,47930,46586,45285,44026,42807,41627,40484,39378,38306};							// 100 -- 119

uint		code	PR8_Data[] = {37268,36263,35289,34345,33430,32543,31683,30850,30042,29258,
								  28498,27761,27045,26352,25678,25025,24391,23775,23178,22598};							// 120 -- 139

uint		code	PR9_Data[] = {22034,21487,20956,20440,19939,19452,18978,18518,18071,17637,
								  17215,16804,16405,16017,15640,15273,14915,14568,14230,13901};							// 140 -- 159

uint		code	PR10_Data[] = {13582,13270,12967,12672,12385,12106,11833,11568,11310,11059,
								   10814,10576,10343,10117,9896,9681,9472,9268,9069,8875};								// 160 -- 179

uint		code	PR11_Data[] = {8686,8501,8321,8146,7975,7808,7646,7487,7332,7181,
								   7034,6890,6749,6612,6479,6348,6221,6096,5975,5856};									// 180 -- 199

uint		code	PR12_Data[] = {5740,5627,5517,5409,5303,5200,5099,5001,4905,4811,
								   4719,4630,4542,4456,4372,4290,4210,4132,4055,3980};									// 200 -- 219

uint		code	PR13_Data[] = {3907,3836,3765,3697,3630,3564,3500,3437,3376,3315,
								   3257,3199,3142,3087,3033,2980,2928,2878,2828,2779};									// 220 -- 239

uint		code	PR14_Data[] = {2732,2685,2639,2594,2550,2507,2465,2424,2384,2344,
								   2305,2267,2230,2193,2157,2122,2088,2054,2021,1988};									// 240 -- 259

uint		code	PR15_Data[] = {1957,1925,1895,1865,1835,1806,1778,1750,1723,1696,
								   1670,1644,1619,1594,1570,1546,1522,1499,1477,1455};									// 260 -- 279

uint		code	PR16_Data[] = {1433,1412,1391,1370,1350,1330,1310,1291,1272,1254,
								   1236,1218,1201,1183,1166,1150,1134,1118,1102,1086};									// 280 -- 299

uint		code	PR17_Data[] = {1071,1056,1042,1027,1013,999,985,972,959,946,
								   933,920,908,896,884,872,860,849,838,827};											// 300 -- 319
/*******************************************************************
										Function name: main()
									Function: main function
											Time: 2018-07-09
********************************************************************/
void	main(void)
{
	EA=0;
	ELVD=0;
	EADC=0;
	ES=0;
	ET0=0;
	ET1=0;
	EX0=0;
	EX1=0;

	P1ASF = 0xff;			//P1.0,P1.1,P1.2,P1.3 All used as AD
	AUXR1 = 0x04;			// ADC_RES is used to store the upper 2 bits; ADC_RESL is used to store the lower 8 bits
	ADC_RES = 0;
	ADC_RESL = 0;

	TR0=0;
	TR1=0;

	Init_IOData();

	TMOD=0x22;				//T0 timer mode 2 timing 100uS T1 mode 2.
	TH0=0x9c;
	TL0=0x9c;

	EA=1;							//Open interrupt
	ET0=1;
	TR0=1;						//Start timing 
	TR1 = 1;					//Timer1 run
	EADC = 1;					//Allow AD conversion interrupt

	Init_Display();

	Init_ADC();

	while(1)
	{
		ADC_Key();			//Button AD conversion subroutine

		while(!ADC_F);
		ADC_F=0;
/***********************/

		if(Flame_F!=1)	ADC_Avg(T_Channel ,20);

		if(Key_F1==1)
		{
			Key_F1=0;
			Start_Scan();
			Probe_Scan();
		}

		if((Flame_F | SENSR_F | OvrTmp_Err | Sensor_Fault)!=1)	
			Switch_Ctr();
		else	
		{
			if(Flame_F==1)	
				Safty_Ctr3();
			else if(SENSR_F==1 | Sensor_Fault)	
				Safty_Ctr4();	
			else if (OvrTmp_Err==1) 
				Safty_Ctr6();			
		}
	}
}
/*******************************************************************
									Function name: Init_IOData()
								Function: Initialize the system
											Time: 2018-07-09
********************************************************************/
void	Init_IOData(void)
{
	P_Temp = 1;									//Button initialization
	StartUp = 1;

	Ctr_Mot = 1;								//Control output initialization
	Ctr_Hot = 1;
	Ctr_Fan = 1;

	SCLK = 0;										//Display control initialization
	LCLK = 0;
	S_DATA = 1;

	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;

	Wait_F1 = 0;
	Key_F1 = 0;	
	Sdown_F =1;
	M10_F = 0;
	Feed_F = 1;
	M7_F = 0;
	Others_F = 1;
	P_Temp_F = 0;
	S_Up_F = 0;
	Wait_Feed = 0;
	PWNON_F = 1;
	T_ERR_F = 0;
	Safe_F = 0;
	Flame_F = 0;
	PRERR_F = 0;
	SENSR_F = 0;
	SEN_F = 1;
	ADC_F = 0;
	T_ADC_F = 0;
	S1_DIS_F = 0;
	In_T_F = 0;
	Out_T_F = 0;

	Ms10_Counter = 0;
	Time_Counter = 0;
	Time_Sum = 0;
	S1_Counter_10 = 0;
	S1_Counter_7 = 0;
	S1_Counter_DIS = 0;
	M10_Counter = 0;
	M7_Counter = 0;
	S3_Counter = 0;
	Other_Counter = 0;
	S1_Counter_Flame = 0;
	Flame_Counter = 0;
	S1_Counter_F = 0;
	Feed_Counter = 0;
	Time_Feed = 0;
	Feed_Num = 1;

	Channel = 0;
	T_Channel = 0;
	Switch_Counter = 1;
	Switch_Code = 1;
	Start_Status = 0;
	Probe_Status = 0;


	In_Temp = 0;
	In_Temp1 = 0;
	In_Temp2 = 0;
	Out_Temp = 0;
	Out_Temp1 = 0;
	Out_Temp2 = 0;
	Temp_DIS1 = 0;
	Temp_DIS2 = 0;
	Temp_DIS3 = 0;
	ADC_Buff = 0;
	Temp_Buff1 = 0;
	Temp_Buff2 = 0;
	Temp_Buff3 = 0;
	Temp_Buff4 = 0; 
	R_Value=0;
	ADC_Counter = 0;

	Display_HData = 0x3f;
	Display_LData = 0xff;
	Display_Bit = 6;
	Data_Bit = 14;

	Feed_Mode_Start_F=0;
	Shutdown_off_F = 0;									//Shutdown is completed, you must turn off the power
	RUN_F=0;
	
//	----------------------------------	NEW variables for PID	
	Temp_Ini = 0;
  Temp_Delta = 0;       
	Temp_Err = 0;
	Auger_On_Ini = 0;
	Auger_On = 0;
	Auger_Off = 0;
	Toggle_Temp_Ini = 0;
	S1_Counter_PID = 0;  	 							 // counter for "I" in PID
	Ms_Counter_Fan = 0;
	S1_Counter_TempAvg = 0;
	Temp_Sum = 0;
	Temp_Avg = 0;
	Temp_Err_Sample = 0;								// toggle sample for PID
	I_Err_Sum	= 0;											// sum of error for "I" in PID
	Temp_Err_Prev = 0;
	D_Err_Slope = 0;
	P_out = 0;
	I_out = 0;
	D_out = 0;
	Smoke_Mult = 1;
	PID_dt = 0;
	Auger_On_Min = 0;					
	Auger_Off_Min = 0;
	Temp_Sum = 0;
	Temp_Avg = 0;
	Temp_Prev = 0;
	Num_Temp_Samples = 0;
	OvrTmp_Err = 0;											//Over Temperature Error  
	Sensor_Fault = 0;
	Switch_Counter_Ini = 0;		
	Flame_Temp = 0;
	Counter_Disp_Timer = 0;
	Temp_Sample_Trigger = 1;
	Temp_Sample = 0;
	S1_Shutdown_Counter = 0;
	S1_Counter_SD = 0;

//  ---------------------------  SELECT MODEL AND FEATURES	--------------------------------------------
		
		MODEL = 0;  													//  0 = PPG (Portable)  1 = DLX, WW, SG  2 = MG
		
		Dis_C = 0;														//	0 = F  1 = C  (temp display units, Fahrenheit or Celsius)
		
		ProbeNum2 = 1;												//  0 = One probe   1 = Two probes (number of meat probes)
		
//	-----------------------------------------------------------------------------------------------------




//  Calculate equation for PWM Auger_On ratio

		if (MODEL == 2)												// MG				REWORK AND TEST FOR MAGNUM  (temp values different)
		{
		Auger_On_Max = 300;
		Auger_On_Min = 0.13 * Auger_On_Max;
		Auger_Ratio_Low = 0.25;
		Auger_Ratio_High = 1.0;
		I_Err_Sum_Max = 18000;
		}	
		else if(MODEL == 1)										// DLX, WW, SG
		{
		Auger_On_Max = 300;
		Auger_On_Min = 0.13 * Auger_On_Max;
		Auger_Ratio_Low = 0.25;
		Auger_Ratio_High = 1.0;
		I_Err_Sum_Max = 36000;								// was 18000
		}
		else if (MODEL == 0)									// PPG
		{
		Auger_On_Max = 200;
		Auger_On_Min = 0.10 * Auger_On_Max;		// PPG Min feed 
		Auger_Ratio_Low = 0.12;
		Auger_Ratio_High = 0.70;
		I_Err_Sum_Max = 12000;
		}
		
		if (MODEL == 2)
		{
			m_slope_auger = (Auger_Ratio_High - Auger_Ratio_Low)/(600.0 - 160.0);  	// adjust auger on times from 160 - 600F (Temp_Set)
			b_offset_auger = Auger_Ratio_High - m_slope_auger * 600.0;							// determine b_offset from High value
		}
		else
		{
			m_slope_auger = (Auger_Ratio_High - Auger_Ratio_Low)/(500.0 - 160.0);  	// adjust auger on times from 160 - 500F (Temp_Set)
			b_offset_auger = Auger_Ratio_High - m_slope_auger * 500.0;							// determine b_offset from High value
		}
		
}
/*******************************************************************
									Function name: Init_ADC()
								Function: ADC initialization
											Time: 2013-07-16
********************************************************************/
void	Init_ADC(void)
{
	ADC_CONTR = ADC_Power | ADC_SpeedHH | ADC_Start | Channel;
	
	Time_Sum=100;			//wait 10ms
	Wait_F1=1;
	while(Wait_F1);
}
/*******************************************************************
									Function name: Init_Display(void)
										Function: ADC initialization
											Time: 2013-07-14
********************************************************************/
void	Init_Display(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;
  
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=100;			//wait 50ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
		
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
										Function name: Timer0()
										Function: Timing 100us
											Time: 2012.06.15
******************************************************/
void	timer0(void) interrupt 1 
{
	Ms10_Counter=++Ms10_Counter;
	Time_Counter=++Time_Counter;
	S1_Counter_F=++S1_Counter_F;
	S1_Counter_10=++S1_Counter_10;
	S1_Counter_7=++S1_Counter_7;
	S1_Counter_Flame = ++S1_Counter_Flame;
	S1_Counter_DIS=++S1_Counter_DIS;
	S1_Counter_PID=++S1_Counter_PID;
	Ms_Counter_Fan=++Ms_Counter_Fan;
	S1_Counter_TempAvg=++S1_Counter_TempAvg;
	Counter_Disp_Timer=++Counter_Disp_Timer;
	S1_Counter_SD=++S1_Counter_SD;
	
	if(Wait_F1==1)
	{
		if(Time_Counter>=Time_Sum)
		{
			Time_Counter=0;
			Wait_F1=0;
		}
	}
	else	Time_Counter = 0;

	if(Wait_Feed==1)
	{
		if(S1_Counter_F>=1000)							// was 10000, changed to 1/10 second for PID adjustments
		{
			S1_Counter_F=0;
			Feed_Counter=++Feed_Counter;
			if(Feed_Counter>=Time_Feed)
			{
				Feed_Counter=0;
				Wait_Feed=0;
				Feed_Num=Feed_Num+1;
				if(Feed_Num>=3)	Feed_Num=1;
			}

		}
	}
	else	S1_Counter_F=0;
 
	if(S1_Counter_PID>=10000)						   // Timer for PID sample, 1s timer 
	{
		PID_dt = ++PID_dt;				
		S1_Counter_PID = 0;
	}

	if(S1_Counter_SD>=10000)							 // Timer for Shutdown, 1s timer 
	{
		S1_Shutdown_Counter = ++S1_Shutdown_Counter;				
		S1_Counter_SD = 0;
	}
	
	if(Ms10_Counter>=100)									//10ms
	{
		Ms10_Counter = 0;
		Key_F1 = 1;
	}

	if(S1_Counter_DIS>=2000)							//0.2S
	{
		S1_Counter_DIS=0;
		S1_DIS_F=1;
	}

	if(M10_F == 0)												//10 min timer
	{
		if(S1_Counter_10>=10000)  					// 1S
		{
			S1_Counter_10=0;
			M10_Counter=++M10_Counter;
			Other_Counter = M10_Counter;
			if(M10_Counter>=600)							//shutdown 10 min
			{																	//This value affects the flame fan after 10 minutes of operation.
				M10_Counter=0;	
				M10_F=1;
			}
		}
	}
	else S1_Counter_10=0;

	if(M7_F == 0)            							// Typical 7 min feed cycle.  Reduced to 4.5 min due to too many pellets
	{
		if(S1_Counter_7>=10000)
		{
			S1_Counter_7=0;
			M7_Counter=++M7_Counter;
			if(MODEL == 0)										// PPG only
			{
				if(M7_Counter>=270)							// 270 = 4.5min (changed for shorter feed tube, PPG)
				{
					M7_Counter=0;
					M7_F=1;
				}
			}
			else
			{
				if(M7_Counter>=420)							// 420 = 7min
				{
					M7_Counter=0;
					M7_F=1;
				}
			}
		}
	}
	else	S1_Counter_7=0;

	if(Flame_F==0)
	{
		if(Safe_F == 1)
		{
			if(S1_Counter_Flame>=10000)
			{
				S1_Counter_Flame=0;
				Flame_Counter=++Flame_Counter;
			}
		}
			else	S1_Counter_Flame=0;
	}
	else 	S1_Counter_Flame=0;

}
/*****************************************************
										Function name: ADC_ISR()
										Function: ADC Interrupt
											Time: 2013.07.16
******************************************************/
void	ADC_ISR(void) interrupt 5
{
	switch(Channel)
	{
		case 4: 
		{
			Switch_Display(); 
			ADC_F=1;
		}break;

		default: 
		{
			ADC_Buff = (ADC_RES*256)+ADC_RESL;
			T_ADC_F = 0;
		}break;
	}

	
	ADC_CONTR &= !ADC_Flag;		//Clear Flag
}
/*****************************************************
										Function name: ADC_Key()
				Function: Rotary switch AD conversion subroutine
											Time: 2013.08.16
******************************************************/
void	ADC_Key(void)
{
	EADC = 0;			//Disable AD conversion interrupt
	ADC_F=0;

	Channel = 4;

	ADC_RES = 0;
	ADC_RESL = 0;
	EADC = 1;			//Allow AD conversion interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	ADC_CONTR = ADC_Power | ADC_SpeedHH | ADC_Start | Channel;
}
/*****************************************************
								Function name: ADC_Avg(uchar x, uchar y)
			Function: Average the AD sample value x: channel 0-3; y: number of cycles
											Time: 2013.08.17
******************************************************/
void	ADC_Avg(uchar x, uchar y)
{
	if(ADC_Counter<=y)
	{
		if(T_ADC_F==0)
		{
			switch(x)
			{
				case 0: Temp_Buff1=Temp_Buff1+ADC_Buff;	break;

				case 1: 
				{
					Temp_Buff2=Temp_Buff2+ADC_Buff;
				}break;

				case 2: Temp_Buff3=Temp_Buff3+ADC_Buff;	break;

				case 3:
			 	{
					Temp_Buff4=Temp_Buff4+ADC_Buff;
				}break;
				case 6:
			 	{
					Temp_Buff5=Temp_Buff5+ADC_Buff;
				}break;
				case 5:
			 	{
					Temp_Buff6=Temp_Buff6+ADC_Buff;
				}break;
			}

			ADC_Counter=ADC_Counter+1;
			ADC_Temp(x);
		}
	}
	else
	{	
		switch(x)
		{
			case 0:	In_Temp1=Temp_Buff1/y; break;
			
			case 1: In_Temp2=Temp_Buff2/y; break;

			case 2:	Out_Temp1=Temp_Buff3/y;	break;

			case 3:	Out_Temp2=Temp_Buff4/y;	break;

			case 6:	Out1_Temp1=Temp_Buff5/y; break;

			case 5:	Out1_Temp2=Temp_Buff6/y; break;

		}

		ADC_Counter=0;
		ADC_Buff=0;
		Temp_Buff1=0;
		Temp_Buff2=0;
		Temp_Buff3=0;
		Temp_Buff4=0;
		Temp_Buff5=0;
		Temp_Buff6=0;

		T_Channel = T_Channel+1;
		if(T_Channel==4) T_Channel = T_Channel+1;			//The fourth ADC is used for the button

		if(T_Channel>6)																//More than 6 indicates that all channels have been collected
		{
			T_Channel=0;

			if(In_Temp2<=2)															//Error	
			{
				if(RUN_F)	 																//Wrong in operation
				{
					SENSR_F = 1;
					In_T_F = 0;
					In_Temp1=0;
					In_Temp2=0;
					Temp_DIS1=0;
				}
				else
				{
					SENSR_F = 0;														//Show 0
					In_T_F = 1;
					In_Temp1=0;
					In_Temp2=0;
					Temp_DIS1=0;
					In_Temp=0;
				}
			}				
			else																				//Calculate furnace temperature
			{
				SENSR_F = 0;
				In_T_F = 1;
		
				R_Value=In_Temp1-In_Temp2;
				R_Value=(49000*R_Value)/In_Temp2;
		
				In_Temp1=0;
				In_Temp2=0;
		
				In_Temp=RtoT_Scan(R_Value)*9/5+32;
			}

			if(Out_Temp2<=2)														//Error
			{
				PRERR_F = 1;
				Out_T_F = 0;
				Out_Temp1=0;
				Out_Temp2=0;
				Temp_DIS2=0;
			}		
			else																				//Calculate barbecue temperature
			{
				PRERR_F = 0;
				Out_T_F = 1;
		
				R_Value=Out_Temp1-Out_Temp2;
				R_Value=(49000*R_Value)/Out_Temp2;
		
				Out_Temp1=0;
				Out_Temp2=0;
		
				Out_Temp=PRtoT_Scan(R_Value)*9/5+32;
				Out_Temp=Out_Temp+5;											//make up
			}


			if(Out1_Temp2<=2)														//Error
			{
				PRERR1_F = 1;
				Out1_T_F = 0;
				Out1_Temp1=0;
				Out1_Temp2=0;
				Temp_DIS3=0;
			}		
			else																				//Calculate the temperature of the barbecue 1
			{
				PRERR1_F = 0;
				Out1_T_F = 1;
		
				R_Value=Out1_Temp1-Out1_Temp2;
				R_Value=(49000*R_Value)/Out1_Temp2;
		
				Out1_Temp1=0;
				Out1_Temp2=0;
		
				Out1_Temp=PRtoT_Scan(R_Value)*9/5+32;
				Out1_Temp=Out1_Temp+5;										//make up
			}

		}
	}
	if(S1_DIS_F==1)
	{
		S1_DIS_F=0;
	
		if(In_T_F==1)
		{
			In_T_F=0;

/*			if(Temp_DIS1==0)			Temp_DIS1=In_Temp;	
			else if(Temp_DIS1>In_Temp)	Temp_DIS1=Temp_DIS1-1;
			else if(Temp_DIS1<In_Temp)	Temp_DIS1=Temp_DIS1+1;
			else 						Temp_DIS1=In_Temp;*/

			if(Temp_DIS1!=In_Temp)	Temp_DIS1=In_Temp;
		}

		if(Out_T_F==1)
		{
			Out_T_F=0;

/*			if(Temp_DIS2==0)			Temp_DIS2=Out_Temp;
			else if(Temp_DIS2>Out_Temp)	Temp_DIS2=Temp_DIS2-1;
			else if(Temp_DIS2<Out_Temp)	Temp_DIS2=Temp_DIS2+1;
			else 						Temp_DIS2=Out_Temp;*/

			if(Temp_DIS2!=Out_Temp)	Temp_DIS2=Out_Temp;
		}

		if(Out1_T_F==1)
		{
			Out1_T_F=0;

/*			if(Temp_DIS3==0)			Temp_DIS3=Out1_Temp;
			else if(Temp_DIS3>Out1_Temp)	Temp_DIS3=Temp_DIS3-1;
			else if(Temp_DIS3<Out1_Temp)	Temp_DIS3=Temp_DIS3+1;
			else 						Temp_DIS3=Out1_Temp;*/

			if(Temp_DIS3!=Out1_Temp)	Temp_DIS3=Out1_Temp;
		}

	}
}
/*****************************************************
								Function name: ADC_Temp(uchar a)
					Function: Temperature AD conversion subroutine
											Time: 2013.08.16
******************************************************/
void	ADC_Temp(uchar a)
{
	EADC = 0;												//Disable AD conversion interrupt

	Channel = a;

	ADC_RES = 0;
	ADC_RESL = 0;
	EADC = 1;												//Allow AD conversion interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	ADC_CONTR = ADC_Power | ADC_SpeedHH | ADC_Start | Channel;
	T_ADC_F = 1;
}
/*****************************************************
								Function name: Switch_Display()
							Function: Rotary switch subroutine
											Time: 2013.07.17
******************************************************/
void	Switch_Display(void)
{
	uint x;

	x = (ADC_RES*256)+ADC_RESL;
	x = x/61;

	if(Switch_Counter_Ini!=Switch_Counter)			// Check to see if selector switch has been changed.  If so, reset Faults and Counters.
	{
		Switch_Counter_Ini = Switch_Counter;
		T_ERR_F=0;
		Flame_Counter=0;
		Feed_Num=1;	
		I_Err_Sum = 0;
		P_out = 0;
		I_out = 0;
		D_out = 0;
		Temp_Sample_Trigger = 1;
		Temp_Sample = 0;
	}	
	
	switch(x)
	{
		case 0:
		{
			Switch_Counter = 1;
			Temp_Set = 0;
			Flame_F = 0;
		}break;
		case 1:												// shut-down
		{
			Switch_Counter = 1;
			Temp_Set = 0;
			Flame_F = 0;
			Smoke_Mult = 1.0;
		}break;
		case 2:												// feed
		{
			Switch_Counter = 2;
			Temp_Set = 0;
			Smoke_Mult = 1.0;							
		}break;
		case 3:												// low smoke
		{
			Switch_Counter = 3;
			Temp_Set = 160;
			Smoke_Mult = 1.0;
		}break;
		case 4:												// high smoke
		{
			Switch_Counter = 4;
			Temp_Set = 220;
			Smoke_Mult = 1.5;	
		}break;
		case 5:												// 175F
		{
			Switch_Counter = 5;
			Temp_Set = 175;
			Smoke_Mult = 1.0;
		}break;
		case 6:												// 200F
		{
			Switch_Counter = 6;
			Temp_Set = 200;
			Smoke_Mult = 1.0;
		}break;
		case 7:												// 225F
		{
			Switch_Counter = 7;
			Temp_Set = 225;
			Smoke_Mult = 1.0;
		}break;
		case 8:												// 250F
		{
			Switch_Counter = 8;
			Temp_Set = 250;
			Smoke_Mult = 1.0;
		}break;
		case 9:												// 275F
		{
			Switch_Counter = 9;
			Temp_Set = 275;
			Smoke_Mult = 1.0;
		}break;
		case 10:											// 300F
		{
			Switch_Counter = 10;
			Temp_Set = 300;
			Smoke_Mult = 1.0;
		}break;
		case 11:											
		{
			Switch_Counter = 11;
			if (MODEL == 2)
				Temp_Set = 350;						// 350F (MG)
			else
				Temp_Set = 325;						// 325F
			Smoke_Mult = 1.0;
		}break;
		case 12:
		{
			Switch_Counter = 12;
			if (MODEL == 2)
				Temp_Set = 400;						// 400F (MG)
			else
				Temp_Set = 350;						// 350F
			Smoke_Mult = 1.0;
		}break;
		case 13:
		{
			Switch_Counter = 13;
			if (MODEL == 2)
				Temp_Set = 450;						// 450F (MG)
			else
				Temp_Set = 375;						// 375F
			Smoke_Mult = 1.0;
		}break;
		case 14:
		{
			Switch_Counter = 14;
			if (MODEL == 2)
				Temp_Set = 500;						// 500F (MG)
			else
				Temp_Set = 400;						// 400F
			Smoke_Mult = 1.0;
		}break;
		case 15:
		{
			Switch_Counter = 15;
			if (MODEL == 2)
				Temp_Set = 580;						// High (580F MG)
			else
				Temp_Set = 500;						// High (500F)
			Smoke_Mult = 1.0;
		}break;
	}
}
/*****************************************************
									Function name: Start_Scan()
					Function: Start_Bypass button scan subroutine
											Time: 2013.08.11
******************************************************/
void	Start_Scan(void)
{
	if(Start_Status==0)
	{
		if(StartUp==0)		Start_Status=1;									//StartUp Press to exit	
		else	Start_Status=0;															//No key press
	}
	else
	{
		switch(Start_Status)
		{
			case 1:
			{
				if(StartUp==0)
				{
					Start_Status=2;															//StartUp button pressed		
				}
				else
				{
					Start_Status=0;
					S3_Counter = 0;
					S_Up_F=0;
				}
			}break;
			case 2:
			{
				if(StartUp!=0)
				{
					Start_Status=3;	
				}
				else
				{
					if((M10_F == 0) & (Switch_Counter==2))		  //Feed function execution
					{
						S3_Counter = ++S3_Counter;
						
						if(S3_Counter>=150)	
						{
							Feed_Mode_Start_F=1;
							M10_F=1;
							Other_Counter = 0;
			
						}
					 }
				
					if((M10_F == 0) & (Switch_Counter>2))
					{
						S3_Counter = ++S3_Counter;
						
						if(S3_Counter>=150)	
						{
							S_Up_F=1;
							M10_F=1;
							Other_Counter = 0;
			
							Ctr_Mot = 1;		
							Ctr_Hot = 1;
							Ctr_Fan = 1;
						}
					 }
				}
			}break;
			case 3:
			{
				if(StartUp==0)	Start_Status=2;
				else	
				{
					Start_Status=0;
					S3_Counter = 0;
					S_Up_F=0;
				}
			}break;
		}
	}
}
/*****************************************************
									Function name: Probe_Scan()
					Function: Probe button scanning subroutine
											Time: 2013.08.11
******************************************************/
void	Probe_Scan(void)
{
	if(Probe_Status==0)
	{
		if(P_Temp==0)	Probe_Status=1;						//P_Temp pressed to exit anti-shake
		else	Probe_Status=0;										//No key press
	}
	else
	{
		switch(Probe_Status)
		{
			case 1:
			{
				if(P_Temp==0)
				{
					Probe_Status=2;										//P_Temp key pressed
					if(!ProbeNum2) P_Temp_F = 1;
				}
				else
				{
					P_Temp_F = 0;
					Probe_Status=0;	
					if(ProbeNum2) S3_Counter=0;
				}
			}break;
			case 2:
			{
				if(P_Temp!=0)
				{
					Probe_Status=3;	
				}
				else if(ProbeNum2)
				{
					if(S3_Counter>=5)									//Long press does not work, does not press case3 to clear, then press to detect
					{
						P_Temp_F = P_Temp_F;
						P1_Temp_F = P1_Temp_F;
						
					}
					else
					{
						S3_Counter = ++S3_Counter;
						if(S3_Counter>=5)	
						{
							S3_Counter=5;

							Probe_Add++;
							if(Probe_Add==1) 
							{
								P_Temp_F=1;
								P1_Temp_F=0;
							}
							else if(Probe_Add==2)
							{
								P1_Temp_F=1;
								P_Temp_F=0;
							}
							else 
							{
								 P_Temp_F=0;		 						//Cleared for the third time
								 P1_Temp_F=0;
							}
							if(Probe_Add>=3)
								Probe_Add=0;
						}		
					}				
				}
			}break;
			case 3:
			{
				if(P_Temp==0)	Probe_Status=2;
				else	
				{
					Probe_Status=0;
					if(!ProbeNum2) P_Temp_F = 0;
					if(ProbeNum2) S3_Counter=0;
				}
			}break;
		}
	}
}
/*****************************************************
					Function name: Feed_XS_YS(uchar x, uchar y)
					Function: Send X seconds to stop Y seconds
											Time: 2012.07.19
******************************************************/
void	Feed_XS_YS(uint x, uint y)
{
	if(x>0)
	{
		if(Switch_Counter==Switch_Code)
		{
			Wait_Feed=1;
		}
		else
		{
			Wait_Feed=0;
			Switch_Code=Switch_Counter;
			S1_Counter_F=0;
			Feed_Counter=0;
			Feed_Num=1;
			Wait_Feed=1;
		}
		
		switch (Feed_Num)
		{
			case 1:
			{
				Time_Feed=x;
				Ctr_Mot = 0;		
//				Ctr_Hot = 1;
				Ctr_Fan = 0;				
			}break;
			case 2:
			{
				Time_Feed=y;
				Ctr_Mot = 1;		
//				Ctr_Hot = 1;
				Ctr_Fan = 0;						
			}break;
		}
	}
	else
	{
		Wait_Feed=0;
		S1_Counter_F=0;
		Feed_Counter=0;
		Feed_Num=1;

		Ctr_Mot = 1;									// was 0, changed to allow no pellet feed
//		Ctr_Hot = 1;
		Ctr_Fan = 0;		
	}
}
/*****************************************************
							Function name: Sdown_Dis(void)
						Function: Sdown Display subroutine
											Time: 2012.07.15
******************************************************/
void	Sdown_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x21;
				Display_LData = 0x36;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x2a;
				Display_LData = 0x36;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x3f;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x12;
				Display_LData = 0x0f;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xed;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Feed_Dis(void)
							Function: Feed Display subroutine
											Time: 2012.07.15
******************************************************/
void	Feed_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x12;
				Display_LData = 0x0f;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf9;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf9;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf1;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Dash_Dis(void)
								Function: Display subroutine
											Time: 2012.07.15
******************************************************/
void	Dash_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				Display_LData = 0xc0;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xc0;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xc0;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xc0;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xc0;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xc0;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Start_Dis(void)
							Function: Start Display subroutine
											Time: 2012.07.15
******************************************************/
void	Start_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x12;
				Display_LData = 0x01;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x20;
				Display_LData = 0xf3;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf7;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x12;
				Display_LData = 0x01;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xed;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Bpass_Dis(void)
							Function: Bpass Display subroutine
											Time: 2012.07.15
******************************************************/
void	Bpass_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				Display_LData = 0xed;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xed;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf7;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf3;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xfc;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Sen_Dis(void)
							Function: Sen Display subroutine
											Time: 2012.07.15
******************************************************/
void	Sen_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x20; // R
				Display_LData = 0xf3;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;	// O
				Display_LData = 0x3f;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;	//S
				Display_LData = 0xed;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x21;	// N
				Display_LData = 0x36;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;	// E
				Display_LData = 0xf9;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;	// S
				Display_LData = 0xed;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Preer_Dis(void)
							Function: Preer Display subroutine
											Time: 2012.07.15
******************************************************/
void	Preer_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x20;
				Display_LData = 0xf3;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x20;
				Display_LData = 0xf3;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf9;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf3;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;
				
				if(!ProbeNum2)
					Display_HData = 0x00;
				else
				{
				if(P_Temp_F == 1) Display_HData = 0x2d;	//x			 //1，第一个probe
				else Display_HData = 0x15;	  //y
				}

				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Flame_Dis(void)
							Function: Flame Display subroutine
											Time: 2012.07.28
******************************************************/
void	Flame_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				Display_LData = 0xf9;

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x15;
				Display_LData = 0x36;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf7;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x38;
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf1;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}

/*****************************************************
								Function name: OvrTmp_Dis(void)
							Function: Flame Over Temp Display
											Time: 2018.07.09
******************************************************/
void	OvrTmp_Dis(void)
{
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				Display_LData = 0xf3;			//  P

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x05;
				Display_LData = 0x36;			//  M
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x12;
				Display_LData = 0x01;			//	T
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x20;
				Display_LData = 0xf3;			//	R
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x0c;
				Display_LData = 0x30;			//	V

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x3f;			//	O

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}

/*****************************************************
								Function name: Temp_Dis(uint Temp)
							Function: Temperature display subroutine
											Time: 2013.07.15
******************************************************/
/*void	Temp_Dis(uint Temp)
{	
//																							TEST DISPLAY		
	uint	dat, num_1, num_10, num_100, number;
	
	number = Feed_Rate;
	num_100 = number/100;
	number = number%100;
	num_10 = number/10;
	num_1 = number%10;
	
	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;

				dat = Temp;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				dat = Temp/10;
				Temp = Temp - dat*10;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				dat = Temp/100;
				Temp = Temp - dat*100;

				Display_HData = 0x00;
				if(dat==0)	
				{	
					Display_LData = 0x00;
					DPY_3 = 1;
				}
				else 
				{
					Display_LData = Section_Data[dat];
					DPY_3 = 0;
				}
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
//				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				dat = num_1;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				dat = num_10;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];				
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;
				
//				dat = num_100;

//				Display_HData = 0x00;
//				Display_LData = Section_Data[dat];
				if(Ctr_Hot == 0)										// H
				{	
					Display_HData = 0x00;
					Display_LData = 0xF6;
				}
				else if(In_Temp > Temp_Sample)		 	// G
				{
					Display_HData = 0x00;
					Display_LData = 0xBD;
				}					
				else if(In_Temp <= Temp_Sample)		 	// L
				{
					Display_HData = 0x00;
					Display_LData = 0x38;
				}		
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);
	
	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
*/
//                             ORIGINAL DISPLAY

void	Temp_Dis(uint Temp)
{
	uchar	dat;

	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				if(Dis_C) Display_LData = 0x39;	//c
				else Display_LData = 0xf1;	//F

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				dat = Temp;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				dat = Temp/10;
				Temp = Temp - dat*10;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 5:
			{
				Display_Bit=--Display_Bit;

				dat = Temp/100;
				Temp = Temp - dat*100;

				Display_HData = 0x00;
				if(dat==0)	
				{	
					Display_LData = 0x00;
					DPY_5 = 1;
				}
				else 
				{
					Display_LData = Section_Data[dat];
					DPY_5 = 0;
				}
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
//				DPY_5 = 0;
				DPY_6 = 1;	 

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;	 
			case 6:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while( Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
//   END ORIGINAL DISPLAY

/*****************************************************
								Function name: PTemp_Dis(uint PTemp)
			Function: Barbecue probe temperature display subroutine
											Time: 2013.07.15
******************************************************/
void	PTemp_Dis(uint PTemp)
{
	uchar	dat;

	do
	{
		switch(Display_Bit)
		{
			case 1:
			{
				Display_Bit=--Display_Bit;
				
				Display_HData = 0x00;
				if(Dis_C) Display_LData = 0x39;	//C
				else Display_LData = 0xf1;	//F

				Shift_595();

				DPY_1 = 0;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 2:
			{
				Display_Bit=--Display_Bit;

				dat = PTemp;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 0;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 3:
			{
				Display_Bit=--Display_Bit;

				dat = PTemp/10;
				PTemp = PTemp - dat*10;

				Display_HData = 0x00;
				Display_LData = Section_Data[dat];
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 0;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 4:
			{
				Display_Bit=--Display_Bit;

				dat = PTemp/100;
				PTemp = PTemp - dat*100;
				
				Display_HData = 0x00;
				if(dat==0)	
				{	
					Display_LData = 0x00;
					DPY_4 = 1;
				}
				else 
				{
					Display_LData = Section_Data[dat];
					DPY_4 = 0;
				}
				
				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
//				DPY_4 = 0;
				DPY_5 = 1;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;

			case 5:
			{
				Display_Bit=--Display_Bit;

				Display_HData = 0x00;
				Display_LData = 0xf3;		// 'P'

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 0;
				DPY_6 = 1;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
			case 6:
			{
				Display_Bit=--Display_Bit;

				if(!ProbeNum2)
					Display_HData = 0x00;
				else
				{
					if(P_Temp_F == 1) Display_HData = 0x2d;				// X probe
					else Display_HData = 0x15;										// Y probe
				}

				Display_LData = 0x00;

				Shift_595();

				DPY_1 = 1;
				DPY_2 = 1;
				DPY_3 = 1;
				DPY_4 = 1;
				DPY_5 = 1;
				DPY_6 = 0;

				Time_Sum=30;			//wait 3ms
				Wait_F1=1;
				while(Wait_F1);
			}break;
		}
	}while(Display_Bit>0);

	Display_Bit=6;
	DPY_1 = 1;
	DPY_2 = 1;
	DPY_3 = 1;
	DPY_4 = 1;
	DPY_5 = 1;
	DPY_6 = 1;
}
/*****************************************************
								Function name: Shift_595(void)
										Function: 595 shift
											Time: 2013.07.15
******************************************************/
void	Shift_595(void)
{
	do
	{					
		Bit_Data();

		SCLK = 1;
		_nop_();
		SCLK = 0;

	}while(Data_Bit>0);

	Data_Bit=14;

	LCLK = 1;	
	_nop_();
	LCLK = 0;
}
/*****************************************************
								Function name: Bit_Data(void)
		Function: Judging whether each digit of the data is "1" or "0"
											Time: 2013.07.14
******************************************************/
void	Bit_Data(void)
{
	uchar x;

	switch(Data_Bit)
	{
		case 1:
		{
			x = 0x01&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 2:
		{
			x = 0x02&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 3:
		{
			x = 0x04&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 4:
		{
			x = 0x08&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 5:
		{
			x = 0x10&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 6:
		{
			x = 0x20&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 7:
		{
			x = 0x40&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 8:
		{
			x = 0x80&Display_LData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 9:
		{
			x = 0x01&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 10:
		{
			x = 0x02&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 11:
		{
			x = 0x04&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 12:
		{
			x = 0x08&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 13:
		{
			x = 0x10&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
		case 14:
		{
			x = 0x20&Display_HData;
			if(x!=0) S_DATA=1;
			else S_DATA=0;
			Data_Bit = --Data_Bit;
		}break;
	}
}
/*****************************************************
								Function name: Sdown_Mode()
							Function: Shutdown mode subroutine
											Time: 2013.09.01
******************************************************/
void	Sdown_Mode(void)
{
	if(PWNON_F != 1)
	{
		SEN_F = 1;
		Feed_F = 1;
		Others_F = 1;
		Other_Counter = 0;
		M7_F = 1;

		Safe_F = 0;																													//Clear Flame mode
		T_ERR_F = 0;
		Flame_Counter = 0;
		S1_Counter_Flame = 0;
		Flame_F = 0;
	
		if(Sdown_F == 1)																										//First entry
		{
			Sdown_F = 0;
			S1_Shutdown_Counter = 0;
			S1_Counter_SD = 0;
	
			Ctr_Mot = 1;																											//Shut Down mode, fan only
			Ctr_Hot = 1;
			Ctr_Fan = 0;
	
			Sdown_Dis();
		}
		else
		{
			if (S1_Shutdown_Counter > 1200)																		//Time up, end SHUTDOWN, @ 20 min
			{
				Ctr_Mot = 1;	
				Ctr_Hot = 1;
				Ctr_Fan = 1;
				Shutdown_off_F = 1;																							//Shutdown is completed, you must turn off the power
			}
			else																															//Slow feed to prevent burn-back into feed tube
			{		
				if((S1_Shutdown_Counter >= 300) && (S1_Shutdown_Counter <= 900))//Start 5% feed rate from 5 min to 15 min																														
					Feed_XS_YS(10,200); 																																	
				else
					Ctr_Mot = 1;
													
				if(P_Temp_F == 1)																								//Temp Displays
				{
					if(PRERR_F == 0)
					{
					//	PTemp_Dis(Temp_DIS2);																			//Show the temperature of the barbecue
						if(Dis_C) 
						{
							if(Temp_DIS2>=32) PTemp_Dis((Temp_DIS2-32)*5/9);					//C
							else PTemp_Dis(0);
						}
						else PTemp_Dis(Temp_DIS2);																	//Show the temperature of the barbecue	//F

					}
					else	Preer_Dis();
				}
				else if(P1_Temp_F == 1)	  																			//Second probe
				{
					if(PRERR1_F == 0)
					{
//						PTemp_Dis(Temp_DIS3);																			//Show the temperature of the barbecue
						if(Dis_C) 
						{
							if(Temp_DIS3>=32) PTemp_Dis((Temp_DIS3-32)*5/9);					//C
							else PTemp_Dis(0);
						}
						else PTemp_Dis(Temp_DIS3);																	//Show the temperature of the barbecue	//F
					}
					else	Preer_Dis();
				}
		
				else
				{
					Sdown_Dis();
				}
			}
		}
	}
	else	
	{
		if(P_Temp_F == 1)	
		{
			if(PRERR_F == 0)
			{
//				PTemp_Dis(Temp_DIS2);																					//Show the temperature of the barbecue
						if(Dis_C) 
						{
							if(Temp_DIS2>=32) PTemp_Dis((Temp_DIS2-32)*5/9);					//c
							else PTemp_Dis(0);
						}
						else PTemp_Dis(Temp_DIS2);																	//Show the temperature of the barbecue	//F
			}
			else	Preer_Dis();
		}
		else if(P1_Temp_F == 1)	  																					//Second probe
		{
			if(PRERR1_F == 0)
			{
//				PTemp_Dis(Temp_DIS3);																					//Show the temperature of the barbecue
						if(Dis_C) 
						{
							if(Temp_DIS3>=32) PTemp_Dis((Temp_DIS3-32)*5/9);					//c
							else PTemp_Dis(0);
						}
						else PTemp_Dis(Temp_DIS3);																	//Show the temperature of the barbecue	//F
			}
			else	Preer_Dis();
		}

		else
		{
			Dash_Dis();	   																										//-----
		}
		
	}
				
		if(In_Temp >= 600)																									//  Flame Over Temp Shutdown at 600F
			OvrTmp_Err = 1;
}
/*****************************************************
								Function name: Start_Mode()
							Function: Start mode subroutine
											Time: 2013.09.01
******************************************************/
void	Start_Mode(void)
{
	SEN_F = 1;
	Sdown_F = 1;
	Feed_F = 1;
	PWNON_F = 0;
	M7_F = 1;
	RUN_F=0;	

	if(Others_F == 1)			
	{
		Others_F = 0;
		M10_F = 0;
		Other_Counter = 0;
		S1_Counter_10=0;
		M10_Counter=0;

		Ctr_Mot = 0;																													//Start
		Ctr_Hot = 0;
		Ctr_Fan = 0;

		Start_Dis();
	}
	else
	{
		if(M10_F == 0)			
		{
			Start_Dis();

			if(Other_Counter > 480) 										 												// 8M End Start mode
			{
				Ctr_Mot = 1;		
				Ctr_Hot = 1;
				Ctr_Fan = 0;

				Other_Counter=0;			
				M10_Counter=0;
				M10_F=1;
				PID_dt = 0;
				I_Err_Sum = 0;
				S1_Counter_TempAvg = 0;
			}
			else if(Other_Counter >= 360)																				//6M Turn off hotrod
			{
				Ctr_Mot = 1;			
				Ctr_Hot = 1;
				Ctr_Fan = 0;
			}
			else if(((MODEL == 2) || (MODEL ==1)) && (Other_Counter >= 210))		//3.5M  Turn auger off  ---> used for DLX
			{
				Ctr_Mot = 1;		
				Ctr_Hot = 0;
				Ctr_Fan = 0;
			}
			else if((MODEL == 0) && (Other_Counter >= 165))											//2.75M  Turn auger off	---> used for PPG			
			{
				Ctr_Mot = 1;		
				Ctr_Hot = 0;
				Ctr_Fan = 0;
			}
			else
			{
				Ctr_Mot = 0;		
				Ctr_Hot = 0;
				Ctr_Fan = 0;
			}
		}
	}
	
			if(In_Temp >= 600)																									//  Flame Over Temp Shutdown at 600F
				OvrTmp_Err = 1;
}
			
/*****************************************************
								Function name: Feed_Mode()
							Function: Feed mode subroutine
											Time: 2013.09.01
******************************************************/
void	Feed_Mode(void)
{
	SEN_F = 1;
	Sdown_F = 1;
	Others_F = 1;
	PWNON_F = 0;
	Other_Counter = 0;
	M10_F = 1;

	Safe_F=0;												//Clear Flame mode
	T_ERR_F=0;
	Flame_Counter=0;
	S1_Counter_Flame=0;
	Flame_F = 0;


	if(Feed_F == 1)									//First entry
	{
		Feed_F = 0;
		M7_F = 0;
		S1_Counter_7=0;
		M7_Counter=0;

		Ctr_Mot = 0;									//Feed
		Ctr_Hot = 1;
		Ctr_Fan = 1;

		Feed_Dis();
	}
	else
	{
		if (M7_F == 1)								//time up, M7_F is feed time (was 7 minutes)  Changed to for PPG.  See Timers routine
		{
			Ctr_Mot = 1;		
			Ctr_Hot = 1;
			Ctr_Fan = 1;

			Dash_Dis();	
	//		Feed_Dis();		
		}
		else	Feed_Dis();
	}
}
/*****************************************************
								Function name: Run_Dis()
					Function: Display subroutine in run mode
											Time: 2013.08.30
******************************************************/
void	Run_Dis(void)
{
	if(S_Up_F==1)
	{
		Bpass_Dis();
	}
	else
	{
		if(P_Temp_F == 1)	
		{
			if(PRERR_F == 0)
			{
//				PTemp_Dis(Temp_DIS2);																//Show the temperature of the barbecue
				if(Dis_C) 
				{
					if(Temp_DIS2>=32) PTemp_Dis((Temp_DIS2-32)*5/9);		//C
					else PTemp_Dis(0);
				}
				else PTemp_Dis(Temp_DIS2);														//Show the temperature of the barbecue	//F
					
			}
			else	Preer_Dis();
		}
		else if(P1_Temp_F == 1)	  																//Second probe
		{
			if(PRERR1_F == 0)
			{
//				PTemp_Dis(Temp_DIS3);																//Show the temperature of the barbecue
				if(Dis_C) 
				{
					if(Temp_DIS3>=32) PTemp_Dis((Temp_DIS3-32)*5/9);		//C
					else PTemp_Dis(0);
				}
				else PTemp_Dis(Temp_DIS3);														//Show the temperature of the barbecue	//F
			}
			else	Preer_Dis();
		}

		else
		{
//			Temp_Dis(Temp_DIS1);																	//显示炉内的温度
				if(Dis_C) 
				{
					if(Temp_DIS1>=32) Temp_Dis((Temp_DIS1-32)*5/9);			//C
					else Temp_Dis(0);
				}
				else Temp_Dis(Temp_DIS1);															//Show the temperature of the barbecue	//F
		}
	}
}
/*****************************************************
								Function name: Run_Ctr()
		Function: Control subroutine of operating mode with sensor
											Time: 2013.09.01
******************************************************/
void	Run_Ctr(void)
{
			RUN_F=1;
//  																		PID Calculations							
//													PID = Kp*e(t) + Ki*SUM[e(t)dt] + Kd*[de(t)/dt]
			if(S1_Counter_TempAvg>=10000)  																				// 1s timer between Temperature samples
			{
				Temp_Sum = Temp_Sum + In_Temp;		
				Num_Temp_Samples = ++Num_Temp_Samples;
				S1_Counter_TempAvg = 0;
			}	
			
			if(PID_dt >= (Auger_On_Max/10))																				// Calculate P,I & D afer time >= dt  (seconds)
			{		
				Temp_Avg = Temp_Sum / Num_Temp_Samples;
				
				Temp_Err = (int)Temp_Set - (int)Temp_Avg;	
				
				if(abs(Temp_Err) <= (0.20*Temp_Set))																// Only calculate I if within % of set temp
				{											
					I_Err_Sum = I_Err_Sum + Temp_Err * (int)PID_dt;										// Ki [integral = integral + error*dt] (error/xx to slow error accumulation) 
						if(I_Err_Sum > I_Err_Sum_Max) I_Err_Sum = I_Err_Sum_Max;				// Larger number reacts slower, change in calc below	
						if(I_Err_Sum <-I_Err_Sum_Max) I_Err_Sum =-I_Err_Sum_Max;																
				}
					
				D_Err_Slope = (float)(Temp_Err - Temp_Err_Prev)/(float)PID_dt;			// Kd [Slope = (Err2-Err1)/dt]
				
				if(MODEL == 2)																											// *** MG ***
				{
//				P_out = 2.0 * (Temp_Err);
				P_out = 3.0 * ((float)Temp_Set/600.0) * ((float)Temp_Err);
					if(P_out > 50.0) P_out = 50.0;																		// Max X seconds for P
					if(P_out < -50.0) P_out =-50.0;
				I_out = (50.0 / I_Err_Sum_Max) * I_Err_Sum;													// Max X seconds for I
//				D_out = 40.0*(D_Err_Slope);																				// was 20
//					if(D_out > 100.0) D_out = 100.0;																// Max 10 seconds for D
//					if(D_out < -100.0) D_out =-100.0;
				}
				else if(MODEL == 1)																									// *** DLX, SE, WW ***
				{
//				P_out = 2.0 * (Temp_Err);
				P_out = 3.0 * ((float)Temp_Set/500.0) * ((float)Temp_Err);
					if(P_out > 50.0) P_out = 50.0;																		// Max X seconds for P
					if(P_out < -50.0) P_out =-50.0;
				I_out = (50.0 / I_Err_Sum_Max) * I_Err_Sum;													// Max X seconds for I    	------------------was 100.0 5/8/18
//				D_out = 40.0*(D_Err_Slope);																				// was 20, 									------------------worked ok at 40 5/9/18
//					if(D_out > 100.0) D_out = 100.0;																// Max 10 seconds for D
//					if(D_out < -100.0) D_out =-100.0;
				}
				else if(MODEL == 0)																									// *** PPG ***
				{
				P_out = 3.0 * ((float)Temp_Set/500.0) * ((float)Temp_Err);
					if(P_out > 50.0) P_out = 50.0;																		// Max 5 seconds for P		was 2  (6/27/18)
					if(P_out < -50.0) P_out =-50.0;
				I_out = (30.0 / I_Err_Sum_Max) * I_Err_Sum;													// Max 5 seconds for I    was 20 (6/27/18)
//				D_out = 50.0*(D_Err_Slope);		
//					if(D_out > 50.0) D_out = 50.0;																	// Max 5 seconds for P
//					if(D_out < -50.0) D_out =-50.0;			
				D_out = 0.0;																												// Remove if "D" used
				}
	
				Temp_Err_Prev = Temp_Err;					
					
				PID_dt = 0;
				S1_Counter_PID = 0;
				Temp_Sum = 0;
				Num_Temp_Samples = 0;
			} 

//	Calculate auger ON and OFF time			
			Auger_On_Ratio = (m_slope_auger * (float)Temp_Set) + b_offset_auger;
			Auger_On_Ini = Auger_On_Ratio * Auger_On_Max;
			Auger_On = Auger_On_Ini + P_out + I_out + D_out;
	
			if((Auger_On < Auger_On_Min)) 																				// || (In_Temp > Temp_Set)) *** used during testing
					Auger_On = Auger_On_Min;
			if(Auger_On > (Auger_On_Max * Auger_Ratio_High))
					Auger_On = Auger_On_Max * Auger_Ratio_High;
			
			Auger_Off = Auger_On_Max-Auger_On;
						
			Auger_On = Auger_On * Smoke_Mult;        															// Add smoke multiplier
			Auger_Off = Auger_Off * Smoke_Mult;

			Feed_XS_YS(Auger_On,Auger_Off);				
										
//  Flame Over Temp Shutdown at 600F
			if(In_Temp >= 600)
				OvrTmp_Err = 1;
		
//	Check for Flame-Out condition.
			
			if(((Temp_Set < 190) || (Temp_Set > 250)) && (In_Temp < 135))
				Safty_Ctr1();
			else if (((Temp_Set >= 190) && (Temp_Set <= 200)) && (In_Temp < (Temp_Set - 10)))
				Safty_Ctr2();
			else if (((Temp_Set > 200) && (Temp_Set <= 250)) && (In_Temp < (Temp_Set - 20)))
				Safty_Ctr2();
			else
				{
					Safe_F=0;
					Flame_Counter=0;
					S1_Counter_Flame=0;
					Flame_F = 0;
					Temp_Sample_Trigger = 1;
					Temp_Sample = 0;
					Ctr_Hot = 1;
				}
			
		Feed_Rate = ((float)Auger_On/(Auger_On + Auger_Off))*100;  						 //  used for testing (Temp_Dis)
				
}
/*****************************************************
								Function name: Safty_Ctr1()
		Function:Flame-out detection, below 190F or above 250F
											Time: 2018.07.09
******************************************************/
void	Safty_Ctr1(void)
{
	Safe_F=1;
	
			if(Flame_Counter >= 1080)																	// Flame-out error and shutdown after 18 min (1080) if temp does not increase
			{
				if(In_Temp <= Temp_Sample)
				{
					Ctr_Hot = 1;
					Ctr_Fan = 1;
					Ctr_Mot = 1;	

					Flame_Counter=0;
					S1_Counter_Flame=0;
					Flame_F = 1;
				}
				else if(In_Temp > Temp_Sample)													// Reset counter to 15 min (900) if temperature increases & record temp
				{
					Flame_Counter = 900;
					Temp_Sample_Trigger = 1;
				}
			}
		  else if((Flame_Counter >= 900) && Temp_Sample_Trigger)		// Record temp at 15 min (900)
			{		
				Temp_Sample = In_Temp;
				Temp_Sample_Trigger = 0;
			}
			
}
/*****************************************************
								Function name: Safty_Ctr2()
				Function:Flame-out detection, 200F to 250F
											Time: 2018.07.09
******************************************************/
void	Safty_Ctr2(void)
{
	Safe_F=1;
	
			if(Flame_Counter >= 1080)													// Flame-out error and shutdown after 18 min (1080)
			{			
				Ctr_Hot = 1;
				Ctr_Fan = 0;
				Ctr_Mot = 1;		

				Flame_Counter=0;
				S1_Counter_Flame=0;
				Flame_F = 1;
			}
			else if(Flame_Counter > 720)											// Start sampling for increase in temp again, 12 min
			{
				if(In_Temp > Temp_Sample)
					{
						Flame_Counter = 0;
						Temp_Sample_Trigger = 1;
					}
			}
			else if(Flame_Counter > 540)   										// turn off hotrod after 6 min (6+3 min = 540)
			{
				Ctr_Hot = 1;
			}
			else if((Flame_Counter >= 180) & Ctr_Hot)					// check temp at 3 min (180), turn on hotrod
			{
				if(In_Temp > Temp_Sample)
					Flame_Counter = 0;
				else 
					Ctr_Hot = 0;
				
				Temp_Sample_Trigger = 1;					
			}

			if(Temp_Sample_Trigger == 1)
			{
				Temp_Sample = In_Temp;
				Temp_Sample_Trigger = 0;
			}

}
/*****************************************************
								Function name: Safty_Ctr3()
				Function: Feed stop, Fan 10, Display "FLAME"
											Time: 2018.07.09
******************************************************/
void	Safty_Ctr3(void)
{
	Feed_F = 1;
	M7_F = 1;
	Others_F = 1;
	Other_Counter = 0;
	Sdown_F = 1;
	
	if(Safe_F == 1)	
	{
		Safe_F = 0;
		M10_F = 0;
		S1_Counter_10=0;
		M10_Counter=0;
	
		Ctr_Mot = 1;												//Shut Down
		Ctr_Hot = 1;
		Ctr_Fan = 0;
	}
	else
	{
		if (M10_F == 1)		
		{
			Ctr_Mot = 1;	
			Ctr_Hot = 1;
			Ctr_Fan = 1;
		}
	}

	Flame_Dis();
}
/*****************************************************
								Function name: Safty_Ctr4(void)
					Function: Feed stop, Fan 10, Display "SENSR"
											Time:2013.07.28
******************************************************/
void	Safty_Ctr4(void)
{
	Feed_F = 1;
	M7_F = 1;
	Others_F = 1;
	Other_Counter = 0;
	Sdown_F = 1;
	Sensor_Fault = 1;
	
	if((SEN_F | (!PWNON_F))== 1)
	{
		SEN_F =0;
		M10_F = 0;
		S1_Counter_10=0;
		M10_Counter=0;
	
		Ctr_Mot = 1;										//Shut Down, fan running
		Ctr_Hot = 1;
		Ctr_Fan = 0;
	}
	else
	{
		if (M10_F == 1)	 								//Turn off fan after 10 min
		{
			Ctr_Mot = 1;	
			Ctr_Hot = 1;
			Ctr_Fan = 1;
		}
		else if(PWNON_F==1)
		{
			Ctr_Mot = 1;	
			Ctr_Hot = 1;
			Ctr_Fan = 1;
		}
	}

	Sen_Dis();
}

/*****************************************************
								Function name: Safty_Ctr5(void)
					Function: 
											Time:xxxx.xx.xx							 
******************************************************/
/*void	Safty_Ctr5(void)
{

}
*/
/*****************************************************
								Function name: Safty_Ctr6(void)
						Function: OverTemp display and shutdown
											Time:2013.07.28
*****************************************************/
void	Safty_Ctr6(void)
{
	Feed_F = 1;
	M7_F = 1;
	Others_F = 1;
	Other_Counter = 0;
	Sdown_F = 1;

		Ctr_Mot = 1;							//Shut Down, No Fan
		Ctr_Hot = 1;
		Ctr_Fan = 1;

		OvrTmp_Dis();
}
/*****************************************************
								Function name: Switch_Ctr()
									Function: Rotary switch
											Time:2013.07.17
******************************************************/
void	Switch_Ctr(void)
{
	if(Shutdown_off_F ==0)						// Shutdown is completed, you must turn off the power
	{
	switch(Switch_Counter)
	{
		case 1:
		{
			RUN_F=0;		
			Sdown_Mode();
			Feed_Mode_Start_F=0;					//Screw in FEED again and do not execute	
		}break;

		case 2:
		{
			RUN_F=0;		
			if(Feed_Mode_Start_F==1)
			{
//				Feed_Mode_Start_F=0;
				Feed_Mode();
			}
			else   
			{
				Dash_Dis();									//display------
				Ctr_Mot = 1;								//Not executed
				Ctr_Hot = 1;
				Ctr_Fan = 1;

				SEN_F = 1;
				Sdown_F = 1;
				Others_F = 1;
				PWNON_F = 0;
				Other_Counter = 0;
				M10_F = 0;
			
				Safe_F=0;										//Clear Flame mode
				T_ERR_F=0;
				Flame_Counter=0;
				S1_Counter_Flame=0;
				Flame_F = 0;

//				SEN_F = 1;
//				Sdown_F = 1;
//				Feed_F = 1;	   						//First time into the feed timing
//				PWNON_F = 0;
				M7_F = 1;



			}
						
		}break;

		default:
		{
			Feed_Mode_Start_F=0;	 				//Screw in FEED again and do not execute
			Start_Mode();
			if(M10_F==1)									//Operating mode
			{
																		/************Display Parts***********/
				Run_Dis();

																		/************Control Parts***********/
				Run_Ctr();
			}					
		}break;
	}
	}
}
/*****************************************************
								Function name: RtoT_Scan(ulon)
	Function: In-furnace resistance and temperature one-to-one correspondence table
											Time:2013.08.27
******************************************************/
uint	RtoT_Scan(ulon i)
{
	uint x = 0;
	uint T = 0;
	ulon R_BUFF1 = 0;
	ulon R_BUFF2 = 0;
	
/*	if(i>=3554465)
	{
		do
		{
			if(i>=R1_Data[x])	
			{
				if(x>0)
				{
					R_BUFF1 = i-R1_Data[x];
					R_BUFF2 = R1_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x-1];
					else	T = T_Data[x];
				}
				else	T = T_Data[x];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}*/
	if(i>=1316788)
	{
		do
		{
			if(i>=R2_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R2_Data[x];
					R_BUFF2 = R2_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+19];
					else	T = T_Data[x+20];
				}
				else	T = T_Data[x+20];

				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=545339)
	{
		do
		{
			if(i>=R3_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R3_Data[x];
					R_BUFF2 = R3_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+39];
					else	T = T_Data[x+40];
				}
				else	T = T_Data[x+40];

				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=247686)
	{
		do
		{
			if(i>=R4_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R4_Data[x];
					R_BUFF2 = R4_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+59];
					else	T = T_Data[x+60];
				}
				else	T = T_Data[x+60];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=121560)
	{
		do
		{
			if(i>=R5_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R5_Data[x];
					R_BUFF2 = R5_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+79];
					else	T = T_Data[x+80];
				}
				else	T = T_Data[x+80];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=63721)
	{
		do
		{
			if(i>=R6_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R6_Data[x];
					R_BUFF2 = R6_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+99];
					else	T = T_Data[x+100];
				}
				else	T = T_Data[x+100];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=35347)
	{
		do
		{
			if(i>=R7_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R7_Data[x];
					R_BUFF2 = R7_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+119];
					else	T = T_Data[x+120];
				}
				else	T = T_Data[x+120];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=20596)
	{
		do
		{
			if(i>=R8_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R8_Data[x];
					R_BUFF2 = R8_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+139];
					else	T = T_Data[x+140];
				}
				else	T = T_Data[x+140];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=12528)
	{
		do
		{
			if(i>=R9_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R9_Data[x];
					R_BUFF2 = R9_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+159];
					else	T = T_Data[x+160];
				}
				else	T = T_Data[x+160];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=7917)
	{
		do
		{
			if(i>=R10_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R10_Data[x];
					R_BUFF2 = R10_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+179];
					else	T = T_Data[x+180];
				}
				else	T = T_Data[x+180];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=5175)
	{
		do
		{
			if(i>=R11_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R11_Data[x];
					R_BUFF2 = R11_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+199];
					else	T = T_Data[x+200];
				}
				else	T = T_Data[x+200];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=3488)
	{
		do
		{
			if(i>=R12_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R12_Data[x];
					R_BUFF2 = R12_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+219];
					else	T = T_Data[x+220];
				}
				else	T = T_Data[x+220];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=2416)
	{
		do
		{
			if(i>=R13_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R13_Data[x];
					R_BUFF2 = R13_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+239];
					else	T = T_Data[x+240];
				}
				else	T = T_Data[x+240];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=1716)
	{
		do
		{
			if(i>=R14_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R14_Data[x];
					R_BUFF2 = R14_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+259];
					else	T = T_Data[x+260];
				}
				else	T = T_Data[x+260];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=1247)
	{
		do
		{
			if(i>=R15_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R15_Data[x];
					R_BUFF2 = R15_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+279];
					else	T = T_Data[x+280];
				}
				else	T = T_Data[x+280];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=925)
	{
		do
		{
			if(i>=R16_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R16_Data[x];
					R_BUFF2 = R16_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+299];
					else	T = T_Data[x+300];
				}
				else	T = T_Data[x+300];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=700)
	{
		do
		{
			if(i>=R17_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-R17_Data[x];
					R_BUFF2 = R17_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+319];
					else	T = T_Data[x+320];
				}
				else	T = T_Data[x+320];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else T=319;				//If the temperature is greater than 606F, the force is 606F
//	T=T+2;
	return T;
}
/*****************************************************
									Function name: PRtoT_Scan(ulon)
	Function: Barbecue resistance and temperature one-to-one correspondence table
											Time:2013.08.31
******************************************************/
uint	PRtoT_Scan(ulon i)
{
	uint x = 0;
	uint T = 0;
	ulon R_BUFF1 = 0;
	ulon R_BUFF2 = 0;
	
	
/*	if(i>=3442375)
	{
		do
		{
			if(i>=PR1_Data[x])	
			{
				if(x>0)
				{
					R_BUFF1 = i-PR1_Data[x];
					R_BUFF2 = PR1_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x-1];
					else	T = T_Data[x];
				}
				else	T = T_Data[x];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}*/
	if(i>=1307749)
	{
		do
		{
			if(i>=PR2_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR2_Data[x];
					R_BUFF2 = PR2_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+19];
					else	T = T_Data[x+20];
				}
				else	T = T_Data[x+20];

				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=553604)
	{
		do
		{
			if(i>=PR3_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR3_Data[x];
					R_BUFF2 = PR3_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+39];
					else	T = T_Data[x+40];
				}
				else	T = T_Data[x+40];

				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=256338)
	{
		do
		{
			if(i>=PR4_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR4_Data[x];
					R_BUFF2 = PR4_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+59];
					else	T = T_Data[x+60];
				}
				else	T = T_Data[x+60];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=127976)
	{
		do
		{
			if(i>=PR5_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR5_Data[x];
					R_BUFF2 = PR5_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+79];
					else	T = T_Data[x+80];
				}
				else	T = T_Data[x+80];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=68115)
	{
		do
		{
			if(i>=PR6_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR6_Data[x];
					R_BUFF2 = PR6_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+99];
					else	T = T_Data[x+100];
				}
				else	T = T_Data[x+100];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=38306)
	{
		do
		{
			if(i>=PR7_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR7_Data[x];
					R_BUFF2 = PR7_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+119];
					else	T = T_Data[x+120];
				}
				else	T = T_Data[x+120];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=22598)
	{
		do
		{
			if(i>=PR8_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR8_Data[x];
					R_BUFF2 = PR8_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+139];
					else	T = T_Data[x+140];
				}
				else	T = T_Data[x+140];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=13901)
	{
		do
		{
			if(i>=PR9_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR9_Data[x];
					R_BUFF2 = PR9_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+159];
					else	T = T_Data[x+160];
				}
				else	T = T_Data[x+160];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=8875)
	{
		do
		{
			if(i>=PR10_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR10_Data[x];
					R_BUFF2 = PR10_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+179];
					else	T = T_Data[x+180];
				}
				else	T = T_Data[x+180];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=5856)
	{
		do
		{
			if(i>=PR11_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR11_Data[x];
					R_BUFF2 = PR11_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+199];
					else	T = T_Data[x+200];
				}
				else	T = T_Data[x+200];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=3980)
	{
		do
		{
			if(i>=PR12_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR12_Data[x];
					R_BUFF2 = PR12_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+219];
					else	T = T_Data[x+220];
				}
				else	T = T_Data[x+220];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=2779)
	{
		do
		{
			if(i>=PR13_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR13_Data[x];
					R_BUFF2 = PR13_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+239];
					else	T = T_Data[x+240];
				}
				else	T = T_Data[x+240];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=1988)
	{
		do
		{
			if(i>=PR14_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR14_Data[x];
					R_BUFF2 = PR14_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+259];
					else	T = T_Data[x+260];
				}
				else	T = T_Data[x+260];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=1455)
	{
		do
		{
			if(i>=PR15_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR15_Data[x];
					R_BUFF2 = PR15_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+279];
					else	T = T_Data[x+280];
				}
				else	T = T_Data[x+280];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else if(i>=1086)
	{
		do
		{
			if(i>=PR16_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR16_Data[x];
					R_BUFF2 = PR16_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+299];
					else	T = T_Data[x+300];
				}
				else	T = T_Data[x+300];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	else 
	{
		do
		{
			if(i>=PR17_Data[x])
			{
				if(x>0)
				{
					R_BUFF1 = i-PR17_Data[x];
					R_BUFF2 = PR17_Data[x-1]-i;

					if(R_BUFF1>=R_BUFF2)	T = T_Data[x+319];
					else	T = T_Data[x+320];
				}
				else	T = T_Data[x+320];
				
				x = 20;
			}
			else x=x+1;

		}while(x<=19);
	}
	
//	T=T+2;
	return T;
}
#endif