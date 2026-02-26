#ifndef __KEYS_H
#define __KEYS_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static struct termios old_termios;

void KenableRawMode();

void KdisableRawMode();

int KgetKey();

#endif //__KEYS_H