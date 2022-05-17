#include "power.h"
#include "math.h"



PowerSet MyPower;
/*     
A*R*Iout = (Vadc - Vref) //电阻R为欧，电流I为毫安 V是mV A为增益
Iout = 1000(Vadc - Vref)/A*mR  //mR为毫欧

当A = 50     mR = 10毫欧  vref = 2.5V时
Iout = 2*Vadc - 5000  mA



Vout = K*Vadc （+ B）  //1k 和9k电阻 K为10

*/



double ADC2Iout_K_Set = 2;
double ADC2Iout_B_Set = -5000;
double ADC2Vout_K_Set = 21;
double ADC2Vout_B_Set = 0;


extern TIM_HandleTypeDef htim1;


//#define SET_DIS_MAX 4000 //5000//
#define SET_DIS_MAX 10000 //5000//用10000代表10.000V
#define VOUT_DIS_MAX 4000//5000//
//#define IOUT_DIS_MAX 3000  //这里限定了最大电流
#define IOUT_DIS_MAX 10000//wangyu modify 2018-4-11



/******************************************************************************/

/******************************************************************************/
/* Private function prototypes -----------------------------------------------*/

double Pi, f_PWM_Pulse, pid_out;

double storeList[5] = {0};

uint32_t PWM_Pulse = 0;

static uint8_t Soft_Star_Flag = 0 ;   //  软启动标记  


void Power_Init(POWER_MODE mode,double Set_Value)
{
	
	
	MyPower.mode         = mode;
	MyPower.status       = POWER_ON;
	MyPower.Iout_Cal     = 0;
	MyPower.Vout_Cal     = 0;
	MyPower.ADC2Iout_K   = ADC2Iout_K_Set ;
	MyPower.ADC2Iout_B   = ADC2Iout_B_Set ;
	MyPower.ADC2Vout_K   = ADC2Vout_K_Set ;
	MyPower.ADC2Vout_B   = ADC2Vout_B_Set ;
	if(MyPower.mode == CONSTANT_Vol)
	{
		MyPower.Vout_Set   = Set_Value;
		MyPower.Iout_Set   = 0;
	}
	else
	{
		MyPower.Iout_Set   = Set_Value;
		MyPower.Vout_Set   = 0;
	}
	


}


//Set_Value 单位为mV
void Control(POWER_MODE mode , POWER_Status status , int32_t Set_Value )
{
	
	
	static uint8_t g_StoreLen = 0 , g_StorePos = 0;
  static double sumVal = 0, avage = 0;

	
	if(status == 0) 
	{		//  输出失能 
		Reset_Output();
	
	}
	else
	{		
			//  输出使能
			if( Soft_Star_Flag < 100 )
			{					//  软启动过程中设定值逐渐增加 。
					Set_Value = Soft_Star_Flag * Set_Value / 100 ; 
					Soft_Star_Flag++ ; 
				EMLOG(LOG_DEBUG,"Soft_Star_Flag = %d \nSet_Value = %d \n",Soft_Star_Flag,Set_Value);
				
			}

			// 否则Set_Value 的值不变 。
				
			if( mode == CONSTANT_Vol ) 
			{    //  恒压
				pid_out = Vol(Set_Value , MyPower.Vout_Cal ) ; 
			}
			else
			{    //  恒流
				
				pid_out = Cur( Set_Value , fabs(MyPower.Iout_Cal)/* s16_Vout_Cal*/ ) ; 
			}
		
			if (g_StoreLen == 0)
        {
            storeList[g_StorePos] = pid_out;
            g_StorePos ++ ;
            sumVal = sumVal + pid_out;
            avage = sumVal / g_StorePos;
            
            if (g_StorePos >= 3)
            {
                g_StoreLen = 1;
                g_StorePos = 0;
            }
        }
			else if (g_StoreLen == 1)
        {
            sumVal = sumVal + pid_out - storeList[g_StorePos];
            avage =     sumVal / 3;
            storeList[g_StorePos] = pid_out;
            g_StorePos ++ ;
            
            if (g_StorePos >= 3)
            {
                g_StorePos = 0 ;
                g_StoreLen = 0 ;
                sumVal = 0;
            }
            
            Pi = avage;
            f_PWM_Pulse = f_PWM_Pulse + Pi * 2.84;
            
            if (f_PWM_Pulse <= 0)
            {
                f_PWM_Pulse = 0;
            }
            else if (f_PWM_Pulse >= 0.95 * htim1.Init.Period)
            {
                f_PWM_Pulse = 0.95 * htim1.Init.Period  ;
							EMLOG(LOG_WARN,"PWM_Pulse > %.3f",f_PWM_Pulse);
            }
            
            PWM_Pulse = (uint32_t)f_PWM_Pulse;
            
					//	TIM_SetCompare3(TIM1, PWM_Pulse);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_Pulse);
						EMLOG(LOG_DEBUG,"PWM_Pulse = %d",PWM_Pulse);
         }
	
	}
			
	
}

void Reset_Output(void)   //  输出失能，同时将输出复位。
{
		Soft_Star_Flag = 0 ;
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		/**   此处还应清除PID运算的相关积分等变量 */
		Reset_iPid();
		Reset_vPid();
	  f_PWM_Pulse = 0 ;
	
}
