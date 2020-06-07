
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include "win.hpp"
#else
#include "linux.hpp"
#endif

struct SshInfo {
    std::string displayName, host, userName;
    int port;
    SshInfo(std::string displayName, std::string host, int port, std::string userName)
        : displayName(displayName)
        , host(host)
        , port(port)
        , userName(userName) {};
    std::string getDisplayText(int maxLenDisplayName, int maxLenUserName) const
    {
        return displayName + std::string(maxLenDisplayName - displayName.size(), ' ') + " (" + userName
            + std::string(maxLenUserName - userName.size(), ' ') + "@" + host
            + (port == 22 ? "" : ":" + std::to_string(port)) + ") ";
    }
};

std::string getConfigDir()
{
    std::string userDir = getUserDir();
    std::string configDir = userDir + "/.sshba";
    return configDir;
}

std::vector<SshInfo> sshInfoList;
int currentIndex = 0;
char upKey = 'e', downKey = 'd';

void readConfig(std::string filePath)
{
    std::ifstream ifs(filePath, std::ifstream::in);
    std::string buff;
    while (ifs >> buff) {
        if (buff == "up:") {
            ifs >> buff;
            upKey = buff[0];
        }
        if (buff == "down:") {
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
    while (ifs >> displayName >> host >> port >> userName) {
        sshInfoList.emplace_back(SshInfo(displayName, host, port, userName));
    }
}

void display()
{
    size_t maxLenDisplayName = 0, maxLenUserName = 0;
    for (const auto& x : sshInfoList) {
        maxLenDisplayName = std::max(maxLenDisplayName, x.displayName.size());
        maxLenUserName = std::max(maxLenUserName, x.userName.size());
    }
    std::vector<std::string> buff;
    for (int i = 0; i < sshInfoList.size(); i++) {
        buff.push_back("");
        const auto& x = sshInfoList[i];
        buff.back() += x.getDisplayText(maxLenDisplayName, maxLenUserName);
    }
    cls();
    std::pair<int, int> terminalSize = getTerminalSize();
    int textHeight = buff.size();
    for (int i = 0; i < sshInfoList.size(); i++) {
        auto& line = buff[i];
        if (line.size() > terminalSize.second) {
            line.resize(terminalSize.second);
        }
    }
    if (textHeight <= terminalSize.first) {
        for (int i = 0; i < buff.size(); i++) {
            if (i) {
                std::cout << std::endl;
            }
            if (i == currentIndex) {
                std::cout << "\033[31m";
            }
            std::cout << buff[i];
            if (i == currentIndex) {
                std::cout << "\033[0m";
            }
        }
    } else {
        int listHeight = terminalSize.first - 2;
        if (listHeight <= 0) {
            std::cout << "Terminal height too small！";
        } else {
            int upIdx = 0;
            int downIdx = buff.size() - 1;
            int upLine = listHeight / 2 + 1;
            int downLine = listHeight - upLine;
            if (currentIndex - upLine + 1 >= 0 && currentIndex + downLine < buff.size()) {
                upIdx = currentIndex - upLine + 1;
                downIdx = currentIndex + downLine;
            } else if (currentIndex - upLine + 1 < 0) {
                downIdx = listHeight - 1;
            } else {
                upIdx = downIdx - listHeight + 1;
            }
            if (upIdx == 0) {
                std::cout << std::string(terminalSize.second, ' ');
            } else {
                std::string more = ". . . " + std::to_string(upIdx) + " more";
                std::cout << more << std::string(terminalSize.second - more.size(), ' ') << std::endl;
            }
            for (int i = upIdx; i <= downIdx; i++) {
                if (i == currentIndex) {
                    std::cout << "\033[31m";
                }
                std::cout << buff[i] << std::string(terminalSize.second - buff[i].size(), ' ') << std::endl;
                if (i == currentIndex) {
                    std::cout << "\033[0m";
                }
            }
            if (downIdx == buff.size() - 1) {
                std::cout << std::string(terminalSize.second, ' ');
            } else {
                std::string more = ". . . " + std::to_string(buff.size() - 1 - downIdx) + " more";
                std::cout << more << std::string(terminalSize.second - more.size(), ' ');
            }
        }
    }
}

void select()
{
    int key;
    while (true) {
        key = getKey();
        if (key == '\n' || key == '\r') {
            const auto& x = sshInfoList[currentIndex];
            std::string cmd = "ssh " + x.userName + "@" + x.host + " -p" + std::to_string(x.port);
            std::cout << std::endl << "> " << cmd << std::endl;
            system(cmd.c_str());
            exit(0);
        } else if (key == upKey) {
            currentIndex = (currentIndex - 1) % int(sshInfoList.size());
        } else if (key == downKey) {
            currentIndex = (currentIndex + 1) % int(sshInfoList.size());
        } else {
            exit(0);
        }
        currentIndex = (currentIndex + int(sshInfoList.size())) % int(sshInfoList.size());
        display();
    }
}

int main()
{
    readConfig(getConfigDir() + "/config");
    readSshInfoList(getConfigDir() + "/ssh_info");
    display();
    select();
}