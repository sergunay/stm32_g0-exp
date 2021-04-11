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

// --------------------------------------------------------------------------------
// USART Functions
// --------------------------------------------------------------------------------

/**
  * @brief  USART2 Setup
  * @param  sys_clk_freq system clock frequency
  * @param  uart_baud_rate baud rate
  * @param  parity 0:No parity, O:odd, E:even
  * @example USART2_Setup(16000000, 9600, 'O');
  *
  */
__INLINE void USART2_Setup(int sys_clk_freq, int uart_baud_rate, char parity)
{
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;

	// RXNE Interrupt Enable
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
    // Baud rate
	USART2->BRR = (uint32_t)(sys_clk_freq/uart_baud_rate);
	// Transmitter Enable
	USART2->CR1 |= USART_CR1_TE;
	// Receiver Enable
	USART2->CR1 |= USART_CR1_RE;
    
    // USART Enable
	USART2->CR1 |= USART_CR1_UE;

	switch (parity) 
	{
		case 'O':
			// Parity control enable
			USART2->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART2->CR1 |= USART_CR1_PS;
			break;
		case 'E':
			// Parity control enable
			USART2->CR1 |= USART_CR1_PCE;
			// PS=0 => even parity
			USART2->CR1 &= ~USART_CR1_PS;
			break;
		default:
			// Parity control enable
			USART2->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART2->CR1 |= USART_CR1_PS;
	}


}

/**
  * @brief  USART write
  * @param  *text
  * @retval None
  * @example USART2_Write("Hello World!!!\r\n");
  *
  */
__INLINE void USART2_Write(const char *text)
{
	const char *tx_ptr = text;
	while(*tx_ptr) 
	{
		USART2->TDR = *tx_ptr;
		// Wait until Transmit Data Register Empty 
		while ((USART2->ISR & USART_ISR_TXE_TXFNF) == 0);
		tx_ptr++;
	}
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
