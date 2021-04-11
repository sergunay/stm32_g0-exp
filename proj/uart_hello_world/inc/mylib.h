void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Toggle(GPIO_TypeDef *, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);
void USART2_Setup(int, int, char);
void USART2_Write(const char *);
void Delay_Loop(unsigned int);