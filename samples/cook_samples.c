#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>

#define MAX_SAMPLES 10
#define DIR_NAME "./data"
#define BINARY_RESULT "./samples.bin"
#define FILENAME_BUFFER_LEGNTH 64

static void proc_dir_to_file(FILE *fp, int id, char * dir_name);
static void write_samples_amount(FILE *fp, uint8_t amount);
static void write_sample_name(FILE *fp, char * name, uint8_t id);
static void create_wav_path(char * filename, char * sample_name);
static int header_size();
static void copy_sample_data(char * filename, FILE * fp);


typedef struct Sample {
    uint32_t downb_start;
    uint32_t downb_size;
    uint32_t offb_start;
    uint32_t offb_size;
} Sample;

void write_samples_amount(FILE *fp, uint8_t amount) {
    fseek(fp, 0, SEEK_SET);
    fwrite(&amount, sizeof(uint8_t), 1, fp);
};

void write_sample_name(FILE *fp, char * name, uint8_t id) {
    fseek(fp, sizeof(uint8_t), SEEK_SET);
    fseek(fp, (sizeof(char) * 9) * id, SEEK_CUR);
    fwrite(name, sizeof(char), 9, fp);
};

void write_sample(FILE *fp, struct Sample * sm, uint8_t id) {
    fseek(fp, sizeof(uint8_t) + 
              ((sizeof(char) * 9) * MAX_SAMPLES) + 
              (sizeof(struct Sample) * id), 
          SEEK_SET);
    fwrite(sm, sizeof(struct Sample), 1, fp);
};

int header_size() {
    return sizeof(uint8_t) + ((sizeof(char) * 9) * MAX_SAMPLES) + (sizeof(struct Sample) * MAX_SAMPLES);
}


int main(void) {
    DIR *dp;
    struct dirent *ep;
    int id = 0;
    FILE *fp;
    dp = opendir (DIR_NAME);
 //   printf("size %d\n", header_size());
    if (dp != NULL) {
        fp = fopen(BINARY_RESULT, "wb");
        for(int j = 0; j < header_size(); j++) { 
            fputc(0, fp);
        }
        while ((ep = readdir(dp))) {
            if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) {
                continue;
            }
            proc_dir_to_file(fp, id, ep->d_name);
            id++;
        };
        fflush(fp);
        fclose(fp);
        (void) closedir(dp);
    } else {
        perror ("Couldn't open the directory");
    }
    return 0;
}

void proc_dir_to_file(FILE * res_file, int id, char * sample_name) {
    char downbeat_filename[FILENAME_BUFFER_LEGNTH];
    char offbeat_filename[FILENAME_BUFFER_LEGNTH];
    struct Sample sample;
    create_wav_path(downbeat_filename, sample_name);
    strcat(downbeat_filename, "downbeat.wav");

    create_wav_path(offbeat_filename, sample_name);
    strcat(offbeat_filename, "offbeat.wav");

    write_samples_amount(res_file, id + 1);
    sample_name[8] = '\0';
    write_sample_name(res_file, sample_name, id);
    fseek(res_file, 0, SEEK_END);
    printf("Id: %d, Name: %s, Downbeat: %s, Offbeat: %s \n", id, sample_name, downbeat_filename, offbeat_filename);
    sample.downb_start = ftell(res_file);
    copy_sample_data(downbeat_filename, res_file);
    sample.downb_size = ftell(res_file) - sample.downb_start;
    sample.offb_start = ftell(res_file);
    copy_sample_data(offbeat_filename, res_file);
    sample.offb_size  = ftell(res_file) - sample.offb_start;
    write_sample(res_file, &sample, id);
    printf("Downbeat size: %d, Offbeat size: %d\n", sample.downb_size, sample.offb_size);


}

void create_wav_path(char * filename, char * sample_name) {
    memset(filename, '\0', FILENAME_BUFFER_LEGNTH);
    strcpy(filename, DIR_NAME);
    strcat(filename, "/");
    strcat(filename, sample_name);
    strcat(filename, "/");  
}

void copy_sample_data(char * filename, FILE * fp) {
    FILE * sf = fopen(filename, "rb");
    uint8_t buf = 0;
    fseek(sf, 0x2C, SEEK_SET);
    while(1) {
        if(feof(sf)) {
            break;
        }
        if(fread(&buf, 1, 1, sf) == 0) {
            break;
        }
        fwrite(&buf, 1, 1, fp);
    }
}

