#ifndef _HAL_STMFLASH_H
#define _HAL_STMFLASH_H
#include <stdint.h>	
#include "stm32f10x_flash.h"

extern uint32_t Last_APP_addr;

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 1024 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

/* 升级参数存储 */
#define UPDATE_PARAM_SAVE_ADDR_BASE         0x08002C00
#define UPDATE_PARAM_MAX_SIZE               (1*1024)//支持参数大小1KB
/* APP Flash首地址 */
#define SYS_APP_SAVE_ADDR_BASE              0x08003000
#define APP_DATA_MAX_SIZE                   (450*1024)//支持APP大小30KB
/* APP BAK Flash首地址 */
#define SYS_APP_BAK_SAVE_ADDR_BASE          0x08073800
#define APP_BAK_DATA_MAX_SIZE               (26*1024)//支持APP_BAK大小21KB

//flash模拟E2ROM
//#define userData_FLASH_ADDR  0x0800FC00	 //用户FLASH E2ROM  大小1KB

#define FLASH_PAGE_SIZE          0x400U
 
void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//从指定地址开始读出指定长度的数据
void writeAppBin(u32 appxaddr,u8 *appbuf,u32 appsize);						   
#endif

















