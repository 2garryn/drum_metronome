#include "fs_manager.h"

void test_fsm(void){
	fsm_sample_iter iter;
	fsm_sample sample;
	char nname[10];
	char db[10];
	char ob[10];

	fsm_init();
	fsm_samples_iter_init(&iter);
	while(1) {
		fsm_samples_iter_continue(&iter, nname);
		if(iter.stop == TRUE) {
			break;
		} else {
			LOGI(nname, 1);
		}
	}
	fsm_open_sample("HIHAT", &sample);
	f_gets(ob, 10, &(sample.offbeat_fil));
	f_gets(db, 10, &(sample.downbeat_fil));
	LOGI("Downbeat ->", 0);
	LOGI(db, 0);
	LOGI("Offbeat ->", 0);
	LOGI(ob, 0);

	fsm_close_sample(&sample);
}


// MUST BE GLOBAL. OTHERWISE DOES NOT WORK
FATFS fat_fs;

uint8_t fsm_init(void) {
	LOGD("fsm_init", 0);
	FRESULT res;
	LOGD("Mounting volume", 0);
	res = f_mount(&fat_fs, "", 1);
	if (res){
		LOGE("Mounting volume", res);
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
uint8_t fsm_read_file(FIL * fil, unsigned char * buffer, uint16_t length) {

	res1 = f_read(fil, buffer, length, &cnt1); 
	if(res1) {
		LOGE("Read sample file error", res1);
		return RET_ERROR;
	}
	if(cnt1 < length) {
		while(cnt1 < length) {
			*(buffer + cnt1) = 0;
			cnt1++;
		}  
		return RET_FILE_FINISHED;
	}
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