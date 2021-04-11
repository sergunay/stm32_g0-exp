/**
  ******************************************************************************
  * @file    main.c
  * @author  SE
  * @brief   UART
  *
  * 1. Hardware: 
  *    STM32G0 Discovery Board.
  * 2. This file configures the system clock as follows:
  *    System Clock source  | HSI
  *    SYSCLK(MHz)          | 16
  * 3. USART2 (PA2/PA3) connected to ST-LINK/V2-1 Virtual COM port.
  * 4. PA2.AF1 = USART2_TX, PA3.AF1 = USART2_RX
  ******************************************************************************
  */


#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	// Config PA2, AF1 USART2_TX

	// GPIOA clock enable
	GPIO_Enable(0);
	// Set GPIOA(2) AF Mode
	GPIO_Mode(GPIOA, 2, 2);
	// Select AF1 for GPIOA(2)
	GPIO_AFSel(GPIOA, 2, 1);

	// Set GPIOA(3) AF Mode
	GPIO_Mode(GPIOA, 3, 2);
	// Select AF1 for GPIOA(2)
	GPIO_AFSel(GPIOA, 3, 1);

	USART2_Setup(16000000, 9600, 'O');

	while (1)
	{
		USART2_Write("Hello world!!!\r\n");
		Delay_Loop(100);
	}

	return 0;
}