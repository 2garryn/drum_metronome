#include "btn_manager.h"
#include "main_manager.h"

void btn_callback_clicked_0_0(uint8_t event) {
    LOGD("Clicked 0 - 0", event);
 //   main_mgr_next_keydown();
}
void btn_callback_clicked_0_1(uint8_t event) {
    LOGD("Clicked 0 - 1", event);
//    main_mgr_prev_keydown();
}
void btn_callback_clicked_0_2(uint8_t event) {
    LOGD("Clicked 0 - 2", event);
  //  main_mgr_start_stop_keydown();
}
void btn_callback_clicked_0_3(uint8_t event) {
    LOGD("Clicked 0 - 3", event);
  //  main_mgr_select_note_keydown();
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