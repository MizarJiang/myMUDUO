#include "logger.h"
#include "timestamp.h"
Logger &Logger::GetInstance()
{
    static Logger ins;
    return ins;
}
// 设置日志级别
void Logger::setLevel(LogLevel level)
{
    loglevel_ = level;
}
// 写日志
void Logger::log(std::string msg)
{
    switch (loglevel_)
    {
    case INFO:
        std::cout << "[INFO]: ";
    case WARN:
        std::cout << "[WARN]: ";
    case ERROR:
        std::cout << "[ERROR]: ";
    case FATAL:
        std::cout << "[FATAL]: ";
    case DEBUG:
        std::cout << "[DEBUG]: ";
    default:
        break;
    }
    // 打印事件和msg
    std::cout << TimeStamp::now().toString() << " : " << msg << std::endl;
}