#ifndef MAINMANAGER_H_INCLUDED
#define MAINMANAGER_H_INCLUDED

#include "lcd_manager.h"
#include "btn_callback.h"
#include "common.h"
#include "ifl_manager.h"
#include "bar_player.h"

void main_mgr_init();
void main_mgr_next_key(uint8_t event);
void main_mgr_prev_key(uint8_t event);
void main_mgr_start_stop_key(uint8_t event);
void main_mgr_select_note_key(uint8_t event);
void main_mgr_select_track_key(uint8_t event);
void main_mgr_increment_current_bpm(uint8_t incr);
void main_mgr_decrement_current_bpm(uint8_t decr);


#endif // MAINMANAGER_H_INCLUDED