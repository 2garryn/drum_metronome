#include "fs_manager.h"

void test_fsm(void){
	fsm_sample_iter iter;
	fsm_sample sample;
	char nname[10];
	char db[10];
	char ws[10];

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
	f_gets(ws, 10, &(sample.weakshare_fil));
	f_gets(db, 10, &(sample.downbeat_fil));
	LOGI("Downbeat ->", 0);
	LOGI(db, 0);
	LOGI("Weakshare ->", 0);
	LOGI(ws, 0);
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


uint8_t fsm_samples_iter_init(fsm_sample_iter * iter) {
	iter->stop = FALSE;
	iter->inited = FALSE;
}

uint8_t fsm_samples_iter_continue(fsm_sample_iter * iter, char * name) {
	LOGD("fsm_samples_iter_continue", 0);
	FRESULT res;
	FILINFO fno;
	DIR dir;
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

}

uint8_t fsm_open_sample(char * sample_name, fsm_sample * sample) {
	LOGD("fsm_open_sample", 0);
	strcpy(sample->name, sample_name);
	LOGD(sample->name, 0);
	return open_sample_files(sample);
}

uint8_t fsm_close_sample(fsm_sample * sample) {
	LOGD("fsm_close_sample", 0);
	close_sample_files(sample);
}


uint8_t open_sample_files(fsm_sample * sample) {
	LOGD("open_sample_files", 0);

	char downbeat_path[40];
	char weakshare_path[40];

	FRESULT res;

	strcpy(downbeat_path, "/samples/");
	strcat(downbeat_path, sample->name);
	strcat(downbeat_path, "/downbeat.wav");

	strcpy(weakshare_path, "/samples/");
	strcat(weakshare_path, sample->name);
	strcat(weakshare_path, "/weaksh.wav");
	LOGD(weakshare_path, 13);
	LOGD(downbeat_path, 14);

	res = f_open(&(sample->downbeat_fil), downbeat_path, FA_OPEN_EXISTING | FA_READ); 
	if (res){
		LOGE("Downbeat open failed", res);
		LOGE(downbeat_path, 0);
		return RET_ERROR;
	}
	res = f_open(&(sample->weakshare_fil), weakshare_path, FA_OPEN_EXISTING | FA_READ); 
	if (res){
		LOGE("Weakshare open failed", res);
		LOGE(weakshare_path, 0);
		return RET_ERROR;
	}
	return RET_OK;
}

uint8_t close_sample_files(fsm_sample *sample) {
	LOGD("close_sample_files", 0);
	f_close(&(sample->downbeat_fil));
	f_close(&(sample->weakshare_fil));
	return RET_OK;
}