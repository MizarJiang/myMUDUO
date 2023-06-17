#pragma once
#include "epollPoller.h"
#include "noncopyable.h"
#include "timeStamp.h"
#include "currentThread.h"

#include <functional>
#include <vector>
#include <memory>
#include <mutex>
// 原子操作
#include <atomic>
// 事件循环类，主要包含了两个模块：channel epoller

class Channel;
class Poller;

class eventLoop : noncopyable
{
public:
    typedef std::function<void()> Function;
    typedef std::vector<Channel *> ChannelList;

private:
    std::atomic_bool looping_; // 原子操作，通过CAS实现
    std::atomic_bool quit_;    // 标志退出事件循环

    std::atomic_bool callingPendingFunctors_; // 标识当前loop是否有需要的回调操作
    std::vector<Function> pendingFunctors_;   // 存储loop需要执行的回调操作
    std::mutex mutex_;                        // 互斥锁用来保护vector的线程安全操作

    const pid_t threadId_; // 记录当前loop线程所在的id

    timeStamp pollReturnTime_; // poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> poller_;

    int wakeupFd; // 使用eventfd创建，当mainloop获取一个新用户的channel，使用轮询选择一个subloop并唤醒
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    Channel *currentActiveChannel_;

public:
    eventLoop();
    ~eventLoop();

    void loop(); // 开启事件循环
    void quit(); // 退出事件循环

    timeStamp pollReturnTime() const
    {
        return pollReturnTime_;
    }
    // 在当前loop中执行
    void runInLoop(Function cb);
    // 把cb放入队列中，唤醒loop所在的线程执行cb
    void queueInLoop(Function cb);
    // 用来唤醒loop所在的线程
    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);
    // 判断eventloop对象是否在自己的县城里面
    bool isInLoopThread() const
    {
        return threadId_ == currentThread::tid();
    }
};