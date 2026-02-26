#include "color8.h"

#ifdef _WIN32
#include <windows.h>

void enable_ansi(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
}
#endif

void fcolor8(FILE *stream, int r, int g, int b){
    fprintf(stream, "\033[38;2;%d;%d;%dm", r, g, b);
}

void bcolor8(FILE *stream, int r, int g, int b){
    fprintf(stream, "\033[48;2;%d;%d;%dm", r, g, b);
}

void rcolor8(FILE *stream) {
    fprintf(stream, "\033[0m");
}