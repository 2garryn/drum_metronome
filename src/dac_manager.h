#ifndef DACMANAGER_H_INCLUDED
#define DACMANAGER_H_INCLUDED

#include <stm32f4xx.h>
#include "uart_manager.h"
#include "fs_manager.h"
/*

void test_dac_manager1(void);
void test_dac_manager(void);
*/

void dac_init(void);
void dac_enable(void);
void dac_disable(void);
uint8_t dac_load_file(FIL * fil);
uint8_t dac_loop(void);
uint8_t dac_file_reset();

#endif /* DACMANAGER_H_INCLUDED */