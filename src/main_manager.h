#ifndef MAINMANAGER_H_INCLUDED
#define MAINMANAGER_H_INCLUDED

#include "lcd_manager.h"
#include "common.h"

void main_mgr_init();
void main_mgr_next_keydown();
void main_mgr_prev_keydown();
void main_mgr_start_stop_keydown();
void main_mgr_select_note_keydown();
void main_mgr_increment_current_bpm(uint8_t incr);
void main_mgr_decrement_current_bpm(uint8_t decr);


#endif // MAINMANAGER_H_INCLUDED