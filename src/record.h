#ifndef _RECORD_H_
#define _RECORD_H_

#include "kvec.h"

typedef struct Record {
    unsigned timestamp;
    double time;
} Record;

/* Dynamic array of records */
typedef kvec_t(Record) r_array;

#endif
