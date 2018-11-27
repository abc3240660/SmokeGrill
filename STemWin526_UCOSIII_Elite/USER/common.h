#ifndef __COMMON_H
#define	__COMMON_H

#include <stdio.h>
#include "delay.h"

//EC11������״̬
typedef enum EC11_STATE {
	EC11_IDLE = 0,// ����̬
	EC11_LEFT,// ��ʱ������
	EC11_RIGHT,// ˳ʱ������
	EC11_BUT,// �̰�
	EC11_BUT_LONG,// 3s����
	EC11_BUT_NO_FREE,// ����δ�ͷ�
	EC11_LEFT_FAST,// ��ʱ�����
	EC11_RIGHT_FAST,// ˳ʱ�����
} EC11_STA;

// ��Ҫ��Ӧ<��Ļ����˵���ĵ�.docx>�������Ľ���1.1
typedef struct event_val{
	int menu_index;// ������ť���л�������ID
	int force_flush;// ǿ�Ƹ���UI
} EVENT_VAL, *PEVENT_VAL;

// temp
typedef struct temperatures{
	int temp1;// �Ҳ��һ���¶�
	int temp2;// �Ҳ�ڶ����¶�
	int temp3;// �Ҳ�������¶�
	int temp4;// �Ҳ���ĸ��¶�
	int temp5;// RUN�������������Ǹ��¶�
	int target_val;// RUN�����Target�¶�
	int target_smoke;// RUN�����Target smoke
	int target_update;// RUN�����Target smoke
	int temp_unit;// 0-���϶�(Ĭ��) 1���϶�
} TEMP_VAL, *PTEMP_VAL;

// Img2Lcd.exe���߽�ͼƬת����C���Դ������Header��Ϣ
__packed typedef struct _HEADCOLOR
{
   unsigned char scan;// ɨ�跽ʽ
   unsigned char gray;// �Ҷ�ֵ
   unsigned short w;// ��
   unsigned short h;// ��
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR; 

void EC11_BUT_Scan(void);
void EC11_Clear(void);

// ����ʱ���棬ʱ�䵥λĬ��Ϊ���ӣ�Ϊ�˲��Է��㣬
// ����ͨ������ֵ����Ϊ1����ʱ�䵥λ�ɷ��Ӹ�Ϊ��
#define FOR_DEBUG_USE 0

#define BIG_GAP 35

#define TEMP_F2C(f) (((f-32)*5)/9)

#define MCU_ZET6 1

#ifdef MCU_ZET6
//#define SRAM_MEMDEV 1
#endif

#define FOR_PID_USE 1 //PID control����
#define FOR_Time_USE 0 //time control


#define UI_CONFITRM_CANCEL_STARTUP       		0
#define UI_CONFITRM_CANCEL_APLPLY        		1
#define UI_CONFITRM_CANCEL_ONLY_USE_FOR  		2
#define UI_CONFITRM_CANCEL_ONLY_USE_BYPASS	3
#define UI_CONFITRM_CANCEL_SHUTDOWN      		4
#define UI_CONFITRM_CANCEL_RESET         		5

#define UI_BYPASS_CANCEL_SHUTDOWN        		6

#define UI_CANCEL_FEED_MODE              		7

#define UI_EXIT_FEED_MODE                		8
#define UI_EXIT_SHUTDOWN                 		9
#define UI_EXIT_FLAME_ERROR              		10
#define UI_EXIT_SENSOR                   		11
#define UI_EXIT_OVER_TEMP                		12
#define UI_EXIT_FEED_NOT                 		13

#define UI_COMPLETE_SHUTDOWN             		14



//GUI define

#define GUI_MAIN				0
#define GUI_TEMP_SET			1
#define GUI_SMOKE_SET			2
#define GUI_START_UP			3
#define GUI_START_UP_TIMER		4
#define GUI_START_UP_BYPASS		5
#define GUI_RUN					6
#define GUI_FEED				7
#define GUI_FEED_TIMER			8
#define GUI_SHUTWOWN			9
#define GUI_SHUTWOWN_TIMER		10
#define GUI_SUB_MAIN			11
#define GUI_HELP				12
#define GUI_RESET				13
#define GUI_ABOUT				14


#define GUI_FEED_COMPELE		15
#define GUI_SHUTDOWN_COMPELE		16

#define GUI_EXIT				17



#define IDLE					100
#define STARTUP					101
#define FEED					102
#define SHUT					103
#define RUN						104
#define SENSOR_ERROR			105
#define VOER_ERROR				106
#define FLAME_ERROR				107


#define GUI_ERROR_OCCUR				1000
#define DIRECT_SHUT_DOWN			1001
#define DIRECT_STARTUP  			1002
#define DIRECT_FEED		  			1003
#define DIRECT_SHUT_COMPELED		1004
#define DIRECT_RUN					1006
#endif /* __COMMON_H */
