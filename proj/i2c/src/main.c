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
	RCC->APBENR1 |= RCC_APBENR1_I2C1EN;

	// Peripheral disable
	I2C1->CR1 &= ~I2C_CR1_PE;

	// f_I2CLK = 16 MHz, PRESC=3 => 4MHz: 250ns
	I2C1->TIMINGR &= ~I2C_TIMINGR_PRESC_Msk;
	I2C1->TIMINGR |= (3 << I2C_TIMINGR_PRESC_Pos);

	// 20 x 250ns = 5us
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLL_Msk; 
	I2C1->TIMINGR |= (19 << I2C_TIMINGR_SCLL_Pos);

	// 16 x 250ns = 4us
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLH_Msk; 
	I2C1->TIMINGR |= (15 << I2C_TIMINGR_SCLH_Pos);

	// t_{SCL} ~ 10us

	// 2 x 250ns = 500ns
	I2C1->TIMINGR &= ~I2C_TIMINGR_SDADEL_Msk; 
	I2C1->TIMINGR |= (2 << I2C_TIMINGR_SDADEL_Pos);

	// 5 x 250ns = 1200ns
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLDEL_Msk; 
	I2C1->TIMINGR |= (4 << I2C_TIMINGR_SCLDEL_Pos);

	// Peripheral disable
	I2C1->CR1 |= I2C_CR1_PE;

	// Automatic end mode (master mode)
	I2C1->CR2 |= I2C_CR2_AUTOEND;
	
	// Slave address (master mode)
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk; 
	I2C1->CR2 |= (0x90 << I2C_CR2_SADD_Pos);

	// Number of bytes
	I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk; 
	I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

	// Transfer direction: read
	// I2C1->CR2 |= I2C_CR2_RD_WRN;

	I2C1->CR2 &= ~I2C_CR2_AUTOEND;
	I2C1->CR2 &= ~I2C_CR2_RELOAD;

	__enable_irq();

	while (1)
	{
		// TX data = reg address
		I2C1->TXDR = 0;

		// Transfer direction: write
		I2C1->CR2 &= ~I2C_CR2_RD_WRN;

		// Start generation
		I2C1->CR2 |= I2C_CR2_START;

		while(!(I2C1->ISR & I2C_ISR_TXE_Msk));

		// Transfer direction: read
		I2C1->CR2 |= I2C_CR2_RD_WRN;

		// Start generation
		I2C1->CR2 |= I2C_CR2_START;

		while(!(I2C1->ISR & I2C_ISR_RXNE));

		USART_Print(USART2, "Temperature = ");
		// RX DATA
		rx_data = I2C1->RXDR;
		USART_Print_Int(USART2, rx_data);
		USART_Print(USART2, "\r\n");

		// Stop generation
		I2C1->CR2 |= I2C_CR2_STOP;

		Delay_Loop(1000);
		
	}

	return 0;
}
