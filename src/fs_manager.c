#include "fs_manager.h"


int test_fs_manager(void) {
    FATFS FatFs;
    FIL fil;
    FRESULT res;
    const unsigned char buffer[6];
    res = f_mount(&FatFs, "", 1); // mount the drive
	res = f_open(&fil, "hello.txt", FA_OPEN_EXISTING | FA_WRITE | FA_READ); // open existing file in read and write mode
    f_gets(buffer, 6, &fil);
    	
    f_puts(buffer, &fil);
    	
    f_close(&fil); // close the file
    f_mount(0, "", 1); // unmount the drive
    
    return 0;
}