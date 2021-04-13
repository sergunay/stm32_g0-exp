#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	int adc_int;
	char adc_str[10];

	__disable_irq();

	// GPIOA Enable
	GPIO_Enable(0);
	// A1: analog
	GPIO_Mode(GPIOA, 1, 3);

	ADC_Config(1);

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

	// Timer count limit
	TIM3->ARR = 100;

	// Enable CH1
	TIM3->CCER |= TIM_CCER_CC1E;

	// Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

	__enable_irq();

	while (1)
	{		
		adc_int = ADC_Read();		
		TIM3->CCR1 = adc_int/100;
	}

	return 0;
}