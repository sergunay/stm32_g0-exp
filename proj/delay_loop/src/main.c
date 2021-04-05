#include "stm32g031xx.h"
#include "mylib.h"

// User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3

int main (void)
{
	// Enable GPIOC port
	GPIO_Enable(2);
	// Configure PC6 as out
	GPIO_Mode(GPIOC, 6, 1);

	while (1) {
		// Toggle PC6
		GPIO_Toggle(GPIOC, 6);
		// Delay in loop
		Delay_Loop(100);
	}
	return 0;
}
