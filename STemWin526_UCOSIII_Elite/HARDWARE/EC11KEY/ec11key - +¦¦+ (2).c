#include "ec11key.h"
#include "stm32f10x_exti.h"
#include "common.h"

u8 int_cnt = 0;
u8 int_cnt2 = 0;
u16 step_left = 0;
u16 step_right = 0;

EC11_STA ec11_int_event = EC11_IDLE;
	
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void Delay_EncoderK(__IO u32 nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

//������ʼ������
void EC11KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_1;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE4,3
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE4,3
}

void EC11_EXTI_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    EC11KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

   //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
   //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{	
  //ȷ���Ƿ������EXTI Line�ж�
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
#if 0
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) == 0 ) {// down
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 0 ) {
				int_cnt++;
			}
		} else {
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) == 1 ) {// up
				if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 1 ) {
					int_cnt--;
					if (0 == int_cnt) {
						ec11_int_event = EC11_RIGHT;
					}
				}
			}
		}
#endif

#if 1
//		if (0 == int_cnt) {
//			ec11_int_event = EC11_LEFT;
			int_cnt2++;
//		} else {
//			int_cnt = 0;
//		}
#endif
		
		//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
	}
}

//�ⲿ�ж�3�������
void EXTI1_IRQHandler(void)
{	
  //ȷ���Ƿ������EXTI Line�ж�
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
#if 1
//		if (0 == int_cnt) {
//			ec11_int_event = EC11_RIGHT;
			int_cnt++;
//		} else {
//			int_cnt = 0;
//		}
#endif
		//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE3�ϵ��жϱ�־λ  
	}
}

EC11_STA EC11_KEY_Scan(int mode)
{	
	EC11_STA ec_sta = EC11_IDLE;

	if (1 == mode) {		
		if ( step_left > 30) {
			ec11_int_event = EC11_LEFT_FAST;
		}
		
		if ( step_right > 30) {
			ec11_int_event = EC11_RIGHT_FAST;
		}
		
		step_left = 0;
		step_right = 0;
	}
	
	// Button first
	if (EC11_IDLE == ec_sta) {
		ec_sta = ec11_int_event;
		ec11_int_event = EC11_IDLE;
	}
	
	return ec_sta;
}

void EC11_BUT_Scan(void)
{	
	int time_remain = 0;
	
	if (EncoderKey == Encoder_ON) {
		delay_ms(5);
		if (EncoderKey == Encoder_ON) {// EC11_BUT
			while (EncoderKey == Encoder_ON) {
				delay_ms(100);
				ec11_int_event = EC11_BUT_NO_FREE;
				if (time_remain < 30) {
					time_remain++;
				} else {
					ec11_int_event = EC11_BUT_LONG;
				}
			}
			
			if (time_remain >= 30) {
				ec11_int_event = EC11_BUT_LONG;
			} else {
				ec11_int_event = EC11_BUT;
			}
		}
	}
}

/***************************END OF FILE***************************/
