#include "GUI.h"
#include "sram.h"
#include "malloc.h"
#include "common.h"

#ifdef SRAM_MEMDEV
#define USE_EXRAM  1	//ʹ���ڲ�RAM,STM32F103��Ӣ��û���ⲿSRAM,ֻ��ʹ���ڲ��ģ�
//����EMWIN�ڴ��С
#define GUI_NUMBYTES  (512*1024)
#define GUI_BLOCKSIZE 0X80  //���С
#else
#define USE_EXRAM  0	//ʹ���ڲ�RAM,STM32F103��Ӣ��û���ⲿSRAM,ֻ��ʹ���ڲ��ģ�
//����EMWIN�ڴ��С
#define GUI_NUMBYTES  (32*1024)
#define GUI_BLOCKSIZE 0X80  //���С
#endif

//GUI_X_Config
//��ʼ����ʱ�����,��������emwin��ʹ�õ��ڴ�
void GUI_X_Config(void) {
	if(USE_EXRAM) //ʹ���ⲿRAM
	{
		U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES); //���ⲿSRAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
		GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}else  //ʹ���ڲ�RAM
	{
		U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES); //���ڲ�RAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
		GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}
}
