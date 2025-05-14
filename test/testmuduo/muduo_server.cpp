/*
muduo网络库给用户提供两个类
TcpServer: 用于编写服务器程序
TcpClient: 用于编写客户端程序

epoll + 线程池
能够把网络I/O的代码和业务代码（用户连接、断开、可读写事件）区分开
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
/*基于muduo网络库开发服务器程序
1. 组合TcpServer对象
2. 创建EventLoop事件循环对象的指针
3. 明确TcpServer构造函数的参数，输出ChatServer的构造函数
4. 在当前服务器类的构造函数中，注册处理连接和回调函数和处理读写时间的回调函数
5. 设置合适的服务端线程数量，muduo库会自己划分I/O线程和worker线程
6.
*/
class ChatServer{
public:
    ChatServer(EventLoop* loop, //事件循环
        const InetAddress& listenAddr,  //IP + port
        const string& nameArg   //服务器名字
    ) : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        //给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::OnConnection, this, _1));
        //给服务器注册用户读写时间回调
        _server.setMessageCallback(std::bind(&ChatServer::OnMessage, this, _1, _2, _3));
        //设置服务器的线程数量 1个I/0 3个worker
        _server.setThreadNum(4);

    }

    //开启事件循环
    void start() {
        _server.start();
    }
private:
    //处理用户连接创建和断开    epoll listenfd accept
    void OnConnection(const TcpConnectionPtr &conn){        
        if (conn->connected()) {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() <<"state:online"<< endl;
        }
        else {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:offline" << endl;
            conn->shutdown(); // close(fd)
            //_loop->quit();    //断开连接
        }
    }
    //处理用户的读写事件
    void OnMessage(const TcpConnectionPtr &conn, //连接
                   Buffer *buffer, //缓冲区
                   Timestamp time)  //接收数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data: " << buf << "  time: " << time.toString() << endl;
        conn->send(buf);
    }

    TcpServer _server;
    EventLoop *_loop;
};



int main(){
    EventLoop loop; //epoll
    unsigned int port = 6000;
    InetAddress addr("192.168.42.128", port);
    cout << "bind port " << port << " is success" << endl;
    ChatServer server(&loop, addr, "ChatServer");
    server.start(); //listenfd epoll_wait -> epoll
    loop.loop();    //epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等。
    return 0;
}