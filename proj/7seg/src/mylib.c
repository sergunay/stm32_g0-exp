#include "stm32g031xx.h"
#include "mylib.h"

// --------------------------------------------------------------------------------
// GPIO Functions
// --------------------------------------------------------------------------------

/**
  * @brief  GPIO port clock enable
  * @param  portNum 0:A, 1:B, 2:C, 3:D, 5:F
  * @retval None
  * @example GPIO_Enable(2); // Enable GPIOC port
  *
  */
__INLINE void GPIO_Enable(unsigned int portNum)
{
	RCC->IOPENR |= (1<<portNum);
}

/**
  * @brief  Select GPIO mode
  * @param  PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param  pinNum
  * @param  pinMode 0:IN, 1:OUT, 2:AF, 3:Analog
  * @retval None
  * @example GPIO_Mode(GPIOC, 6, 1);
  *
  */
__INLINE void GPIO_Mode(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int pinMode)
{
	PORT->MODER &= ~(0x3UL << 2*pinNum);
	PORT->MODER |= (pinMode << 2*pinNum);
}

/**
  * @brief  This function selects the alternate function of a GPIO pin.
  * @param  pinNum
  * @param  afNum
  * @retval None
  * @example GPIO_AFSel(GPIOC, 6, 1);
  *
  */

__INLINE void GPIO_AFSel(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int afNum)
{
	PORT->AFR[0] &= ~(0x15UL << 4*pinNum);
	PORT->AFR[0] |= (afNum << 4*pinNum);
}

/**
  * @brief  Delay in loop
  * @param  time
  * @retval None
  * @example Delay_Loop(100);
  *
  */
void Delay_Loop(unsigned int time)
{
	for (unsigned int i = 0; i < time; i++)
		for (volatile unsigned int j = 0; j < 1000; j++);
}

/**
  * @brief  I2C1 clock enable
  * @retval None
  * @example I2C1_Enable(); // Enable I2C1 clk
  *
  */
__INLINE void I2C1_Enable()
{
  // I2C1 clock enable
  RCC->APBENR1 |= RCC_APBENR1_I2C1EN;
}


/**
  * @brief  I2C1 Configuration for 100 MHz
  * @param I2C  I2C1 | I2C2
  * @retval None
  * @example I2C1_Config_100MHz(I2C1); // Config I2C1 for 100 MHz
  *
  */
__INLINE void I2C_Config_100MHz(I2C_TypeDef * I2C)
{
  // Peripheral disable
	I2C->CR1 &= ~I2C_CR1_PE;

	// f_I2CLK = 16 MHz, PRESC=3 => 4MHz: 250ns
	I2C->TIMINGR &= ~I2C_TIMINGR_PRESC_Msk;
	I2C->TIMINGR |= (3 << I2C_TIMINGR_PRESC_Pos);

	// 20 x 250ns = 5us
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLL_Msk; 
	I2C->TIMINGR |= (19 << I2C_TIMINGR_SCLL_Pos);

	// 16 x 250ns = 4us
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLH_Msk; 
	I2C->TIMINGR |= (15 << I2C_TIMINGR_SCLH_Pos);

	// t_{SCL} ~ 10us

	// 2 x 250ns = 500ns
	I2C->TIMINGR &= ~I2C_TIMINGR_SDADEL_Msk; 
	I2C->TIMINGR |= (2 << I2C_TIMINGR_SDADEL_Pos);

	// 5 x 250ns = 1200ns
	I2C->TIMINGR &= ~I2C_TIMINGR_SCLDEL_Msk; 
	I2C->TIMINGR |= (4 << I2C_TIMINGR_SCLDEL_Pos);

	// Peripheral enable
	I2C->CR1 |= I2C_CR1_PE;
}

/**
  * @brief  I2C slave address
  * @param I2C  I2C1 | I2C2
  * @param slave_addr  slave address
  * @param reg_addr
  * @retval rx_data
  * @example I2C_Read_Byte(I2C1, 0x48, 0x0); // I2C read
  *
  */
__INLINE char I2C_Read_Byte(I2C_TypeDef *I2C, char slave_addr, char reg_addr)
{
  char rx_data;
	// Slave address (master mode)
	I2C->CR2 &= ~I2C_CR2_SADD_Msk;
  slave_addr = (slave_addr << 1);
	I2C->CR2 |= (slave_addr << I2C_CR2_SADD_Pos);

  // TX data = reg address
  I2C->TXDR = reg_addr;

  // Transfer direction: write
  I2C->CR2 &= ~I2C_CR2_RD_WRN;

  // Start generation
  I2C->CR2 |= I2C_CR2_START;

  // Wait until TX register is empty
  while(!(I2C->ISR & I2C_ISR_TXE_Msk));

  // Transfer direction: read
  I2C->CR2 |= I2C_CR2_RD_WRN;

  // Start generation
  I2C->CR2 |= I2C_CR2_START;

  // Wait until RX register is not empty
  while(!(I2C->ISR & I2C_ISR_RXNE));

  // RX DATA
  rx_data = I2C->RXDR;

  // Stop generation
  I2C->CR2 |= I2C_CR2_STOP;

  return rx_data;

}