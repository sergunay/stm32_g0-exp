
// Schematic info:
//   - User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3
//   - User Button (B1) -- PF2, needs Pull Up

#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	int tmp;

	__disable_irq();

	// GPIOC(6) : output
	GPIOC_Config(6, 1);

	// GPIOF(2) : input
	Button_PF2_Config();

	__enable_irq();

	while (1)
	{
		tmp = Button_PF2_In();
		if(tmp == 0)
		{
			GPIOC_Set(6);
		}
		else
		{
			GPIOC_Reset(6);
		}
		
	}

	return 0;
}