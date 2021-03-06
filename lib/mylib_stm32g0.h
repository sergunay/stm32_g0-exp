void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Toggle(GPIO_TypeDef *, unsigned int);
int  GPIO_Read_Bit(GPIO_TypeDef *, unsigned int);
void GPIO_Write_Bit(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Speed(GPIO_TypeDef *, unsigned int, unsigned int);

void PLL_SetVCO(unsigned int, unsigned int);
void PLLR_Enable(unsigned int);
void MCO_Sel(unsigned int);

void USART_Tx_En(USART_TypeDef *);
void USART_Config(USART_TypeDef *, int, int, char);
void USART2_Enable(void);
void USART_Print(USART_TypeDef *, const char *);

void I2C1_Enable(void);
void I2C_Config_100MHz(I2C_TypeDef *);
char I2C_Read_Byte(I2C_TypeDef *, char, char);

void TIM3_Enable();
void TIM_CH1_Mode(TIM_TypeDef *, unsigned int);

int ADC_Read(void);
void ADC_Config(unsigned int);

void Delay_Loop(unsigned int);



void TIM3_CH1_PWM(uint32_t, uint32_t);
void TIM3_UIF_IRQ(uint32_t);
void RCC_SysClk_PLL_HSI(unsigned int, unsigned int, unsigned int);
void RCC_SysClk_HSI(void);
void USART1_Write(const char *);
void USART1_Setup(int, int, char);
void ADC_Config(unsigned int chNum);
int ADC_Read(void);
void MCO_PLLRCLK_64MHz();