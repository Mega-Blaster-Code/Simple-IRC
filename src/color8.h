#ifndef __COLOR8_H
#define __COLOR8_H

#include <stdio.h>

void enable_ansi(void);
void fcolor8(FILE *stream, int r, int g, int b);
void bcolor8(FILE *stream, int r, int g, int b);
void rcolor8(FILE *stream);

#endif