#ifndef LCDMAINPAGE_H_INCLUDED
#define LCDMAINPAGE_H_INCLUDED

#include "lcd_5110_imp.h"
#include "lcd_5110_fonts.h"
#include "uart_manager.h"


void lcd_main_page_init_imp();
void lcd_main_page_init();

void lcd_main_page_set_bpm(uint16_t bpm);

void lcd_main_page_set_note(uint8_t note);

void lcd_main_page_set_track(uint8_t track);

#endif