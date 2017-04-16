#ifndef DACTESTDEMO_H_INCLUDED
#define DACTESTDEMO_H_INCLUDED

#include <stm32f4xx.h>
#include "common.h"
#include "fs_manager.h"

void dac_init(void);
void play_sample_dac(fsm_sample * sample, uint8_t beat, uint8_t volume);

#endif