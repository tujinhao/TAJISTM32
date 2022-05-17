/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _power_H_
#define _power_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pid.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */





typedef enum
{
    CONSTANT_Vol = 0,
    CONSTANT_Cur
}POWER_MODE;

typedef enum
{
    POWER_OFF = 0,
    POWER_ON
}POWER_Status;

typedef enum
{
	Current_Forward = 0,
  Current_Reverse
}Current_Direction;


typedef struct PowerSet {
	POWER_MODE mode;
	POWER_Status status;
	double Vout_Set;  // ���õ�ѹĿ��ֵ��λΪ mV
	double Iout_Set;  // ���õ���Ŀ��ֵ��λΪ mA
	double Vout_ADC;   // ADC����ת��ǰ��ֵ��λΪ mV  ��Χ 0~3300mv
	double Iout_ADC;   //ADC����ת��ǰ��ֵ��λΪ mV	  ��Χ 0~3300mv
	double Vout_Cal;   // ADC����ת�����ֵ��λΪ mV
	double Iout_Cal;   //ADC����ת�����ֵ��λΪ mA
	double ADC2Vout_K;
	double ADC2Vout_B;
	double ADC2Iout_K;
	double ADC2Iout_B;
	Current_Direction Iout_Direction;   //��������������
} PowerSet;

extern PowerSet MyPower;


void Power_Init(POWER_MODE mode,double Set_Value);

void Control( POWER_MODE mode , POWER_Status status , int32_t Set_Value );
void Reset_Output(void);


#endif 

