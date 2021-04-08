
// Schematic info:
//   - User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3
//   - User Button (B1) -- PF2, needs Pull Up

#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	int tmp;

	__disable_irq();

	// GPIOC clk enable
	GPIO_Enable(2);

	// GPIOC(6) : output
	GPIO_Mode(GPIOC, 6, 1);

	// GPIOF clk enable
	GPIO_Enable(5);

	// GPIOF(2) : input
	GPIO_Mode(GPIOF, 2, 0);

	// Enable pullup resistor for PF2
	GPIOF->PUPDR &= ~(0x3UL << 4);
	GPIOF->PUPDR |= (0x1UL << 4);

	__enable_irq();

	while (1)
	{
		// Read button();
		tmp = GPIO_Read_Bit(GPIOF, 2); 

		if(tmp == 0)
		{
			// Write 1 to GPIOC(6) - User LED
			GPIO_Write_Bit(GPIOC, 6, 1);
		}
		else
		{
			// Write 0 to GPIOC(6) - User LED
			GPIO_Write_Bit(GPIOC, 6, 0);
		}
	}
	return 0;
}