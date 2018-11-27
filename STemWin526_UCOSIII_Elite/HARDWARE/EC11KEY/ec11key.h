#ifndef __ENCODER_H
#define	__ENCODER_H

#include "stm32f10x.h"
#include "common.h"

/************************************************
* 结构体:	    _Key_EncoderSentence
* 功  模:     
*************************************************/
typedef struct _Key_EncoderSentence{
		u8 encoderAcount;
	  u8 encoderBcount;
} Key_EncoderSentence;

//引脚定义
#define EncoderKey 				GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)

#define Encoder_ON	0
#define Encoder_OFF	1

void EC11_EXTI_Init(void);
EC11_STA EC11_KEY_Scan(int mode);

#define MaxTurningPwm    250
#define MinTurningPwm    250


#endif /* __ENCODER_H */
