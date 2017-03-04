#include "systick_manager.h"

static volatile uint32_t current_delay = 0;

void systick_init() {
    SysTick_Config(SystemCoreClock / PRESCALER);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void systick_delay(volatile uint32_t ms) {
    current_delay = ms;
    while(current_delay != 0);
}

void SysTick_Handler(void) {
	if (current_delay > 0x00) {
		current_delay--;
	}
}
