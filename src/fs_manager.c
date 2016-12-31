#include "fs_manager.h"


int test_fs_manager(void) {
	FATFS FatFs;
	FIL fil;
	FRESULT res;
	
	unsigned char buffer[20];

	print_UART("Mounting volume...");
	res = f_mount(&FatFs, "", 1); // mount the drive
	if (res)
	{
		print_UART("error occured!\n");
		while(1);
	}
	print_UART("success!\n");
	
	print_UART("Opening file: \"test.txt\"...");
	res = f_open(&fil, "test.txt", FA_OPEN_EXISTING | FA_WRITE | FA_READ); // open existing file in read and write mode
	if (res)
	{
		print_UART( "error occured!\n");
		while(1);
	}
	print_UART( "success!\n");
	
	f_gets(buffer, 20, &fil);
	
	print_UART( "I read: \"");
	print_UART(buffer);
	print_UART( "\"\n");
	
	//f_puts(buffer, &fil);
	
	f_close(&fil); // close the file
	f_mount(0, "", 1); // unmount the drive
    
    return 0;
}