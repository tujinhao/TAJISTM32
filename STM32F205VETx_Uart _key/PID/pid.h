/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __pid_H
#define __pid_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported types ------------------------------------------------------------*/
typedef struct PID {
	double SetPoint; // �趨Ŀ��Desired value
	double Proportion; // ��������Proportional Const
	double Integral; // ���ֳ���Integral Const
	double Derivative; // ΢�ֳ���Derivative Const
	double LastError; // Error[-1]	
	double PrevError; // Error[-2]
	double SumError; // Sums of Errors
} PID;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
double PIDCalc( PID *pp, double NextPoint  );
double Cur(double iSet,double i3);
double Vol(double vol_set,double vol_adc);


void Set_vPid(double p,double i,double d);
void Reset_vPid(void);
void Set_iPid(double p,double i,double d);
void Reset_iPid(void);
void Pid_Init(void);


#endif /* __pid_H */

