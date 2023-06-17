#include "timeStamp.h"
#include <time.h>
timeStamp::timeStamp() : microSecondsSinceEpoch_(0)
{
}
timeStamp::timeStamp(int64_t microSecondsSinceEpoch)
    : microSecondsSinceEpoch_(microSecondsSinceEpoch)
{
}

timeStamp timeStamp::now()
{
    time_t ti = time(NULL);
    return timeStamp(ti);
}
std::string timeStamp::toString()
{
    char buf[128] = {0};
    tm *tm_time = localtime(&microSecondsSinceEpoch_);
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
             tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday,
             tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
    return buf;
}

// 测试代码：

// int main()
// {
//     timeStamp time;
//     std::cout << time.now().toString() << std::endl;
//     return 0;
// }