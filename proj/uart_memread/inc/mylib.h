void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Toggle(GPIO_TypeDef *, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_Speed(GPIO_TypeDef *, unsigned int, unsigned int);
int GPIO_Read_Bit(GPIO_TypeDef *, unsigned int );
void GPIO_Write_Bit(GPIO_TypeDef *, unsigned int, unsigned int);
int GPIO_Read_Bus(GPIO_TypeDef *);
void USART_Tx_En(USART_TypeDef *);
void USART_Config(USART_TypeDef *, int, int, char);
void USART2_Enable(void);
void USART_Print(USART_TypeDef *, const char *);
void USART_Print_Int(USART_TypeDef *USART, int);
void Delay_Loop(unsigned int);