#ifndef _SSHBA_HEADERS_CONFIG_H_
#define _SSHBA_HEADERS_CONFIG_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "utils.hpp"

#ifdef _WIN32

#include "win.hpp"

#else

#include "linux.hpp"

#endif


std::string configDir() {
    std::string userDir = getUserDir();
    return path(userDir, ".sshba");
}

std::string hostsPath() {
    return path(configDir(), "hosts");
}

struct Config {
    int upKey;
    int downKey;
};

struct Host {
    std::string displayName, address, userName;
    int port;

    Host(std::string displayName, std::string address, int port, std::string userName)
            : displayName(displayName), address(address), port(port), userName(userName) {};

    std::string getDisplayText(int maxLenDisplayName, int maxLenUserName) const {
        return displayName + std::string(maxLenDisplayName - displayName.size(), ' ') + " (" + userName
               + std::string(maxLenUserName - userName.size(), ' ') + "@" + address
               + (port == 22 ? "" : ":" + std::to_string(port)) + ") ";
    }
};

typedef std::vector<Host> Hosts;

void readHosts(std::string filePath, Hosts &hosts) {
    std::ifstream ifs(filePath, std::ifstream::in);
    std::string displayName, host, userName;
    int port;
    while (ifs >> displayName >> host >> port >> userName) {
        hosts.emplace_back(Host(displayName, host, port, userName));
    }
}

void readConfig(std::string filePath, Config &config) {
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.read((char *) &config, sizeof(config))) {
        config.upKey = config.downKey = 0;
    }
    if (config.upKey == 0) {
        config.upKey = 'e';
    }
    if (config.downKey == 0) {
        config.downKey = 'd';
    }
}

void saveConfig(std::string filePath, const Config &config) {
    std::ofstream output_file(filePath, std::ios::binary);
    output_file.write((char *) &config, sizeof(config));
}

#endif