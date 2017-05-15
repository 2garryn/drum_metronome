// Internal flash manager
#ifndef IFLANAGER_H_INCLUDED
#define IFLANAGER_H_INCLUDED

#include <stdint.h>
#include <string.h>
#include "common.h"

uint8_t ifl_samples_count();
void ifl_id2name(uint8_t id, char * buffer);
void ifl_downbeat_pos(uint8_t id, uint32_t * offset, uint32_t * size);
void ifl_offbeat_pos(uint8_t id, uint32_t * offset, uint32_t * size);


#endif /* IFLANAGER_H_INCLUDED */