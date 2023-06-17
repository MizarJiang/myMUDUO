#include "epollPoller.h"
#include "logger.h"
#include "channel.h"

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
// Channel未添加到poller中
const int kNew = -1; // Channel的成员index_初始化为-1
// Channel已添加到poller中
const int kAdded = 1;
// Channel从poller中删除
const int kDEleted = 2;
epollPoller::epollPoller(eventLoop *loop)
    : Poller(loop),
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(kInitEventListSize)
{
    if (epollfd_ < 0)
    {
        LOG_FATAL("EPollPoller::EPollPoller,error: %d", errno);
    }
}
epollPoller::~epollPoller()
{
    close(epollfd_);
}
/**
 * epoll_create
 * epoll_ctl add/delete
 * epoll_wait
 */
timeStamp epollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    LOG_DEBUG("func=%s => fd total count %d", __FUNCTION__, channels_.size());
    // 使用&*vec.begin()获取首个元素的地址
    int numEvents = epoll_wait(epollfd_, &(*events_.begin()), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    timeStamp now(timeStamp::now());

    if (numEvents > 0)
    {
        LOG_DEBUG("%d events happened \n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents >= events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
        // 没有fd发生
        LOG_DEBUG("%s time out \n", __FUNCTION__);
    }
    else
    {
        // 发生错误
        if (saveErrno != EINTR)
        {
            errno = saveErrno;
            LOG_ERROR("pollPoller::poll() error!");
        }
    }
    return now;
}

// Channel update->eventLoop updateChannel->epollPoller updateChannel
void epollPoller::updateChannel(Channel *channel)
{
    const int index = channel->index();
    LOG_INFO("fd= %d, events= %d, index= %d", channel->fd(), channel->events(), index);
    if (index == kNew || index == kDEleted)
    {
        // 创建新的fd
        if (index == kNew)
        {
            int fd = channel->fd();
            channels_[fd] = channel;
        }
        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        // 重新修改fd
        int fd = channel->fd();
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDEleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}
// 从poller中删除channel
void epollPoller::removeChannel(Channel *channel)
{
    int fd = channel->fd();
    channels_.erase(fd);

    LOG_INFO("func=%s => fd= %d", __FUNCTION__, channel->fd());

    int index = channel->index();
    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }

    channel->setIndex(kNew);
}

// 填写活跃的list
void epollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for (int i = 0; i < numEvents; i++)
    {
        Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        // eventLoop拿到了poller给它返回的所有发生事件的channel列表
        activeChannels->push_back(channel);
    }
}

// 更新channel通道 epoll_ctl add/mod/del
void epollPoller::update(int operation, Channel *channel)
{
    epoll_event events;
    memset(&events, 0, sizeof(events));
    int fd = channel->fd();

    events.events = channel->events();
    events.data.fd = fd;
    events.data.ptr = channel;

    if (epoll_ctl(epollfd_, operation, fd, &events) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        }
        else
        {
            LOG_FATAL("epoll_ctl del fatal:%d\n", errno);
        }
    }
}