#include "channel.h"
#include "eventLoop.h"
#include <sys/epoll.h>
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(eventLoop *loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      tied_(false)
{
}
Channel::~Channel()
{
}

void Channel::handleEvent(timeStamp receiveTime)
{
}

// 防止当channel被手动remove掉，channel还在执行回调操作
void Channel::tie(const std::shared_ptr<void> &obj)
{
    // weak_ptr=shared_ptr，弱智能指针绑定强智能指针
    tie_ = obj;
    tied_ = true;
}

void Channel::remove()
{
}

// 当改变channel所表示的events事件后，update负责在epoller里面更改fd相应的事件epoll_ctl
// eventLoop->ChannelList  epoller
void Channel::update()
{
    // 通过channel所属的eventloop，调用epoller的响应方法，注册fd的events事件
    // TO DO:
    // loop_->updateChannel(this);
}
void Channel::handleEventWithGuard(timeStamp receiveTime)
{
}