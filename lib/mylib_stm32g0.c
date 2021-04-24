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
  * @brief  This function selects the alternate function of a GPIO pin.
  * @param  TIM
  * @param  mode  3: Toggle - out toggles when TIMx_CNT=TIMx_CCR1.
  *               6: PWM mode 1 - H if TIMx_CNT < TIMx_CCR1 else L.
  * @retval None
  * @example TIM_CH1_Mode(TIM3, 6); // TIM3CH1 PWM mode
  *
  */
__INLINE void TIM_CH1_Mode(TIM_TypeDef *TIM, unsigned int mode)
{
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM3->CCMR1 |= (mode << TIM_CCMR1_OC1M_Pos);
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
// I2C Functions
// --------------------------------------------------------------------------------

/**
  * @brief  I2C1 clock enable
  * @retval None
  * @example I2C1_Enable(); // Enable I2C1 clk
  *
  */
__INLINE void I2C1_Enable()
{
  // I2C1 clock enable
  RCC->APBENR1 |= RCC_APBENR1_I2C1EN;
}


/**
  * @brief  I2C1 Configuration for 100 MHz
  * @param I2C  I2C1 | I2C2
  * @retval None
  * @example I2C1_Config_100MHz(I2C1); // Config I2C1 for 100 MHz
  *
  */
__INLINE void I2C_Config_100MHz(I2C_TypeDef * I2C)
{
  // Peripheral disable
	I2C->CR1 &= ~I2C_CR1_PE;

	// f_I2CLK = 16 MHz, PRESC=3 => 4MHz: 250ns
	I2C->TIMINGR &= ~I2C_TIMINGR_PRESC_Msk;
	I2C->TIMINGR |= (3 << I2C_TIMINGR_PRESC_Pos);

	// 20 x 250ns = 5us
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLL_Msk; 
	I2C->TIMINGR |= (19 << I2C_TIMINGR_SCLL_Pos);

	// 16 x 250ns = 4us
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLH_Msk; 
	I2C->TIMINGR |= (15 << I2C_TIMINGR_SCLH_Pos);

	// t_{SCL} ~ 10us

	// 2 x 250ns = 500ns
	I2C->TIMINGR &= ~I2C_TIMINGR_SDADEL_Msk; 
	I2C->TIMINGR |= (2 << I2C_TIMINGR_SDADEL_Pos);

	// 5 x 250ns = 1200ns
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLDEL_Msk; 
	I2C->TIMINGR |= (4 << I2C_TIMINGR_SCLDEL_Pos);

	// Peripheral enable
	I2C->CR1 |= I2C_CR1_PE;
}

/**
  * @brief  I2C slave address
  * @param I2C  I2C1 | I2C2
  * @param slave_addr  slave address
  * @param reg_addr
  * @retval rx_data
  * @example I2C_Read_Byte(I2C1, 0x48, 0x0); // I2C read
  *
  */
__INLINE char I2C_Read_Byte(I2C_TypeDef *I2C, char slave_addr, char reg_addr)
{
  char rx_data;
	// Slave address (master mode)
	I2C->CR2 &= ~I2C_CR2_SADD_Msk;
  slave_addr = (slave_addr << 1);
	I2C->CR2 |= (slave_addr << I2C_CR2_SADD_Pos);

  // TX data = reg address
  I2C->TXDR = reg_addr;

  // Transfer direction: write
  I2C->CR2 &= ~I2C_CR2_RD_WRN;

  // Start generation
  I2C->CR2 |= I2C_CR2_START;

  // Wait until TX register is empty
  while(!(I2C->ISR & I2C_ISR_TXE_Msk));

  // Transfer direction: read
  I2C->CR2 |= I2C_CR2_RD_WRN;

  // Start generation
  I2C->CR2 |= I2C_CR2_START;

  // Wait until RX register is not empty
  while(!(I2C->ISR & I2C_ISR_RXNE));

  // RX DATA
  rx_data = I2C->RXDR;

  // Stop generation
  I2C->CR2 |= I2C_CR2_STOP;

  return rx_data;

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
