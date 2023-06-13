#pragma once
#include "poller.h"
#include <vector>
#include <sys/epoll.h>
class epollPoller : public Poller
{
public:
    typedef std::vector<epoll_event> eventList;

public:
    epollPoller();
    ~epollPoller() override;

    timeStamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
};