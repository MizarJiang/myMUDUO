#pragma once

#include "noncopyable.h"
#include "timeStamp.h"
#include <vector>
#include <unordered_map>
class Channel;
class eventLoop;
// muduo库中多路事件分发器的核心IO复用模块
class Poller : noncopyable
{
public:
    typedef std::vector<Channel *> ChannelList;

    Poller(eventLoop *loop);
    virtual ~Poller();

    // 给所有IO复用保留统一的接口
    virtual timeStamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;

    // 判断参数Channel是否在当前的Poller当中
    bool hasChannel(Channel *channel) const;

    // eventLoop可以通过该接口获取默认的IO复用的具体实现
    static Poller *newDefaultPoller(eventLoop *loop);

protected:
    // map的key：sockfd，value：sockfd所属的channel通道类型
    typedef std::unordered_map<int, Channel *> channelMap;
    channelMap channels_;

private:
    // 定义Poller所属的事件循环eventLoop
    eventLoop *ownerLoop_;
};