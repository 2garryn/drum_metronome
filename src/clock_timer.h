#ifndef CLOCKTIMER_H_INCLUDED
#define CLOCKTIMER_H_INCLUDED

#include <stm32f4xx.h>

void clt_init(void);
void clt_tim_enable(void);
void clt_tim_disable(void);
uint8_t clt_flag_is_set(void);
void clt_flag_reset(void);

#endif /* CLOCKTIMER_H_INCLUDED */