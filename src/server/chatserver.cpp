/*
muduo网络库给用户提供两个类
TcpServer: 用于编写服务器程序
TcpClient: 用于编写客户端程序

epoll + 线程池
能够把网络I/O的代码和业务代码（用户连接、断开、可读写事件）区分开
*/
#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,               // 事件循环
    const InetAddress &listenAddr, // IP + port
    const string &nameArg          // 服务器名字
    ) : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 给服务器注册用户连接的创建和断开回调
    _server.setConnectionCallback(std::bind(&ChatServer::OnConnection, this, _1));
    // 给服务器注册用户读写时间回调
    _server.setMessageCallback(std::bind(&ChatServer::OnMessage, this, _1, _2, _3));
    // 设置服务器的线程数量 1个I/0 3个worker
    _server.setThreadNum(4);
}

void ChatServer::start(){
    _server.start();
}

// 处理用户连接创建和断开    epoll listenfd accept
void ChatServer::OnConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:online" << endl;
    }
    else
    {
        cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:offline" << endl;
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown(); // close(fd)
        //_loop->quit();    //断开连接
    }
}

// 处理用户的读写事件
void ChatServer::OnMessage(const TcpConnectionPtr &conn, // 连接
                           Buffer *buffer,               // 缓冲区
                           Timestamp time)               // 接收数据的时间信息
{
    string buf = buffer->retrieveAllAsString();
    cout << "recv data: " << buf << "  time: " << time.toString() << endl;
    // 数据反序列化
    json js = json::parse(buf);
    // 完全解耦网络模块和业务模块的代码
    // 通过js["msgid"] 获取->业务处理器-> conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}
