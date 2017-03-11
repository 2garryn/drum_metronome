#ifndef LCD5110IMPLEMENTATION_H_INCLUDED
#define LCD5110IMPLEMENTATION_H_INCLUDED

#include "systick_manager.h"

#define RCC_GPIO RCC_AHB1Periph_GPIOE

#define GPIO_PORT GPIOE

#define CS_PIN GPIO_Pin_7

#define RST_PIN GPIO_Pin_8

#define DC_PIN GPIO_Pin_9

#define DIN_PIN GPIO_Pin_10

#define SCK_PIN GPIO_Pin_11

#define LED_PIN GPIO_Pin_12

#define COMMAND 0

#define DATA 1

//void lcd_demo();
void lcd_implementation_init();
void write_command(uint8_t command);
void write_data(uint8_t data);

#endif /* LCD5110IMPLEMENTATION_H_INCLUDED */