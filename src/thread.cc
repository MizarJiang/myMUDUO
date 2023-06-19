#include "thread.h"
#include "currentThread.h"

// 信号量
#include <semaphore.h>
std::atomic_int Thread::numCreated_;
Thread::Thread(threadFunc func, const std::string &name)
    : started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name_(name)
{
    setDefaultName();
}
Thread::~Thread()
{
    if (started_ && !joined_)
    {
        thread_->detach();
    }
}

void Thread::start()
// 一个Thread对象，记录的就是一个新线程的详细信息
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    // 开启线程
    thread_ = std::make_shared<std::thread>(std::thread([&]()
                                                        {
                                                            // lambda表达式
                                                            // 获取线程tid
                                                            tid_ = currentThread::tid();
                                                            sem_post(&sem);
                                                            func_(); // 开启一个线程函数
                                                        }));
    // thread_ = std::shared_ptr<std::thread>(new std::thread([&]()
    //                                                        {
    //                                                            // lambda表达式
    //                                                            // 获取线程tid
    //                                                            tid_ = currentThread::tid();
    //                                                            sem_post(&sem);
    //                                                            func_(); // 开启一个线程函数
    //                                                        }));
    // 必须等待获取上面新创建的线程的tid值
    sem_wait(&sem);
}
void Thread::join()
{
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName()
{
    int num = ++numCreated_;
    if (name_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "THread%d", num);
        name_ = buf;
    }
}