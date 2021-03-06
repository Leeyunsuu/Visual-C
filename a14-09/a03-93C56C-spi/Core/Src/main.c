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
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t utx[16];
uint8_t urx[16];

char SPI_TICK_1 = 'n';
char SPI_TICK_2 = 'n';
char SPI_TICK_3 = 'n';
char SPI_TICK_4 = 'n';
char SPI_CS_HIGH = 'n';
char SPI_CS_LOW = 'n';
char SPI_CLK_HIGH = 'n';
char SPI_CLK_LOW = 'n';
char SPI_DI_LOAD = 'n';
char SPI_DO_CHECK = 'n';
char SPI_BREAK = 'n';

int tim4Cnt;
int SPI_on = 0;
int SPI_BitCnt = 0;

char pc13flag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART2 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim4.Instance){
		// test pin
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1); // test
		if(1 == SPI_on){
			tim4Cnt += 1;
			if((tim4Cnt % 4) == 1){
				SPI_TICK_1 = 'y';
			}
			if((tim4Cnt % 4) == 2){
				SPI_TICK_2 = 'y';
			}
			if((tim4Cnt % 4) == 3){
				SPI_TICK_3 = 'y';
			}
			if((tim4Cnt % 4) == 0){
				SPI_TICK_4 = 'y';
				tim4Cnt = 0;
			}
		}
	}
}

int spi_93c56c_EWEN(void)
{
	int SPI_DI_OUTPUT = 0x0980;
	int SPI_Data_Cnt = 12;
	int SPI_Data_Comp = 0x0800;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // CS
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // CLK
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // DI
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // DO

	SPI_TICK_1 = 'n';
	SPI_TICK_2 = 'n';
	SPI_TICK_3 = 'n';
	SPI_TICK_4 = 'n';
	SPI_CS_HIGH = 'n';
	SPI_CS_LOW = 'n';
	SPI_CLK_HIGH = 'n';
	SPI_CLK_LOW = 'n';
	SPI_DI_LOAD = 'n';
	SPI_DO_CHECK = 'n';
	SPI_BREAK = 'n';

	SPI_BitCnt = 0;
	tim4Cnt = 0;
	SPI_on = 1;

	while(1){
		if('y' == SPI_TICK_1){
			SPI_TICK_1 = 'n';
			SPI_BitCnt += 1;
			if(1 == SPI_BitCnt){
				SPI_CS_HIGH = 'y';
			}
			if((SPI_Data_Cnt + 1) == SPI_BitCnt){
				SPI_CS_LOW = 'y';
				SPI_BREAK = 'y';
			}
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_DI_LOAD = 'y';
			}
		}
		if('y' == SPI_TICK_2){
			SPI_TICK_2 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_CLK_HIGH = 'y';
			}
		}
		if('y' == SPI_TICK_3){
			SPI_TICK_3 = 'n';
		}
		if('y' == SPI_TICK_4){
			SPI_TICK_4 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_CLK_LOW = 'y';
			}
		}

		if('y' == SPI_CS_HIGH){
			SPI_CS_HIGH = 'n';
			//--- ?????? ?????? --- // CS:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		}else if('y' == SPI_CS_LOW){
			SPI_CS_LOW = 'n';
			//--- ?????? ?????? --- // CS:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		}

		if('y' == SPI_CLK_HIGH){
			SPI_CLK_HIGH = 'n';
			//--- ?????? ?????? --- // CLK:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		}else if('y' == SPI_CLK_LOW){
			SPI_CLK_LOW = 'n';
			//--- ?????? ?????? --- // CLK:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}

		if('y' == SPI_DI_LOAD){
			SPI_DI_LOAD = 'n';
			if((SPI_DI_OUTPUT & SPI_Data_Comp) == SPI_Data_Comp){
				//--- ?????? ?????? --- // DI:1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
			}else{
				//--- ?????? ?????? --- // DI:0
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			}
			SPI_Data_Comp >>= 1;
		}

		if('y' == SPI_BREAK){
			SPI_BREAK = 'n';
			SPI_on = 0;
			break;
		}
	}

	return(0);
}

int spi_93c56c_WRITE(int SPI_ADDR, int SPI_DATA_WRITE)
{
	int SPI_DI_OUTPUT = 0x0A0000 + (SPI_ADDR << 8) + SPI_DATA_WRITE;
	int SPI_Data_Cnt = 20;
	int SPI_Data_Comp = 0x80000;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // CS
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // CLK
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // DI
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // DO

	SPI_TICK_1 = 'n';
	SPI_TICK_2 = 'n';
	SPI_TICK_3 = 'n';
	SPI_TICK_4 = 'n';
	SPI_CS_HIGH = 'n';
	SPI_CS_LOW = 'n';
	SPI_CLK_HIGH = 'n';
	SPI_CLK_LOW = 'n';
	SPI_DI_LOAD = 'n';
	SPI_DO_CHECK = 'n';
	SPI_BREAK = 'n';

	SPI_BitCnt = 0;
	tim4Cnt = 0;
	SPI_on = 1;

	while(1){
		if('y' == SPI_TICK_1){
			SPI_TICK_1 = 'n';
			SPI_BitCnt += 1;
			if(1 == SPI_BitCnt){
				SPI_CS_HIGH = 'y';
			}
			if((SPI_Data_Cnt + 1) == SPI_BitCnt){
				SPI_CS_LOW = 'y';
				SPI_BREAK = 'y';
			}
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_DI_LOAD = 'y';
			}
		}
		if('y' == SPI_TICK_2){
			SPI_TICK_2 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_CLK_HIGH = 'y';
			}
		}
		if('y' == SPI_TICK_3){
			SPI_TICK_3 = 'n';
		}
		if('y' == SPI_TICK_4){
			SPI_TICK_4 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_CLK_LOW = 'y';
			}
		}

		if('y' == SPI_CS_HIGH){
			SPI_CS_HIGH = 'n';
			//--- ?????? ?????? --- // CS:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		}else if('y' == SPI_CS_LOW){
			SPI_CS_LOW = 'n';
			//--- ?????? ?????? --- // CS:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		}

		if('y' == SPI_CLK_HIGH){
			SPI_CLK_HIGH = 'n';
			//--- ?????? ?????? --- // CLK:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		}else if('y' == SPI_CLK_LOW){
			SPI_CLK_LOW = 'n';
			//--- ?????? ?????? --- // CLK:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}

		if('y' == SPI_DI_LOAD){
			SPI_DI_LOAD = 'n';
			if((SPI_DI_OUTPUT & SPI_Data_Comp) == SPI_Data_Comp){
				//--- ?????? ?????? --- // DI:1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
			}else{
				//--- ?????? ?????? --- // DI:0
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			}
			SPI_Data_Comp >>= 1;
		}

		if('y' == SPI_BREAK){
			SPI_BREAK = 'n';
			SPI_on = 0;
			break;
		}
	}

	return(0);
}

int spi_93c56c_READ(int SPI_ADDR, int* SPI_DATA_READ)
{
	int SPI_DI_OUTPUT = 0x0C00 + SPI_ADDR;
	int SPI_DO_INPUT = 0;
	int SPI_Data_Cnt = 12;
	int SPI_Data_Comp = 0x800;
	int SPI_Data_IN = 8;
	int SPI_WTF = 0;

	GPIO_PinState pb9pin;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // CS
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // CLK
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // DI
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // DO

	SPI_TICK_1 = 'n';
	SPI_TICK_2 = 'n';
	SPI_TICK_3 = 'n';
	SPI_TICK_4 = 'n';
	SPI_CS_HIGH = 'n';
	SPI_CS_LOW = 'n';
	SPI_CLK_HIGH = 'n';
	SPI_CLK_LOW = 'n';
	SPI_DI_LOAD = 'n';
	SPI_DO_CHECK = 'n';
	SPI_BREAK = 'n';

	SPI_BitCnt = 0;
	tim4Cnt = 0;
	SPI_on = 1;

	while(1){
		if('y' == SPI_TICK_1){
			SPI_TICK_1 = 'n';
			SPI_BitCnt += 1;
			if(1 == SPI_BitCnt){
				SPI_CS_HIGH = 'y';
			}
			if((SPI_Data_Cnt + SPI_Data_IN + 1) == SPI_BitCnt){
				SPI_CS_LOW = 'y';
				SPI_BREAK = 'y';
			}
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= SPI_Data_Cnt)){
				SPI_DI_LOAD = 'y';
			}
		}
		if('y' == SPI_TICK_2){
			SPI_TICK_2 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= (SPI_Data_Cnt + SPI_Data_IN))){
				SPI_CLK_HIGH = 'y';
			}
		}
		if('y' == SPI_TICK_3){
			SPI_TICK_3 = 'n';
		}
		if('y' == SPI_TICK_4){
			SPI_TICK_4 = 'n';
			if((1 <= SPI_BitCnt) && (SPI_BitCnt <= (SPI_Data_Cnt + SPI_Data_IN))){
				SPI_CLK_LOW = 'y';
			}
			if((SPI_Data_Cnt < SPI_BitCnt) && (SPI_BitCnt <= (SPI_Data_Cnt + SPI_Data_IN))){
				SPI_DO_CHECK = 'y';
			}
		}

		if('y' == SPI_CS_HIGH){
			SPI_CS_HIGH = 'n';
			//--- ?????? ?????? --- // CS:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		}else if('y' == SPI_CS_LOW){
			SPI_CS_LOW = 'n';
			//--- ?????? ?????? --- // CS:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		}

		if('y' == SPI_CLK_HIGH){
			SPI_CLK_HIGH = 'n';
			//--- ?????? ?????? --- // CLK:1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		}else if('y' == SPI_CLK_LOW){
			SPI_CLK_LOW = 'n';
			//--- ?????? ?????? --- // CLK:0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}

		if('y' == SPI_DI_LOAD){
			SPI_DI_LOAD = 'n';
			if((SPI_DI_OUTPUT & SPI_Data_Comp) == SPI_Data_Comp){
				//--- ?????? ?????? --- // DI:1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
			}else{
				//--- ?????? ?????? --- // DI:0
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			}
			SPI_Data_Comp >>= 1;
		}

		if('y' == SPI_DO_CHECK){
			SPI_DO_CHECK = 'n';

			//--- ?????? ?????? ---//
			pb9pin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
			if(pb9pin == GPIO_PIN_SET){
				SPI_DO_INPUT = SPI_DO_INPUT + (0x80 >> SPI_WTF);
				//printf("hi\r\n");
			}
			//printf("hi455\r\n");
			SPI_WTF++;


			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1); // test
		}

		if('y' == SPI_BREAK){
			SPI_BREAK = 'n';
			SPI_on = 0;
			break;
		}
	}

	*SPI_DATA_READ = SPI_DO_INPUT;

	return(0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_13){
		if('n' == pc13flag){
			pc13flag = 'y';
			printf("-----pc13flag = %c-----\r\n",pc13flag);
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

	HAL_StatusTypeDef tim4it;
	HAL_StatusTypeDef uart2state;
	int retvalue;
	//GPIO_PinState pc13pin;
	int spi_read;

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
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

	printf("---93c56c spi(gpio) TEST---\r\n");

	tim4it = HAL_TIM_Base_Start_IT(&htim4);
	if(HAL_OK == tim4it){
		printf("HAL_TIM_Base_Start_IT=%d OK\r\n",tim4it);
	}else{
		printf("HAL_TIM_Base_Start_IT=%d error\r\n",tim4it);
	}

	SPI_on = 0;
	tim4Cnt = 0;

	pc13flag = 'n';

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  uart2state = HAL_UART_Receive(&huart2, urx, 3, 500);
 	  if(HAL_OK == uart2state){
		  if('w' == urx[0]){ // 0x77='w'
			  retvalue = spi_93c56c_EWEN();
			  if(0 == retvalue){
				  //printf("spi_93c56c_EWEN=%d\r\n",retvalue);
			  }else{
				  printf("spi_93c56c_EWEN error\r\n");
			  }
			  HAL_Delay(10);
			  retvalue = spi_93c56c_WRITE(urx[1],urx[2]);
			  if(0 == retvalue){
				  //printf("spi_93c56c_WRITE=%d\r\n",retvalue);
			  }else{
				  printf("spi_93c56c_WRITE error\r\n");
			  }
			  printf("addr=%3d(0x%02x),write data=%3d(0x%02x)\r\n",urx[1],urx[1],urx[2],urx[2]);
		  }else if('r' == urx[0]){ // 0x72='r'
			  retvalue = spi_93c56c_READ(urx[1],&spi_read);
			  if(0 == retvalue){
				  //printf("spi_93c56c_READ=%d\r\n",retvalue);
			  }else{
				  printf("spi_93c56c_READ error\r\n");
			  }
			  printf("addr=%3d(0x%02x),read data=%3d(0x%02x)\r\n",urx[1],urx[1],spi_read,spi_read);
		  }
  	  }

	  HAL_Delay(100);
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 63;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|LD2_Pin|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 LD2_Pin PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|LD2_Pin|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
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
