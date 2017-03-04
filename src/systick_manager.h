#ifndef SYSTICKMANAGER_H_INCLUDED
#define SYSTICKMANAGER_H_INCLUDED

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"

#define PRESCALER 1000

void systick_init();
void systick_delay(volatile uint32_t ms);
void SysTick_Handler(void);

#endif