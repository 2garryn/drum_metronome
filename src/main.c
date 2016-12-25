#include "fs_manager.h"
#include "uart_manager.h"


void main(void) {
//    test_fs_manager();
    init_USART();
    print_USART("TEST 1111 TEST 2222\n");
    
    
    while(1) {};
}
