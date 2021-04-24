#include "stm32g031xx.h"
#include "mylib.h"

uint16_t GPIOB_Mask = 0x033F;   // 0000_0011_0011_1111
                                //        HG   FE DCBA 

uint16_t GPIOA_Mask = 0x000F;   // 0000_0000_0011_0011
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

	GPIOA->ODR &= ~GPIOA_Mask;
    GPIOA->ODR |= 0x0032;
	

	while (1)
	{
		GPIOB->ODR &= ~GPIOB_Mask;
    	GPIOB->ODR |= SEG7[digit];
		Delay_Loop(500);
		if(digit<9)
		{
			digit++;
		}
		else 
		{
			digit = 0;
		}
	}

	return 0;
}
