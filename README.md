# STM32-G0 experiments


## Introduction

Experimenting Nucleo-G031K8 board with small projects.
Each project has minimum set of source files.
They don't use HAL or LL libraries, instead use direct
access to the registers with CMSIS library.


## Hardware

STM32 Nucleo-32 development board with STM32G031K8 MCU:
* Arm Cortex-M0+
* Flash memory: 64 Kbyte
* SRAM: 8 Kbyte
* Communication: 2 x I2C + 2 x USART
* Price: ~10 $
* Package: LQFP32

Clock:
* LSE at PC14-PC15: 32.768KHz
* HSI: 16 MHz internal RC clock
* HSE: N/A

* SYSCLK: LSE | LSI | PLLRCLK | HSISYS
  * Max: 64 MHz with PLLRCLK
* PLLQCLK -- TIM1 : max 128 MHz
* PLLPCLK -- ADC  : max 122 MHz


Schematic info:
* User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3
* User Button (B1) -- PF2, needs pullup resistor 

Timers
* TIM1: Advanced Control Timer. 16-bit, 4x Capture/Compare Channels, max 128 MHz.
* TIM2: General Purpose. 32-bit, 4x Capture/Compare Channels, max 64 MHz.
* TIM3: General Purpose. 16-bit, 4x Capture/Compare Channels, max 64 MHz.



## Setup
 
	git clone https://github.com/STMicroelectronics/STM32CubeG0

## Experiments

### Delay loop

User LED is toggled with a for loop delay.

### PWM LED

LED dimming with Timer PWM output.

### Button polling

Polling user button in a loop 

### Button interrupt

Handling interrupt created by the push-button.

### Clock out

MCO (Microcontroller Clock Output).
Max frequency at 64 MHz.

### Frequency measurement

TODO

### USART

Printing text and numbers.

### I2C

Reading temperature from TMP102 with I2C and transferring it to serial port.

### SPI

TODO


