# STM32-G0 experiments

## Hardware

STM32 Nucleo-32 development board with STM32G031K8 MCU:
* Arm Cortex-M0+ at 64 MHz
* Accurate internal high speed clock +/-1% for 0/90°C
* Flash memory: 64 Kbyte
* SRAM: 8 Kbyte
* Communication: 2 x I2C + 2 x USART
* ~10 $

Schematic info:
* User LED (LD3) -- PC6 -- AF1=TIM3_CH1, AF2 = TIM2_CH3

## Setup

	git clone https://github.com/STMicroelectronics/STM32CubeG0

## Experiments

### Delay loop

User LED is toggled with a for loop delay.
