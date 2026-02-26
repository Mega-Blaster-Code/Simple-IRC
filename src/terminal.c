#include "terminal.h"

#ifndef __CLAMP_RGB_VALUES
#define __CLAMP_RGB_VALUES
int clamp_rgb(int v) {
	if (v < 0)   return 0;
	if (v > 255) return 255;
	return v;
}
#endif

#ifdef __linux__
#ifndef __TERMINAL_CONFIGURATION_LUA54_LINUX
#define __TERMINAL_CONFIGURATION_LUA54_LINUX

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void init_terminal(void) {
	return;
}

int get_terminal_size(int *w, int *h) {
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return 0;

	*w = ws.ws_col;
	*h = ws.ws_row;
	return 1;
}

void set_terminal_background_color(int r, int g, int b) {
	printf("\033[48;2;%d;%d;%dm", clamp_rgb(r), clamp_rgb(g), clamp_rgb(b));
	fflush(stdout);
}

void set_terminal_forground_color(int r, int g, int b) {
	printf("\033[38;2;%d;%d;%dm", clamp_rgb(r), clamp_rgb(g), clamp_rgb(b));
	fflush(stdout);
}

void reset_terminal_color(void) {
	printf("\033[0m");
	fflush(stdout);
}

void set_cursor_pos(int x, int y) {
	printf("\033[%d;%dH", y + 1, x + 1);
	fflush(stdout);
}

void clear_terminal(const char _C) {
	printf("\033[2J");
	printf("\033[H");

	int width = 0;
	int height = 0;

	if (!get_terminal_size(&width, &height)) {
		return;
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			putchar(_C);
		}
	}

	printf("\033[H");
	fflush(stdout);
}

void hide_cursor(){
	printf("\033[?25l");
	fflush(stdout);
}

void show_cursor(){
	printf("\033[?25h");
	fflush(stdout);
}

#endif

#else

#ifdef _WIN32

#include <windows.h>
#ifndef __TERMINAL_CONFIGURATION_LUA54_WINDOWS
#define __TERMINAL_CONFIGURATION_LUA54_WINDOWS

static void init_terminal(void) {
	static int initialized = 0;
	if (initialized) return;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Habilita UTF-8
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Habilita ANSI
	DWORD mode = 0;
	if (GetConsoleMode(hOut, &mode)) {
		SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}

	initialized = 1;
}

int get_terminal_size(int *w, int *h) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return 0;

	*w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return 1;
}

void set_terminal_background_color(int r, int g, int b) {
	printf("\033[48;2;%d;%d;%dm", clamp_rgb(r), clamp_rgb(g), clamp_rgb(b));
	fflush(stdout);
}

void set_terminal_foreground_color(int r, int g, int b) {
	printf("\033[38;2;%d;%d;%dm", clamp_rgb(r), clamp_rgb(g), clamp_rgb(b));
	fflush(stdout);
}

void reset_terminal_color(void) {
	printf("\033[0m");
	fflush(stdout);
}

void set_cursor_pos(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD pos;
	pos.X = x; // 0-based
	pos.Y = y; // 0-based

	SetConsoleCursorPosition(hConsole, pos);
}

void clear_terminal(const char _C) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD written;
	DWORD size;
	COORD home = {0, 0};

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	size = csbi.dwSize.X * csbi.dwSize.Y;

	FillConsoleOutputCharacterA(hConsole, _C, size, home, &written);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, size, home, &written);
	SetConsoleCursorPosition(hConsole, home);
}

void hide_cursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hConsole, &info);
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
}

void show_cursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hConsole, &info);
	info.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &info);
}

#endif

#else

static void init_terminal(void) {
}

int get_terminal_size(int *w, int *h) {
	return 1;
}

void set_terminal_background_color(int r, int g, int b) {
}

void set_terminal_foreground_color(int r, int g, int b) {
}

void reset_terminal_color(void) {
}

void set_cursor_pos(int x, int y) {
}

void clear_terminal(const char _C) {
}

void hide_cursor() {
}

void show_cursor() {
}

#endif
#endif
