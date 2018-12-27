#ifndef _HAL_STMFLASH_H
#define _HAL_STMFLASH_H
#include <stdint.h>	
#include "stm32f10x_flash.h"

extern uint32_t Last_APP_addr;

//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 1024 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

/* ���������洢 */
#define UPDATE_PARAM_SAVE_ADDR_BASE         0x08002C00
#define UPDATE_PARAM_MAX_SIZE               (1*1024)//֧�ֲ�����С1KB
/* APP Flash�׵�ַ */
#define SYS_APP_SAVE_ADDR_BASE              0x08003000
#define APP_DATA_MAX_SIZE                   (450*1024)//֧��APP��С30KB
/* APP BAK Flash�׵�ַ */
#define SYS_APP_BAK_SAVE_ADDR_BASE          0x08073800
#define APP_BAK_DATA_MAX_SIZE               (26*1024)//֧��APP_BAK��С21KB

//flashģ��E2ROM
//#define userData_FLASH_ADDR  0x0800FC00	 //�û�FLASH E2ROM  ��С1KB

#define FLASH_PAGE_SIZE          0x400U
 
void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void writeAppBin(u32 appxaddr,u8 *appbuf,u32 appsize);						   
#endif

















