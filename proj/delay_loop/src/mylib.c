#include "stm32g031xx.h"
#include "mylib.h"

// --------------------------------------------------------------------------------
// GPIO Functions
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

__INLINE void GPIOC_Toggle(unsigned int pinNum)
{
	GPIOC->ODR ^= (0x1UL << pinNum);
}



// --------------------------------------------------------------------------------

void Delay_Loop(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 1000; j++);
}

