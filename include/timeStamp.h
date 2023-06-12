#pragma once
#include <iostream>
#include <string>
class timeStamp
{
public:
    explicit timeStamp();
    explicit timeStamp(int64_t microSecondsSinceEpoch);
    static timeStamp now();
    std::string toString();

private:
    int64_t microSecondsSinceEpoch_;
};