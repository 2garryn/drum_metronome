#include "bar_player.h"

static uint16_t n_notes_counter;
static uint8_t given_track;
static uint8_t current_track_counter;
static uint8_t given_pattern;
static uint8_t current_pattern;
static uint8_t current_sample_id;

static uint16_t calculate_interval(uint16_t bpm, uint8_t note_pattern);
static void play_sample();
static uint8_t is_play_subnote();
static uint8_t get_offbeat_volume();
static uint8_t get_downbeat_volume();
static uint8_t get_subnote_volume();



/*
bpm: tempo,
track: track number: 1 - 9,
note_pattern: actual note
*/
uint8_t bpl_start_sample(uint8_t sample_id,  uint16_t bpm, uint8_t track, uint8_t note_pattern, uint16_t n_notes) {
    uint16_t bpm_interval = calculate_interval(bpm, note_pattern);
    LOGD("Given BPM: ", bpm);
    LOGD("Current bpm interval: ", bpm_interval);
    n_notes_counter = n_notes;
    current_track_counter = 0;
    given_track = track;
    given_pattern = note_pattern;
    current_pattern = 1;
    current_sample_id = sample_id;
    clt_ms_trigger_clear();
    clt_ms_trigger_interval_set(bpm_interval);
    clt_tim_enable();
    return RET_OK;
}


void bpl_stop_sample() {
    clt_tim_disable();
    clt_ms_trigger_clear();
}

uint16_t calculate_interval(uint16_t bpm, uint8_t note_pattern) {
    if(note_pattern & SiXTEENTH__) {
        return (uint16_t) lroundf(60000.0f / (bpm * 4));
    } else if(note_pattern & EIGHTH__) {
        return (uint16_t) lroundf(60000.0f / (bpm * 2));
    } else if(note_pattern & QUARTER__) {
        return (uint16_t) lroundf(60000.0f / bpm);
    }
    return 60000.0f / 60;
}

void bpl_loop(void) {
    if(clt_ms_trigger_is_set()) {
        clt_ms_trigger_clear();
        play_sample();
    };
    dac_loop();
}

void play_sample() {
    if(!is_play_subnote()) {
        if(given_track == 0) {
            dac_play_sample(current_sample_id, DOWNBEAT, get_downbeat_volume());
        } else if(given_track == 1) {
            dac_play_sample(current_sample_id, OFFBEAT, get_offbeat_volume());
        } else if(current_track_counter == 0) {
            dac_play_sample(current_sample_id, DOWNBEAT, get_downbeat_volume());
            current_track_counter = given_track - 1;
        } else {
            dac_play_sample(current_sample_id, OFFBEAT, get_offbeat_volume());
            current_track_counter--;
        }
    }
}

uint8_t is_play_subnote() {
    if(current_pattern == 1) {
        current_pattern = given_pattern;
        return FALSE;
    };

    if(current_pattern & 1) {
        dac_play_sample(current_sample_id, OFFBEAT, get_subnote_volume());
    };
    current_pattern = current_pattern >> 1;
    return TRUE;
};

uint8_t get_offbeat_volume() {
    return 5;
};

uint8_t get_downbeat_volume() {
    return 5;
};

uint8_t get_subnote_volume() {
    return 3;
};
