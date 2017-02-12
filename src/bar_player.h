#ifndef BARPLAYER_H_INCLUDED
#define BARPLAYER_H_INCLUDED

#include "fs_manager.h"
#include "common.h"
#include "dac_manager.h"
#include <math.h>

uint8_t bpl_start_sample(fsm_sample * sample,  uint16_t bpm, uint8_t * track);
void bpl_stop_sample(void);
void bpl_loop(void);
uint8_t bpl_track_length();


void set_next_track_position();

#endif