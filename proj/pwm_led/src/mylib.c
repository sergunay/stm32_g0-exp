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
//
void Delay_Loop(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 1000; j++);
}
