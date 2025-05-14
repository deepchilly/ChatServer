#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP + port
               const string &nameArg          // 服务器名字
               );

    // 启动服务
    void start();

private:
    // 处理用户连接创建和断开    epoll listenfd accept
    void OnConnection(const TcpConnectionPtr &conn);

    // 处理用户的读写事件
    void OnMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time);              // 接收数据的时间信息

    TcpServer _server;  //muduo库，实现服务器功能的类对象
    EventLoop *_loop;   //指向时间循环对象的指针
};

#endif