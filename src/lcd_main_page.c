#include "lcd_main_page.h"

static uint8_t current_hundreds = 0;
static uint8_t current_tens = 0;
static uint8_t current_integs = 0;

static void render_bpm_num(uint8_t num, uint8_t position);


void lcd_main_page_init_imp() {
    lcd_implementation_init();
}

void lcd_main_page_init() {
    render_bpm_num(00, 0xAA);
    render_bpm_num(00, 0xB8);
    render_bpm_num(00, 0xC6);
}


void lcd_main_page_set_bpm(uint16_t bpm) {
    LOGD("lcd_main_page::lcd_main_page_set_bpm", bpm);
    uint8_t hundreds = bpm / 100;
    uint8_t tens = (bpm % 100) / 10;
    uint8_t integs = (bpm % 100) % 10;
    if(hundreds == 0) {
        current_hundreds = 0;
        render_bpm_num(10, 0xAA);
    } else if(current_hundreds != hundreds) {
        current_hundreds = hundreds;
        render_bpm_num(hundreds, 0xAA);
    }    
    if(current_tens != tens) {
        current_tens = tens;
        render_bpm_num(tens, 0xB8);
    }
    if(current_integs != integs) {
        current_integs = integs;
        render_bpm_num(integs, 0xC6);
    }
}

void lcd_main_page_set_note(uint8_t note) {

}

static void render_bpm_num(uint8_t num, uint8_t position) {
    uint8_t i = 0;
    for(uint8_t y = 0x40; y <= 0x42; y++) {
        write_command(y);
        write_command(position);
        for(uint8_t j = 0; j < 12; j++) {
            write_data(get_bpm_main_font(num, i));
            i++;
        }
    }    
}

