#include "stm32g031xx.h"
#include "mylib.h"

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

// --------------------------------------------------------------------------------
//
void Delay_Loop(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 1000; j++);
}
