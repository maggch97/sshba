#ifndef _SSHBA_HEADERS_SSH_CONFIG_H_
#define _SSHBA_HEADERS_SSH_CONFIG_H_

#include <string>
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

struct SshConfig {
    string host;
    string hostName;
    string user;
    int port;
# 其他配置
    map<string, string> other;
};

pair<string, string> getKvOfLine(string line) {
    int keyStart = 0;
    while (keyStart < line.size() && line[keyStart] == ' ')keyStart++;
    if (keyStart == line.size()) {
        throw "";
    }
    int keyEnd = line.find(' ', keyStart);
    if (keyEnd == line.size()) {
        throw "";
    }
    int valueStart = keyEnd+1;
    while(valueStart < )
}

vector<SshConfig> readSshConfig(string path) {
    fstream configStream(path);
    string line;
    vector<SshConfig> config;
    shared_ptr<SshConfig> currentConfig;
    while (getline(configStream, line)) {
        if (line.find("Host") == 0) {
            if (currentConfig) {
                config.push_back(currentConfig.get());
                currentConfig.reset();
            }
            currentConfig.reset(new SshConfig);
            currentConfig->host =
        }
    }
}

#endif