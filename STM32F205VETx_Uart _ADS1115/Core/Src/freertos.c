/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "usart.h"
#include "ADC.h"
#include "string.h"
#include "tim.h"
#include "ADS1115.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId LEDTaskHandle;
osThreadId UartSendTaskHandle;
osThreadId KeyTaskHandle;
osThreadId UartReceiveTaskHandle;
osThreadId GetADCHandle;
osThreadId PowerControlHandle;
osMessageQId CmdQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartLEDTask(void const * argument);
void StartUartSendTask(void const * argument);
void Key_Task(void const * argument);
void StartUartReceiveTask(void const * argument);
void StartGetADC(void const * argument);
void StartPowerControl(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* definition and creation of CmdQueue */
    osMessageQDef(CmdQueue, 16, uint16_t);
    CmdQueueHandle = osMessageCreate(osMessageQ(CmdQueue), NULL);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of LEDTask */
    osThreadDef(LEDTask, StartLEDTask, osPriorityNormal, 0, 128);
    LEDTaskHandle = osThreadCreate(osThread(LEDTask), NULL);

    /* definition and creation of UartSendTask */
    osThreadDef(UartSendTask, StartUartSendTask, osPriorityNormal, 0, 128);
    UartSendTaskHandle = osThreadCreate(osThread(UartSendTask), NULL);

    /* definition and creation of KeyTask */
    osThreadDef(KeyTask, Key_Task, osPriorityAboveNormal, 0, 128);
    KeyTaskHandle = osThreadCreate(osThread(KeyTask), NULL);

    /* definition and creation of UartReceiveTask */
    osThreadDef(UartReceiveTask, StartUartReceiveTask, osPriorityHigh, 0, 128);
    UartReceiveTaskHandle = osThreadCreate(osThread(UartReceiveTask), NULL);

    /* definition and creation of GetADC */
    osThreadDef(GetADC, StartGetADC, osPriorityNormal, 0, 128);
    GetADCHandle = osThreadCreate(osThread(GetADC), NULL);

    /* definition and creation of PowerControl */
    osThreadDef(PowerControl, StartPowerControl, osPriorityHigh, 0, 128);
    PowerControlHandle = osThreadCreate(osThread(PowerControl), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartLEDTask */
/**
  * @brief  Function implementing the LEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void const * argument)
{
    /* USER CODE BEGIN StartLEDTask */
    /* Infinite loop */
    for(;;)
    {
        HAL_GPIO_TogglePin(LED_t_GPIO_Port,LED_t_Pin);

        osDelay(1000);
    }
    /* USER CODE END StartLEDTask */
}

/* USER CODE BEGIN Header_StartUartSendTask */
/**
* @brief Function implementing the UartSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartSendTask */
void StartUartSendTask(void const * argument)
{
    /* USER CODE BEGIN StartUartSendTask */
    //uint8_t AT_get_time[50] = "AT+UPDATETIME=1,time.windows.com,10,\"E8\",0\r\n";






    /* Infinite loop */
    for(;;)
    {
//    osDelay(1000);//等待1秒
//    osMessagePut(CmdQueueHandle, 1, 0);//发送消息1

        //HAL_UART_Transmit_DMA(&huart2, (uint8_t*)AT_get_time,  sizeof(AT_get_time));  //发送命令
//        EMLOG(LOG_DEBUG,"mode = %d  Vout_ADC = %.3f   Vout_Cal = %.3f\n",MyPower.mode,MyPower.Vout_ADC,MyPower.Vout_Cal);
//        EMLOG(LOG_DEBUG,"mode = %d  Iout_ADC = %.3f   Iout_Cal = %.3f\n",MyPower.mode,MyPower.Iout_ADC,MyPower.Iout_Cal);



        osDelay(1000);//等待1秒
//    osMessagePut(CmdQueueHandle, 2, 0);//发送消息2
    }
    /* USER CODE END StartUartSendTask */
}

/* USER CODE BEGIN Header_Key_Task */
/**
* @brief Function implementing the KeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Key_Task */
void Key_Task(void const * argument)
{
    /* USER CODE BEGIN Key_Task */
    /* Infinite loop */
    for(;;)
    {
        if(KeyState == KEY_DOWN) {
            vTaskSuspend(LEDTaskHandle);
            vTaskSuspend(GetADCHandle);

            KeyState = KEY_UNKNOW;
        } else if(KeyState == KEY_UP) {
            vTaskResume(LEDTaskHandle);

            vTaskResume(GetADCHandle);
            KeyState = KEY_UNKNOW;
            // printf("Key_Task\n");
        }
        osDelay(1000);
    }
    /* USER CODE END Key_Task */
}

/* USER CODE BEGIN Header_StartUartReceiveTask */
/**
* @brief Function implementing the UartReceiveTask thread.
* @param argument: Not used
* @retval None
*/




/* USER CODE END Header_StartUartReceiveTask */
void StartUartReceiveTask(void const * argument)
{
    /* USER CODE BEGIN StartUartReceiveTask */
    //osEvent theEvent;//声明一个事件


    //uint32_t test = 1;
    /* Infinite loop */
    for(;;)
    {
        //	printf("tw %d\n",test);

        //test = scanf("%d",&test);
        //  EMLOG(LOG_DEBUG,"串口接收任务运行中\n");

        if(UART_RX_STA & 0X8000)
        {
            //	HAL_UART_Transmit(&huart1, UART_RX_BUF, UART_RX_STA & 0X7FFF, 100);    // 将接收到的数据发送回去



            //	EMLOG(LOG_DEBUG,"%d\n",strcmp("AT\n",(char *)UART_RX_BUF ));
            if(0 == strcmp("AT\r\n",(char *)UART_RX_BUF ))
            {
                printf("OK\n");
                EMLOG(LOG_DEBUG,"收到%s\n",UART_RX_BUF);

            }
            else if(0 == strcmp("AT+GetADC1CH0\r\n",(char *)UART_RX_BUF ))
            {
                printf("ADC1CH0 = %.3f\n",Get_ADC_Voltage(ADC1_In0_Buf,ADC1_CHANNEL_IN0));

            }
            else if(0 == strcmp("AT+GetADC1CH1\r\n",(char *)UART_RX_BUF ))
            {
                printf("ADC1CH1 = %.3f\n",Get_ADC_Voltage(ADC1_In0_Buf,ADC1_CHANNEL_IN1));

            }

            //   HAL_UART_Transmit_DMA(&huart1, (uint8_t*)UART_RX_BUF,  UART_RX_STA & 0X7FFF);  //回显




            UART_RX_STA = 0;
        }

        //	printf("Receive message %d\n",test);
        osDelay(1);

//		theEvent = osMessageGet(CmdQueueHandle, osWaitForever);
//      if(theEvent.status == osEventMessage)
//      {
//          printf("Receive message %d\n",theEvent.value.v);
//      }
    }
    /* USER CODE END StartUartReceiveTask */
}

/* USER CODE BEGIN Header_StartGetADC */
/**
* @brief Function implementing the GetADC thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_StartGetADC */


void StartGetADC(void const * argument)
{
    /* USER CODE BEGIN StartGetADC */

    HAL_ADC_Start_DMA(&hadc1,ADC1_Buf,ADC1_CHANNEL_CNT * ADC1_Filter_Num);  //DMA 数组大小为ADC1的通道数乘以ADC1需要滤波的数据量
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    ADS1115_UserConfig_SingleConver(&ADS1115_ADDR_GND,ADS1115_ADDRESS_GND);
	ADS1115_UserConfig_SingleConver(&ADS1115_ADDR_VDD,ADS1115_ADDRESS_VDD);
	//uint8_t i = 0;

    /* Infinite loop */
    for(;;)
    {
//       printf("ADC1 CH0 vol = %.3f  \n",Get_ADC_Voltage(ADC1_In0_Buf,ADC1_CHANNEL_IN0));   //单片机内部ADC ch0
//       printf("ADC1 CH1 vol = %.3f  \n",Get_ADC_Voltage(ADC1_In1_Buf,ADC1_CHANNEL_IN1));   //单片机内部ADC ch1

      ADS1115_GetVoltage(&ADS1115_ADDR_GND);
			ADS1115_RefreshAllChannel(&ADS1115_ADDR_GND);
			printf("ADS1115 GND CH %d vol = %.3f  \n\n",ADS1115_ADDR_GND.CHANNEL,ADS1115_ADDR_GND.ADS1115_Vol[ADS1115_ADDR_GND.CHANNEL]);
			
			
      ADS1115_GetVoltage(&ADS1115_ADDR_VDD);
			ADS1115_RefreshAllChannel(&ADS1115_ADDR_VDD);
			printf("ADS1115 VDD CH %d vol = %.3f  \n\n",ADS1115_ADDR_VDD.CHANNEL,ADS1115_ADDR_VDD.ADS1115_Vol[ADS1115_ADDR_VDD.CHANNEL]);
			
			
        osDelay(1000);
    }
    /* USER CODE END StartGetADC */
}

/* USER CODE BEGIN Header_StartPowerControl */
/**
* @brief Function implementing the PowerControl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPowerControl */
void StartPowerControl(void const * argument)
{
    /* USER CODE BEGIN StartPowerControl */
    double CONSTANT_Vol_Set = 15000,CONSTANT_Cur_Set = 2000;    //稳压模式22000mV  恒流模式2000mA

    Pid_Init();
    Power_Init(CONSTANT_Vol,CONSTANT_Vol_Set);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    /* Infinite loop */
    for(;;)
    {

        MyPower.Vout_ADC = Get_ADC_Voltage(ADC1_In0_Buf,ADC1_CHANNEL_IN0);    //读取电压采样ADC通道电压
        MyPower.Iout_ADC = Get_ADC_Voltage(ADC1_In0_Buf,ADC1_CHANNEL_IN1);    //读取电流采样ADC通道电压

        MyPower.Vout_Cal = MyPower.ADC2Vout_K * MyPower.Vout_ADC  + MyPower.ADC2Vout_B;  //ADC电压转换为实际电压  mV

        MyPower.Iout_Cal = MyPower.ADC2Iout_K * MyPower.Iout_ADC  + MyPower.ADC2Iout_B;  //ADC电压转换为实际电流  mA





        if( MyPower.Vout_Cal < CONSTANT_Vol_Set ) //  小于设置最大电压就恒流
        {
            printf("恒流\n");
            MyPower.mode = CONSTANT_Cur;
            MyPower.Iout_Set = CONSTANT_Cur_Set;
            Control(MyPower.mode, MyPower.status, MyPower.Iout_Set);    //恒流

        }
        else    // 测量电压大于最大设置最大电压就恒压
        {
            printf("恒压\n");
            MyPower.mode = CONSTANT_Vol;
            MyPower.Vout_Set = CONSTANT_Vol_Set;
            Control(MyPower.mode, MyPower.status, MyPower.Vout_Set);    //恒压
        }



        osDelay(10000);     //pid控制间隔时间 单位为ms    调试时时间间隔可以调长
    }
    /* USER CODE END StartPowerControl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
