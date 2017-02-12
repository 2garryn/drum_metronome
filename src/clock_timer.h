#ifndef CLOCKTIMER_H_INCLUDED
#define CLOCKTIMER_H_INCLUDED

#include <stm32f4xx.h>
#include "uart_manager.h"
#include "common.h"

void clt_init(void);
void clt_tim_enable(void);
void clt_tim_disable(void);

void clt_ms_trigger_interval_set(uint16_t ms);
void clt_ms_trigger_clear(void);
uint8_t clt_ms_trigger_is_set(void);

#endif /* CLOCKTIMER_H_INCLUDED */