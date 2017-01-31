#include "bar_player.h"



uint8_t bpl_start_sample(fsm_sample * sample,  uint16_t bpm) {
    uint16_t bpm_interval = (uint16_t) lroundf(60000.0f / bpm);
    LOGD("Bpm is set", bpm_interval);
    dac_enable();
    dac_load_file(&(sample->downbeat_fil));;
    clt_ms_trigger_clear();
    clt_ms_trigger_set(bpm_interval);
    clt_tim_enable();
    return RET_OK;
}


void bpl_stop_sample(void) {
    clt_tim_disable();
}
uint8_t dac_result = RET_OK;

void bpl_loop(void) {
    if(clt_ms_trigger_is_set()) {
        clt_ms_trigger_clear();
        dac_timer_enable();
    }
    dac_result = dac_loop();
    if(dac_result == RET_ERROR || dac_result == RET_FILE_FINISHED ) {
        dac_timer_disable();
    }
}
