#include "main_manager.h"

//fsm_sample sample;
static uint8_t start_stop;
static uint16_t current_bpm = 120;
static uint8_t current_note = 0;

void main_mgr_init() {
    lcd_set_current_page(MAIN_PAGE);
    lcd_set_bpm(current_bpm);
    lcd_set_note(current_note);
//    fsm_open_sample("frank", &sample);
    start_stop = 0;
}

void main_mgr_next_keydown() {
    if(lcd_get_current_page() == MAIN_PAGE) {
        main_mgr_increment_current_bpm(1);
        lcd_set_bpm(current_bpm);
  //      bpl_start_sample(sample, current_bpm, current_note);
    }
}

void main_mgr_prev_keydown() {
    if(lcd_get_current_page() == MAIN_PAGE) {
        main_mgr_decrement_current_bpm(1);
        lcd_set_bpm(current_bpm);
   //     bpl_start_sample(sample, current_bpm, current_note);
    }
}

void main_mgr_start_stop_keydown() {
    if(start_stop)  {
  //      bpl_stop_sample();
        start_stop = 0;
    } else {
    //    bpl_start_sample(sample, current_bpm, current_note);
        start_stop = 1;
    }
}

void main_mgr_select_note_keydown() {
    switch (current_note) {
        case QUARTER_NOTE:
            current_note = EIGHTH_NOTE;
            break;
        case EIGHTH_NOTE:
            current_note = SiXTEENTH_NOTE;
            break;
        case SiXTEENTH_NOTE:
            current_note = QUARTER_NOTE;
            break;
    }
    lcd_set_note(current_note);
}

void main_mgr_increment_current_bpm(uint8_t incr) {
    if(incr + current_bpm >= MAX_BPM) {
        current_bpm = MAX_BPM;
    } else {
        current_bpm += incr;
    }
}

void main_mgr_decrement_current_bpm(uint8_t decr) {
    if(current_bpm - decr <= MIN_BPM) {
        current_bpm = MIN_BPM;
    } else {
        current_bpm -= decr;
    }
}