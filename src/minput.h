#ifndef __MINPUT_H
#define __MINPUT_H

#include <stdlib.h>
#include "safema.h"
#include "mkeys.h"

typedef void(*callback)(void*);

typedef struct {
    char  *data;
    size_t capacity;
    size_t length;
    size_t cursor;
	size_t special_key;
	callback func;
	int enter;
} InputBuffer;

InputBuffer *createMinput(size_t size);

void setKeyCallback(InputBuffer *I, callback func);

void updateMinput(InputBuffer *I);

void flushMinput(InputBuffer *I);

void freeMinput(InputBuffer *I);

#endif //__MINPUT_H