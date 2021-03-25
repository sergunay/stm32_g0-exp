#include "stm32g031xx.h"
#include "mylib.h"

int main (void)
{
	GPIOC_Config(6, 1);

	while (1) {
		GPIOC_Toggle(6);
		Delay_Loop(100);
	}
	return 0;
}
