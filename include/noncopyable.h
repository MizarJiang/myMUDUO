#pragma once
// 被继承以后，派生类对象可以正常构造析构，但是不能拷贝构造和赋值操作
class noncopyable
{
protected:
    noncopyable() = default;
    ~noncopyable() = default;

private:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;
};