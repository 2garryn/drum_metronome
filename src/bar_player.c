#include "bar_player.h"

static uint16_t n_notes_counter;
static fsm_sample * current_sample;
static uint8_t given_track;
static uint8_t current_track_counter;
static uint8_t given_pattern;
static uint8_t current_pattern;

static uint16_t calculate_internval(uint16_t bpm, uint8_t note_pattern);
static void play_sample();
static uint8_t is_play_subnote();
static uint8_t get_offbeat_volume();
static uint8_t get_downbeat_volume();
static uint8_t get_subnote_volume();

uint8_t bpl_start_sample(fsm_sample * sample,  uint16_t bpm, uint8_t track, uint8_t note_pattern, uint16_t n_notes) {
    uint16_t bpm_interval = calculate_interval(bpm, note_pattern);
    n_notes_counter = n_notes;
    current_track_counter = given_track = track;
    given_pattern = current_pattern = note_pattern;
    clt_ms_trigger_clear();
    clt_ms_trigger_interval_set(bpm_interval);
    clt_tim_enable();
    return RET_OK;
}


uint16_t calculate_internval(uint16_t bpm, uint8_t note_pattern) {
    if(note_pattern & SiXTEENTH__) {
        return (uint16_t) lroundf(60000.0f / (bpm * 4));
    } else if(note_pattern & EIGHTH__) {
        return (uint16_t) lroundf(60000.0f / (bpm * 2));
    } else if(note_pattern & QUARTER__) {
        return (uint16_t) lroundf(60000.0f / bpm);
    }
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
        if(!given_track) {
            play_sample_dac(current_sample, DOWNBEAT, get_downbeat_volume());
        } else if(given_track == 1) {
            play_sample_dac(current_sample, OFFBEAT, get_offbeat_volume());
        } else if(!current_track_counter) {
            play_sample_dac(current_sample, DOWNBEAT, get_downbeat_volume());
            current_track_counter = given_track - 1;
        } else {
            play_sample_dac(current_sample, OFFBEAT, get_offbeat_volume());
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
        play_sample_dac(current_sample, OFFBEAT, get_subnote_volume());
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

void play_sample_dac(fsm_sample sample, uint8_t beat, uint8_t volume) {
    return;
}