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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

//----- uart rx mode ------------------------------------------------------------------------------
typedef struct ts_rxType{
	char start;
	unsigned char data;
	char parity;
	char stop;
} rxType;
//----- uart rx mode ------------------------------------------------------------------------------

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//----- uart timer2 -------------------------------------------------------------------------------
int tim2_base_cnt;
//----- uart timer2 -------------------------------------------------------------------------------

uint8_t urx[16], utx[16];
uint16_t Distance;
uint16_t Temperature;

//----- uart tx mode ------------------------------------------------------------------------------
char Uart_Tx_Parity_Mode;
int txBit_cnt;
char Uart_Tx_flag;
char Uart_Tx_trigger;
char UartTxData[32];
//----- uart tx mode ------------------------------------------------------------------------------

//----- uart rx mode ------------------------------------------------------------------------------
char Uart_Rx_Parity_Mode;
rxType UartRxData[32];
int rxByte_cnt;
int rxBit_cnt;
int rxBit_value;
int rxSamp_cnt;
int rxParity_bit;
char Uart_Rx_flag;
char Uart_Rx_active;
char Uart_Rx_Temp;
char Uart_Rx_trigger;
char Uart_Rx_data;
char Uart_Rx_parity;
char Uart_Rx_stop;
//----- uart rx mode ------------------------------------------------------------------------------


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

//----- timer callback ----------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance){
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
		if('y' == Uart_Tx_flag){
			tim2_base_cnt += 1;
			if((tim2_base_cnt % 4) == 0){
				txBit_cnt += 1;
				Uart_Tx_trigger = 'y';
				tim2_base_cnt = 0;
			}

		}
		if('y' == Uart_Rx_flag){
			if('n' == Uart_Rx_active){
				Uart_Rx_Temp <<= 1;
				if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){
					Uart_Rx_Temp += 1;
				}
				if((Uart_Rx_Temp & 0x0f) == 0x0c){
					Uart_Rx_active = 'y';
					Uart_Rx_Temp = 0x00;
					rxByte_cnt += 1;
					UartRxData[rxByte_cnt].start = 'y';
					UartRxData[rxByte_cnt].data = 0x00;
					UartRxData[rxByte_cnt].parity = 'n';
					UartRxData[rxByte_cnt].stop = 'x';
					rxBit_cnt = 0;
					rxSamp_cnt = 0;
					rxParity_bit = 0;
					Uart_Rx_trigger = 'n';
					Uart_Rx_data = 'n';
					Uart_Rx_parity = 'n';
					Uart_Rx_stop = 'n';
				}
			}else{
				Uart_Rx_trigger = 'n';
				rxSamp_cnt += 1;
				if((rxSamp_cnt % 4) == 0){
					Uart_Rx_trigger = 'y';
					if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){
						rxBit_value = 1;
					}else{
						rxBit_value = 0;
					}
					rxBit_cnt += 1;
					if((1 <= rxBit_cnt) && (rxBit_cnt <= 8)){
						Uart_Rx_data = 'y';
					}else if(9 == rxBit_cnt){
						Uart_Rx_data = 'n';
						if('n' == Uart_Rx_Parity_Mode){
							Uart_Rx_parity = 'n';
							Uart_Rx_stop = 'y';
						}else if('e' == Uart_Rx_Parity_Mode){
							Uart_Rx_parity = 'y';
						}else if('o' == Uart_Rx_Parity_Mode){
							Uart_Rx_parity = 'y';
						}
					}else if(10 == rxBit_cnt){
						Uart_Rx_parity = 'n';
						Uart_Rx_stop = 'y';
					}
				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_data)){
					Uart_Rx_trigger = 'n';
					UartRxData[rxByte_cnt].data >>= 1;
					if(1 == rxBit_value){
						UartRxData[rxByte_cnt].data += 0x80;
						rxParity_bit += 1;
					}
				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_parity)){
					Uart_Rx_trigger = 'n';
					if('e' == Uart_Rx_Parity_Mode){
						if(((rxParity_bit % 2) == 0) && (0 == rxBit_value)){
							UartRxData[rxByte_cnt].parity = 'e';
						}else if(((rxParity_bit % 2) == 1) && (1 == rxBit_value)){
							UartRxData[rxByte_cnt].parity = 'e';
						}else{
							UartRxData[rxByte_cnt].parity = 'f';
						}
					}else if('o' == Uart_Rx_Parity_Mode){
						if(((rxParity_bit % 2) == 0) && (1 == rxBit_value)){
							UartRxData[rxByte_cnt].parity = 'o';
						}else if(((rxParity_bit % 2) == 1) && (0 == rxBit_value)){
							UartRxData[rxByte_cnt].parity = 'o';
						}else{
							UartRxData[rxByte_cnt].parity = 'f';
						}
					}
				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_stop)){
					Uart_Rx_trigger = 'n';
					Uart_Rx_active = 'n';
					if(1 == rxBit_value){
						UartRxData[rxByte_cnt].stop = 'y';
					}else{
						UartRxData[rxByte_cnt].stop = 'n';
					}
				}
			}
		}
	}
}
//----- timer callback ----------------------------------------------------------------------------

//----- uart tx mode ------------------------------------------------------------------------------
void Uart_Tx_Byte(char TxData, char Parity)
{
	char chExit = 'n';
	char chComp = 0x01;
	int ParityBit_cnt = 0;

	txBit_cnt = 0;
	Uart_Tx_flag = 'y';
	Uart_Tx_trigger = 'n';

	while(1){
		if(('y' == Uart_Tx_trigger) && (txBit_cnt == 1)){ // idle
			Uart_Tx_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		}else if(('y' == Uart_Tx_trigger) && (txBit_cnt == 2)){ // start
			Uart_Tx_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		}else if(('y' == Uart_Tx_trigger) && ((txBit_cnt >= 3) && (txBit_cnt <= 10))){ // bit 0~7
			Uart_Tx_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == Uart_Tx_trigger) && (txBit_cnt == 11)){ // [parity : none/even/odd] or [stop]
			Uart_Tx_trigger = 'n';
			if('n' == Parity){	// stop
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				chExit = 'y';
			}else if('e' == Parity){
				if((ParityBit_cnt % 2) == 0){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				}else{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				}
			}else if('o' == Parity){
				if((ParityBit_cnt % 2) == 0){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				}
			}
		}else if(('y' == Uart_Tx_trigger) && (txBit_cnt == 12)){ // stop
			Uart_Tx_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
			chExit = 'y';
		}
		if(('y' == Uart_Tx_trigger) && ('y' == chExit)){
			Uart_Tx_trigger = 'n';
			Uart_Tx_flag = 'n';
			break;
		}
	}
}

int Uart_Tx_multiByte(int Tx_cnt, char* TxData, char Parity)
{
	int m;

	for(m = 0; m < Tx_cnt; m++){
		Uart_Tx_Byte(TxData[m], Parity);
	}

	return(0);
}
//----- uart tx mode ------------------------------------------------------------------------------

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	HAL_StatusTypeDef uart2status;
	HAL_StatusTypeDef tim2_it_start;
	int m;

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  printf("---uc100 uart : gpio---\r\n");

  //----- timer2 -----------------------------------------------------------------------------------
  tim2_base_cnt = 0;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

  tim2_it_start = HAL_TIM_Base_Start_IT(&htim2);
  if(HAL_OK == tim2_it_start){
	  printf("HAL_TIM_Base_Start_IT OK\r\n");
  }else{
	  printf("HAL_TIM_Base_Start_IT error\r\n");
  }
  //----- timer2 -----------------------------------------------------------------------------------

  //----- uart tx mode ------------------------------------------------------------------------------
  Uart_Tx_Parity_Mode = 'n'; // 'n':none, 'e':even, 'o':odd
  txBit_cnt = 0;
  Uart_Tx_flag = 'n';
  Uart_Tx_trigger = 'n';

  UartTxData[0] = 'a';
  //----- uart tx mode ------------------------------------------------------------------------------

  //----- uart rx mode ------------------------------------------------------------------------------
  Uart_Rx_Parity_Mode = 'n'; // 'n':none, 'e':even, 'o':odd
  rxByte_cnt = 0;
  rxBit_cnt = 0;
  rxSamp_cnt = 0;
  Uart_Rx_flag = 'n';
  Uart_Rx_active = 'n';
  Uart_Rx_Temp = 0x00;

  //-----uart rx mode : start-----
  Uart_Rx_flag = 'y';
  Uart_Rx_active = 'n';
  //----- uart rx mode ------------------------------------------------------------------------------


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  HAL_Delay(1000);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

	  //----- distance -----
	  Uart_Tx_Byte(0x55, 'n');
	  //printf("Uart_Tx_Byte(Distance)\r\n");
	  HAL_Delay(100);
	  if(('n' == Uart_Rx_active) && (2 <= rxByte_cnt)){
		  for(m = 1; m <= rxByte_cnt; m++){
			  //printf("---rx:%c---data[%d]=0x%02x,parity=%c,stop=%c\r\n",Uart_Rx_Parity_Mode,m,UartRxData[m].data,UartRxData[m].parity,UartRxData[m].stop);
		  }
		  if(2 == rxByte_cnt){
			  Distance = (UartRxData[1].data << 8) + UartRxData[2].data;
			  printf("---UC-100 : Distance = %d---\r\n",Distance);
		  }
		  rxByte_cnt = 0;
	  }else{
		  printf("--Uart_Rx_Byte error(distance)\r\n");
	  }
	  //----- distance -----

	  HAL_Delay(1000);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

	  //----- temperature -----
	  Uart_Tx_Byte(0x50, 'n');
	  //printf("Uart_Tx_Byte(temperature)\r\n");
	  HAL_Delay(100);
	  if(('n' == Uart_Rx_active) && (1 <= rxByte_cnt)){
		  for(m = 1; m <= rxByte_cnt; m++){
			 // printf("---rx:%c---data[%d]=0x%02x,parity=%c,stop=%c\r\n",Uart_Rx_Parity_Mode,m,UartRxData[m].data,UartRxData[m].parity,UartRxData[m].stop);
		  }
		  if(1 == rxByte_cnt){
			  Temperature = UartRxData[1].data - 45;
			  printf("---UC-100 : Temperature = %d---\r\n",Temperature);
		  }
		  rxByte_cnt = 0;
	  }else{
		  printf("--Uart_Rx_Byte error(temperature)\r\n");
	  }
	  //----- temperature -----

	  printf("---\r\n");



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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 64-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 26-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
