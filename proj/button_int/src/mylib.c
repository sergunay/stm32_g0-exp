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

__INLINE void Button_EXTI_Setup()
{
	// User button PF2
	// External interrupt selection, 5 = PF Pins
	EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI2_Msk;
	EXTI->EXTICR[0] |= (0x05 << EXTI_EXTICR1_EXTI2_Pos);

	NVIC_EnableIRQ(EXTI2_3_IRQn);

	// EXTI Interrupt Mask Register
	EXTI->IMR1 |= (1 << 2);
	// EXTI CPU wakeup with event mask register
	EXTI->EMR1 |= (1 << 2);
	// Rising trigger selection
	EXTI->RTSR1 |= (1 << 2);
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
