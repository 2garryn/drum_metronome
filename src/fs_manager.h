// File system manager
#ifndef FSMANAGER_H_INCLUDED
#define FSMANAGER_H_INCLUDED

#include "ff.h"
#include "uart_manager.h"
#include "common.h"
#include <string.h>

typedef struct fsm_sample {
    char name[13];
    FIL downbeat_fil;
    FIL offbeat_fil;
} fsm_sample;

typedef struct fsm_sample_iter {
    DIR dir;
    BOOL stop;
    BOOL inited;
} fsm_sample_iter;

uint8_t fsm_init(void);

uint8_t fsm_samples_iter_continue(fsm_sample_iter * iter, char * name);
void fsm_samples_iter_init(fsm_sample_iter * iter);

uint8_t fsm_open_sample(char * sample_name, fsm_sample * sample);
uint8_t fsm_close_sample(fsm_sample * sample);

uint8_t open_sample_files(fsm_sample *sample);
uint8_t close_sample_files(fsm_sample *sample);


void test_fsm(void);

#endif /* FSMANAGER_H_INCLUDED */