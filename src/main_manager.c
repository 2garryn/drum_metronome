#include "main_manager.h"

//fsm_sample sample;
static void main_mgr_main_page_next_key(uint8_t event);
static void main_mgr_main_page_prev_key(uint8_t event);


static uint8_t start_stop = 0;
static uint16_t current_bpm = 100;
static uint8_t current_note = QUARTER_NOTE;

void main_mgr_init() {
    lcd_set_current_page(MAIN_PAGE);
    lcd_set_bpm(current_bpm);
    lcd_set_note(current_note);
//    fsm_open_sample("frank", &sample);
    btn_callback_set_mode_next_key(MULTICLICK_MODE);
    btn_callback_set_mode_prev_key(MULTICLICK_MODE);
    btn_callback_set_mode_start_stop_key(SIMPLE_MODE);
    btn_callback_set_mode_select_note_key(SIMPLE_MODE);
    start_stop = 0;
    btn_timer_enable();
}

void main_mgr_next_key(uint8_t event) {
    if(lcd_get_current_page() == MAIN_PAGE) {
        main_mgr_main_page_next_key(event);
        return;
    };
}

void main_mgr_prev_key(uint8_t event) {
    if(lcd_get_current_page() == MAIN_PAGE) {
        main_mgr_main_page_prev_key(event);
        return;
    };

}

void main_mgr_start_stop_key(uint8_t event) {
    if(event == KEY_DOWN) {
        if(start_stop)  {
            start_stop = 0;
            stop_play();
        } else {
            start_stop = 1;
            start_play();    
        }
    }
}

void main_mgr_select_note_key(uint8_t event) {
    if(event == KEY_DOWN) {
        stop_play();
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
        LOGD("Current note: ", current_note);
        lcd_set_note(current_note);
    }
    if(event == KEY_UP_CLICK) {
        start_play();
        return;
    }
}

void main_mgr_main_page_next_key(uint8_t event) {
    if(event == KEY_DOWN) {
        stop_play();
    }
    if(event == KEY_UP_CLICK) {
        start_play();
        return;
    }
    main_mgr_increment_current_bpm(1);
    lcd_set_bpm(current_bpm);
  
}

void main_mgr_main_page_prev_key(uint8_t event) {
    if(event == KEY_DOWN) {
        stop_play();
    }
    if(event == KEY_UP_CLICK) {
        start_play();
        return;
    }
    main_mgr_decrement_current_bpm(1);
    lcd_set_bpm(current_bpm);
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

void start_play() {
    if(start_stop) {
        bpl_start_sample(0, current_bpm, 4, current_note, 0);
    }
}

void stop_play() {
    bpl_stop_sample();
}