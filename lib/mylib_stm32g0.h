void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Toggle(GPIO_TypeDef *, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);


void TIM3_Enable();

void Delay_Loop(unsigned int);



void GPIOA_OSpeed(unsigned int, unsigned int);
void GPIOC_Set(unsigned int);
void GPIOC_Reset(unsigned int);
void Button_PF2_Config();
int  Button_PF2_In();
void TIM3_CH1_PWM(uint32_t, uint32_t);
void TIM3_UIF_IRQ(uint32_t);
void RCC_SysClk_PLL_HSI(unsigned int, unsigned int, unsigned int);
void RCC_SysClk_HSI(void);
void USART1_Write(const char *);
void USART1_Setup(int, int, char);
void ADC_Config(unsigned int chNum);
int ADC_Read(void);
void MCO_PLLRCLK_64MHz();