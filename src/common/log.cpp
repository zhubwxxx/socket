#include "log.h"

#include <cstdio>
#include <cstring>

#include <iostream>

const size_t MSG_SIZE = 512;

void writeLog(LogLevel logLevel, std::ostringstream &os, const char *fileName, 
    unsigned int line, const char *function)
{
    char prefixMsg[MSG_SIZE] = {0};
    int ret = snprintf(prefixMsg, MSG_SIZE, "[%s():%s:%d] ", function, fileName, line);
    if (ret < 0) {
        std::cerr << "log.cpp:snprintf error!" << std::endl;
        return; 
    }
    std::string str(prefixMsg);

    switch (logLevel) {
        case LogLevel::FATAL:
            str.append("[FATAL] ");
            break;
        case LogLevel::ERROR:
            str.append("[ERROR] ");
            break;
        case LogLevel::WARN:
            str.append("[WARN] ");
            break;
        case LogLevel::INFO:
            str.append("[INFO] ");
            break;
        case LogLevel::DEBUG:
            str.append("[DEBUG] ");
            break;
        case LogLevel::TRACE:
            str.append("[TRACE] ");
            break;
        default:
            break;
    }
    str.append(os.str());
    std::cout << str << std::endl;
}


std::string errno2Str()
{
    char errBuf[MSG_SIZE] = {0};
    strerror_r(errno, errBuf, sizeof(errBuf));
    return std::string(errBuf);
}