#pragma once
#include "poller.h"
#include <vector>
#include <sys/epoll.h>

// class Channel;
class epollPoller : public Poller
{
public:
    typedef std::vector<epoll_event> eventList;

public:
    epollPoller(eventLoop *loop);
    ~epollPoller() override;

    timeStamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
    int epollfd_;
    eventList events_;
    // 给std::vector<epoll_event> eventList一个初始的长度
    static const int kInitEventListSize = 16;

private:
    // 填写活跃的list
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    // 更新channel通道
    void update(int operation, Channel *channel);
};