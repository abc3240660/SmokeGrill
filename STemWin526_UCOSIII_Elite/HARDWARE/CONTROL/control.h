#ifndef __control_H
#define __control_H	 
#include "sys.h"

#define FAN_I PBin(5)// PB5
#define MOT_I PBin(4)// PB6	
#define HOT_I PBin(3)// PC15

#define FAN PBout(5)// PB5
#define MOT PBout(4)// PB6	
#define HOT PBout(3)// PC15
#define Control_ON 0// 开	
#define Control_OFF 1// 开

#define HOT_Status  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) 
	
extern u16 g_flame_update_status;               

void IDLE_Mode(void);
void Control_Init(void);//初始化
void Startup_Mode(u16 counter); 
void Feed_Mode(u16 counter); 
void Shutdown_Mode(u16 counter); 
void Run_Mode_Start(void);
void Run_Mode_Later(void);
//void GetPeriod(u16 In_Temp_Set, u16 temp);
void Run_Mode_HOT_ON(int ON);
void Run_Mode_MOT_ON(int ON);
void Run_Mode_FLAME_OVER(void);
void Run_Mode_Startup_OVER(void);
void Sensor_error_Mode(u16 counter);
void Flame_error_Mode(u16 counter);
void Enter_RUN_MODE_SET(void);
#endif
