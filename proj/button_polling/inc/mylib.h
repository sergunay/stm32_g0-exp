void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
int  GPIO_Read_Bit(GPIO_TypeDef *, unsigned int);
void GPIO_Write_Bit(GPIO_TypeDef *, unsigned int, unsigned int);


void GPIOC_Set(unsigned int);
void GPIOC_Reset(unsigned int);
void Button_Setup();
void Button_PF2_Config();
int  Button_PF2_In();