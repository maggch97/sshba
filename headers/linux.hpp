#ifndef _SSHBA_HEADERS_LINUX_H_
#define _SSHBA_HEADERS_LINUX_H_


#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON; // local modes = Non Canonical mode
    old.c_lflag &= ~ECHO; // local modes = Disable echo.
    old.c_cc[VMIN] = 1; // control chars (MIN value) = 1
    old.c_cc[VTIME] = 0; // control chars (TIME value) = 0 (No time)
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON; // local modes = Canonical mode
    old.c_lflag |= ECHO; // local modes = Enable echo.
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

int getKey() {
    int key = getch();
    // 禁止ESC
    if (key == 27) {
        key = getch();
        if (key != 91) {
            perror("Esc is unsupported!");
        } else {
            key = getch();
            key = key << 8;
        }
    }
    return key;
}

void cls() { printf("\033c"); }

std::string getUserDir() { return getenv("HOME"); }

std::pair<int, int> getTerminalSize() {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_row, w.ws_col};
}

std::string pathSeparator() {
    return "/";
}

void editFile(std::string path) {
    system(("vim " + path).c_str());
}

#endif