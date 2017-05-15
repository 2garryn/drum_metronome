#ifndef LCDMANAGER_H_INCLUDED
#define LCDMANAGER_H_INCLUDED

#include "lcd_main_page.h"
#include "uart_manager.h"

#define MAIN_PAGE 0

void lcd_set_current_page(uint8_t page);

void lcd_set_bpm(uint16_t bpm);

void lcd_set_note(uint8_t note);

void lcd_set_track(uint8_t track);

uint8_t lcd_get_current_page();

void lcd_init();

#endif