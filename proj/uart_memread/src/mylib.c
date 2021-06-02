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

  data_word = PORT->IDR & 3;
  // +DATA[2] @PA4
  data_word += 4*(PORT->IDR & (1<<4));
  // +DATA[3] @PA5
  data_word += 8*(PORT->IDR & (1<<5));
  // +DATA[4] @PA12
  data_word += 16*(PORT->IDR & (1<<12));
  // +DATA[5] @PA11
  data_word += 32*(PORT->IDR & (1<<11));
  // +DATA[6] @PA6
  data_word += 64*(PORT->IDR & (1<<6));
  // +DATA[7] @PA7
  data_word += 128*(PORT->IDR & (1<<7));
  // +DATA[8] @PA15
  data_word += 256*(PORT->IDR & (1<<15));
  // +DATA[9] @PA10
  data_word += 512*(PORT->IDR & (1<<15));

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
