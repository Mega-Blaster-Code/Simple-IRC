#include <stdio.h>
#ifndef __TERMINAL_H
#define __TERMINAL_H

int clamp_rgb(int v);

void init_terminal(void);

int get_terminal_size(int *w, int *h);

void set_terminal_background_color(int r, int g, int b);

void set_terminal_foreground_color(int r, int g, int b);

void reset_terminal_color(void);

void set_cursor_pos(int x, int y);

void clear_terminal(const char _C);

void hide_cursor();

void show_cursor();

#endif