

uint8_t bpl_start_sample(fsm_sample * sample, uint8_t plan[], uint16_t bpm) {
    uint16_t bpm_interval = (uint16_t) lroundf(60000.0f / tempo);




    clt_ms_trigger_set(bpm_interval);
    clt_tim_enable();
    clt_ms_trigger_clear();
}


void bpl_stop_sample(void) {
    clt_tim_disable();
}

void bpl_loop(void) {
    if(clt_ms_trigger_is_set()) {
        next_beat();
        clt_ms_trigger_clear();
    }
}
