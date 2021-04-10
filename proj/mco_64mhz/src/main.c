
// PA9.AF0 = MCO

#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	__disable_irq();

	// Enable GPIOA
	GPIO_Enable(0);

	// PA9 mode = AF
	GPIO_Mode(GPIOA, 9, 2);

	// GPIO speed max 80 MHz
	GPIO_Speed(GPIOA, 9, 3);

	// Set PLL VCO: 16/1x8 = 128MHz
	PLL_SetVCO(1, 8);  

	// Enable PLLR, 128/2=64MHz
	PLLR_Enable(2);  

	// Select MCO=PLLRCLK
	MCO_Sel(5);  

	__enable_irq();

	while (1)
	{
	}

	return 0;
}