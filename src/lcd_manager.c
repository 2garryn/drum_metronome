#include "lcd_manager.h"

static uint8_t current_page;

void lcd_init() {
    lcd_main_page_init_imp();
}

uint8_t lcd_get_current_page() {
    return current_page;
}


// setter interfaces
void lcd_set_current_page(uint8_t page) {
    if(page == MAIN_PAGE) {
        lcd_main_page_init();
    }
    current_page = page;
}

void lcd_set_bpm(uint16_t bpm) {
    LOGD("lcd_manager::lcd_set_bpm", bpm);
    if(current_page == MAIN_PAGE) {
        lcd_main_page_set_bpm(bpm);
    }
}
void lcd_set_note(uint8_t note) {
    if(current_page == MAIN_PAGE) {
        lcd_main_page_set_note(note);
    }
}

