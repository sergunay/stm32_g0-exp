#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	__disable_irq();

	GPIOA_Config(9, 2);
	GPIOA_OSpeed(9, 3);
	MCO_PLLRCLK_64MHz();

	__enable_irq();

	while (1)
	{
	}

	return 0;
}