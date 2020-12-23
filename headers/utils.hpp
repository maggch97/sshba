#ifndef _SSHBA_HEADERS_UTILS_H_
#define _SSHBA_HEADERS_UTILS_H_

#ifdef _WIN32

#include "win.hpp"

#else

#include "linux.hpp"

#endif

template<class ... T>
std::string path(T... pathList) {
    std::string path;
    auto append = [&path](const std::string &s) {
        if ((path.empty() || path.back() == '\\') || (!s.empty() && s.front() == '\\')) {
            path.append(s);
        } else {
            path.append(pathSeparator()).append(s);
        }
    };
    std::initializer_list<int>{(append(pathList), 0)...};
    return path;
}

#endif