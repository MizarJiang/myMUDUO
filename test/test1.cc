#include <iostream>
#include <string>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

class ChatServer{
public:
    ChatServer(muduo::net::EventLoop* loop,//事件循环
            const muduo::net::InetAddress& listenAddr,//IP+PORT
            const std::string& nameArg)//名字
            :_server(loop,listenAddr,nameArg),_loop(loop){
                //给服务器注册用户连接的创建和断开回调

                //给服务器注册用户读写事件回调
                
            }
private:
    muduo::net::TcpServer _server;
    muduo::net::EventLoop* _loop;
};