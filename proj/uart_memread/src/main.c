/**
  ******************************************************************************
  * @file    main.c
  * @author  SE
  * @brief   Read 10-bit data from memory, send to UART
  *
  * 1. Hardware: 
  *    STM32 Nucleo-32 development board with STM32G031K8 MCU
  * 2. System Clock:
  *    System Clock source  | HSI
  *    SYSCLK(MHz)          | 16
  * 3. USART2 (PA2/PA3) connected to ST-LINK/V2-1 Virtual COM port.
  * 4. Pin configuration:
  *    PA2.AF1  - USART2_TX   PA3.AF1 - USART2_RX
  *    PA0 :IN  - DATA[0];    PA1 :IN - DATA[1];    PA4 :IN - DATA[2];
  *    PA5 :IN  - DATA[3];    PA12:IN - DATA[4];    PA11:IN - DATA[5];
  *    PA6 :IN  - DATA[6];    PA7 :IN - DATA[7];    PA15:IN - DATA[8];
  *    PA10:IN  - DATA[9];    PA9 :OUT- read_en;    PA8 :OUT- read_clk;
  *    PB0 :OUT - adcmem_rst;
  *    PF2 :IN  - User Button (B1) -- needs Pull Up
  *    PC6 :OUT - User LED (LD3)
  * 5. UART configuration:
  *        Baud=9600, parity=odd
  * 6. SPI Configuration:
  *        PB0.AF0=SPI1_NSS  PB3.AF0=SPI1_SCK  PB4.AF0=SPI1_MISO  PB5.AF0=SPI1_MOSI
  *        
  * 7. Algorithm:
  *    1. Poll user button at PF2, LED off, read_en=0
  *    2. If PF2 HIGH, LED on, for loop data_idx in 0 to 4096:
  *       * read_en=1
  *       * read_clk=0; delay;
  *       * read_clk=1; delay;  // create rising edge
  *       * read data;
  *       * convert to bcd;
  *       * send uart
  *    3. Go to 1
  ******************************************************************************
  */


#include "stm32g031xx.h"
#include "mylib.h"

int data_idx = 0;
int adc_data = 0;

char button_pressed = 0;

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

  // INPUT DATA Pins configuration:
  // Set GPIOA(0) INPUT Mode  -- DATA[0]
	GPIO_Mode(GPIOA, 0, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[1]
	GPIO_Mode(GPIOA, 1, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[2]
	GPIO_Mode(GPIOA, 4, 0);
  // Set GPIOA(0) INPUT Mode  -- DATA[3]
	GPIO_Mode(GPIOA, 5, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[4]
	GPIO_Mode(GPIOA, 12, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[5]
	GPIO_Mode(GPIOA, 11, 0);
  // Set GPIOA(0) INPUT Mode  -- DATA[6]
	GPIO_Mode(GPIOA, 6, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[7]
	GPIO_Mode(GPIOA, 7, 0);
  // Set GPIOA(1) INPUT Mode  -- DATA[8]
	GPIO_Mode(GPIOA, 15, 0);
  // Set GPIOA(0) INPUT Mode  -- DATA[9]
	GPIO_Mode(GPIOA, 10, 0);

  // Set GPIOA(8) OUTPUT Mode  -- read_clk
  GPIO_Mode(GPIOA, 8, 1);
  // Set GPIOA(9) OUTPUT Mode  -- read_en
  GPIO_Mode(GPIOA, 9, 1);

	// GPIOF clk enable
	GPIO_Enable(5);

  // GPIOB clock enable
	GPIO_Enable(1);
	// Set GPIOB(0) Out Mode -- adcmem_reset
	GPIO_Mode(GPIOB, 0, 1);

  // Configure user button (User Button (B1))
	// GPIOF(2) : input
	GPIO_Mode(GPIOF, 2, 0);

	// Enable pullup resistor for PF2
	GPIOF->PUPDR &= ~(0x3UL << 4);
	GPIOF->PUPDR |= (0x1UL << 4);

  // Configure User LED
  // GPIOC clk enable
	GPIO_Enable(2);

	// GPIOC(6) : output
	GPIO_Mode(GPIOC, 6, 1);

	USART_Print(USART2, "Start");

  // Send reset signal
  GPIO_Write_Bit(GPIOB, 0, 1);
  Delay_Loop(10);
  GPIO_Write_Bit(GPIOB, 0, 0);

  // PA9, read_en=0
  GPIO_Write_Bit(GPIOA, 9, 0);

  // --------------------------- SPI BEGIN ------------------------//

  // SPI pins
  GPIO_Mode(GPIOB, 0, 2);
  GPIO_Mode(GPIOB, 3, 2);
  GPIO_Mode(GPIOB, 4, 2);
  GPIO_Mode(GPIOB, 5, 2);

  // GPIO speed max 80 MHz
	GPIO_Speed(GPIOB, 0, 3);
  GPIO_Speed(GPIOB, 3, 3);
  GPIO_Speed(GPIOB, 4, 3);
  GPIO_Speed(GPIOB, 5, 3);

  // SPI1 clk enable
	RCC->APBENR2 |= RCC_APBENR2_SPI1EN;

  // Configure the serial clock baud rate
  // 000
  SPI1->CR1 &= ~SPI_CR1_BR_Msk;
  SPI1->CR1 |= SPI_CR1_BR_1; // f_PCLK/8 = 16/8=2MHz
  SPI1->CR1 |= SPI_CR1_BR_0; // f_PCLK/8 = 16/8=2MHz

  // Configure the CPOL and CPHA bits combination

  // Select simplex or half-duplex mode

  // Configure the LSBFIRST bit to define the frame format

  // Configure SSM and SSI

  // Configure the MSTR bit
  SPI1->CR1 |= SPI_CR1_MSTR;

  // Configure the DS[3:0] bits to select the data length for the transfer.
  SPI1->CR2 &= ~SPI_CR2_DS_Msk;
  // 1111: 16-bit
  SPI1->CR2 |= (15 << SPI_CR2_DS_Pos);

  // SPI en
  SPI1->CR1 |= SPI_CR1_SPE;

  // SPI1->CR2 |= SPI_CR2_SSOE;

  SPI1->DR = 0x1234;
  // wait if transmit buffer is not empty
  while(SPI1->SR & SPI_SR_TXE == 0)
  {
  }

// --------------------------- SPI END ------------------------//

  while (1)
	{
    button_pressed = 0;
    while(button_pressed == 0)
    {
      if(GPIO_Read_Bit(GPIOF, 2) == 0)
      {
        button_pressed = 1;
      }
    }

    // PA9, read_en=1 
    GPIO_Write_Bit(GPIOA, 9, 1);

		for(data_idx=0; data_idx<4096; data_idx++)
		{
      // PA8, read_clk=1 (rising edge)
      GPIO_Write_Bit(GPIOA, 8, 1);
      GPIO_Write_Bit(GPIOC, 6, 1);
      // wait for data
      Delay_Loop(6);

      adc_data = GPIO_Read_Bus(GPIOA);
			USART_Print_Int(USART2, adc_data);
      
			USART_Print(USART2, " \n\r");


      // PA8, read_clk=0 (falling edge)
      GPIO_Write_Bit(GPIOA, 8, 0);
      GPIO_Write_Bit(GPIOC, 6, 0);
      
			Delay_Loop(10);
		}
	}

	return 0;
}
