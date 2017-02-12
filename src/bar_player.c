#include "bar_player.h"

#define TRACK_LENGTH 9

uint8_t next_track_position;
uint8_t * track_array;
fsm_sample * current_sample;

uint8_t bpl_start_sample(fsm_sample * sample,  uint16_t bpm, uint8_t * track) {
    uint16_t bpm_interval = (uint16_t) lroundf(60000.0f / bpm);
    next_track_position = 0;
    current_sample = sample;
    track_array = track;
    LOGD("Bpm is set", bpm_interval);
    dac_enable();
    dac_load_file(&(sample->downbeat_fil));
    clt_ms_trigger_clear();
    clt_ms_trigger_interval_set(bpm_interval);
    clt_tim_enable();
    return RET_OK;
}

uint8_t bpl_track_length() {
    return TRACK_LENGTH;
}


void bpl_stop_sample(void) {
    next_track_position = 0;
    track_array = 0;
    clt_tim_disable();
}
uint8_t dac_result = RET_OK;

void bpl_loop(void) {
    if(clt_ms_trigger_is_set()) {
        clt_ms_trigger_clear();
        dac_timer_enable();
    }
    dac_result = dac_loop();
    if(dac_result == RET_ERROR || dac_result == RET_FILE_FINISHED ) {
        dac_timer_disable();
      //  dac_file_reset();
        load_next_file();
    }
}

void load_next_file() {
    uint8_t current_position = next_track_position;
    set_next_track_position();
    load_next_file_do(*(track_array + next_track_position));
    return;
}

void load_next_file_do(uint8_t beat) {
    switch (beat) {
        case DOWNBEAT:
            dac_load_file(&(current_sample->downbeat_fil));
            break;
        case OFFBEAT:
            dac_load_file(&(current_sample->offbeat_fil));
            break;  
        default:
            break;
    }
}

void set_next_track_position() {
    next_track_position++;
    if(next_track_position == TRACK_LENGTH) {
        next_track_position = 0;
        return;
    } 
    if(*(track_array + next_track_position) == NOTHING) {
        next_track_position = 0;
        return;
    }
}

