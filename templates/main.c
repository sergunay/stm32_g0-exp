#include "stm32g031xx.h"
#include "mylib.h"

int main(void)
{
	__disable_irq();

	__enable_irq();

	while (1)
	{
	}

	return 0;
}
