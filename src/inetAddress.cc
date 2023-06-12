#include "inetAddress.h"
#include <strings.h>
inetAddress::inetAddress(const sockaddr_in &in) : addr_(in)
{
}
inetAddress::inetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string inetAddress::toIp() const
{
    char buf[64];
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return buf;
}
std::string inetAddress::toIpPort() const
{
    // 返回ip:port
    char buf[128];
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    sprintf(buf, "%s:%u", buf, ntohs(addr_.sin_port));
    return buf;
}

uint16_t inetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}

const sockaddr *inetAddress::getSockAddr() const
{
    return (sockaddr *)(&addr_);
}

// // 测试
// #include <iostream>
// int main()
// {
//     inetAddress addr(8000);
//     std::cout << addr.toIpPort() << " " << addr.toIp() << " " << addr.toPort() << " " << std::endl;
//     return 0;
// }