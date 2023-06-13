#pragma once
#include <string>
#include <iostream>
#include "noncopyable.h"

// 定义日志的级别    INFO    WARN    ERROR   FATAL  DEBUG

enum LogLevel
{
    INFO,
    WARN,
    ERROR,
    FATAL,
    DEBUG
};

class Logger : noncopyable
{
public:
    // 因为继承了noncopyable，无法拷贝实例，所以这里的单例返回值应该为&
    static Logger &GetInstance();
    // 设置日志级别
    void setLevel(LogLevel level);
    // 写日志
    void log(std::string msg);

private:
    int loglevel_;
    Logger(){};
};

// 定义宏
// LOG_INFO("%s,%d",arg1,arg2)

// #define LOG_DEBUG(format, ...) Logger::GetInstance()->write_log(0, format, ##__VA_ARGS__)
// #define LOG_INFO(format, ...) Logger::GetInstance()->write_log(1, format, ##__VA_ARGS__)
// #define LOG_WARN(format, ...) Logger::GetInstance()->write_log(2, format, ##__VA_ARGS__)
// #define LOG_ERROR(format, ...) Logger::GetInstance()->write_log(3, format, ##__VA_ARGS__)
#define LOG_INFO(logmsgFormat, ...)                       \
    do                                                    \
    {                                                     \
        Logger &logger = Logger::GetInstance();           \
        logger.setLevel(INFO);                            \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_WARN(logmsgFormat, ...)                       \
    do                                                    \
    {                                                     \
        Logger logger = Logger::GetInstance();            \
        logger.setLevel(WARN);                            \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_ERROR(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger logger = Logger::GetInstance();            \
        logger.setLevel(ERROR);                           \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_FATAL(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger logger = Logger::GetInstance();            \
        logger.setLevel(FATAL);                           \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#ifdef DEBUG
#define LOG_DEBUG(logmsgFormat, ...)                      \
    do                                                    \
    {                                                     \
        Logger logger = Logger::GetInstance();            \
        logger.setLevel(DEBUG);                           \
        char buf[1024] = {0};                             \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif
