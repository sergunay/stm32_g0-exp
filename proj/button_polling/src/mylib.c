#include "stm32g031xx.h"
#include "mylib.h"

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
  * @param  pinMode 0:IN, 1:OUT, 2:AF
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
  * @brief   Read bit from a IDR of a PORT/PIN
  * @param   PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param   pinNum
  * @param   val 0, 1
  * @example GPIO_Write_Bit(GPIOC, 6, 1); // Write 1 to GPIOC(6) - User LED
  * @retval  None
  *
  */
void GPIO_Write_Bit(GPIO_TypeDef *PORT, unsigned int pinNum, unsigned int val)
{
  PORT->ODR &= ~(1 << pinNum);
	PORT->ODR |= (val << pinNum);
}

/**
  * @brief   Read bit from a IDR of a PORT/PIN
  * @param   PORT GPIOA, GPIOB, GPIOC, GPIOD, GPIOF
  * @param   pinNum
  * @example GPIO_Read_Bit(GPIOF, 2); // Read button
  * @retval  0, 1
  *
  */
int GPIO_Read_Bit(GPIO_TypeDef *PORT, unsigned int pinNum)
{
	return (PORT->IDR & (1<<pinNum)) >> pinNum;
}