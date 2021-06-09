#include "stm32g031xx.h"
#include "mylib.h"

// --------------------------------------------------------------------------------
// GPIO Functions
// --------------------------------------------------------------------------------

/**
  * @brief  GPIO port clock enable
  * @param  portNum 0:A, 1:B, 2:C, 3:D, 5:F
  * @retval None
  * @example GPIO_Enable(2); // Enable GPIOC port
  *
  */
__INLINE void GPIO_Enable(unsigned int portNum)
{
	RCC->IOPENR |= (1<<portNum);
}

/**
  * @brief  Select GPIO mode
  * @param  PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param  pinNum
  * @param  pinMode 0:IN, 1:OUT, 2:AF
  * @retval None
  * @example GPIO_Mode(GPIOC, 6, 1);
  *
  */
__INLINE void GPIO_Mode(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int pinMode)
{
	PORT->MODER &= ~(0x3UL << 2*pinNum);
	PORT->MODER |= (pinMode << 2*pinNum);
}


/**
  * @brief  This function selects the alternate function of a GPIO pin.
  * @param  pinNum
  * @param  afNum
  * @retval None
  * @example GPIO_AFSel(GPIOC, 6, 1);
  *
  */

__INLINE void GPIO_AFSel(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int afNum)
{
	PORT->AFR[0] &= ~(0x15UL << 4*pinNum);
	PORT->AFR[0] |= (afNum << 4*pinNum);
}

/**
  * @brief  Select GPIO speed
  * @param  PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param  pinNum
  * @param  speed 0:3MHz; 1:15MHz; 2:60 MHz; 3:80 MHz
  * @retval None
  * @example GPIO_Speed(GPIOA, 9, 3);
  *
  */
__INLINE void GPIO_Speed(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int speed)
{
	PORT->OSPEEDR &= ~(0x3UL << 2*pinNum);
	PORT->OSPEEDR |= (speed << 2*pinNum);
}

/**
  * @brief   Read bit from a IDR of a PORT/PIN
  * @param   PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param   pinNum
  * @example GPIO_Read_Bit(GPIOF, 2); // Read button
  * @retval  0, 1
  *
  */
int GPIO_Read_Bit(GPIO_TypeDef *PORT, unsigned int pinNum)
{
	return (PORT->IDR & (1<<pinNum)) >> pinNum;
}


/**
  * @brief   Read bit from a IDR of a PORT/PIN
  * @param   PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param   pinNum
  * @example GPIO_Read_Bit(GPIOF, 2); // Read button
  * @retval  0, 1
  *
  */
int GPIO_Read_Bus(GPIO_TypeDef *PORT)
{
  int data_word=0;

  // +DATA[0] @PA0
  if(PORT->IDR & 1 != 0)
  {
    data_word += 1;
  }
  // +DATA[1] @PA1
  if(PORT->IDR & (1<<1) != 0)
  {
    data_word += 2;
  }
  // +DATA[2] @PA4
  if(PORT->IDR & (1<<4) != 0)
  {
    data_word += 4;
  }
  // +DATA[3] @PA5
  if(PORT->IDR & (1<<5) != 0)
  {
    data_word += 8;
  }
  // +DATA[4] @PA12
  if(PORT->IDR & (1<<12) != 0)
  {
    data_word += 16;
  }
  // +DATA[5] @PA11
  if(PORT->IDR & (1<<11) != 0)
  {
    data_word += 32;
  }
  // +DATA[6] @PA6
  if(PORT->IDR & (1<<6) != 0)
  {
    data_word += 64;
  }
  // +DATA[7] @PA7
  if(PORT->IDR & (1<<7) != 0)
  {
    data_word += 128;
  }
  // +DATA[8] @PA15
  if(PORT->IDR & (1<<15) != 0)
  {
    data_word += 256;
  }
  // +DATA[9] @PA10
  if(PORT->IDR & (1<<10) != 0)
  {
    data_word += 512;
  }

	return data_word;
}

/**
  * @brief   Read bit from a IDR of a PORT/PIN
  * @param   PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param   pinNum
  * @param   val 0, 1
  * @example GPIO_Write_Bit(GPIOC, 6, 1); // Write 1 to GPIOC(6) - User LED
  * @retval  None
  *
  */
void GPIO_Write_Bit(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int val)
{
  PORT->ODR &= ~(1 << pinNum);
	PORT->ODR |= (val << pinNum);
}

/**
  * @brief  Delay in loop
  * @param  time
  * @retval None
  * @example Delay_Loop(100);
  *
  */
void Delay_Loop(unsigned int time)
{
	for (unsigned int i = 0; i < time; i++)
		for (volatile unsigned int j = 0; j < 1000; j++);
}

// --------------------------------------------------------------------------------
// USART Functions
// --------------------------------------------------------------------------------

/**
  * @brief  USART2 Enable
  * @retval None
  * @example USART2_Enable(); // Enable USART clk
  *
  */
__INLINE void USART2_Enable()
{
    // USART2 clk enable
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
    // USART Enable
	USART2->CR1 |= USART_CR1_UE;
}

/**
  * @brief  USART transmit enable
  * @retval None
  * @example USART_Tx_En(USART2); // Enable USART clk
  *
  */
__INLINE void USART_Tx_En(USART_TypeDef *USART)
{
	USART->CR1 |= USART_CR1_TE;
}

/**
  * @brief  USART config
  * @param  sys_clk_freq system clock frequency
  * @param  uart_baud_rate baud rate
  * @param  parity 0:No parity, O:odd, E:even
  * @example USART_Config(USART2, 16000000, 9600, 'O');
  *
  */
__INLINE void USART_Config(USART_TypeDef *USART, int sys_clk_freq, int uart_baud_rate, char parity)
{
    // Baud rate
	USART->BRR = (uint32_t)(sys_clk_freq/uart_baud_rate);

	switch (parity) 
	{
		case 'O':
			// Parity control enable
			USART->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART->CR1 |= USART_CR1_PS;
			break;
		case 'E':
			// Parity control enable
			USART->CR1 |= USART_CR1_PCE;
			// PS=0 => even parity
			USART->CR1 &= ~USART_CR1_PS;
			break;
		default:
			// Parity control enable
			USART->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART->CR1 |= USART_CR1_PS;
	}
}

/**
  * @brief  USART print
  * @param  *text
  * @retval None
  * @example USART_Print(USART2, "Hello World!!!\r\n");
  *
  */
__INLINE void USART_Print(USART_TypeDef *USART, const char *text)
{
	const char *tx_ptr = text;
	while(*tx_ptr) 
	{
		USART->TDR = *tx_ptr;
		// Wait until Transmit Data Register Empty 
		while ((USART->ISR & USART_ISR_TXE_TXFNF) == 0);
		tx_ptr++;
	}
}

/**
  * @brief  USART print integer
  * @param  USART USART1, USART2
  * @param  num number to be printed
  * @retval None
  * @example USART_Print_Int(USART2, 123456789);
  *
  */

void USART_Print_Int(USART_TypeDef *USART, int num)
{
	char numdigits[10];
  char temp;
	char digit_idx=0;
  char i=0;

  if(num == 0)
  {
    numdigits[0] = 48;
    numdigits[1] = 0;
    digit_idx++;
  }

  while(num > 0)
  {
    numdigits[digit_idx] = num%10 + 48;
    num = num/10;
    digit_idx++;
  }
  digit_idx--;
  if(digit_idx>0)
  {
    for(i=0; i<=digit_idx/2; i++)
    {
      temp = numdigits[i];
      numdigits[i] = numdigits[digit_idx-i];
      numdigits[digit_idx-i] = temp;
    }
  }
  numdigits[digit_idx+1] = 0;
  USART_Print(USART, numdigits);
}

// --------------------------------------------------------------------------------
// SPI Functions
// --------------------------------------------------------------------------------

/**
  * @brief  SPI1 Enable
  * @retval None
  * @example SPI1_Enable(); // Enable SPI1 clk
  *
  */
  void SPI1_Enable()
  {
    // SPI1 clk enable
	  RCC->APBENR2 |= RCC_APBENR2_SPI1EN;
  }


/**
  * @brief  SPI config
  * @param  data_nbits data length
  * @param  pclk_div 0: div2, 1: div4, 2: div8
  * @example SPI_Config(SPI1, 16, 2);
  *
  */
__INLINE void SPI_Config(SPI_TypeDef *SPI, int data_nbits, int pclk_div)
{
  // Configure the serial clock baud rate
  SPI->CR1 &= ~SPI_CR1_BR_Msk;
  SPI->CR1 |= (pclk_div<<SPI_CR1_BR_Pos); // f_PCLK/8 = 16/8=2MHz

  // Configure SSM and SSI
  SPI->CR1 |= SPI_CR1_SSM;
  SPI->CR1 |= SPI_CR1_SSI;

  // Configure the MSTR bit
  SPI->CR1 |= SPI_CR1_MSTR;

  // Configure the DS[3:0] bits to select the data length for the transfer.
  SPI->CR2 &= ~SPI_CR2_DS_Msk;
  SPI->CR2 |= ((data_nbits-1) << SPI_CR2_DS_Pos);

  // SPI en
  SPI->CR1 |= SPI_CR1_SPE;
}

/**
  * @brief  SPI transmit data
  * @param  data
  * @retval None
  * @example SPI_Tx_Data(SPI1, 0x1234);
  *
  */
__INLINE void SPI_Tx_Data(SPI_TypeDef *SPI, int data)
{
  SPI1->DR = data;
  // wait if transmit buffer is not empty
  while(SPI1->SR & SPI_SR_TXE == 0)
  {
  }
}