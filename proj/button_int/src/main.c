#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	__disable_irq();

	// GPIOC(6) : output
	GPIOC_Config(6, 1);

	// GPIOF(2) : input
	Button_PF2_Config();

	Button_EXTI_Setup();

	__enable_irq();

	while (1)
	{
		//GPIOC->ODR &= ~(0x1UL << 6);
	}

	return 0;
}