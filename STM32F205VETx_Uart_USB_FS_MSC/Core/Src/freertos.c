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
#include "fatfs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint16_t Timer1, Timer2;   //fatfs_sd ³¬Ê±ÅÐ¶Ï
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

FATFS fs;
FATFS *pfs;
FIL fil;
FIL fil1;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];
void SDTest()
{
			/* Mount SD Card */
  FRESULT res2 = f_mount(&fs, "", 0x01);
  printf("f_mount result: %02X\r\n", res2);
  if(res2 != FR_OK)
  {
    printf("f_mount failed\r\n");
  
  }
  
	printf("f_mount ok\r\n");
  /* Check freeSpace space */
  if(f_getfree("", &fre_clust, &pfs) != FR_OK)
  {
    printf("f_getfree failed\r\n");
   
  }
	printf("f_getfree ok\r\n");

  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
  printf("total:%dKB, free:%dKB\r\n", totalSpace, freeSpace);

  /* free space is less than 1kb */
  if(freeSpace < 1)
  {
    printf("freeSpace not enough\r\n");

  }

  /* Open file to write */
  printf("f_open first.txt\r\n");
	//res2 = f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	//printf("f_open  return:%d\n",res2);
	
  if(f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
  {
    printf("f_open failed : %d\r\n",f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE));
  
  }

  /* Writing text */
  f_puts("STM32 FreeRTOS SD Card I/O Example via SPI\n", &fil);
  f_puts("Black Sheep Wall!!!\n", &fil);
	f_puts("163ewrwes22", &fil);

  /* Close file */
  printf("f_close first.txt\r\n");
	
	
  if(f_close(&fil) != FR_OK)
  {
    printf("f_close failed:%d\r\n",f_close(&fil));

  }

  /* Open file to read */
  printf("f_open first.txt\r\n");
  if(f_open(&fil, "first.txt", FA_READ) != FR_OK)
  {
    printf("f_open failed\r\n");

  }

  printf("f_gets first.txt\r\n");
  while(f_gets(buffer, sizeof(buffer), &fil))
  {
    /* SWV output */
    printf("%s", buffer);
    fflush(stdout);

  }
//	f_gets(buffer, sizeof(buffer), &fil);
	printf("%s", buffer);
	
  printf("\r\ndone\r\n");

  /* Close file */
  printf("f_close first.txt\r\n");
  if(f_close(&fil) != FR_OK)
  {
    printf("f_close failed\r\n");

  }

  /* Unmount SDCARD */
  printf("f_mount unmount\r\n");
	
  if(f_mount(NULL, "", 1) != FR_OK)
  {
    printf("f_mount failed\r\n");
 
  }


}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId UartDebugTaskHandle;
osThreadId SPI_SD_TaskHandle;
osThreadId YuyinbobaoTaskHandle;
osThreadId Uart_4GTaskHandle;
osTimerId myTimer01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartDebugUartTask(void const * argument);
void StartSPI_SD_Task(void const * argument);
void StartYuyinbobaoTask(void const * argument);
void Start4G_Task(void const * argument);
void Callback01(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
	
	
	osTimerStart(myTimer01Handle,1);
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
		
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of UartDebugTask */
  osThreadDef(UartDebugTask, StartDebugUartTask, osPriorityAboveNormal, 0, 128);
  UartDebugTaskHandle = osThreadCreate(osThread(UartDebugTask), NULL);

  /* definition and creation of SPI_SD_Task */
  osThreadDef(SPI_SD_Task, StartSPI_SD_Task, osPriorityBelowNormal, 0, 128);
  SPI_SD_TaskHandle = osThreadCreate(osThread(SPI_SD_Task), NULL);

  /* definition and creation of YuyinbobaoTask */
  osThreadDef(YuyinbobaoTask, StartYuyinbobaoTask, osPriorityIdle, 0, 1024);
  YuyinbobaoTaskHandle = osThreadCreate(osThread(YuyinbobaoTask), NULL);

  /* definition and creation of Uart_4GTask */
  osThreadDef(Uart_4GTask, Start4G_Task, osPriorityIdle, 0, 1024);
  Uart_4GTaskHandle = osThreadCreate(osThread(Uart_4GTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */

    /* Infinite loop */
    for(;;)
    {
			
//			HAL_GPIO_WritePin(LED_t_GPIO_Port, LED_t_Pin, GPIO_PIN_RESET);
//			osDelay(100);
//			HAL_GPIO_WritePin(LED_t_GPIO_Port, LED_t_Pin, GPIO_PIN_SET);


      osDelay(100);
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartDebugUartTask */
/**
* @brief Function implementing the UartDebugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugUartTask */
void StartDebugUartTask(void const * argument)
{
  /* USER CODE BEGIN StartDebugUartTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDebugUartTask */
}

/* USER CODE BEGIN Header_StartSPI_SD_Task */
/**
* @brief Function implementing the SPI_SD_Task thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_StartSPI_SD_Task */
void StartSPI_SD_Task(void const * argument)
{
  /* USER CODE BEGIN StartSPI_SD_Task */
	printf("SPI_SD_Task Start\r\n");
	


  /* Infinite loop */
	


  for(;;)
  {
		EMLOG(LOG_INFO,"SPI_SD_Task");
		SDTest();

    osDelay(1000);
  }
  /* USER CODE END StartSPI_SD_Task */
}

/* USER CODE BEGIN Header_StartYuyinbobaoTask */
/**
* @brief Function implementing the YuyinbobaoTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartYuyinbobaoTask */
void StartYuyinbobaoTask(void const * argument)
{
  /* USER CODE BEGIN StartYuyinbobaoTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartYuyinbobaoTask */
}

/* USER CODE BEGIN Header_Start4G_Task */
/**
* @brief Function implementing the Uart_4GTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start4G_Task */
void Start4G_Task(void const * argument)
{
  /* USER CODE BEGIN Start4G_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start4G_Task */
}

/* Callback01 function */
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */
	Timer1--;
	Timer2--;

  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
