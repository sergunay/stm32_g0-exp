#include "stm32g031xx.h"
#include "mylib.h"

// --------------------------------------------------------------------------------
//
//


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

	// Select mode
	GPIOC->MODER &= ~(0x3UL << 2*pinNum);
	GPIOC->MODER |= (pinMode << 2*pinNum);
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

/**
  * @brief  This function toggles GPIOCx pin.
  * @param  pinNum
  * @retval None
  *
  */
__INLINE void GPIOC_Set(unsigned int pinNum)
{
	GPIOC->ODR |= (0x1UL << pinNum);
}


/**
  * @brief  This function toggles GPIOCx pin.
  * @param  pinNum
  * @retval None
  *
  */
__INLINE void GPIOC_Reset(unsigned int pinNum)
{
	GPIOC->ODR &= ~(0x1UL << pinNum);
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

	// Select mode
	GPIOF->MODER &= ~(0x3UL << 2*pinNum);
	GPIOF->MODER |= (pinMode << 2*pinNum);
}


/**
  * @brief  This function configures the selected GPIOB pin.
  * @param  pinNum
  * @param  pinMode  0 = IN, 1 = OUT, 2 = AF
  * @retval None
  *
  */

__INLINE void Button_PF2_Config()
{
	GPIOF_Config(2, 0);

	GPIOF->PUPDR &= ~(0x3UL << 4);
	GPIOF->PUPDR |= (0x1UL << 4);
}

int Button_PF2_In()
{
	return (GPIOF->IDR & (1<<2)) >> 2;
}

__INLINE void Button_Setup()
{
	// User button PF2
	// External interrupt selection
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
