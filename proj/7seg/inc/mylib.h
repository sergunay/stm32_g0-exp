void GPIO_Enable(unsigned int);
void GPIO_Mode(GPIO_TypeDef *, unsigned int, unsigned int);
void GPIO_AFSel(GPIO_TypeDef *, unsigned int, unsigned int);
void Delay_Loop(unsigned int);
void I2C1_Enable(void);
void I2C_Config_100MHz(I2C_TypeDef *);
char I2C_Read_Byte(I2C_TypeDef *, char, char);