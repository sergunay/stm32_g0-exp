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
//
void Delay_Loop(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 1000; j++);
}
