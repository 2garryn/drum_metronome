#ifndef DACMANAGER_H_INCLUDED
#define DACMANAGER_H_INCLUDED

#include <stm32f4xx.h>
#include "uart_manager.h"
#include "ifl_manager.h"

void dac_init(void);
void dac_enable(void);
void dac_disable(void);
void dac_play_sample(uint8_t id, uint8_t beat, uint8_t volume);

void dac_loop(void);

#endif /* DACMANAGER_H_INCLUDED */