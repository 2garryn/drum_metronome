#include "fs_manager.h"

/*
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
*/

void test_fsm(void) {
	LOGI("fsm_list_samples", 0);
	LOGI("fasd sm_list_samples", 123);
	LOGE("fsasd asdm_list_samples", 4);

	fsm_sample sample;
	fsm_init();
	fsm_structure(&sample);
	while(1) {
		fsm_list_samples(&sample);
		if(sample.stop == TRUE) {
			break;
		} else {
			LOGI(sample.name, 1);
		}
	}
}

// MUST BE GLOBAL. OTHERWISE DOES NOT WORK
FATFS fat_fs;

uint8_t fsm_init(void) {
	LOGI("fsm_init", 0);
	FRESULT res;
	LOGI("Mounting volume", 0);
	res = f_mount(&fat_fs, "", 1);
	if (res){
		LOGE("Mounting volume", res);
		return RET_ERROR;
	}
	LOGI("Volume mounted", 0);
	return RET_OK;
};

void fsm_structure(fsm_sample *sample) {
	sample->stop = FALSE;
	sample->inited = FALSE;
}

uint8_t fsm_list_samples(fsm_sample *sample) {
	LOGI("fsm_list_samples", 0);
	FRESULT res;
	FILINFO fno;
	DIR dir;
	if(sample->inited == FALSE) {
		res = f_opendir(&(sample->dir), "/samples"); 
		if(res != FR_OK) {
			LOGE("Init list samples", res);
			return RET_ERROR;
		}
		LOGI("List samples inited", 0);
		sample->inited = TRUE;
	}
	res = f_readdir(&(sample->dir), &fno);
	if(res != FR_OK) {
		LOGE("Read dir", res);
		return RET_ERROR;
	}
	LOGI("Read dir success", 0);
	if(fno.fname[0] == 0) {
		sample->stop = TRUE;
		return RET_OK;
	}
	if(fno.fattrib & AM_DIR) {  
		strcpy(sample->name, fno.fname);
		return RET_OK;
	}
}
