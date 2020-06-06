
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
char getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON; // local modes = Non Canonical mode
    old.c_lflag &= ~ECHO;   // local modes = Disable echo.
    old.c_cc[VMIN] = 1;     // control chars (MIN value) = 1
    old.c_cc[VTIME] = 0;    // control chars (TIME value) = 0 (No time)
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON; // local modes = Canonical mode
    old.c_lflag |= ECHO;   // local modes = Enable echo.
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
#endif

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <conio.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

void cls()
{
#ifdef _WIN32
    // windows下使用033c清屏效果不好
    COORD pos = {0, 0};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, pos);
#endif
#ifndef _WIN32
    printf("\033c");
#endif
}

std::string getConfigDir()
{
    std::string userDir;
#ifdef _WIN32
    userDir = getenv("USERPROFILE");
#endif
#ifndef _WIN32
    userDir = getenv("HOME");
#endif
    std::string configDir = userDir + "/.sshba";
    // windows编译有问题
    // if (!std::filesystem::exists(configDir))
    // {
    //     std::filesystem::create_directory(configDir);
    // }
    return configDir;
}

struct SshInfo
{
    std::string displayName, host, userName;
    int port;
    SshInfo(std::string displayName, std::string host, int port, std::string userName) : displayName(displayName),
                                                                                         host(host),
                                                                                         port(port),
                                                                                         userName(userName){};
    std::string getDisplayText(int maxLenDisplayName, int maxLenUserName) const
    {
        return displayName + std::string(maxLenDisplayName - displayName.size(), ' ') + " (" + userName + std::string(maxLenUserName - userName.size(), ' ') + "@" + host + (port == 22 ? "" : ":" + std::to_string(port)) + ") ";
    }
};

std::vector<SshInfo> sshInfoList;
int currentIndex = 0;
char upKey = 'e', downKey = 'd';

void readConfig(std::string filePath)
{
    std::ifstream ifs(filePath, std::ifstream::in);
    std::string buff;
    while (ifs >> buff)
    {
        if (buff == "up:")
        {
            ifs >> buff;
            upKey = buff[0];
        }
        if (buff == "down:")
        {
            ifs >> buff;
            downKey = buff[0];
        }
    }
}

void readSshInfoList(std::string filePath)
{
    std::ifstream ifs(filePath, std::ifstream::in);
    std::string displayName, host, userName;
    int port;
    while (ifs >> displayName >> host >> port >> userName)
    {
        sshInfoList.emplace_back(SshInfo(displayName, host, port, userName));
    }
}

void display()
{
    size_t maxLenDisplayName = 0, maxLenUserName = 0;
    for (const auto &x : sshInfoList)
    {
        maxLenDisplayName = std::max(maxLenDisplayName, x.displayName.size());
        maxLenUserName = std::max(maxLenUserName, x.userName.size());
    }
    std::string buff;
    for (int i = 0; i < sshInfoList.size(); i++)
    {
        if (i == currentIndex)
        {
            buff += "\033[31m";
        }
        const auto &x = sshInfoList[i];
        buff += x.getDisplayText(maxLenDisplayName, maxLenUserName) + "\n";
        if (i == currentIndex)
        {
            buff += "\033[0m";
        }
    }
    cls();
    std::cout << buff << std::endl;
    std::cout << "up: " << upKey << " down: " << downKey << " connect: enter" << std::endl;
}

void select()
{
    char key;
    while (key = getch())
    {
        if (key == '\n' || key == '\r')
        {
            const auto &x = sshInfoList[currentIndex];
            std::string cmd = "ssh " + x.userName + "@" + x.host + " -p" + std::to_string(x.port);
            std::cout << "> " << cmd << std::endl;
            system(cmd.c_str());
            exit(0);
        }
        else if (key == upKey)
        {
            currentIndex = (currentIndex - 1) % int(sshInfoList.size());
        }
        else if (key == downKey)
        {
            currentIndex = (currentIndex + 1) % int(sshInfoList.size());
        }
        else
        {
            exit(0);
        }
        currentIndex = (currentIndex + int(sshInfoList.size())) % int(sshInfoList.size());

        display();
    }
}
int main()
{
#ifdef _WIN32
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(handle, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(handle, mode);
    // windows下的cls()不带清屏功能，初始时清屏
    printf("\033c");
#endif
    readConfig(getConfigDir() + "/config");
    readSshInfoList(getConfigDir() + "/ssh_info");
    display();
    select();
}