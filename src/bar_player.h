#ifndef BARPLAYER_H_INCLUDED
#define BARPLAYER_H_INCLUDED

#include "common.h"
#include "clock_timer.h"
#include "dac_manager.h"
#include <math.h>

#define QUARTER__ 0x01
#define EIGHTH__ 0x02
#define SiXTEENTH__ 0x08

uint8_t bpl_start_sample(uint8_t sample_id,  uint16_t bpm, uint8_t track, uint8_t note_pattern, uint16_t n_notes);
void bpl_stop_sample();
void bpl_loop(void);

#endif