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
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	char character;
	uint8_t number;
} Button;

typedef struct {
	char characters[2];
	uint8_t number;
} ValueDef;
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
#define VIAS_DEFAULT      VIAS_8
#define LEFT_ARROW				0x00
#define RIGHT_ARROW				0x01

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef enum {left, right, both, none}ScreenArrow;
typedef enum {screen1, screen2, screen3, screen4, screen5, screen6, screen7, wrongPassword, event1, event2, event3, event4, event5, noScreen}Screens;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Button kb_keys[4][4] = {
	{{'1',1},{'2',2},{'3',3},{'u',0xfe}},
	{{'4',4},{'5',5},{'6',6},{'d',0xfe}},
	{{'7',7},{'8',8},{'9',9},{'l',0xfe}},
	{{'*',0xff},{'0',0},{'#',0xff},{'r',0xfe}}
};

uint8_t arrowsCharacter [2][7] = {
	{0x00, 0x04, 0x08, 0x1f, 0x08, 0x04, 0x00},
	{0x00, 0x04, 0x02, 0x1f, 0x02, 0x04, 0x00}
};

char number[10] = {'0','1','2','3','4','5','6','7','8','9'};

ValueDef aqValue [3] = {
	{{'3','2'}, 32},
	{{'3','2'}, 32},
	{{'3','2'}, 32}
};

ValueDef rsfValue [3] = {
	{{'1','7'}, 17},
	{{'1','7'}, 17},
	{{'1','7'}, 17}
};

char passwords[3][4] = {
	{'0','0','0','1'},
	{'0','0','0','2'},
	{'0','0','0','3'}
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void lcdInitialization(void);
void lcdSender(uint8_t, uint8_t, uint8_t);
void lcdWriteMessage(char *);
void lcdWriteCharacter(char);
void lcdWriteFormatedNumber(char);
void breakLine(void);
void cleanScreen(void);
void enableKBCol(uint8_t);
uint8_t selectedRow(uint8_t);
void lcdCreateCharacter(uint8_t [2][7]);
void turnOutputVias(void);
void turnInputVias(void);
char lcdReader(void);
void moveCursorTo(uint8_t row, uint8_t col);
Button numberButton(void);
void createScreen(char *msg1, char *msg2, ScreenArrow arrows) {
	cleanScreen();
	lcdWriteMessage(msg1);
	breakLine();
	lcdWriteMessage(msg2);
	
	switch(arrows) {
		case left:
			moveCursorTo(2,1);
			lcdWriteCharacter(LEFT_ARROW);
			break;
		case right:
			moveCursorTo(2,16);
			lcdWriteCharacter(RIGHT_ARROW);
			break;
		case both:
			moveCursorTo(2,1);
			lcdWriteCharacter(LEFT_ARROW);
			moveCursorTo(2,16);
			lcdWriteCharacter(RIGHT_ARROW);
			break;
		case none:
			break;
	}
}

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
	lcdInitialization();
	lcdCreateCharacter(arrowsCharacter);

	Screens currentScreen = noScreen, nextScreen = screen1;
	uint8_t profile = 1;
	char *msg1, *msg2;
	Button bt;
	uint8_t passwordChecked;
	ValueDef aqValueTemp;
	ValueDef rsfValueTemp;
	uint8_t i;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		if(currentScreen != nextScreen) {
			currentScreen = nextScreen;
			switch(currentScreen) {
				case screen1:
					createScreen("  TEMPORIZADOR  ", "    PERFIL 1    ", right);
					profile = 1;
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'r':
								nextScreen = screen2;
								break;
							case 'd':
								nextScreen = screen4;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case screen2:
					createScreen("  TEMPORIZADOR  ", "    PERFIL 2    ", both);
					profile = 2;
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'l':
								nextScreen = screen1;
								break;
							case 'r':
								nextScreen = screen3;
								break;
							case 'd':
								nextScreen = screen4;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case screen3:
					createScreen("  TEMPORIZADOR  ", "    PERFIL 3    ", left);
					profile = 3;
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'l':
								nextScreen = screen2;
								break;
								break;
							case 'd':
								nextScreen = screen4;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case screen4:
					msg1 = "    PERFIL X    ";
					msg2 = "AQ: XXs RSF: XXs";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 12);
					lcdWriteCharacter(number[profile]);
					moveCursorTo(2, 5);
					lcdWriteCharacter(aqValue[profile-1].characters[0]);
					lcdWriteCharacter(aqValue[profile-1].characters[1]);
					moveCursorTo(2, 14);
					lcdWriteCharacter(rsfValue[profile-1].characters[0]);
					lcdWriteCharacter(rsfValue[profile-1].characters[1]);
					HAL_Delay(1000);
					nextScreen = screen5;
					break;
				case screen5:
					msg1 = "    PERFIL X    ";
					msg2 = "    INICIAR     ";
					createScreen(msg1, msg2, right);
					moveCursorTo(1, 12);
					lcdWriteCharacter(number[profile]);
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'u':
								nextScreen = screen1;
								break;
							case 'd':
								nextScreen = screen6;
								break;
							case 'r':
								nextScreen = screen7;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case screen6:
					msg1 = "    PERFIL X    ";
					msg2 = " INICIAR CICLO  ";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 12);
					lcdWriteCharacter(number[profile]);
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'u':
								nextScreen = screen1;
								break;
							// Go to task E
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case screen7:
					msg1 = "    PERFIL X    ";
					msg2 = "     EDITAR     ";
					createScreen(msg1, msg2, left);
					moveCursorTo(1, 12);
					lcdWriteCharacter(number[profile]);
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'u':
								nextScreen = screen1;
								break;
							case 'l':
								nextScreen = screen5;
								break;
							case 'd':
								nextScreen = event1;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case wrongPassword:
					msg1 = "     SENHA      ";
					msg2 = "   INCORRETA    ";
					createScreen(msg1, msg2, none);
					HAL_Delay(1000);
					nextScreen = screen4;
					break;
				case event1:
					msg1 = "EDITAR PERFIL X ";
					msg2 = "SENHA:          ";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 15);
					lcdWriteCharacter(number[profile]);
					moveCursorTo(2,7);
				  int8_t counter = 0;
					char passwordCheck[4];
					while(counter < 4) {
						do {
							bt = numberButton();
						} while(bt.number == 0xfe || bt.number == 0xff);
						passwordCheck[counter] = bt.character;
						lcdWriteCharacter('*');
						counter++;
					}
					passwordChecked = 1;
					for(counter = 3; counter >= 0; counter--) {
						if(passwords[profile-1][counter] != passwordCheck[counter]) {
							passwordChecked = 0;
							break;
						}
					}
					if(passwordChecked == 0) {
						nextScreen = wrongPassword;
					} else {
						nextScreen = event2;
					}
					break;
				case event2:
					msg1 = "EDITAR PERFIL X ";
					msg2 = "AQUECIMENTO: XXs";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 15);
					lcdWriteCharacter(number[profile]);
					moveCursorTo(2, 14);
					lcdWriteCharacter(aqValue[profile-1].characters[0]);
					lcdWriteCharacter(aqValue[profile-1].characters[1]);
					do {
						aqValueTemp.number = 0;
						moveCursorTo(2, 14);
						lcdWriteMessage("  ");
						moveCursorTo(2, 14);
						for(i = 0; i < 2; i++) {
							do {
								bt = numberButton();
							}while(bt.number == 0xfe || bt.number == 0xff);
							if(i == 0) {
								aqValueTemp.number += (bt.number * 10);
							} else {
								aqValueTemp.number += bt.number;
							}
							aqValueTemp.characters[i] = bt.character;
							lcdWriteCharacter(bt.character);
						}
					} while(aqValueTemp.number > 32);
					
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'd':
								nextScreen = event3;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case event3:
					msg1 = "EDITAR PERFIL X ";
					msg2 = "RESFRIAMENTO:XXs";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 15);
					lcdWriteCharacter(number[profile]);
					moveCursorTo(2, 14);
					lcdWriteCharacter(rsfValue[profile-1].characters[0]);
					lcdWriteCharacter(rsfValue[profile-1].characters[1]);
					do {
						rsfValueTemp.number = 0;
						moveCursorTo(2, 14);
						lcdWriteMessage("  ");
						moveCursorTo(2, 14);
						for(i = 0; i < 2; i++) {
							do {
								bt = numberButton();
							}while(bt.number == 0xfe || bt.number == 0xff);
							if(i == 0) {
								rsfValueTemp.number += (bt.number * 10);
							} else {
								rsfValueTemp.number += bt.number;
							}
							rsfValueTemp.characters[i] = bt.character;
							lcdWriteCharacter(bt.character);
						}
					} while(rsfValueTemp.number > 17);
					
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'd':
								nextScreen = event4;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case event4:
					msg1 = "SALVAR PERFIL X?";
					msg2 = "AQ: XXs RSF: XXs";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 15);
					lcdWriteCharacter(number[profile]);
					moveCursorTo(2, 5);
					lcdWriteCharacter(aqValueTemp.characters[0]);
					lcdWriteCharacter(aqValueTemp.characters[1]);
					moveCursorTo(2, 14);
					lcdWriteCharacter(rsfValueTemp.characters[0]);
					lcdWriteCharacter(rsfValueTemp.characters[1]);
					do {
						bt = numberButton();
						switch(bt.character) {
							case 'u':
								nextScreen = screen1;
								break;
							case 'd':
								rsfValue[profile-1].characters[0] = rsfValueTemp.characters[0];
								rsfValue[profile-1].characters[1] = rsfValueTemp.characters[1];
								rsfValue[profile-1].number = rsfValueTemp.number;
							
								aqValue[profile-1].characters[0] = aqValueTemp.characters[0];
								aqValue[profile-1].characters[1] = aqValueTemp.characters[1];
								aqValue[profile-1].number = aqValueTemp.number;
							
								nextScreen = event5;
								break;
							default:
								bt.number = 0xff;
								break;
						}
					} while(bt.number != 0xfe);
					break;
				case event5:
					msg1 = " PERFIL X SALVO ";
					msg2 = "                ";
					createScreen(msg1, msg2, none);
					moveCursorTo(1, 9);
					lcdWriteCharacter(number[profile]);
					HAL_Delay(1000);
					nextScreen = screen5;
					break;
				case noScreen:
					break;
			}
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
void lcdInitialization(void) {
	
	switch(VIAS_DEFAULT) {
		case VIAS_8:
			HAL_Delay(15);
			lcdSender(0x30, INSTRUCTION, WRITE_MODE);
			HAL_Delay(5);
			lcdSender(0x30, INSTRUCTION, WRITE_MODE);
			lcdSender(0x30, INSTRUCTION, WRITE_MODE);
			lcdSender(0x38, INSTRUCTION, WRITE_MODE);
			lcdSender(0x0F, INSTRUCTION, WRITE_MODE);
			lcdSender(0x06, INSTRUCTION, WRITE_MODE);
			HAL_Delay(2);
			lcdSender(0x01, INSTRUCTION, WRITE_MODE);
			HAL_Delay(2);
			lcdSender(0x0C, INSTRUCTION, WRITE_MODE);
			HAL_Delay(2);
			break;
		case VIAS_4:
			HAL_Delay(15);
			lcdSender(0x33, INSTRUCTION, WRITE_MODE);
			lcdSender(0x32, INSTRUCTION, WRITE_MODE);
			lcdSender(0x2F, INSTRUCTION, WRITE_MODE);
			lcdSender(0x08, INSTRUCTION, WRITE_MODE);
			lcdSender(0x0C, INSTRUCTION, WRITE_MODE);
			lcdSender(0x01, INSTRUCTION, WRITE_MODE);
			HAL_Delay(2);
			break;
	}
	
}

void lcdSender(uint8_t data, uint8_t rs_mode, uint8_t rw_mode) {
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (rs_mode == DATA ? SET:RESET));
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, rw_mode);
	
	switch(VIAS_DEFAULT) {
		case VIAS_8:
			HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, ((data & 0x01) == 0x01 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, ((data & 0x02) == 0x02 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, ((data & 0x04) == 0x04 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, ((data & 0x08) == 0x08 ? SET:RESET));
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, ((data & 0x10) == 0x10 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, ((data & 0x20) == 0x20 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, ((data & 0x40) == 0x40 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, ((data & 0x80) == 0x80 ? SET:RESET));
			break;
		case VIAS_4:
			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, ((data & 0x10) == 0x10 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, ((data & 0x20) == 0x20 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, ((data & 0x40) == 0x40 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, ((data & 0x80) == 0x80 ? SET:RESET));
		
			HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
		
			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, (((data << 4) & 0x10) == 0x10 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, (((data << 4) & 0x20) == 0x20 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, (((data << 4) & 0x40) == 0x40 ? SET:RESET));
			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, (((data << 4) & 0x80) == 0x80 ? SET:RESET));
	}
	
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	
}

void lcdWriteMessage(char *msg) {
	do {
			lcdSender(*msg, DATA, WRITE_MODE);
	} while(*msg++ && *msg != '\0');
}

void lcdWriteCharacter(char data) {
	lcdSender(data, DATA, WRITE_MODE);
}

void breakLine(void) {
	lcdSender(0xC0, INSTRUCTION, WRITE_MODE);
}

void cleanScreen(void) {
	lcdSender(0x01, INSTRUCTION, WRITE_MODE);
}

void enableKBCol(uint8_t col) {
	HAL_GPIO_WritePin(KB_COL1_GPIO_Port, KB_COL1_Pin, (col == 0 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL2_GPIO_Port, KB_COL2_Pin, (col == 1 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL3_GPIO_Port, KB_COL3_Pin, (col == 2 ? GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(KB_COL4_GPIO_Port, KB_COL4_Pin, (col == 3 ? GPIO_PIN_SET:GPIO_PIN_RESET));
}

uint8_t selectedRow(uint8_t col) {
	uint8_t row = 0xff;
	if(HAL_GPIO_ReadPin(KB_ROW1_GPIO_Port, KB_ROW1_Pin)) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW1_GPIO_Port, KB_ROW1_Pin)) {
			row = 0;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW2_GPIO_Port, KB_ROW2_Pin)) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW2_GPIO_Port, KB_ROW2_Pin)) {
			row = 1;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW3_GPIO_Port, KB_ROW3_Pin)) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW3_GPIO_Port, KB_ROW3_Pin)) {
			row = 2;
		}
	} else if(HAL_GPIO_ReadPin(KB_ROW4_GPIO_Port, KB_ROW4_Pin)) {
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(KB_ROW4_GPIO_Port, KB_ROW4_Pin)) {
			row = 3;
		}
	}
	
	return row;
}

void lcdCreateCharacter(uint8_t arr[2][7]) {
	uint8_t i = 0;
	lcdSender(0x40, INSTRUCTION, WRITE_MODE);
	for(i = 0; i < 7;i++) {
		lcdSender(arr[0][i], DATA, WRITE_MODE);
	}
	lcdSender(0x00, DATA, WRITE_MODE);
	for(i = 0; i < 7;i++) {
		lcdSender(arr[1][i], DATA, WRITE_MODE);
	}
	lcdSender(0x00, DATA, WRITE_MODE);
	lcdSender(0x00, INSTRUCTION, WRITE_MODE);
}

void turnOutputVias(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = LCD_DB7_Pin|LCD_DB6_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_DB3_Pin|LCD_DB4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_DB5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void turnInputVias(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = LCD_DB7_Pin|LCD_DB6_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_DB3_Pin|LCD_DB4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_DB5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

char lcdReader() {
	char data[9];
	data[8] = 0x00;
	uint8_t i;
	
	turnInputVias();
	
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, DATA);
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, READ_MODE);
	
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, SET);
	HAL_Delay(1);
	
	switch(VIAS_DEFAULT) {
		case VIAS_4:
			break;
		case VIAS_8:
			data[0] = (HAL_GPIO_ReadPin(LCD_DB0_GPIO_Port, LCD_DB0_Pin) == 1 ? (0x01 << 0):0x00);
			data[1] = (HAL_GPIO_ReadPin(LCD_DB1_GPIO_Port, LCD_DB1_Pin) == 1 ? (0x01 << 1):0x00);
			data[2] = (HAL_GPIO_ReadPin(LCD_DB2_GPIO_Port, LCD_DB2_Pin) == 1 ? (0x01 << 2):0x00);
			data[3] = (HAL_GPIO_ReadPin(LCD_DB3_GPIO_Port, LCD_DB3_Pin) == 1 ? (0x01 << 3):0x00);
			data[4] = (HAL_GPIO_ReadPin(LCD_DB4_GPIO_Port, LCD_DB4_Pin) == 1 ? (0x01 << 4):0x00);
			data[5] = (HAL_GPIO_ReadPin(LCD_DB5_GPIO_Port, LCD_DB5_Pin) == 1 ? (0x01 << 5):0x00);
			data[6] = (HAL_GPIO_ReadPin(LCD_DB6_GPIO_Port, LCD_DB6_Pin) == 1 ? (0x01 << 6):0x00);
			data[7] = (HAL_GPIO_ReadPin(LCD_DB7_GPIO_Port, LCD_DB7_Pin) == 1 ? (0x01 << 7):0x00);
			break;
	}
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, RESET);
	turnOutputVias();

	for(i = 0; i < 8; i++) {
		data[8] |= data[i];
	}
	
	return data[8];
}

void moveCursorTo(uint8_t row, uint8_t col) {
	if(row > 2 || col > 16) {
		return;
	}
	uint8_t cursorPositions[2][16] = {
		{0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f},
		{0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf}
	};
	lcdSender((cursorPositions[row-1][col-1] | 0x80), INSTRUCTION, WRITE_MODE); 
}

Button numberButton(void) {
	uint8_t col = 0;
	uint8_t row;
	Button number = {'\0',0xff};
	while (number.number == 0xff) {
		switch(col) {
			case 0:
				enableKBCol(col);
				row = selectedRow(col);
				number.number = (row != 0xff ? kb_keys[row][col].number:0xff);
				if (number.number != 0xff) {
					number.character = kb_keys[row][col].character;
				}
				col = 1;
				break;
			case 1:
				enableKBCol(col);
				row = selectedRow(col);
				number.number = (row != 0xff ? kb_keys[row][col].number:0xff);
				if (number.number != 0xff) {
					number.character = kb_keys[row][col].character;
				}
				col = 2;
				break;
			case 2:
				enableKBCol(col);
				row = selectedRow(col);
				number.number = (row != 0xff ? kb_keys[row][col].number:0xff);
				if (number.number != 0xff) {
					number.character = kb_keys[row][col].character;
				}
				col = 3;
				break;
			case 3:
				enableKBCol(col);
				row = selectedRow(col);
				number.number = (row != 0xff ? kb_keys[row][col].number:0xff);
				if (number.number != 0xff) {
					number.character = kb_keys[row][col].character;
				}
				col = 0;
				break;
		}
	}
	
	return number;
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
