#ifndef BTNMANAGER_H_INCLUDED
#define BTNMANAGER_H_INCLUDED

#include <stm32f4xx.h>
#include "uart_manager.h"
#include "common.h"

// Enable buttons on port 1
#define BTN_PORT_0_ENABLED 1

// EXTERNAL 
void btn_init(void);


// INTERNAL
void btn_pins_init(void);
void btn_init_port_pins(GPIO_TypeDef* GpioPort, uint32_t RCClocking, uint32_t pins);
void btn_enable(void);
void btn_disable(void);
void btn_loop(void);

void btn_callback_clicked_0_0();
void btn_callback_clicked_0_1();
void btn_callback_clicked_0_2();
void btn_callback_clicked_0_3();

void btn_callback_clicked_1_0();
void btn_callback_clicked_2_1();
void btn_callback_clicked_3_2();
void btn_callback_clicked_4_3();

#ifdef BTN_PORT_0_ENABLED
#define BTN_PORT_0 GPIOC
#define BTN_PORT_0_RCC RCC_AHB1Periph_GPIOC
#define BTN_PORT_0_PIN_0 GPIO_Pin_6
#define BTN_PORT_0_PIN_1 GPIO_Pin_7
#define BTN_PORT_0_PIN_2 GPIO_Pin_8
#define BTN_PORT_0_PIN_3 GPIO_Pin_9
#define BTN_PORT_0_PINS BTN_PORT_0_PIN_0 | BTN_PORT_0_PIN_1 | BTN_PORT_0_PIN_2 | BTN_PORT_0_PIN_3
#endif

#ifdef BTN_PORT_1_ENABLED
#define BTN_PORT_1 GPIOD
#define BTN_PORT_1_RCC RCC_AHB1Periph_GPIOD
#define BTN_PORT_1_PIN_0 GPIO_Pin_0
#define BTN_PORT_1_PIN_1 GPIO_Pin_1
#define BTN_PORT_1_PIN_2 GPIO_Pin_2
#define BTN_PORT_1_PIN_3 GPIO_Pin_3
#define BTN_PORT_1_PINS BTN_PORT_1_PIN_0 | BTN_PORT_1_PIN_1 | BTN_PORT_1_PIN_2 | BTN_PORT_1_PIN_3
#endif





#endif /* BTNMANAGER_H_INCLUDED */
