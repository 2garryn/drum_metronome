#include "btn_callback.h"

void btn_callback_clicked_0_0(uint8_t event) {
    LOGD("Clicked 0 - 0", event);
    main_mgr_next_key(event);
}
void btn_callback_clicked_0_1(uint8_t event) {
    LOGD("Clicked 0 - 1", event);
    main_mgr_prev_key(event);
}
void btn_callback_clicked_0_2(uint8_t event) {
    LOGD("Clicked 0 - 2", event);
    main_mgr_start_stop_key(event);
}
void btn_callback_clicked_0_3(uint8_t event) {
    LOGD("Clicked 0 - 3", event);
    main_mgr_select_note_key(event);
}
void btn_callback_clicked_1_0(uint8_t event) {
    LOGD("Clicked 1 - 0", event);
}
void btn_callback_clicked_1_1(uint8_t event) {
    LOGD("Clicked 1 - 1", event);
}
void btn_callback_clicked_1_2(uint8_t event) {
    LOGD("Clicked 1 - 2", event);
}
void btn_callback_clicked_1_3(uint8_t event) {
    LOGD("Clicked 1 - 3", event);
}

void btn_callback_set_mode_next_key(uint8_t mode) {
    btn_set_button_mode(BTN_PORT_0, BTN_PORT_0_PIN_0, mode);
}

void btn_callback_set_mode_prev_key(uint8_t mode) {
    btn_set_button_mode(BTN_PORT_0, BTN_PORT_0_PIN_1, mode);
}

void btn_callback_set_mode_start_stop_key(uint8_t mode) {
    btn_set_button_mode(BTN_PORT_0, BTN_PORT_0_PIN_2, mode);
}

void btn_callback_set_mode_select_note_key(uint8_t mode) {
    btn_set_button_mode(BTN_PORT_0, BTN_PORT_0_PIN_3, mode);
}