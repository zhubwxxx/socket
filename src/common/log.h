#ifndef LOG_H
#define LOG_H

#include <sstream>
#include <cstring>


enum class LogLevel {
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE
};

#define LOG_MACRO_BODY(loglevel, msg)           \
        do {                                    \
            std::ostringstream os;              \
            os << msg;                          \
            writeLog(loglevel, os, __FILE__, __LINE__, __FUNCTION__); \
        } while (0);                            \

#define LOG_FATAL(msg) LOG_MACRO_BODY(LogLevel::FATAL, msg)
#define LOG_ERROR(msg) LOG_MACRO_BODY(LogLevel::ERROR, msg)
#define LOG_WARN(msg) LOG_MACRO_BODY(LogLevel::WARN, msg)
#define LOG_INFO(msg) LOG_MACRO_BODY(LogLevel::INFO, msg)
#define LOG_DEBUG(msg) LOG_MACRO_BODY(LogLevel::DEBUG, msg)
#define LOG_TRACE(msg) LOG_MACRO_BODY(LogLevel::TRACE, msg)


void writeLog(LogLevel logLevel, std::ostringstream &os, const char *fileName, unsigned int line, const char *function);

std::string errno2Str();


#endif


