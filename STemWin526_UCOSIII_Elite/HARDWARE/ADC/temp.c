 #include "temp.h"
 #include "adc.h"
//rtd						
const u32	RTD_TAB_NTC4050[70]={3372138,2604488,2027718,1590698,1256904,1000000,800832,645348,523162,426532,//0--45		5?????
					   349650,288124,238613,198558,165988,139373,117523,99503,84576,72161,//50--95
					   61792,53099,45783,39604,34367,29914,26114,22862,20070,17667,//100--145
					   15591,13794,12233,10875,9690,8653,7744,6945,6241,5620,//150--195
					   5071,4583,4151,3765,3422,3115,2841,2594,2373,2175,//200--245
					   1995,1834,1688,1555,1435,1327,1228,1138,1055,980,//250--295
					   912,849,792,739,691,646,605,567,533,500};//300--345	
//probe
const u32	RTD_TAB_NTC3950[70]={3270195,2542428,1992007,1572290,1249734,1000000,805274,652411,531635,435621,//0--45		5?????
					   358842,297096,247171,206594,173452,146251,123825,105254,89809,76912,//50--95
					   66101,57003,49319,42807,37268,32543,28498,25025,22034,19452,//100--145
					   17215,15273,13582,12106,10814,9681,8686,7808,7034,6348,//150--195
					   5740,5200,4719,4290,3907,3564,3257,2980,2732,2507,//200--245
					   2305,2122,1957,1806,1670,1546,1433,1330,1236,1150,//250--295
					   1071,999,933,872,861,765,717,673,633,595};//300--345

// 0-OK 1-�¶Ȱ��쳣
extern int g_temp1_error;
extern int g_temp2_error;
extern int g_temp3_error;
extern int g_temp4_error;
extern int g_temp5_error;
	
						 
//========================================================================
// ����: float NTC4050_CalculateTemperature(u32 fR)    
// ����: ���ó�NTC���¶�
// ����: �Ŵ�10000���ĵ���ֵ
// ����: �¶�
//========================================================================
float NTC4050_CalculateTemperature(u32 fR)
{
    float fTem;
    float fLowRValue;
    float fHighRValue;        
    s16   iTem;
    u8 i;

    u8 cBottom, cTop;  //����Ԫ����ߺ����

    if (fR > RTD_TAB_NTC4050[0])     // ����ֵС�ڱ����Сֵ�������������ޡ�
    {
            return 0;
    }

    if (fR < RTD_TAB_NTC4050[69])    // ����ֵ���ڱ�����ֵ�������������ޡ�
    {
            return 345;				//�����80Ԫ�أ�400��
    }

    cBottom = 0; 
    cTop    =  69;

    for (i=35; (cTop-cBottom)!=1; )        // 2�ַ����i=Ԫ���ܸ���/2
    {
            if (fR > RTD_TAB_NTC4050[i])
            {
                    cTop = i;
                    i = (cTop + cBottom) / 2;
            }
            else if (fR < RTD_TAB_NTC4050[i])
            {
                    cBottom = i;
                    i = (cTop + cBottom) / 2;
            }
            else								//�����ҵ���ֵ��������
            {
                    iTem = (u16)i * 5;	 //�˼�����Ԫ�ر�ź��¶ȵĶ�Ӧ�����������1Ԫ���У���Ӧ���¶�Ϊ5
                    fTem = (float)iTem;

                    return fTem;
            }
    }

    iTem = (u16)i * 5;

    fLowRValue  = RTD_TAB_NTC4050[cBottom];
    fHighRValue = RTD_TAB_NTC4050[cTop];

    fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;        // �������5��Ϊһ���ġ�
                                                                                                                                                    // �����ڲ�������㡣

    return fTem;
}
//========================================================================
// ����: float NTC3950_CalculateTemperature(u32 fR)    
// ����: ���ó�NTC���¶�
// ����: �Ŵ�10000���ĵ���ֵ
// ����: �¶�
//========================================================================
float NTC3950_CalculateTemperature(u32 fR)
{
    float fTem;
    float fLowRValue;
    float fHighRValue;        
    s16   iTem;
    u8 i;

    u8 cBottom, cTop;  //����Ԫ����ߺ����

    if (fR > RTD_TAB_NTC3950[0])     // ����ֵС�ڱ����Сֵ�������������ޡ�
    {
            return 0;
    }

    if (fR < RTD_TAB_NTC3950[69])    // ����ֵ���ڱ�����ֵ�������������ޡ�
    {
            return 345;				//�����80Ԫ�أ�400��
    }

    cBottom = 0; 
    cTop    =  69;

    for (i=35; (cTop-cBottom)!=1; )        // 2�ַ����i=Ԫ���ܸ���/2
    {
            if (fR > RTD_TAB_NTC3950[i])
            {
                    cTop = i;
                    i = (cTop + cBottom) / 2;
            }
            else if (fR < RTD_TAB_NTC3950[i])
            {
                    cBottom = i;
                    i = (cTop + cBottom) / 2;
            }
            else								//�����ҵ���ֵ��������
            {
                    iTem = (u16)i * 5;	 //�˼�����Ԫ�ر�ź��¶ȵĶ�Ӧ�����������1Ԫ���У���Ӧ���¶�Ϊ5
                    fTem = (float)iTem;

                    return fTem;
            }
    }

    iTem = (u16)i * 5;

    fLowRValue  = RTD_TAB_NTC3950[cBottom];
    fHighRValue = RTD_TAB_NTC3950[cTop];

    fTem = ( ((fR - fLowRValue)*5) / (fHighRValue - fLowRValue) ) + iTem;        // �������5��Ϊһ���ġ�
                                                                                                                                                    // �����ڲ�������㡣

    return fTem;
}
//========================================================================
// ����: int Get_Temperature(u8 channel) 
// ����: ������Ӧͨ���¶�
// ����: adcͨ��
// ����: �¶�
//========================================================================
int Get_Temperature(u8 channel) 
{
	int temp;	
	u16 adc_value;
	u32 r_value_in;
	adc_value=Get_Adc_Average(channel,10);
	
	if(adc_value<10)//���°���������
	{
#if FOR_DEBUG_USE
#else
		if(channel==ADC_Channel_6) g_temp1_error=1;
		else if(channel==ADC_Channel_5) g_temp2_error=1;
		else if(channel==ADC_Channel_4) g_temp3_error=1;
		else if(channel==ADC_Channel_3) g_temp4_error=1;
		else if(channel==ADC_Channel_2) g_temp5_error=1;
#endif
		temp=0;
	
	}
	else
	{
		if(channel==ADC_Channel_6) g_temp1_error=0;
		else if(channel==ADC_Channel_5) g_temp2_error=0;
		else if(channel==ADC_Channel_4) g_temp3_error=0;
		else if(channel==ADC_Channel_3) g_temp4_error=0;
		else if(channel==ADC_Channel_2) g_temp5_error=0;
		
		r_value_in=(4096-(u32)adc_value)*20000/(u32)adc_value;//2k��������10��
		temp=(int)NTC4050_CalculateTemperature(r_value_in);
		temp=temp*9/5+32;  //ת�ɻ��϶�
	}
	return temp;	

}	
