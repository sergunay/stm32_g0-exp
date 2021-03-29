#include "stm32g031xx.h"
#include "mylib_stm32g0.h"

// --------------------------------------------------------------------------------
// GPIO Functions
// --------------------------------------------------------------------------------

/**
  * @brief  This function configures the selected GPIOA pin.
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void GPIOA_Config(unsigned int pinNum, unsigned int pinMode)
{
	// Enable the peripheral clock of GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Select output mode
	GPIOA->MODER &= ~(0x3UL << 2*pinNum);
	GPIOA->MODER |= (pinMode << 2*pinNum);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function configures the selected GPIOB pin.
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void GPIOB_Config(unsigned int pinNum, unsigned int pinMode)
{
	// Enable the peripheral clock of GPIOB
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

	// Select output mode
	GPIOB->MODER &= ~(0x3UL << 2*pinNum);
	GPIOB->MODER |= (pinMode << 2*pinNum);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function configures the selected GPIOC pin.
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void GPIOC_Config(unsigned int pinNum, unsigned int pinMode)
{
	// Enable the peripheral clock of GPIOC
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Select output mode 0
	GPIOC->MODER &= ~(0x3UL << 2*pinNum);
	GPIOC->MODER |= (pinMode << 2*pinNum);
}




// --------------------------------------------------------------------------------

/**
  * @brief  This function configures the selected GPIOB pin.
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void GPIOF_Config(unsigned int pinNum, unsigned int pinMode)
{
	// Enable the peripheral clock of GPIOB
	RCC->IOPENR |= RCC_IOPENR_GPIOFEN;

	// Select output mode
	GPIOF->MODER &= ~(0x3UL << 2*pinNum);
	GPIOF->MODER |= (pinMode << 2*pinNum);
}



// --------------------------------------------------------------------------------

/**
  * @brief  This function selects the alternate function of a GPIOB pin.
  * @param  pinNum
  * @param  afNum
  * @retval None
  *
  */

__INLINE void GPIOB_AFSel(unsigned int pinNum, unsigned int afNum)
{
	GPIOB->AFR[0] &= ~(0x15UL << 4*pinNum);
	GPIOB->AFR[0] |= (afNum << 4*pinNum);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function selects the alternate function of a GPIOC pin.
  * @param  pinNum
  * @param  afNum
  * @retval None
  *
  */

__INLINE void GPIOC_AFSel(unsigned int pinNum, unsigned int afNum)
{
	GPIOC->AFR[0] &= ~(0x15UL << 4*pinNum);
	GPIOC->AFR[0] |= (afNum << 4*pinNum);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function toggles GPIOBx pin.
  * @param  pinNum
  * @retval None
  *
  */
__INLINE void GPIOB_Toggle(unsigned int pinNum)
{
	GPIOB->ODR ^= (0x1UL << pinNum);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function toggles GPIOCx pin.
  * @param  pinNum
  * @retval None
  *
  */
__INLINE void GPIOC_Toggle(unsigned int pinNum)
{
	GPIOC->ODR ^= (0x1UL << pinNum);
}


// --------------------------------------------------------------------------------
// SYSCLK Functions
// --------------------------------------------------------------------------------

/**
  * @brief  This configures RCC to set HSI as system clock.
  * @retval None
  *
  */
__INLINE void RCC_SysClk_HSI(void)
{
    uint32_t reg32;

    // Select system clock HSI
    RCC->CFGR &= ~RCC_CFGR_SW_Msk;
    RCC->CFGR |= (RCC_CFGR_SWS_HSI << RCC_CFGR_SW_Pos);

    // HSI oscillator on
    // HSI frequency : 16 MHz
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);
}

// --------------------------------------------------------------------------------

/**
  * @brief  This function switches the system clock to PLL
  * @param  pllPreDiv PLL Division M (4-16 MHz)
  * @param  pllMult PLL multiplication N (16-48 MHz)
  * @param  sysClkDiv Division R
  * @retval None
  *
  */

void RCC_SysClk_PLL_HSI(unsigned int pllPreDiv, unsigned int pllMult, unsigned int sysClkDiv)
{
    uint32_t reg32;

    // HSI oscillator (16 MHz) clock selected as PLL clock entry
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC_Msk;
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSI << RCC_PLLCFGR_PLLSRC_Pos);

    // Division factor M of the PLL input clock divider
    // PLL input frequency after the /M divider must be between 2.66 and 16 MHz.
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
    RCC->PLLCFGR |= ((pllPreDiv-1) << RCC_PLLCFGR_PLLM_Pos);

    // PLL frequency multiplication factor N
    // The PLL output frequency must be set in the range 64-344 MHz.
    // PLLN must be between [8-86]
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
    RCC->PLLCFGR |= (pllMult << RCC_PLLCFGR_PLLN_Pos);

    // SysClk must be max. 64 MHz
    // PLLR must be between [2-8]
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
    RCC->PLLCFGR |= ((sysClkDiv-1) << RCC_PLLCFGR_PLLR_Pos);

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // Main PLL enable
    RCC->CR |= RCC_CR_PLLON;
    // Wait until the output is stable:
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    // Select system clock: PLL
    RCC->CFGR &= ~RCC_CFGR_SW_Msk;
    RCC->CFGR |= (2 << RCC_CFGR_SW_Pos);

    // Wait until the PLL is switched on
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

}

// --------------------------------------------------------------------------------
// TIMER Functions
// --------------------------------------------------------------------------------

/**
  * @brief  This function switches the system clock to PLL
  * @param  arr
  * @retval None
  *
  */

void TIM3_CH1_PWM(uint32_t arr, uint32_t ccr)
{
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;

	TIM3->PSC = 1;
	TIM3->ARR = arr;
	TIM3->CCR1 = ccr;

	// 0110: PWM mode 1 - Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else inactive.
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);

	TIM3->CCMR1 |= TIM_CCMR1_OC1CE;

	// Enable CH1
	TIM3->CCER |= TIM_CCER_CC1E;

	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
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
