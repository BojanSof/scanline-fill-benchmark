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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "cycle_counter.h"
#include "lcd_ili9488.h"
#include "artist.h"
#include "constants.h"
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
  char buf[100] = {};
  Lcd lcd = {};
  color_t textBuf[FONT_HEIGHT][LCD_WIDTH] = {};
  Canvas textCanvas = {
		  .data = (color_t*)textBuf,
		  .numRows = FONT_HEIGHT,
		  .numCols = LCD_WIDTH
  };
  color_t polygonBuf[POLYGON_BUF_HEIGHT][POLYGON_BUF_WIDTH] = {};
  Canvas polygonCanvas = {
		  .data = (color_t*)polygonBuf,
		  .numRows = POLYGON_BUF_HEIGHT,
		  .numCols = POLYGON_BUF_WIDTH
  };
//  Point polygonPoints[] = {
//		  {40, 60},
//		  {50, 20},
//		  {20, 90},
//		  {1, 1}
//  };
  Point polygonPoints[] = {
		  {40, 40},
		  {60, 60},
		  {100, 40},
		  {100, 80},
		  {160, 80},
		  {180, 120},
		  {160, 160},
		  {120, 180},
		  {80, 120},
		  {40, 160},
		  {20, 100}
  };
  const size_t numPolygonPoints = sizeof(polygonPoints)/sizeof(polygonPoints[0]);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  CycleCounter_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init(&lcd);

  // Draw header text
  strcpy(buf, "Scanline Fill Algorithm Benchmark");
  Point bufPos = {0, 0}, screenStartPos = {0, 0}, screenEndPos = {LCD_WIDTH - 1, FONT_HEIGHT - 1};
  Artist_ClearCanvas(&textCanvas, BLACK);
  Artist_DrawText(&textCanvas, buf, bufPos, GREEN);
  LCD_SetArea(&lcd, &screenStartPos, &screenEndPos);
  LCD_DrawArea(&lcd, textCanvas.data, textCanvas.numRows, textCanvas.numCols);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);

	  Artist_ClearCanvas(&polygonCanvas, BLACK);
	  uint32_t start_ticks = CycleCounter_GetValue();
	  Artist_FillPolygon(&polygonCanvas, polygonPoints, numPolygonPoints, BLUE);
	  uint32_t duration_ticks = CycleCounter_GetValue() - start_ticks;
	  float execution_time = (float)(duration_ticks) / (SystemCoreClock/1000000);

	  screenStartPos.x = 60; screenStartPos.y = 100;
	  screenEndPos.x = screenStartPos.x + POLYGON_BUF_WIDTH - 1; screenEndPos.y = screenStartPos.y + POLYGON_BUF_HEIGHT - 1;
	  LCD_SetArea(&lcd, &screenStartPos, &screenEndPos);
	  LCD_DrawArea(&lcd, polygonCanvas.data, polygonCanvas.numRows, polygonCanvas.numCols);

	  sprintf(buf, "Execution ticks: %lu", duration_ticks);
	  bufPos.x = 0; bufPos.y = 0;
	  screenStartPos.x = 0; screenStartPos.y = 3*LCD_HEIGHT/4;
	  screenEndPos.x = screenStartPos.x + LCD_WIDTH - 1; screenEndPos.y = screenStartPos.y + FONT_HEIGHT - 1;
	  Artist_ClearCanvas(&textCanvas, BLACK);
	  Artist_DrawText(&textCanvas, buf, bufPos, RED);
	  LCD_SetArea(&lcd, &screenStartPos, &screenEndPos);
	  LCD_DrawArea(&lcd, textCanvas.data, textCanvas.numRows, textCanvas.numCols);

	  sprintf(buf, "Execution time : %.3f us", execution_time);
	  bufPos.x = 0; bufPos.y = 0;
	  screenStartPos.x = 0; screenStartPos.y = 3*LCD_HEIGHT/4 + FONT_HEIGHT;
	  screenEndPos.x = screenStartPos.x + LCD_WIDTH - 1; screenEndPos.y = screenStartPos.y + FONT_HEIGHT - 1;
	  Artist_ClearCanvas(&textCanvas, BLACK);
	  Artist_DrawText(&textCanvas, buf, bufPos, YELLOW);
	  LCD_SetArea(&lcd, &screenStartPos, &screenEndPos);
	  LCD_DrawArea(&lcd, textCanvas.data, textCanvas.numRows, textCanvas.numCols);
	  HAL_Delay(500);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

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
