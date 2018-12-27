#include "control.h"
#include "PID.h"

u16 g_idle_mode_counter=0;
u16 g_run_mode_timeout_counter=0;
u16 g_startup_mode_counter=0;
u16 g_run_mode_counter_mins=0;
u16 g_run_mode_counter_sec=0;
u16 g_run_mode_counter_hour=0;
u16 g_feed_mode_counter=0;
u16 g_shutdown_mode_counter=0;
u16 g_flame_update_status=0;

// ����ģʽ�Ŀ�ʼ�ͽ�����־
// 0-end 1-start
extern int g_run_mode;


//����MOT HOT FAN��ʼ��
void Control_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PC�˿�ʱ��

 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //ʹ�ܽ���JTAG
	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz

/* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //�������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_15); 						 //PC15 ����� 
// GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6);//���0	  */

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); 						 //PA15 �����

// GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); 						 //PA15 �����
}
 //========================================================================
// ����: void Startup_Mode(void) 
// ����: ���ģʽ
// ����: ��
// ����: ��
//========================================================================
void Startup_Mode(u16 counter) 
{	
	if (counter<10) {
		FAN = Control_ON;
		HOT = Control_ON;
		MOT = Control_ON;
	} else if(counter>210) {
		MOT = Control_OFF;
	} else if(counter>=360) {
		HOT = Control_OFF;
	}
}	
//========================================================================
// ����: void Feed_Mode(void) 
// ����: FEEDģʽ
// ����: ��
// ����: ��
//========================================================================
void Feed_Mode(u16 counter) 
{
	if (counter<10) {
		FAN = Control_OFF;
		HOT = Control_OFF;
		MOT = Control_ON;
	} else if(counter>=420) {
		MOT = Control_OFF;
	}
}	
//========================================================================
// ����: void Shutdown_Mode(void) 
// ����: �ػ�ģʽ
// ����: ��
// ����: ��
//========================================================================
void Shutdown_Mode(u16 counter) 
{
	if (counter<10) {
		FAN = Control_ON;
		HOT = Control_OFF;
		//MOT = Control_OFF;
	} else if(counter>=1200) {
		FAN = Control_OFF;
	}
	
	if((counter<5*60)||(counter>15*60))
		MOT = Control_OFF;
	else
	{
		if((counter-5*60)%210 == 0)
			MOT = Control_ON;//����10s
		if((counter-5*60)%210 == 10)
			MOT = Control_OFF;//�ر�200s
	}

}	

void Sensor_error_Mode(u16 counter) 
{
	if (counter<10) {
		HOT = Control_OFF;//ֱ�ӹرռ��Ȱ�
		FAN = Control_ON;//ֱ�ӿ�������		
	} else if(counter>=1200) {
		FAN = Control_OFF;//20���ӵ����رշ���
	}
	
	if((counter<5*60)||(counter>15*60))
		MOT = Control_OFF;//0~5 15~20�ر����
	else
	{//�������
		if((counter-5*60)%210 == 0)
			MOT = Control_ON;//����10s
		if((counter-5*60)%210 == 10)
			MOT = Control_OFF;//�ر�200s
	}

}	

void Flame_error_Mode(u16 counter) 
{
	if (counter<10) {
		HOT = Control_OFF;//ֱ�ӹرռ��Ȱ�
		FAN = Control_ON;//ֱ�ӿ�������
	} else if(counter>=1200) {
		FAN = Control_OFF;//20���ӵ����رշ���
	}
	if((counter<5*60)||(counter>15*60))
		MOT = Control_OFF;//0~5 15~20�ر����
	else
	{//�������
		if((counter-5*60)%210 == 0)
			MOT = Control_ON;//����10s
		if((counter-5*60)%210 == 10)
			MOT = Control_OFF;//�ر�200s
	}

}	
//========================================================================
// ����: void Run_Mode(void) 
// ����: ����ģʽ
// ����: ��
// ����: ��
//========================================================================
void Run_Mode_Start(void) 
{
//	HOT = Control_OFF;
	FAN = Control_ON;
}
extern int g_fatal_error;
void Run_Mode_Later() 
{	
	if(iTemp == 0) MOT=Control_OFF;
	else MOT=Control_ON;
	
	if(iTemp) iTemp--;
}

// this func will be called every 60 secs
/*void GetPeriod(u16 In_Temp_Set, u16 temp)
{
    PID.uKP_Coe=30;
	PID.uKI_Coe=10;
	PID.uKD_Coe=5;
	PID.iSetVal=In_Temp_Set;

	if(In_Temp_Set>=180&&In_Temp_Set<=225)
	{
		Pid_down=15;
		Pid_up=40;
	}
	else if(In_Temp_Set>225&&In_Temp_Set<=250)
	{
		Pid_down=15;
		Pid_up=60;
	}
	else if(In_Temp_Set>250&&In_Temp_Set<=350)
	{
		Pid_down=15;
		Pid_up=80;
	}
	else if(In_Temp_Set>350&&In_Temp_Set<=400)
	{
		Pid_down=30;
		Pid_up=95;
	}
	else 
	{
		Pid_down=50;
		Pid_up=100;
	}
	
	PID_Operation(temp);
}*/
void IDLE_Mode(void)
{
		MOT = Control_OFF;
		HOT = Control_OFF;
		FAN = Control_OFF;
}
void Run_Mode_HOT_ON(int ON)
{
	if(ON==0)
		HOT = Control_OFF;
	else
		HOT = Control_ON;
}
void Run_Mode_MOT_ON(int ON)
{
	if(ON==0)
		MOT = Control_OFF;
	else
		MOT = Control_ON;
}
void Run_Mode_FLAME_OVER(void)
{
		MOT = Control_OFF;
		HOT = Control_OFF;
		FAN = Control_OFF;
}
void Run_Mode_Startup_OVER(void)
{
		MOT = Control_OFF;
		HOT = Control_OFF;
	//	FAN = Control_OFF;
}
void Enter_RUN_MODE_SET(void)
{
	FAN = Control_ON;
	MOT = Control_OFF;
	HOT = Control_OFF;
}