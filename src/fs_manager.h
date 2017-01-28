// File system manager
#ifndef FSMANAGER_H_INCLUDED
#define FSMANAGER_H_INCLUDED

#include "ff.h"
#include "uart_manager.h"
#include "common.h"

typedef struct fsm_sample {
    char name[13];
    DIR dir;
    BOOL stop;
    BOOL inited;
} fsm_sample;

uint8_t fsm_init(void);
uint8_t fsm_list_samples(fsm_sample *sample);
void fsm_structure(fsm_sample *sample);

void test_fsm(void);

#endif /* FSMANAGER_H_INCLUDED */