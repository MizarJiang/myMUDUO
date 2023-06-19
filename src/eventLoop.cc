#include "eventLoop.h"
#include "logger.h"
#include "channel.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
// 防止一个线程创建多个eventLoop
__thread eventLoop *t_loopInThisThread = nullptr;

// 定义默认的poller IO复用接口的超时事件
const int kPollTimeMs = 10000;

// 创建wakeupfd，用来notify唤醒subReactor处理新来的channel
int createEventFd()
{
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_FATAL("event fd error:%d ", errno);
    }
    return evtfd;
}

eventLoop::eventLoop()
    : looping_(false),
      quit_(false),
      callingPendingFunctors_(false),
      threadId_(currentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventFd()),
      wakeupChannel_(new Channel(this, wakeupFd_))
{
    LOG_DEBUG("eventLoop created %p in thread %d \n", this, threadId_);
    if (t_loopInThisThread)
    {
        LOG_FATAL("Another eventLoop %p exits in this thread %d \n", t_loopInThisThread, threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }
    // 设置wakeupfd的事件类型以及发生事件后的回调操作
    wakeupChannel_->setReadCallback(std::bind(&eventLoop::handleRead, this));
    // 每一个eventLoop都将监听wakeupchannel的epollin事件
    wakeupChannel_->enableReading();
}
eventLoop::~eventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void eventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR("eventLoop::handleRead() reads %d bytes instead of 8", n);
    }
}
void eventLoop::loop()
{
    looping_ = true;
    quit_ = false;

    LOG_INFO("eventLoop %p start looping", this);

    while (!quit_)
    {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for (Channel *channel : activeChannels_)
        {
            // poller监听哪些channel发生事件了，然后上报给eventLoop，通知channel处理相应事件
            channel->handleEvent(pollReturnTime_);
        }
        // 执行当前ecentLoop事件循环需要处理的回调操作
        doPendingFunctors();
    }

    LOG_INFO("eventLoop %p stop looping. \n", this);
    looping_ = false;
}
void eventLoop::quit()
{
    quit_ = true;
    if (!isInLoopThread())
    {
        wakeup();
    }
}

// 在当前loop中执行
void eventLoop::runInLoop(Function cb)
{
    if (isInLoopThread())
    {
        // 在当前的loop线程中执行cb
        cb();
    }
    else
    {
        // 在非当前loop线程中执行cb，需要唤醒loop所在线程执行cb
        queueInLoop(cb);
    }
}
// 把cb放入队列中，唤醒loop所在的线程执行cb
void eventLoop::queueInLoop(Function cb)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

    // 唤醒相应的需要执行上面回调操作的loop线程
    if (!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup(); // 唤醒loop所在线程
    }
}
// 唤醒线程，向wakeupfd写数据,wakeupChannel发生读事件，当前loop被唤醒
void eventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR("eventLoop::wakeup() reads %d bytes instead of 8", n);
    }
}

void eventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void eventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}

bool eventLoop::hasChannel(Channel *channel)
{
    return poller_->hasChannel(channel);
}

void eventLoop::doPendingFunctors()
{
    std::vector<Function> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (const Function functor : functors)
    {
        functor(); // 执行当前loop需要执行的回调操作
    }
    callingPendingFunctors_ = false;
}