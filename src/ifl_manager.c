#include "ifl_manager.h"

static struct sample_header * id2sample_header_offset(uint8_t id);
// we start from SECTOR 6 (RM0090 page 75)
#define INTERNAL_DATA_OFFSET  0x08040000
typedef struct sample_header {
    uint32_t downb_start;
    uint32_t downb_size;
    uint32_t offb_start;
    uint32_t offb_size;
} sample_header;

uint8_t ifl_samples_count() {
    return *(uint8_t *)(INTERNAL_DATA_OFFSET);
}

void ifl_id2name(uint8_t id, char * buffer) {
    uint32_t name_offset = INTERNAL_DATA_OFFSET + sizeof(uint8_t) + ((sizeof(char) * 9) * id);
  //  buffer[8] = '\0';
    memcpy(buffer, (uint8_t *)name_offset, 9);
    buffer[8] = '\0';
}

void ifl_downbeat_pos(uint8_t id, uint32_t * offset, uint32_t * size) {
    struct sample_header * smp = id2sample_header_offset(id);
    *offset = INTERNAL_DATA_OFFSET + smp->downb_start;
    *size   = smp->downb_size;
}

void ifl_offbeat_pos(uint8_t id, uint32_t * offset, uint32_t * size) {
    struct sample_header * smp = id2sample_header_offset(id);
    *offset = INTERNAL_DATA_OFFSET + smp->offb_start;
    *size   = smp->offb_size;
}


struct sample_header * id2sample_header_offset(uint8_t id) {
    uint32_t header_offset = INTERNAL_DATA_OFFSET + sizeof(uint8_t) + ((sizeof(char) * 9) * 10) + (sizeof(struct sample_header) * id);
    return (struct sample_header *)header_offset;
}