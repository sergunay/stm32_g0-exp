// D1: PB6.AF6 = I2C1_SCL
// D0: PB7.AF6 = I2C1_SDA

#include "stm32g031xx.h"
#include "mylib.h"

char rx_data=0;

int main(void)
{
	__disable_irq();

	// Enable GPIOB port
	GPIO_Enable(1); 
	// Select PB6 mode: AF
	GPIO_Mode(GPIOB, 6, 2);
	// Select PB6 AF: AF6 (I2C_SCL)
	GPIO_AFSel(GPIOB, 6, 6);

	// Select PB7 mode: AF
	GPIO_Mode(GPIOB, 7, 2);
	// Select PB6 AF: AF6 (I2C_SDA)
	GPIO_AFSel(GPIOB, 7, 6);

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

	// I2C initialization: standard mode, 100kHz

	// I2C1 clock enable
	I2C1_Enable();

	// Config I2C1 for 100 MHz
	I2C_Config_100MHz(I2C1); 

	// Number of bytes
	I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk; 
	I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

	I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	I2C1->CR2 &= ~I2C_CR2_RELOAD;

	__enable_irq();

	while (1)
	{
		rx_data = I2C_Read_Byte(I2C1, 0x48, 0x0);

		USART_Print(USART2, "Temperature = ");

		USART_Print_Int(USART2, rx_data);
		USART_Print(USART2, "\r\n");

		Delay_Loop(1000);
		
	}

	return 0;
}
