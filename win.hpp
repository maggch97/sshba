#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <conio.h>
#include <string>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void cls()
{
    // windows下使用033c清屏效果不好
    // SetConsoleCursorPosition不清屏在终端大小改变的时候也会有问题，暂时用033c
    // COORD pos = { 0, 0 };
    // HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    // SetConsoleCursorPosition(out, pos);
    printf("\033c");
}

std::string getUserDir() { return getenv("USERPROFILE"); }

void windowsInit()
{
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(handle, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(handle, mode);
    // windows下的cls()不带清屏功能，初始时清屏
    printf("\033c");
}

std::pair<int, int> getWinSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return { rows, columns };
}