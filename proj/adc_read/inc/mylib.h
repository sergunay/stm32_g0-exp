void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);

void TIM3_Enable();
void TIM_CH1_Mode(TIM_TypeDef *, unsigned int);

int ADC_Read(void);
void ADC_Config(unsigned int);

