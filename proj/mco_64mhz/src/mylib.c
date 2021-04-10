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