#include "fs_manager.h"

void test_fsm(void){
	fsm_sample_iter iter;
	fsm_sample sample;
	char nname[10];
	char db[10];
	char ob[10];

	fsm_init();
        FIL fil;       /* File object */
    char line[82]; /* Line buffer */
    FRESULT fr;    /* FatFs return code */
    UINT cnt;

  //  fsm_sample sample;
    uint16_t buf[512];
    uint16_t cnt1;

    fsm_open_sample("sine1", &sample);
    fsm_seek_raw_wav(&(sample.downbeat_fil));
  //  fsm_read_file(&(sample.downbeat_fil), buf, 256);
    f_read(&(sample.downbeat_fil), buf, 1024, &cnt1); 
    for(uint16_t i = 0; i < 512; i++) {
        buf[i] = buf[i] + 32768;
        LOGD("Sample: ", buf[i]);
     //   buffer[i] & 0xFFF0;
     //     buffer[i] = 32768;
    }


/*
    fr = f_open(&fil, "test.txt", FA_OPEN_EXISTING | FA_READ);
    if (fr) {
        LOGD("opnefile error", fr);
        return;
    }
    LOGD("Print file", 0);
    f_read(&fil, line, 10, &cnt);
    LOGD(line, 0);
 */ 

    
//	fsm_samples_iter_init(&iter);
//	fsm_samples_iter_init(&iter);
 //   fsm_samples_iter_continue(&iter, nname);
//	while(1) {
//		fsm_samples_iter_continue(&iter, nname);
//		if(iter.stop == TRUE) {
//			break;
//		} else {
//			LOGI(nname, 1);
//		}
//	}
    /*
	fsm_open_sample("wav1", &sample);
	f_gets(ob, 10, &(sample.offbeat_fil));
	f_gets(db, 10, &(sample.downbeat_fil));
	LOGI("Downbeat ->", 0);
	LOGI(db, 0);
	LOGI("Offbeat ->", 0);
	LOGI(ob, 0);
*/
//	fsm_close_sample(&sample);
  
}


// MUST BE GLOBAL. OTHERWISE DOES NOT WORK
FATFS fat_fs;

uint8_t fsm_init(void) {
	LOGD("fsm_init", 0);
	FRESULT res;
	LOGD("fsm_init: Mounting volume", 0);
	res = f_mount(&fat_fs, "", 1);
    LOGD("fsm_init: fs_type", fat_fs.fs_type);
    LOGD("fsm_init: drv", fat_fs.drv);
    LOGD("fsm_init: n_fats", fat_fs.n_fats);
	if (res){
		LOGE("fsm_init: Mounting volume error", res);
		return RET_ERROR;
	}
	LOGD("Volume mounted", 0);
	return RET_OK;
};


void fsm_samples_iter_init(fsm_sample_iter * iter) {
	iter->stop = FALSE;
	iter->inited = FALSE;
}

uint8_t fsm_samples_iter_continue(fsm_sample_iter * iter, char * name) {
	LOGD("fsm_samples_iter_continue", 0);
	FRESULT res;
	FILINFO fno;
	if(iter->inited == FALSE) {
		res = f_opendir(&(iter->dir), "/samples"); 
		if(res != FR_OK) {
			LOGE("Init list samples", res);
			return RET_ERROR;
		}
		LOGD("List samples inited", 0);
		iter->inited = TRUE;
	}
	res = f_readdir(&(iter->dir), &fno);
	if(res != FR_OK) {
		LOGE("Read dir", res);
		return RET_ERROR;
	}
	LOGD("Read dir success", 0);
	if(fno.fname[0] == 0) {
		iter->stop = TRUE;
		return RET_OK;
	}
	if(fno.fattrib & AM_DIR) {  
		strcpy(name, fno.fname);
		return RET_OK;
	}
	return RET_NODIR;
}

uint8_t fsm_open_sample(char * sample_name, fsm_sample * sample) {
	LOGD("fsm_open_sample", 0);
	strcpy(sample->name, sample_name);
	LOGD(sample->name, 0);
	return open_sample_files(sample);
}

uint8_t fsm_close_sample(fsm_sample * sample) {
	LOGD("fsm_close_sample", 0);
	return close_sample_files(sample);
}


uint8_t fsm_seek_raw_wav(FIL * fil) {
	FRESULT res;
	res = f_lseek(fil, 0x2c); 
	if(res) {
		LOGE("Error during go to raw offset", res);
		return RET_ERROR;
	}
	return RET_OK;
}

FRESULT res1;
UINT cnt1;
uint8_t fsm_read_file(FIL * fil, uint16_t * buffer, uint16_t length) { 
//    LOGD("fsm_read_file", length);
	res1 = f_read(fil, buffer, length * 2, &cnt1); 
    for(uint16_t i = 0; i < length; i++) {
        buffer[i] = buffer[i] + 32768;
    //    LOGD("Sample: ", buffer[i]);
        buffer[i] & 0xFFF0;
     //     buffer[i] = 32768;
    }
  //  LOGD("fsm_read_file finish read", cnt1);
	if(res1) {
		LOGE("Read sample file error", res1);
		return RET_ERROR;
	}
 //   cnt1 = cnt1/2;

	if(cnt1 < length * 2) {
	//	while(cnt1 < length) {
//			buffer[cnt1] = 32768;
//			cnt1++;
//		}  
    
		return RET_FILE_FINISHED;
	}
 //   LOGD("FILE OK", cnt1);
	return RET_OK;
}


uint8_t open_sample_files(fsm_sample * sample) {
	LOGD("open_sample_files", 0);

	char downbeat_path[40];
	char offbeat_path[40];

	FRESULT res;

	strcpy(downbeat_path, "/samples/");
	strcat(downbeat_path, sample->name);
	strcat(downbeat_path, "/downbeat.wav");

	strcpy(offbeat_path, "/samples/");
	strcat(offbeat_path, sample->name);
	strcat(offbeat_path, "/offbeat.wav");
	LOGD(offbeat_path, 13);
	LOGD(downbeat_path, 14);

	res = f_open(&(sample->downbeat_fil), downbeat_path, FA_OPEN_EXISTING | FA_READ); 
	if (res){
		LOGE("Downbeat open failed", res);
		LOGE(downbeat_path, 0);
		return RET_ERROR;
	}
	res = f_open(&(sample->offbeat_fil), offbeat_path, FA_OPEN_EXISTING | FA_READ); 
	if (res){
		LOGE("Offbeat open failed", res);
		LOGE(offbeat_path, 0);
		return RET_ERROR;
	}
	return RET_OK;
}

uint8_t close_sample_files(fsm_sample *sample) {
	LOGD("close_sample_files", 0);
	f_close(&(sample->downbeat_fil));
	f_close(&(sample->offbeat_fil));
	return RET_OK;
}

/******************************************************
TEST SD CARD
*******************************************************/

void fsm_sdcard_test() {
    if(fsm_init() != RET_OK) {
        LOGE("FSM was not initialized", 1);
        return;
    };

    FIL test_fil;
    FRESULT res;
    UINT test_cnt;
    unsigned char test_buffer[4];
    
    res = f_open(&test_fil, "/test.txt", FA_READ);
    if(res) {
        LOGE("Can't open test file", res);
        return;
    };
    res = f_read(&test_fil, test_buffer, 4, &test_cnt); 
    if(res) {
        LOGE("Can't read test file 1", res);
        return;
    };
    LOGI("Print test string", 0);
    LOGI(test_buffer, 0);
    LOGI("Remain bytes", test_cnt);
    res = f_read(&test_fil, test_buffer, 4, &test_cnt); 
    if(res) {
        LOGE("Can't read test file 2", res);
        return;
    };
    LOGI("Remain bytes", test_cnt);
    f_close(&test_fil);
    LOGI("Read test passed", 0);

    f_unlink("/create.txt");

    res = f_open(&test_fil, "/create.txt", FA_WRITE | FA_CREATE_NEW);
        if(res) {
        LOGE("Can't create test file", res);
        return;
    };
    

    res = f_write(&test_fil, test_buffer, 4, &test_cnt);
        if(res) {
        LOGE("Can't write test file", res);
        return;
    };
    f_close(&test_fil);
    LOGI("Write test passed", 0);

}