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

//RX
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

typedef struct ts_rxType{
	char start;
	unsigned char data;
	char parity;
	char stop;
	char error;
} rxType;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//TX
char Uart_Tx_Parity_Mode;
int tim2_cnt;
char tim2_flag;
char tim2_trigger;
char chUartTxData[32];

//RX
char Uart_Rx_Parity_Mode;
rxType rxData[32];
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
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

//TX
void Uart_Tx_Byte(char TxData, char Parity)
{
	char chExit = 'n';
	char chComp = 0x01;
	int ParityBit_cnt = 0;

	tim2_cnt = 0;
	tim2_flag = 'y';
	tim2_trigger = 'n';

	while(1){
		if(('y' == tim2_trigger) && (tim2_cnt == 1)){ // idle
			tim2_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		}else if(('y' == tim2_trigger) && (tim2_cnt == 2)){ // start
			tim2_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		}else if(('y' == tim2_trigger) && (tim2_cnt == 3)){ // bit 0
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 4)){ // bit 1
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 5)){ // bit 2
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 6)){ // bit 3
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 7)){ // bit 4
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 8)){ // bit 5
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 9)){ // bit 6
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 10)){ // bit 7
			tim2_trigger = 'n';
			if((TxData & chComp) == chComp){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				ParityBit_cnt += 1;
			}else{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			chComp <<= 1;
		}else if(('y' == tim2_trigger) && (tim2_cnt == 11)){ // [parity : none/even/odd] or [stop]
			tim2_trigger = 'n';
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
		}else if(('y' == tim2_trigger) && (tim2_cnt == 12)){ // stop
			tim2_trigger = 'n';
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
			chExit = 'y';
		}
		if(('y' == tim2_trigger) && ('y' == chExit)){
			tim2_trigger = 'n';
			tim2_flag = 'n';
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


//RX
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance){
			//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
			if('y' == tim2_flag){
				tim2_cnt += 1;
				tim2_trigger = 'y';
			}
		}
	if(htim->Instance == htim3.Instance){
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
					rxData[rxByte_cnt].start = 'y';
					rxData[rxByte_cnt].data = 0x00;
					rxData[rxByte_cnt].parity = 'n';
					rxData[rxByte_cnt].stop = 'x';
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
					//---
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
					//---
					/*
					else if(9 == rxBit_cnt){
						Uart_Rx_data = 'n';
						Uart_Rx_parity = 'y';

						//----- pairyt bit ??????? ----------------------

					}else if(10 == rxBit_cnt){
						Uart_Rx_parity = 'n';
						Uart_Rx_stop = 'y';

						//----- stop bit ??????? ----------------------

					}
					*/
				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_data)){
					rxData[rxByte_cnt].data >>= 1;
					if(1 == rxBit_value){
						rxData[rxByte_cnt].data += 0x80;
						rxParity_bit += 1;
					}
				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_parity)){
					//---
					if('e' == Uart_Rx_Parity_Mode){
						if(((rxParity_bit % 2) == 0) && (0 == rxBit_value)){
							rxData[rxByte_cnt].parity = 'e';
						}else if(((rxParity_bit % 2) == 1) && (1 == rxBit_value)){
							rxData[rxByte_cnt].parity = 'e';
						}else{
							rxData[rxByte_cnt].parity = 'f';
						}
					}else if('o' == Uart_Rx_Parity_Mode){
						if(((rxParity_bit % 2) == 0) && (1 == rxBit_value)){
							rxData[rxByte_cnt].parity = 'o';
						}else if(((rxParity_bit % 2) == 1) && (0 == rxBit_value)){
							rxData[rxByte_cnt].parity = 'o';
						}else{
							rxData[rxByte_cnt].parity = 'f';
						}
					}
					//---

					//----- pairyt bit ??????? ----------------------

				}
				if(('y' == Uart_Rx_trigger) && ('y' == Uart_Rx_stop)){
					//---
					Uart_Rx_active = 'n';
					if(1 == rxBit_value){
						rxData[rxByte_cnt].stop = 'y';
					}else{
						rxData[rxByte_cnt].stop = 'n';
					}
					//---

					//Uart_Rx_active = 'n';
					//----- stop bit ??????? ----------------------

				}
			}
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//RX
	HAL_StatusTypeDef tim2_it_start;
	HAL_StatusTypeDef tim3_it_start;
		GPIO_PinState pc13pin;
		GPIO_PinState pa8pin, pb8pin;
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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //TX
  printf("---uart tx mode\r\n---");

   Uart_Tx_Parity_Mode = 'n'; // 'n':none, 'e':even, 'o':odd
   tim2_cnt = 0;
   tim2_flag = 'n';
   tim2_trigger = 'n';

   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // tx idle : high

   tim2_it_start = HAL_TIM_Base_Start_IT(&htim2);
   if(HAL_OK == tim2_it_start){
 	  printf("HAL_TIM_Base_Start_IT OK\r\n");
   }else{
 	  printf("HAL_TIM_Base_Start_IT error\r\n");
   }

   chUartTxData[0] = 'a';


  //RX
  printf("---uart rx mode\r\n---");

    //-----parameter initial-----
    Uart_Rx_Parity_Mode = 'n'; // 'n':none, 'e':even, 'o':odd
    rxByte_cnt = 0;
    rxBit_cnt = 0;
    rxSamp_cnt = 0;
    Uart_Rx_flag = 'n';
    Uart_Rx_active = 'n';
    Uart_Rx_Temp = 0x00;

    tim3_it_start = HAL_TIM_Base_Start_IT(&htim3);
    if(HAL_OK == tim3_it_start){
  	  printf("HAL_TIM_Base_Start_IT OK\r\n");
    }else{
  	  printf("HAL_TIM_Base_Start_IT error\r\n");
    }

    //-----uart rx mode : start-----
    Uart_Rx_flag = 'y';
    Uart_Rx_active = 'n';

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  //----- parity mode ------------------------------------------------------------------------------
	  pa8pin = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
	  pb8pin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
	  if((GPIO_PIN_SET == pa8pin) && (GPIO_PIN_RESET == pb8pin)){
		  Uart_Tx_Parity_Mode = 'e';
		  Uart_Rx_Parity_Mode = 'e';
	  }else if((GPIO_PIN_RESET == pa8pin) && (GPIO_PIN_SET == pb8pin)){
		  Uart_Tx_Parity_Mode = 'o';
		  Uart_Rx_Parity_Mode = 'o';
	  }else{
		  Uart_Tx_Parity_Mode = 'n';
		  Uart_Rx_Parity_Mode = 'n';
	  }
	  //----- parity mode ------------------------------------------------------------------------------

	  //----- uart tx mode ------------------------------------------------------------------------------

	  //----- one byte -----
	  pc13pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  if(GPIO_PIN_RESET == pc13pin){
		  Uart_Tx_Byte(chUartTxData[0], Uart_Tx_Parity_Mode);
		  printf("-a14-Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[0],chUartTxData[0],Uart_Tx_Parity_Mode);
		  if('z' == chUartTxData[0]){
			  chUartTxData[0] = 'a';
		  }else{
			  chUartTxData[0] += 1;
		  }
	  }
	  //----- one byte -----

	  /*
	  //----- multi byte -----
	  pc13pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  if(GPIO_PIN_RESET == pc13pin){
		  //chUartTxData[0]
		  chUartTxData[1] = (chUartTxData[0] + 1);
		  chUartTxData[2] = (chUartTxData[1] + 1);
		  chUartTxData[3] = (chUartTxData[2] + 1);
		  chUartTxData[4] = (chUartTxData[3] + 1);
		  Uart_Tx_multiByte(5, chUartTxData, 'n'); // 'n':none, 'e':even, 'o':odd
		  printf("Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[0],chUartTxData[0],Uart_Tx_Parity_Mode);
		  printf("Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[1],chUartTxData[1],Uart_Tx_Parity_Mode);
		  printf("Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[2],chUartTxData[2],Uart_Tx_Parity_Mode);
		  printf("Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[3],chUartTxData[3],Uart_Tx_Parity_Mode);
		  printf("Uart_Tx_Byte=%c[0x%02x]---parity=%c---\r\n",chUartTxData[4],chUartTxData[4],Uart_Tx_Parity_Mode);
		  if('z' == chUartTxData[0]){
			  chUartTxData[0] = 'a';
		  }else{
			  chUartTxData[0] += 1;
		  }
		  HAL_Delay(500);
	  }
	  //----- multi byte -----
	  */

	  //----- uart tx mode ------------------------------------------------------------------------------


	  //----- uart rx mode ------------------------------------------------------------------------------
	  if(('n' == Uart_Rx_active) && (1 <= rxByte_cnt)){
		  for(m = 1; m <= rxByte_cnt; m++){
			  printf("-a14----rx:%c---data[%d]=0x%02x,parity=%c,stop=%c\r\n",Uart_Rx_Parity_Mode,m,rxData[m].data,rxData[m].parity,rxData[m].stop);
		  }
		  rxByte_cnt = 0;
	  }
	  //----- uart rx mode ------------------------------------------------------------------------------

	  HAL_Delay(50);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

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
  htim2.Init.Prescaler = 63;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 103;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 63;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 25;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
