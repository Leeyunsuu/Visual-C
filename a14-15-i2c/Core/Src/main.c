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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYDDRAMINCREMENT 0x02
#define LCD_ENTRYDDRAMDECREMENT 0x00
#define LCD_ENTRYDISPLAYSHIFTLEFT 0x03
#define LCD_ENTRYDISPLAYSHIFTRIGHT 0x01

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSORON 0x02
#define LCD_BLINKON 0x01

// flags for cursor / display shift
#define LCD_SHIFTCURSORLEFTDECRESE 0x00
#define LCD_SHIFTCURSORRIGHTINCRESE 0x04
#define LCD_SHIFTALLDISPLAYLEFTCURSORLEFT 0x08
#define LCD_SHIFTALLDISPLAYLEFTCURSORRIGHT 0x0C

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t urx[16], utx[16];
uint16_t i2cAddress = 0x4e;
uint8_t _displayfunction = 0x00;
uint8_t _cursordisplayshift = 0x00;
uint8_t _displaycontrol = 0x00;
uint8_t _displaymode = 0x00;
char clcdData[32];

uint8_t Heart[] = {
  0x00, //B00000,
  0x0a, //B01010,
  0x1f, //B11111,
  0x1f, //B11111,
  0x0e, //B01110,
  0x04, //B00100,
  0x00, //B00000,
  0x00, //B00000
};

uint8_t Bell[] = {
  0x01, //B00100,
  0x01, //B01110,
  0x01, //B01110,
  0x1f, //B01110,
  0x01, //B11111,
  0x01, //B00000,
  0x01, //B00100,
  0x01, //B00000
};

uint8_t Alien[] = {
	0x04, //B11111,
	0x04, //B10101,
	0x04, //B11111,
	0x04, //B11111,
	0x04, //B01110,
	0x04, //B01010,
	0x04, //B11011,
	0x1f, //B00000
};

uint8_t Check[] = {
	0x1f, //B00000,
	0x04, //B00001,
	0x04, //B00011,
	0x04, //B10110,
	0x04, //B11100,
	0x04, //B01000,
	0x04, //B00000,
	0x04, //B00000
};

uint8_t Speaker[] = {
	0x05, //B00001,
	0x05, //B00011,
	0x05, //B01111,
	0x00, //B01111,
	0x05, //B01111,
	0x05, //B00011,
	0x05, //B00001,
	0x05, //B00000
};

uint8_t Sound[] = {
	0x11, //B00001,
	0x11, //B00011,
	0x00, //B00101,
	0x00, //B01001,
	0x11, //B01001,
	0x11, //B01011,
	0x11, //B11011,
	0x11, //B11000
};

uint8_t Skull[] = {
	0x00, //B00000,
	0x00, //B01110,
	0x00, //B10101,
	0xaa, //B11011,
	0x0a, //B01110,
	0xaa, //B01110,
	0x00, //B00000,
	0xa0, //B00000
};

uint8_t Lock[] = {
	0x00, //B01110,
	0x00, //B10001,
	0x0a, //B10001,
	0x0a, //B11111,
	0x0a, //B11011,
	0x0a, //B11011,
	0x00, //B11111,
	0x00, //B00000
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C2_Init(void);
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

//----- character LCD ------------------------------------------------------------------
void lcd_send_cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C;  //[3]backlight=1, [2]en=1, [1]r/w=0, [0]rs=0
	data_t[1] = data_u | 0x08;  //[3]backlight=1, [2]en=0, [1]r/w=0, [0]rs=0
	data_t[2] = data_l | 0x0C;  //[3]backlight=1, [2]en=1, [1]r/w=0, [0]rs=0
	data_t[3] = data_l | 0x08;  //[3]backlight=1, [2]en=0, [1]r/w=0, [0]rs=0
	HAL_I2C_Master_Transmit(&hi2c2, i2cAddress,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //[3]backlight=1, [2]en=1, [1]r/w=0, [0]rs=1
	data_t[1] = data_u | 0x09;  //[3]backlight=1, [2]en=0, [1]r/w=0, [0]rs=1
	data_t[2] = data_l | 0x0D;  //[3]backlight=1, [2]en=1, [1]r/w=0, [0]rs=1
	data_t[3] = data_l | 0x09;  //[3]backlight=1, [2]en=0, [1]r/w=0, [0]rs=1
	HAL_I2C_Master_Transmit(&hi2c2, i2cAddress,(uint8_t *) data_t, 4, 100);
}

void lcd_init(void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd(0x30);
	HAL_Delay(10);  // wait for >4.1ms
	lcd_send_cmd(0x30);
	HAL_Delay(10);  // wait for >100us
	lcd_send_cmd(0x30);
	HAL_Delay(10);
	lcd_send_cmd(0x20);  // 4bit mode
	HAL_Delay(10);

	// dislay initialisation
	_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS; // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	lcd_send_cmd(LCD_FUNCTIONSET | _displayfunction);
	HAL_Delay(10);

	_displaymode = LCD_ENTRYDDRAMINCREMENT; //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	lcd_send_cmd(LCD_ENTRYMODESET | _displaymode);
	HAL_Delay(10);

	_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON; //Display on/off control --> D=1, C=1, B=1
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
	HAL_Delay(10);

	lcd_send_cmd(LCD_CLEARDISPLAY); // clear display
	HAL_Delay(100);
}

void lcd_send_string(char *str)
{
	while (*str) lcd_send_data(*str++);
}

void lcd_clear(void)
{
	lcd_send_cmd(LCD_CLEARDISPLAY);
}

void lcd_home(void)
{
	lcd_send_cmd(LCD_RETURNHOME);
}

void lcd_setCursor(uint8_t row, uint8_t col)
{
	if(((row >= 0) && (row <= 1)) && ((col >= 0) && (col < 16))){
		if(row == 0){
			lcd_send_cmd(LCD_SETDDRAMADDR | (0x00 + col));
		}else if(row == 1){
			lcd_send_cmd(LCD_SETDDRAMADDR | (0x40 + col));
		}
	}
}

void lcd_characterDatainput(void) // a~z data
{
	  //printf("%c\r\n",clcdData[0]);

	  lcd_send_data(clcdData[0]);
	  if('z' == clcdData[0]){
		  clcdData[0] = 'a';
	  }else{
		  clcdData[0] += 1;
	  }
}

void lcd_createSpecialCharacter(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; 				// we only have 8 locations 0-7
	lcd_send_cmd(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		lcd_send_data(charmap[i]);
	}
}

//----- LCD_ENTRYMODESET : inc/dec DDRAM address, shift of entire display
void lcd_incrementDDRAM(void) // cmd : d
{
	_displaymode = LCD_ENTRYDDRAMINCREMENT;	//0x02
	lcd_send_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd_decrementDDRAM(void) // cmd : e
{
	//----- ?????? ??????? -----
	_displaymode = LCD_ENTRYDDRAMDECREMENT;	//0x02
	lcd_send_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd_shiftEntiredisplayLeft(void)  // cmd : f
{
	//----- ?????? ??????? -----
	_displaymode = LCD_ENTRYDISPLAYSHIFTLEFT;	//0x02
	lcd_send_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd_shiftEntiredisplayRight(void) // cmd : g
{
	//----- ?????? ??????? -----
	_displaymode = LCD_ENTRYDISPLAYSHIFTRIGHT;	//0x02
	lcd_send_cmd(LCD_ENTRYMODESET | _displaymode);
}
//----- LCD_ENTRYMODESET : inc/dec DDRAM address, shift of entire display

//----- LCD_DISPLAYCONTROL : display on/off cursor, blink -----
void lcd_display(void) // cmd : m
{
	_displaycontrol |= LCD_DISPLAYON; // 0x04
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_nodisplay(void) // cmd : n
{
	_displaycontrol &= (0xFF ^ LCD_DISPLAYON);
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_cursor(void) // cmd : o
{
	//----- ?????? ??????? -----
	_displaycontrol |= LCD_CURSORON; // 0x04
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_nocursor(void) // cmd : p
{
	//----- ?????? ??????? -----
	_displaycontrol &= (0xFF ^ LCD_CURSORON);
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_blink(void) // cmd : q
{
	//----- ?????? ??????? -----
	_displaycontrol |= LCD_BLINKON; // 0x04
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_noblink(void) // cmd : r
{
	//----- ?????? ??????? -----
	_displaycontrol &= (0xFF ^ LCD_BLINKON);
	lcd_send_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}
//----- LCD_DISPLAYCONTROL : display on/off cursor, blink -----

//----- LCD_CURSORSHIFT : cursor & dispaly shift -----
void lcd_shiftCursorLeft(void) // cmd : s
{
	_cursordisplayshift = LCD_SHIFTCURSORLEFTDECRESE ; // 0x00
	lcd_send_cmd(LCD_CURSORSHIFT | _cursordisplayshift);
}

void lcd_shiftCursorRight(void) // cmd : t
{
	//----- ?????? ??????? -----
	_cursordisplayshift = LCD_SHIFTCURSORRIGHTINCRESE ; // 0x00
	lcd_send_cmd(LCD_CURSORSHIFT | _cursordisplayshift);
}

void lcd_shiftAllDisplayLeft(void) // cmd : u
{
	//----- ?????? ??????? -----
	_cursordisplayshift = LCD_SHIFTALLDISPLAYLEFTCURSORLEFT ; // 0x00
	lcd_send_cmd(LCD_CURSORSHIFT | _cursordisplayshift);
}

void lcd_shiftAllDisplayRight(void) // cmd : v
{
	//----- ?????? ??????? -----
	_cursordisplayshift = LCD_SHIFTALLDISPLAYLEFTCURSORRIGHT ; // 0x00
	lcd_send_cmd(LCD_CURSORSHIFT | _cursordisplayshift);
}
//----- LCD_CURSORSHIFT : cursor & dispaly shift -----

void lcd_address_search(void)
{
	HAL_StatusTypeDef i2cstatus;

	for(uint16_t i = 0; i < 128; i++){
		i2cstatus = HAL_I2C_IsDeviceReady(&hi2c2, i << 1, 1, 10);
		if(HAL_OK == i2cstatus){
		  printf("OK addr=0x%02x\r\n",(i << 1));
		  //break;
		}else{
		  //printf("err addr=0x%02x\r\n",(i << 1));
		}
	}
	printf("lcd_address_search\r\n");
}

//----- character LCD ------------------------------------------------------------------


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	HAL_StatusTypeDef uart2status;

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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  printf("---character LCD(i2c) test---\r\n");

  HAL_Delay(10);
  lcd_init();
  HAL_Delay(20);
  sprintf(clcdData,"lys");
  lcd_send_string(clcdData);
  HAL_Delay(20);
  clcdData[0] = 'a';

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  uart2status = HAL_UART_Receive(&huart2, urx, 1, 300);
	  if(HAL_OK == uart2status){
		  printf("*%c*\r\n",urx[0]);
		  switch(urx[0]){
		  case '1':
			  lcd_address_search();
			  break;
		  case 'c':
			  lcd_clear();
			  break;
		  case 'h':
			  lcd_home();
			  break;
		  case 'i':
			  lcd_init();
			  sprintf(clcdData,"lys");
			  lcd_send_string(clcdData);
			  clcdData[0] = 'a';
			  break;
		  case 'a':
			  lcd_setCursor(0, 0);
			  break;
		  case 'b':
			  lcd_setCursor(1, 0);
			  break;
		  case 'k':
			  lcd_characterDatainput();
			  break;
		  //----- LCD_ENTRYMODESET : inc/dec DDRAM address, shift of entire display
		  case 'd':
			  lcd_incrementDDRAM();
			  break;
		  case 'e':
			  lcd_decrementDDRAM();
			  break;
		  case 'f':
			  lcd_shiftEntiredisplayLeft();
			  break;
		  case 'g':
			  lcd_shiftEntiredisplayRight();
			  break;
		  //----- LCD_ENTRYMODESET : inc/dec DDRAM address, shift of entire display
		  //----- LCD_DISPLAYCONTROL : display on/off cursor, blink -----
		  case 'm':
			  lcd_display();
			  break;
		  case 'n':
			  lcd_nodisplay();
			  break;
		  case 'o':
			  lcd_cursor();
			  break;
		  case 'p':
			  lcd_nocursor();
			  break;
		  case 'q':
			  lcd_blink();
			  break;
		  case 'r':
			  lcd_noblink();
			  break;
		  //----- LCD_DISPLAYCONTROL : display on/off cursor, blink -----
		  //----- LCD_CURSORSHIFT : cursor & dispaly shift -----
		  case 's':
			  lcd_shiftCursorLeft();
			  break;
		  case 't':
			  lcd_shiftCursorRight();
			  break;
		  case 'u':
			  lcd_shiftAllDisplayLeft();
			  break;
		  case 'v':
			  lcd_shiftAllDisplayRight();
			  break;
		  //----- LCD_CURSORSHIFT : cursor & dispaly shift -----
		  //----- special character -----
		  case 'x':
			  lcd_createSpecialCharacter(0, Heart);
			  lcd_createSpecialCharacter(1, Bell);
			  lcd_createSpecialCharacter(2, Alien);
			  lcd_createSpecialCharacter(3, Check);
			  lcd_createSpecialCharacter(4, Speaker);
			  lcd_createSpecialCharacter(5, Sound);
			  lcd_createSpecialCharacter(6, Skull);
			  lcd_createSpecialCharacter(7, Lock);
			  break;
		  case 'y':
			  lcd_setCursor(1, 0); lcd_send_data(0);
			  lcd_setCursor(1, 1); lcd_send_data(1);
			  lcd_setCursor(1, 2); lcd_send_data(2);
			  lcd_setCursor(1, 3); lcd_send_data(3);
			  lcd_setCursor(1, 4); lcd_send_data(4);
			  lcd_setCursor(1, 5); lcd_send_data(5);
			  lcd_setCursor(1, 6); lcd_send_data(6);
			  lcd_setCursor(1, 7); lcd_send_data(7);
			  break;
			  //----- special character -----
		  default:
			  break;
		  }
	  }

	  //pc13pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
   	  //if(GPIO_PIN_RESET == pc13pin){
   	  //}

	  HAL_Delay(100);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


  }
  /*     szUSER CODE END 3 */
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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

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
