#include "stm32g031xx.h"
#include "mylib.h"

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
  * @param  pinMode 0:IN, 1:OUT, 2:AF, 3:Analog
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
  USART->CR1 |= USART_CR1_RE;
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