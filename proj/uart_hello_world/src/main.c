/**
  ******************************************************************************
  * @file    main.c
  * @author  SE
  * @brief   UART
  *
  * 1. Hardware: 
  *    STM32 Nucleo-32 development board with STM32G031K8 MCU
  * 2. System Clock:
  *    System Clock source  | HSI
  *    SYSCLK(MHz)          | 16
  * 3. USART2 (PA2/PA3) connected to ST-LINK/V2-1 Virtual COM port.
  * 4. PA2.AF1 = USART2_TX, PA3.AF1 = USART2_RX
  ******************************************************************************
  */


#include "stm32g031xx.h"
#include "mylib.h"

int cnt = 0;

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

	// USART2 Enable
	USART2_Enable();
	// USART Config
	USART_Config(USART2, 16000000, 9600, 'O');
	// USART transmit enable
	USART_Tx_En(USART2);

	USART_Print_Int(USART2, 12345678);
	USART_Print(USART2, " \n\r");
	USART_Print_Int(USART2, 123456789);

	while (1)
	{
		for(cnt=0; cnt<10000; cnt++)
		{
			USART_Print_Int(USART2, cnt);
			USART_Print(USART2, " \n\r");
			Delay_Loop(1000);
		}
	}

	return 0;
}