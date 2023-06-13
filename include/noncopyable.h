#pragma once
// 被继承以后，派生类对象可以正常构造析构，但是不能拷贝构造和赋值操作
// 使用public继承，子类的子类也会禁止拷贝和赋值，使用private，子类的子类不会禁止拷贝和赋值
class noncopyable
{
protected:
    noncopyable() = default;
    ~noncopyable() = default;

private:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;
};