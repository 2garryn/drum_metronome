#ifndef BTNCALLBACK_H_INCLUDED
#define BTNCALLBACK_H_INCLUDED

#include "btn_manager.h"
#include "main_manager.h"

void btn_callback_set_mode_next_key(uint8_t mode);
void btn_callback_set_mode_prev_key(uint8_t mode);
void btn_callback_set_mode_start_stop_key(uint8_t mode);
void btn_callback_set_mode_select_note_key(uint8_t mode);

#endif 