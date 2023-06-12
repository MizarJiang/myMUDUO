#pragma once
#include <arpa/inet.h>
#include <string>
class inetAddress
{
public:
    explicit inetAddress(const sockaddr_in &in);
    explicit inetAddress(uint16_t port, std::string ip = "127.0.0.1");

    std::string toIp() const;
    std::string toIpPort() const;

    uint16_t toPort() const;

    const sockaddr *getSockAddr() const;

private:
    sockaddr_in addr_;
};