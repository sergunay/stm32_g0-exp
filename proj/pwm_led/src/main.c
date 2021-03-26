
// Schematic info:
//   - User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3


#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	// Set PC6 AF Mode
	GPIOC_Config(6, 2);

	// Set AF Mode 1 = TIM3_CH1
	GPIOC_AFSel(6, 1);

	while (1)
	{
		TIM3_CH1_PWM(100, 10);
		Delay_Loop(1000);
		TIM3_CH1_PWM(100, 60);
		Delay_Loop(1000);
	}

	return 0;
}