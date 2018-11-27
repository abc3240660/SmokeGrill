#include "ec11key.h"
#include "stm32f10x_exti.h"
#include "common.h"

u16 step_left = 0;
u16 step_right = 0;

EC11_STA ec11_int_event = EC11_IDLE;
	
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void Delay_EncoderK(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

//按键初始化函数
void EC11KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_1;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE4,3
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE4,3
}

void EC11_EXTI_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    EC11KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{	
  //确保是否产生了EXTI Line中断
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {	
		if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == Encoder_ON) {
//			if (step < MaxTurningPwm) {
				step_left += 5;
//			}
				
			ec11_int_event = EC11_LEFT;
		} else {
//			if (step > MinTurningPwm) {
					step_right += 5;
//			}
				ec11_int_event = EC11_RIGHT;
		}
		
		//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
	}
}

EC11_STA EC11_KEY_Scan(int mode)
{	
	EC11_STA ec_sta = EC11_IDLE;

	if (0 == mode) {
		if ( step_left > step_right) {
			ec11_int_event = EC11_LEFT;
		} else if ( step_left < step_right) {
			ec11_int_event = EC11_RIGHT;
		}
	}
	
	if ( step_left != 0) {
		step_left = 0;
		if (step_left > 20) {
			if (1 == mode) {
				ec11_int_event = EC11_LEFT_FAST;
			}
		}
	}
	
	if ( step_right != 0) {
		step_right = 0;
		if ( step_right > 20) {
			if (1 == mode) {
				ec11_int_event = EC11_RIGHT_FAST;
			}
		}
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
