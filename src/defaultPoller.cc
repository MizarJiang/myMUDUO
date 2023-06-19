#include "poller.h"
#include "epollPoller.h"
#include <stdlib.h>
// eventLoop可以通过该接口获取默认的IO复用的具体实现
Poller *Poller::newDefaultPoller(eventLoop *loop)
{
    if (getenv("MUDUO_USE_POLL"))
    {
        return nullptr; // 生成poll的实例
    }
    else
    {
        return new epollPoller(loop); // 生成epoll的实例
    }
}