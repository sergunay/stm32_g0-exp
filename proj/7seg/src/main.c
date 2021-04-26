#include "stm32g031xx.h"
#include "mylib.h"

uint16_t GPIOB_Mask = 0x033F;   // 0000_0011_0011_1111
                                //        HG   FE DCBA 

uint16_t GPIOA_Mask = 0x0033;   // 0000_0000_0011_0011
                                //             54   10 

const uint16_t SEG7[]={
	0x003F,  // 0: A+B+C+D+E+F  = 0000_0000_0011_1111
	0x0006,  // 1: B+C          = 0000_0000_0000_0110
	0x011B,  // 2: A+B+D+E+G    = 0000_0001_0001_1011
	0x010F,  // 3: A+B+C+D+G    = 0000_0001_0000_1111
	0x0126,  // 4: B+C+F+G      = 0000_0001_0010_0110
	0x012D,  // 5: A+C+D+F+G    = 0000_0001_0010_1101
	0x013D,  // 6: A+C+F+E+F+G  = 0000_0001_0011_1101
	0x0007,  // 7: A+B+C        = 0000_0000_0000_0111
	0x013F,  // 8:              = 0000_0001_0011_1111
	0x012F   // 9:              = 0000_0001_0010_1111
};

int digit = 0;
unsigned int num=0;

int main(void)
{
	GPIO_Enable(0);
	GPIO_Enable(1);

	GPIO_Mode(GPIOB, 0, 1);
	GPIO_Mode(GPIOB, 1, 1);
	GPIO_Mode(GPIOB, 2, 1);
	GPIO_Mode(GPIOB, 3, 1);
	GPIO_Mode(GPIOB, 4, 1);
	GPIO_Mode(GPIOB, 5, 1);
	GPIO_Mode(GPIOB, 8, 1);
	GPIO_Mode(GPIOB, 9, 1);

	GPIO_Mode(GPIOA, 0, 1);
	GPIO_Mode(GPIOA, 1, 1);
	GPIO_Mode(GPIOA, 4, 1);
	GPIO_Mode(GPIOA, 5, 1);

	// Select PB6 mode: AF
	GPIO_Mode(GPIOB, 6, 2);
	// Select PB6 AF: AF6 (I2C_SCL)
	GPIO_AFSel(GPIOB, 6, 6);

	// Select PB7 mode: AF
	GPIO_Mode(GPIOB, 7, 2);
	// Select PB6 AF: AF6 (I2C_SDA)
	GPIO_AFSel(GPIOB, 7, 6);

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


	while (1)
	{
		num = I2C_Read_Byte(I2C1, 0x48, 0x0);

		// digit 0

        digit = num%10;
        num = num/10;
		
		GPIOB->ODR &= ~GPIOB_Mask;
		GPIOB->ODR |= SEG7[digit];
		GPIOA->ODR &= ~GPIOA_Mask;
		GPIOA->ODR |= 0x0032;
		
		Delay_Loop(10);

		if(num == 0)
			continue;

		// digit 1

		digit = num%10;
        num = num/10;

		GPIOB->ODR &= ~GPIOB_Mask;
		GPIOB->ODR |= SEG7[digit];
		GPIOA->ODR &= ~GPIOA_Mask;
		GPIOA->ODR |= 0x0031;

		Delay_Loop(10);

		if(num == 0)
			continue;

		// digit 2

		digit = num%10;
        num = num/10;

		GPIOB->ODR &= ~GPIOB_Mask;
		GPIOB->ODR |= SEG7[digit];
		GPIOA->ODR &= ~GPIOA_Mask;
		GPIOA->ODR |= 0x0023;

		Delay_Loop(10);

		if(num == 0)
			continue;

		// digit 3

		digit = num%10;
        num = num/10;

		GPIOB->ODR &= ~GPIOB_Mask;
		GPIOB->ODR |= SEG7[digit];
		GPIOA->ODR &= ~GPIOA_Mask;
		GPIOA->ODR |= 0x0013;

		Delay_Loop(10);
			
	}

	return 0;
}
