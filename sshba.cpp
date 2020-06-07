
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include "win.hpp"
#else
#include "linux.hpp"
#endif
#include "config.hpp"

std::string getConfigDir()
{
    std::string userDir = getUserDir();
    std::string configDir = userDir + "/.sshba";
    return configDir;
}

Config config;
Hosts hosts;
int currentIndex = 0;

void display()
{
    size_t maxLenDisplayName = 0, maxLenUserName = 0;
    for (const auto& x : hosts) {
        maxLenDisplayName = std::max(maxLenDisplayName, x.displayName.size());
        maxLenUserName = std::max(maxLenUserName, x.userName.size());
    }
    std::vector<std::string> buff;
    for (int i = 0; i < hosts.size(); i++) {
        buff.push_back("");
        const auto& x = hosts[i];
        buff.back() += x.getDisplayText(maxLenDisplayName, maxLenUserName);
    }
    cls();
    std::pair<int, int> terminalSize = getTerminalSize();
    int textHeight = buff.size();
    for (int i = 0; i < hosts.size(); i++) {
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
            const auto& x = hosts[currentIndex];
            std::string cmd = "ssh " + x.userName + "@" + x.address + " -p" + std::to_string(x.port);
            std::cout << std::endl << "> " << cmd << std::endl;
            system(cmd.c_str());
            exit(0);
        } else if (key == config.upKey) {
            currentIndex = (currentIndex - 1) % int(hosts.size());
        } else if (key == config.downKey) {
            currentIndex = (currentIndex + 1) % int(hosts.size());
        } else {
            exit(0);
        }
        currentIndex = (currentIndex + int(hosts.size())) % int(hosts.size());
        display();
    }
}

void changeConfig()
{
    std::cout << "Current up key code: " << config.upKey << std::endl;
    std::cout << "Please press new up key..." << std::endl;
    config.upKey = getKey();
    std::cout << "New up key code: " << config.upKey << std::endl;
    std::cout << "Current down key code: " << config.downKey << std::endl;
    std::cout << "Please press new down key..." << std::endl;
    config.downKey = getKey();
    std::cout << "New down key code: " << config.downKey << std::endl;
    saveConfig(getConfigDir() + "/config", config);
}

int main(int argc, char* argv[])
{
    readConfig(getConfigDir() + "/config", config);
    readHosts(getConfigDir() + "/hosts", hosts);
    if (argc == 2) {
        std::string command = argv[1];
        if (command == "key") {
            changeConfig();
        } else if (command == "config") {
            std::cout << getConfigDir() << std::endl;
        }
        return 0;
    }
    if (hosts.empty()) {
        std::cout << getConfigDir() + "/hosts is empty" << std::endl;
        return 0;
    }
    display();
    select();
    return 0;
}