/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
FATFS fs;
FATFS *pfs;
FIL fil;
FIL fil1;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(500);
	/* Mount SD Card */
  FRESULT res2 = f_mount(&fs, "", 0x01);
  printf("f_mount result: %02X\r\n", res2);
  if(res2 != FR_OK)
  {
    printf("f_mount failed\r\n");
    Error_Handler();
  }
  
	printf("f_mount ok\r\n");
  /* Check freeSpace space */
  if(f_getfree("", &fre_clust, &pfs) != FR_OK)
  {
    printf("f_getfree failed\r\n");
    Error_Handler();
  }
	printf("f_getfree ok\r\n");

  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
  printf("total:%dKB, free:%dKB\r\n", totalSpace, freeSpace);

  /* free space is less than 1kb */
  if(freeSpace < 1)
  {
    printf("freeSpace not enough\r\n");
    Error_Handler();
  }

  /* Open file to write */
  printf("f_open first.txt\r\n");
	res2 = f_open(&fil, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	printf("f_open  return:%d\n",res2);
	
//  if(f_open(&fil, "first.txt", FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW|FA_READ | FA_WRITE) != FR_OK)
//  {
//    printf("f_open failed : %d\r\n",f_open(&fil, "first.txt",FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW|FA_READ | FA_WRITE));
//    Error_Handler();
//  }

  /* Writing text */
  f_puts("STM32 SD Card I/O Example via SPI\n", &fil);
  f_puts("Black Sheep Wall!!!", &fil);

  /* Close file */
  printf("f_close first.txt\r\n");
	
	
  if(f_close(&fil) != FR_OK)
  {
    printf("f_close failed:%d\r\n",f_close(&fil));
    Error_Handler();
  }

  /* Open file to read */
  printf("f_open first.txt\r\n");
  if(f_open(&fil, "first.txt", FA_READ) != FR_OK)
  {
    printf("f_open failed\r\n");
    Error_Handler();
  }

  printf("f_gets first.txt\r\n");
  while(f_gets(buffer, sizeof(buffer), &fil))
  {
    /* SWV output */
    printf("%s", buffer);
    fflush(stdout);
  }
  printf("\r\ndone\r\n");

  /* Close file */
  printf("f_close first.txt\r\n");
  if(f_close(&fil) != FR_OK)
  {
    printf("f_close failed\r\n");
    Error_Handler();
  }

  /* Unmount SDCARD */
  printf("f_mount unmount\r\n");
  if(f_mount(NULL, "", 1) != FR_OK)
  {
    printf("f_mount failed\r\n");
    Error_Handler();
  }
	
	
	f_mount(&fs, "", 0x01);
	f_open(&fil, "test.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
  f_puts("15:18\n", &fil);
  f_puts("Black Sheep Wall!!!", &fil);
	f_close(&fil);
	f_mount(NULL, "", 1);

  /* Close file */

	
	
  f_close(&fil);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 195;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
