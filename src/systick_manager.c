#include "systick_manager.h"

#define N_TIMERS 3

static volatile uint32_t current_delay = 0;
static volatile uint16_t timers_store[] = {0, 0, 0};


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

void systick_set_timer(uint8_t timer, uint16_t time) {
    timers_store[timer] = time;
}

uint8_t systick_get_timer(uint8_t timer) {
    if(timers_store[timer] > 0) { return 1; }
    else { return 0; }
}

void SysTick_Handler(void) {
	if (current_delay > 0x00) {
		current_delay--;
	}
    for(uint16_t i = 0; i < N_TIMERS; i++) {
        if(timers_store[i] > 0) {
            timers_store[i]--;
        }
    }
 }
