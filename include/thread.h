#pragma once
#include "noncopyable.h"
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
class Thread : noncopyable
{
public:
    typedef std::function<void()> threadFunc;

private:
    bool started_;
    bool joined_;

    // 为了防止初始化就创建线程，使用智能指针
    std::shared_ptr<std::thread> thread_;
    pid_t tid_;
    threadFunc func_;
    std::string name_;

    static std::atomic_int numCreated_;

public:
    explicit Thread(threadFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    void join();

    void setDefaultName();

    bool started() const
    {
        return started_;
    }
    pid_t tid() const
    {
        return tid_;
    }
    const std::string &name() const
    {
        return name_;
    }

    static int numCreated()
    {
        return numCreated_;
    }
};