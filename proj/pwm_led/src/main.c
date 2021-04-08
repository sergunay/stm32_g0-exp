
// Schematic info:
//   - User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3


#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	// GPIOC clock enable
	GPIO_Enable(2);

	// Set PC6 AF Mode
	GPIO_Mode(GPIOC, 6, 2);

	// Set AF Mode 1 = TIM3_CH1
	GPIO_AFSel(GPIOC, 6, 1);

	// Enable TIM3 clock
	TIM3_Enable(); 

	// TIM3CH1 PWM mode
	TIM_CH1_Mode(TIM3, 6); 

	TIM3->PSC = 1;
	TIM3->ARR = 100;
	TIM3->CCR1 = 60;

	// Enable CH1
	TIM3->CCER |= TIM_CCER_CC1E;

	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

	while (1)
	{
		TIM3->CCR1 = 60;
		Delay_Loop(1000);
		TIM3->CCR1 = 10;
		Delay_Loop(1000);
	}

	return 0;
}
