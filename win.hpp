#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <conio.h>
#include <string>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

std::pair<int, int> getTerminalSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return { rows, columns };
}

void cls()
{
    static int preRow = -1, preCol = -1;
    std::pair<int, int> size = getTerminalSize();
    if (preRow != size.first || preCol != size.second) {
        system("cls");
        preRow = size.first, preCol = size.second;
    } else {
        COORD pos = { 0, 0 };
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(out, pos);
    }
}

std::string getUserDir()
{
    std::string userDir;
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, "USERPROFILE") == 0 && buf != nullptr) {
        printf("EnvVarName = %s\n", buf);
        userDir = buf;
        free(buf);
    };
    return userDir;
}

int getKey()
{
    int key = _getch();
    if (key == 0 || key == 224) {
        int specialKey = _getch();
        specialKey = specialKey << 8;
        return specialKey;
    } else {
        return key;
    }
}