
#include "PID.h"
#include "includes.h"

u16					iTemp=0;
u16					PID_dt = 0;
u16					S1_Counter_TempAvg = 0;
long				I_Err_Sum = 0;
float				P_out = 0;
float				I_out = 0;
float				D_out = 0;
float				Smoke_Mult = 1.0;

int					Auger_On;
int					Auger_Off;
extern int	g_smoke_val_percent;
extern int	g_target_temp_val;


	
//  ---------------------------------------  SELECT MODEL AND FEATURES	--------------------------------------------
		
int			MODEL = 1;  					//  0 = PPG (Portable)  1 = DLX, WW, SG  2 = MG  3 = XXL
		
int			Dis_C = 0;						//	0 = F  1 = C  (temp display units, Fahrenheit or Celsius)
		
//	----------------------------------------------------------------------------------------------------------------


void PID_Ctr(u16 In_Temp,u16 Temp_Set)
{
	static int			Auger_On_Max;
	static int			Auger_On_Min;					
	static float		Auger_Ratio_Low, Auger_Ratio_High, m_slope_auger, b_offset_auger, Auger_On_Ratio;
	
	static u16			Temp_Sum;
	static u16			Num_Temp_Samples;
	static u16			Temp_Avg;
	static int			Temp_Err;
	static long			I_Err_Sum_Max;
	static int			Temp_Err_Prev=0;	
	static float		D_Err_Slope;
	static int			Auger_On_Ini;
	static float		Temp_Max;
	static float		Temp_Min;
	
	static float		m_slope_temp;
	static float		b_offset_temp;
	static float		m_slope_smoke;
	static float		b_offset_smoke;
	static float		Smoke_Max;
	static float		Smoke_Min;
	


//  Calculate equation for PWM Auger_On ratio

		if (MODEL == 3)												// XXL
		{
		Auger_On_Max = 200;
		Auger_On_Min = 0.13 * Auger_On_Max;
		Auger_Ratio_Low = 0.29;
		Auger_Ratio_High = 1.0;
		I_Err_Sum_Max = 18000;
		Temp_Max = 350;
		Temp_Min = 150;
		}	
		else if (MODEL == 2)									// MG				REWORK AND TEST FOR MAGNUM  (temp values different)
		{
		Auger_On_Max = 200;
		Auger_On_Min = 0.13 * Auger_On_Max;
		Auger_Ratio_Low = 0.25;
		Auger_Ratio_High = 1.0;
		I_Err_Sum_Max = 18000;
		Temp_Max = 600;
		Temp_Min = 160;
		}	
		else if(MODEL == 1)										// DLX, WW, SG
		{
		Auger_On_Max = 200;
		Auger_On_Min = 0.13 * Auger_On_Max;
		Auger_Ratio_Low = 0.17;
		Auger_Ratio_High = 0.75;
		I_Err_Sum_Max = 18000;
		Temp_Max = 500;
		Temp_Min = 160;
		}
		else if (MODEL == 0)									// PPG
		{
		Auger_On_Max = 200;
		Auger_On_Min = 0.10 * Auger_On_Max;		// PPG Min feed 
		Auger_Ratio_Low = 0.12;
		Auger_Ratio_High = 0.70;
		I_Err_Sum_Max = 12000;
		Temp_Max = 500;
		Temp_Min = 160;
		}

		if(Temp_Set >= 455) Temp_Set = Temp_Max;
		
		m_slope_auger = (Auger_Ratio_High - Auger_Ratio_Low)/(Temp_Max - Temp_Min);
		b_offset_auger = Auger_Ratio_High - m_slope_auger * Temp_Max;
	

			if(S1_Counter_TempAvg>=1)  																						// 1s timer between Temperature samples
			{
				Temp_Sum = Temp_Sum + In_Temp;		
				Num_Temp_Samples++;
				S1_Counter_TempAvg = 0;
			}	
			
			if(PID_dt >= 5)				//(Auger_On_Max/10))				??? Auger_On_Max * Smoke_Mult   ???											// Calculate P,I & D afer time >= dt  (seconds)
			{		
				Temp_Avg = Temp_Sum / Num_Temp_Samples;
				
				Temp_Err = (int)Temp_Set - (int)Temp_Avg;	
				
				if(abs(Temp_Err) <= (0.20*Temp_Set))																// Only calculate I if within % of set temp
				{											
					I_Err_Sum = I_Err_Sum + Temp_Err * (int)PID_dt;										// Ki [integral = integral + error*dt] (error/xx to slow error accumulation) 
						if(I_Err_Sum > I_Err_Sum_Max) I_Err_Sum = I_Err_Sum_Max;				// Larger number reacts slower, change in calc below	
						if(I_Err_Sum <-I_Err_Sum_Max) I_Err_Sum =-I_Err_Sum_Max;																
				}
					
				D_Err_Slope = (float)(Temp_Err - Temp_Err_Prev)/(float)PID_dt;			// Kd [Slope = (Err2-Err1)/dt]

				if(MODEL == 3)																											// *** XXL ***
				{
				P_out = 3.0 * ((float)Temp_Set/Temp_Max) * ((float)Temp_Err);
					if(P_out > (50.0 * Smoke_Mult)) P_out = (50.0 * Smoke_Mult);
					if(P_out < (-50.0 * Smoke_Mult)) P_out = (-50.0 * Smoke_Mult);
				I_out = ((30.0 * Smoke_Mult) / I_Err_Sum_Max) * I_Err_Sum;
				}				
				else if(MODEL == 2)																									// *** MG ***
				{
//				P_out = 2.0 * (Temp_Err);
				P_out = 3.0 * ((float)Temp_Set/Temp_Max) * ((float)Temp_Err);
					if(P_out > (50.0 * Smoke_Mult)) P_out = (50.0 * Smoke_Mult);
					if(P_out < (-50.0 * Smoke_Mult)) P_out = (-50.0 * Smoke_Mult);
				I_out = ((30.0 * Smoke_Mult) / I_Err_Sum_Max) * I_Err_Sum;
//				D_out = 40.0*(D_Err_Slope);
//					if(D_out > 100.0) D_out = 100.0;
//					if(D_out < -100.0) D_out =-100.0;
				}
				else if(MODEL == 1)																									// *** DLX, SE, WW ***
				{
//				P_out = 2.0 * (Temp_Err);
				P_out = 3.0 * ((float)Temp_Set/Temp_Max) * ((float)Temp_Err);				// ***************   TESTING NEW CONTROLLER  *******
					if(P_out > 50.0) P_out = 50.0; 			//   ****  MAKE MODS FOR SMOKE CONTROL  *********
					if(P_out < -50.0) P_out = -50.0;		//  ***  USE SAME FOR OTHER MODELS   ******
				I_out = ((50.0 / I_Err_Sum_Max) * I_Err_Sum);
//				D_out = 40.0*(D_Err_Slope);
//					if(D_out > 100.0) D_out = 100.0;
//					if(D_out < -100.0) D_out =-100.0;
				}
				else if(MODEL == 0)																									// *** PPG ***
				{
				P_out = 3.0 * ((float)Temp_Set/Temp_Max) * ((float)Temp_Err);
					if(P_out > (50.0 * Smoke_Mult)) P_out = (50.0 * Smoke_Mult);
					if(P_out < (-50.0 * Smoke_Mult)) P_out = (-50.0 * Smoke_Mult);
				I_out = ((30.0 * Smoke_Mult) / I_Err_Sum_Max) * I_Err_Sum;
//				D_out = 50.0*(D_Err_Slope);		
//					if(D_out > 50.0) D_out = 50.0;
//					if(D_out < -50.0) D_out =-50.0;			
				D_out = 0.0;																												// Remove if "D" used
				}
	
				Temp_Err_Prev = Temp_Err;					
					
				PID_dt = 0;
				Temp_Sum = 0;
				Num_Temp_Samples = 0;
			} 

//	Calculate auger ON and OFF time			
			
			Auger_On_Ratio = (m_slope_auger * (float)Temp_Set) + b_offset_auger;
			Auger_On_Ini = Auger_On_Ratio * Auger_On_Max;
			Auger_On = Auger_On_Ini + P_out + I_out + D_out;
	
//			if((Auger_On < Auger_On_Min) || (Temp_Err < (-0.10*Temp_Set)))
//					Auger_On = Auger_On_Min;
//			if(Auger_On > (Auger_On_Max * Auger_Ratio_High))
//					Auger_On = Auger_On_Max * Auger_Ratio_High;
			
			if(MODEL == 0)
			{
				if(Auger_On > (Auger_On_Max * Auger_Ratio_High))
						Auger_On = Auger_On_Max * Auger_Ratio_High;
			}
			else 
			{
				if	(Auger_On > Auger_On_Max)
						Auger_On = Auger_On_Max;
			}			
			
			Auger_Off = Auger_On_Max-Auger_On;

//		***  SMOKE MULTIPLIER CALCULATIONS  ***
			
			if(abs(Temp_Err) <= (0.20*Temp_Set))																		// Only Smoke_Mult if within % of set temp
			{ 			
				if (g_target_temp_val == 150)								// LOW Smoke Setting
					Smoke_Mult = 1.5;
				else if (g_target_temp_val == 155)					// HIGH Smoke Setting
					Smoke_Mult = 2.25;
				else if ((Temp_Set >= 160) && (Temp_Set <= 250))
				{
					Smoke_Max = 4.5;
					Smoke_Min = 1.0;
					
					m_slope_temp = (Smoke_Max - 3.0)/(250.0 - 160.0);
					b_offset_temp = Smoke_Max - (m_slope_temp * 250);
					
					Smoke_Max = (m_slope_temp * Temp_Set) + b_offset_temp;

					m_slope_smoke = (Smoke_Max - Smoke_Min)/(10.0 - 1.0);
					b_offset_smoke = Smoke_Max - (m_slope_smoke * 10.0);
					
					Smoke_Mult = (m_slope_smoke * g_smoke_val_percent) + b_offset_smoke;
					
					/*
					Smoke_Max = 1.5;
					Smoke_Min = 1.0;
					
					m_slope_smoke = (Smoke_Max - Smoke_Min)/(10.0 - 1.0);
					b_offset_smoke = Smoke_Max - (m_slope_smoke * 10.0);
					
					Smoke_Mult = (m_slope_smoke * g_smoke_val_percent) + b_offset_smoke;
					*/
				}
				else if ((Temp_Set > 250) && (Temp_Set <= 300))
				{
					Smoke_Max = 10.0;
					Smoke_Min = 1.0;
					
					m_slope_temp = (Smoke_Max - 4.5)/(300 - 250);
					b_offset_temp = Smoke_Max - (m_slope_temp * 300);
					
					Smoke_Max = (m_slope_temp * Temp_Set) + b_offset_temp;

					m_slope_smoke = (Smoke_Max - Smoke_Min)/(10.0 - 1.0);
					b_offset_smoke = Smoke_Max - (m_slope_smoke * 10.0);
					
					Smoke_Mult = (m_slope_smoke * g_smoke_val_percent) + b_offset_smoke;
				}
			}
			else 
				Smoke_Mult = 1.0;

					Auger_On = Auger_On * Smoke_Mult;        															// Add smoke multiplier to Auger On/Off times
					Auger_Off = Auger_Off * Smoke_Mult;		
				

}
