#include "mkeys.h"

void KenableRawMode() {
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;

    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void KdisableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

int KgetKey() {
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) == 1)
        return ch;
    return 0;
}