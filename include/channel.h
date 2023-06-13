#pragma once
#include <functional>
#include <memory>
#include "noncopyable.h"
#include "timeStamp.h"

// 前置声明
class eventLoop;
class timeStamp;
/**
 * Channel理解为通道，封装了sockfd和其他感兴趣的event，如EPOLLIN，EPOLLOUT等事件
 * 还绑定了poller返回的具体事件
 */
class Channel : noncopyable
{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(timeStamp)> ReadEventCallback;

public:
    Channel(eventLoop *loop, int fd);
    ~Channel();

    // fd得到epoller通知以后，处理事件
    void handleEvent(timeStamp receiveTime);

    // 设置回调函数对象
    void setReadCallback(ReadEventCallback cb)
    {
        readCallBack_ = std::move(cb);
    }
    void setWriteCallback(EventCallback cb)
    {
        writeCallBack_ = std::move(cb);
    }
    void setCloseCallback(EventCallback cb)
    {
        closeCallBack_ = std::move(cb);
    }
    void setErrorCallback(EventCallback cb)
    {
        errorCallBack_ = std::move(cb);
    }

    // 防止当channel被手动remove掉，channel还在执行回调操作
    void tie(const std::shared_ptr<void> &);

    int fd() const
    {
        return fd_;
    }

    int events() const
    {
        return events_;
    }

    void set_revents(int revt)
    {
        revents_ = revt;
    }

    bool isNoneEvent() const
    {
        return events_ == kNoneEvent;
    }

    // 设置fd相应的事件状态
    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }
    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }
    void enableWriteing()
    {
        events_ |= kWriteEvent;
        update();
    }
    void disableWriteing()
    {
        events_ &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    // 返回fd当前的事件状态
    bool isNodeEvent() const
    {
        return events_ == kNoneEvent;
    }
    bool isReadEvent() const
    {
        return events_ & kReadEvent;
    }
    bool isWriteEvent() const
    {
        return events_ & kWriteEvent;
    }

    int index() const
    {
        return index_;
    }
    int setIndex(int index)
    {
        index_ = index;
    }

    eventLoop *ownerLoop()
    {
        return loop_;
    }

    void remove();

private:
    void update();
    void handleEventWithGuard(timeStamp receiveTime);

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    eventLoop *loop_;
    const int fd_; // 监听的对象

    int events_;  // 注册fd反省去的事件
    int revents_; // poller返回的具体发生的事件

    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;

    // 因为channel通道里面能够获知fd最终发生的具体时间revents
    // 所以负责调用具体时间的回调
    ReadEventCallback readCallBack_;
    EventCallback writeCallBack_;
    EventCallback closeCallBack_;
    EventCallback errorCallBack_;
};