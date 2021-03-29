#include "stm32g031xx.h"
#include "mylib.h"

/**
  * @brief  This function handles SysTick Handler.
  * @retval None
  */
void SysTick_Handler(void)
{
}

void TIM2_IRQHandler(void)
{
}


void TIM3_IRQHandler(void)
{
}

void ADC1_IRQHandler(void)
{
}

void USART1_IRQHandler(void)
{
}

void USART2_IRQHandler(void)
{
}

void EXTI0_1_IRQHandler(void)
{
}


void EXTI2_3_IRQHandler(void)
{
	GPIOC_Toggle(6);
}


void EXTI4_15_IRQHandler(void)
{
}


void I2C1_IRQHandler(void)
{
}


void I2C2_IRQHandler(void)
{
}


void SPI1_IRQHandler(void)
{
}


void SPI2_IRQHandler(void)
{
}
