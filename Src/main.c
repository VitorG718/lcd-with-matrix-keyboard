/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	char character;
	uint8_t pressed;
} Button;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INSTRUCTION 			0
#define DATA 							1
#define WRITE_MODE 				GPIO_PIN_RESET
#define READ_MODE 				GPIO_PIN_SET
#define RESET 						GPIO_PIN_RESET
#define SET 							GPIO_PIN_SET
#define VIAS_4						0
#define VIAS_8						1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Button kb_keys[4][4] = {
	{{'1',0},{'2',0},{'3',0},{'\0',0}},
	{{'4',0},{'5',0},{'6',0},{'\0',0}},
	{{'7',0},{'8',0},{'9',0},{'\0',0}},
	{{'*',0},{'0',0},{'#',0},{'\0',0}}
};
uint8_t newLine = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void lcdInitialization(uint8_t);
void lcdSender(uint8_t, uint8_t, uint8_t);
void lcdWriteMessage(char *, uint8_t);
void lcdWriteCharacter(char, uint8_t);
void breakLine(uint8_t);
void cleanScreen(uint8_t);
void enableKBCol(uint8_t);
uint8_t selectedRow(uint8_t);
	
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	lcdInitialization(VIAS_8);
	lcdWriteMessage("Let's make       ", VIAS_8);
	lcdWriteMessage("a call           ", VIAS_8);
	uint8_t col = 0;
	uint8_t row;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		switch(col) {
			case 0:
				enableKBCol(col);
				row = selectedRow(col);
				if (row != 0xff && kb_keys[row][col].character != '\0') {
					switch(kb_keys[row][col].character) {
						case '*':
							cleanScreen(VIAS_8);
							break;
						case '#':
							breakLine(VIAS_8);
							lcdWriteMessage("Calling....", VIAS_8);
							break;
						default:
							lcdWriteCharacter(kb_keys[row][col].character, VIAS_8);
					}
				}
				col = 1;
				break;
			case 1:
				enableKBCol(col);
				row = selectedRow(col);
				if (row != 0xff && kb_keys[row][col].character != '\0') {
					switch(kb_keys[row][col].character) {
						case '*':
							cleanScreen(VIAS_8);
							break;
						case '#':
							breakLine(VIAS_8);
							lcdWriteMessage("Calling....", VIAS_8);
							break;
						default:
							lcdWriteCharacter(kb_keys[row][col].character, VIAS_8);
					}
				}
				col = 2;
				break;
			case 2:
				enableKBCol(col);
				row = selectedRow(col);
				if (row != 0xff && kb_keys[row][col].character != '\0') {
					switch(kb_keys[row][col].character) {
						case '*':
							cleanScreen(VIAS_8);
							break;
						case '#':
							breakLine(VIAS_8);
							lcdWriteMessage("Calling....", VIAS_8);
							break;
						default:
							lcdWriteCharacter(kb_keys[row][col].character, VIAS_8);
					}
					
				}
				col = 3;
				break;
			case 3:
				enableKBCol(col);
				row = selectedRow(col);
				if (row != 0xff&& kb_keys[row][col].character != '\0') {
					switch(kb_keys[row][col].character) {
						case '*':
							cleanScreen(VIAS_8);
							break;
						case '#':
							breakLine(VIAS_8);
							lcdWriteMessage("Calling....", VIAS_8);
							break;
						default:
							lcdWriteCharacter(kb_keys[row][col].character, VIAS_8);
					}
				}
				col = 0;
				break;
		}
		
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void lcdInitialization(uint8_t vias) {
	HAL_Delay(15);
	lcdSender(0x30, INSTRUCTION, vias);
	HAL_Delay(5);
	lcdSender(0x30, INSTRUCTION, vias);
	lcdSender(0x30, INSTRUCTION, vias);
	lcdSender(0x38, INSTRUCTION, vias);
	lcdSender(0x0F, INSTRUCTION, vias);
	lcdSender(0x06, INSTRUCTION, vias);
	HAL_Delay(2);
	lcdSender(0x01, INSTRUCTION, vias);
	HAL_Delay(2);
	lcdSender(0x0C, INSTRUCTION, vias);
	HAL_Delay(2);
	
}

void lcdSender(uint8_t data, uint8_t mode, uint8_t vias) {
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (mode == DATA ? SET:RESET));
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, WRITE_MODE);
	
	switch(vias) {
		case VIAS_8:
			HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, ((data & 0x01) == 0x01 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, ((data & 0x02) == 0x02 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, ((data & 0x04) == 0x04 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, ((data & 0x08) == 0x08 ? SET:RESET));
		case VIAS_4:
			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, ((data & 0x10) == 0x10 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, ((data & 0x20) == 0x20 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, ((data & 0x40) == 0x40 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, ((data & 0x80) == 0x80 ? SET:RESET));
	}
	
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	
}

void lcdWriteMessage(char *msg, uint8_t vias) {
	uint8_t msgCounter = 0;
	do {
		switch(msgCounter) {
			case 16:
				breakLine(vias);
				break;
			case 32:
				cleanScreen(vias);
				break;
			default:
				msgCounter++;
		}
		if (*msg != '\0') {
			lcdSender(*msg, DATA, vias);
		}
	} while(*msg++);
}

void lcdWriteCharacter(char data, uint8_t vias) {
	lcdSender(data, DATA, vias);
}

void breakLine(uint8_t vias) {
	lcdSender(0xC0, INSTRUCTION, vias);
}

void cleanScreen(uint8_t vias) {
	lcdSender(0x01, INSTRUCTION, vias);
}


void enableKBCol(uint8_t col) {
	HAL_GPIO_WritePin(KB_COL1_GPIO_Port, KB_COL1_Pin, (col == 0 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL2_GPIO_Port, KB_COL2_Pin, (col == 1 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL3_GPIO_Port, KB_COL3_Pin, (col == 2 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL4_GPIO_Port, KB_COL4_Pin, (col == 3 ? GPIO_PIN_SET:GPIO_PIN_RESET));
}

uint8_t selectedRow(uint8_t col) {
	uint8_t row = 0xff;
	if(HAL_GPIO_ReadPin(KB_ROW1_GPIO_Port, KB_ROW1_Pin) && kb_keys[0][col].pressed == 0) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW1_GPIO_Port, KB_ROW1_Pin)) {
			row = 0;
			kb_keys[row][col].pressed = 1;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW2_GPIO_Port, KB_ROW2_Pin) && kb_keys[1][col].pressed == 0) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW2_GPIO_Port, KB_ROW2_Pin)) {
			row = 1;
			kb_keys[row][col].pressed = 1;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW3_GPIO_Port, KB_ROW3_Pin) && kb_keys[2][col].pressed == 0) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW3_GPIO_Port, KB_ROW3_Pin)) {
			row = 2;
			kb_keys[row][col].pressed = 1;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW4_GPIO_Port, KB_ROW4_Pin) && kb_keys[3][col].pressed == 0) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW4_GPIO_Port, KB_ROW4_Pin)) {
			row = 3;
			kb_keys[row][col].pressed = 1;
		}
	}
	
	if(row == 0xff) {
		kb_keys[0][col].pressed = 0;
		kb_keys[1][col].pressed = 0;
		kb_keys[2][col].pressed = 0;
		kb_keys[3][col].pressed = 0;
	}
	
	return row;
}
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
