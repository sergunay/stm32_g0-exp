#include "stm32g031xx.h"
#include "mylib_stm32g0.h"

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
  * @brief  Toggle GPIO out reg
  * @param  PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param  pinNum
  * @retval None
  * @example GPIO_Toggle(GPIOC, 6);
  *
  */
__INLINE void GPIO_Toggle(GPIO_TypeDef *PORT, unsigned int pinNum)
{
	PORT->ODR ^= (0x1UL << pinNum);
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

// --------------------------------------------------------------------------------
// TIMER Functions
// --------------------------------------------------------------------------------

/**
  * @brief  TIM3 clock enable
  * @retval None
  * @example TIM3_Enable(); // Enable GPIOC port
  *
  */
__INLINE void TIM3_Enable()
{
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
}

/**
  * @brief  This function switches the system clock to PLL
  * @param  arr
  * @retval None
  *
  */

void TIM3_CH1_PWM(uint32_t arr, uint32_t ccr)
{
	TIM3->PSC = 1;
	TIM3->ARR = arr;
	TIM3->CCR1 = ccr;

	// 0110: PWM mode 1 - Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else inactive.
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);

	// Enable CH1
	TIM3->CCER |= TIM_CCER_CC1E;

	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
}

// --------------------------------------------------------------------------------
// PLL Functions
// --------------------------------------------------------------------------------

/**
  * @brief  Set PLL VCO
  * @param  div 4MHz<=f<=16MHz
  * @param  mult [8-86] 64MHz<=f<=344MHz
  * @retval None
  * @example PLL_SetVCO(1, 8);  // 16/1x8 = 128MHz
  *
  */
void PLL_SetVCO(unsigned int div, unsigned int mult)
{
    uint32_t reg32;

    // PLL disable
    RCC->CR &= ~RCC_CR_PLLON;

    // HSI oscillator (16 MHz) clock selected as PLL clock entry
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC_Msk;
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSI << RCC_PLLCFGR_PLLSRC_Pos);

    // Division factor M of the PLL input clock divider
    // PLL input frequency after the /M divider must be between 4 and 16 MHz.
    // PLLM_reg = 0 => PLLM = 1 => 16MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
    RCC->PLLCFGR |= ((div-1) << RCC_PLLCFGR_PLLM_Pos);

    // PLL frequency multiplication factor N
    // The PLL output frequency must be set in the range 64-344 MHz.
    // PLLN must be between [8-86]
    // PLLN = 8 => VCO = 16 x 8 = 128 MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
    RCC->PLLCFGR |= (mult << RCC_PLLCFGR_PLLN_Pos);

    // Main PLL enable
    RCC->CR |= RCC_CR_PLLON;
    // Wait until the output is stable:
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);
}

/**
  * @brief  Enable PLLR out
  * @param  div [2-8] f<=64MHz
  * @retval None
  * @example PLLR_Enable(div);  // 128/2=64MHz
  *
  */
void PLLR_Enable(unsigned int div)
{
    // PLLR must be between [2-8], PLLRCLK max = 64 MHz
    // PLLR_reg = 1 => PLLR = 2 => PLLRCLK = 128/2 = 64 MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
    RCC->PLLCFGR |= (div << RCC_PLLCFGR_PLLR_Pos);

    // Enable PLLR out
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
}



// --------------------------------------------------------------------------------
// Other Functions
// --------------------------------------------------------------------------------

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


/**
  * @brief  Select MCO
  * @param  src 1:SYSCLK, 3:HSI, 4:HSE, 5:PLLR, 6:LSI, 7:LSE
  * @retval None
  * @example MCO_Sel(5);  // MCO=PLLRCLK
  *
  */
void MCO_Sel(unsigned int src)
{
    // Select MCO clock = PLLRCLK
    RCC->CFGR &= ~RCC_CFGR_MCOSEL_Msk;
    RCC->CFGR |= (src << RCC_CFGR_MCOSEL_Pos);
}
















void TIM3_UIF_IRQ(uint32_t arr)
{
  RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
  TIM3->PSC = 1;
	TIM3->ARR = arr;

	// Enable TIM3
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->SR = 0;

	// Start timer counter
  TIM3->CR2 &= 0;

  NVIC_EnableIRQ(TIM3_IRQn);

  // Update interrupt enable
	TIM3->DIER |= TIM_DIER_UIE;

  // Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
}

// --------------------------------------------------------------------------------
// USART Functions
// --------------------------------------------------------------------------------

__INLINE void USART1_Setup(int sys_clk_freq, int uart_baud_rate, char parity)
{
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	RCC->APBENR2 |= RCC_APBENR2_USART1EN;

	USART1->BRR = (uint32_t)(sys_clk_freq/uart_baud_rate);
	//USART2->CR3 = USART_CR3_HDSEL;

	// Transmitter Enable
	USART1->CR1 |= USART_CR1_TE;
	// Receiver Enable
	USART1->CR1 |= USART_CR1_RE;
	// RXNE Interrupt Enable
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	// USART Enable
	USART1->CR1 |= USART_CR1_UE;
	// Parity control enable
	USART1->CR1 |= USART_CR1_PCE;

	switch (parity)
	{
		case 'O':
			// Parity control enable
			USART1->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART1->CR1 |= USART_CR1_PS;
			break;
		case 'E':
			// Parity control enable
			USART1->CR1 |= USART_CR1_PCE;
			// PS=0 => even parity
			USART1->CR1 &= ~USART_CR1_PS;
			break;
		default:
			// Parity control enable
			USART1->CR1 |= USART_CR1_PCE;
			// PS=1 => odd parity
			USART1->CR1 |= USART_CR1_PS;
	}
  NVIC_EnableIRQ(USART1_IRQn);
}

__INLINE void USART1_Write(const char *text)
{
	const char *tx_ptr = text;
	while(*tx_ptr)
	{
		USART1->TDR = *tx_ptr;
		// Wait until Transmit Data Register Empty
		while ((USART1->ISR & USART_ISR_TXE_TXFNF) == 0);
		tx_ptr++;
	}
}

// --------------------------------------------------------------------------------
// ADC Functions
// --------------------------------------------------------------------------------

__INLINE void ADC_Config(unsigned int chNum)    // PA0 = ADC IN 0
{
    uint32_t reg32;
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;
    ADC1->SMPR = ADC_SMPR_SMP1_0;    // 001: 7.5 ADC clock cycles
    ADC1->CHSELR |= (1 << chNum);
    ADC1->CR |= ADC_CR_ADEN;
}

int ADC_Read(void)
{
    ADC1->CR |= ADC_CR_ADSTART;
    while(ADC1->CR & ADC_CR_ADSTART);
    while ((ADC1->ISR & ADC_ISR_EOC) == 0);
    return (int) ADC1->DR;
}

// --------------------------------------------------------------------------------
// OTHER Functions
// --------------------------------------------------------------------------------

void Delay_Loop(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 1000; j++);
}
