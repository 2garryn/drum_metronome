#include "btn_manager.h"
#include "main_manager.h"

void btn_callback_clicked_0_0() {
    LOGD("Clicked 0 - 0", 0);
    main_mgr_next_keydown();
}
void btn_callback_clicked_0_1() {
    LOGD("Clicked 0 - 1", 0);
    main_mgr_prev_keydown();
}
void btn_callback_clicked_0_2() {
    LOGD("Clicked 0 - 2", 0);
    main_mgr_start_stop_keydown();
}
void btn_callback_clicked_0_3() {
    LOGD("Clicked 0 - 3", 0);
    main_mgr_select_note_keydown();
}
void btn_callback_clicked_1_0() {
    LOGD("Clicked 1 - 0", 0);
}
void btn_callback_clicked_1_1() {
    LOGD("Clicked 1 - 1", 0);
}
void btn_callback_clicked_1_2() {
    LOGD("Clicked 1 - 2", 0);
}
void btn_callback_clicked_1_3() {
    LOGD("Clicked 1 - 3", 0);
}