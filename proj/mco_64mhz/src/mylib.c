#include "stm32g031xx.h"
#include "mylib.h"

/**
  * @brief  This function configures the selected port/pin
  * @param  PORT     GPIOA, GPIOB, GPIOC, GPIOF
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void GPIOA_Config(GPIO_TypeDef * PORT, unsigned int pinNum, unsigned int pinMode)
{
	// Enable the peripheral clock of GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Select output mode
	PORT->MODER &= ~(0x3UL << 2*pinNum);
	PORT->MODER |= (pinMode << 2*pinNum);
}

/**
  * @brief  This function configures the speed of the selected GPIOA pin.
  * @param  pinNum
  * @param  speed  0 = 3MHz, 1 = 15MHz, 2 = 60MHz, 3 = 80MHz
  * @retval None
  *
  */

__INLINE void GPIOA_OSpeed(unsigned int pinNum, unsigned int speed)
{
  // C=10 pF, 2.7V ≤ VDDIO1 ≤ 3.6V =>
  //   0: 3 MHz; 1: 15 MHz; 2:60 MHz; 3: 80 MHz

	GPIOA->OSPEEDR &= ~(0x3UL << 2*pinNum);
	GPIOA->OSPEEDR |= (speed << 2*pinNum);
}

/**
  * @brief  This function switches the system clock to PLL
  * @retval None
  *
  */

void MCO_PLLRCLK_64MHz()
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
    RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLM_Pos);

    // PLL frequency multiplication factor N
    // The PLL output frequency must be set in the range 64-344 MHz.
    // PLLN must be between [8-86]
    // PLLN = 8 => VCO = 16 x 8 = 128 MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
    RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLN_Pos);

    // PLLR must be between [2-8], PLLRCLK max = 64 MHz
    // PLLR_reg = 1 => PLLR = 2 => PLLRCLK = 128/2 = 64 MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
    RCC->PLLCFGR |= (1 << RCC_PLLCFGR_PLLR_Pos);

    // Main PLL enable
    RCC->CR |= RCC_CR_PLLON;
    // Wait until the output is stable:
    // while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // Select MCO clock = PLLRCLK
    RCC->CFGR &= ~RCC_CFGR_MCOSEL_Msk;
    RCC->CFGR |= (5 << RCC_CFGR_MCOSEL_Pos);
}