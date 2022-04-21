/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "sort.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC1_Filter_Num 50 //采样滤波点数

#define ADC1_Vref 1200.0  //需要根据单片机情况调整

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

typedef enum
{
  ADC1_CHANNEL_IN0 = 0U,      //ADC1通道1   rank 1
  ADC1_CHANNEL_IN1,           //rank  2
	ADC1_CHANNEL_Temp,          //rank  3
	ADC1_CHANNEL_Vref,          //rank  4
	
	ADC1_CHANNEL_CNT,         //ADC通道数量
	
	ADCn_CHANNEL_IN0 = 0,
	ADCn_CHANNEL_CNT,   
	
}ADC_CHANNEL;




extern uint32_t ADC1_Buf[ADC1_CHANNEL_CNT * ADC1_Filter_Num];
extern uint32_t ADC1_In0_Buf[ADC1_Filter_Num];
extern uint32_t ADC1_In1_Buf[ADC1_Filter_Num];
extern uint32_t ADC1_Vref_Buf[ADC1_Filter_Num];

double Get_ADC_Voltage(uint32_t *ADCBuf,ADC_CHANNEL CHANNEL);



/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
