#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
// RETURNS
#define RET_OK 0

#define RET_ERROR 1

#define RET_NOTFOUND 2

#define RET_NODIR 3

#define RET_FILE_FINISHED 4

// BOOLEAN
typedef unsigned char BOOL;

#define TRUE 1

#define FALSE 0

// SET/RESET state

#define SET_STATE 1

#define RESET_STATE 0

//BEATS INDEX FOR TRACK

#define DOWNBEAT 1

#define OFFBEAT 2

#define NOTHING 0

#define MAX_BPM 250

#define MIN_BPM 30

//RHYTM NOTE PATTERNS

#define  QUARTER_NOTE 0x01

#define EIGHTH_NOTE 0x03

#define SiXTEENTH_NOTE 0x0F

#define SiXTEENTH_NOTE_1_1_0 0x0E

#define SiXTEENTH_NOTE_1_0_1 0x0D

#define SiXTEENTH_NOTE_0_1_1 0x0B


#endif