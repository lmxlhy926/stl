
#include <cstdlib>
#include <ctype.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event2/event.h>

using namespace std;

/**
 * 端点有数据可读才会调用该函数
 * 读取端点数据：
 *      读取正确：处理读取的数据，将结果返回给客户端
 *      读取错误：不再监听该端点，关闭该端点
*/
void read_cb(evutil_socket_t clientfd, short what, void* arg){
    // 读取客户端发送的内容
    char buf[1024];
    int nRead = read(clientfd, buf, sizeof buf);

    // 如果对端端点关闭，或者读操作发生错误；不再监听该端点事件，并且释放该端点
    if(nRead <= 0){
        fprintf(stderr, "%d: the other size is closed or error occurs...\n", clientfd);
        struct event* ev = reinterpret_cast<struct event*>(arg);
        event_del(ev);
        event_free(ev);
        close(clientfd);
    }

    std::cout << "received content: " << string(buf, nRead);
    // 数据转换后发送给客户端
    for(int i = 0; i < nRead; ++i){
        buf[i] = toupper(buf[i]);
    }
    write(clientfd, buf, nRead);
}

/**
 * 当listenfd上有请求连接待处理时：
 *      * 处理一个请求连接，和客户端建立连接，生成一个通信端点
 *      * 监听建立的通信端点，当有数据到达该端点时，调用回调函数处理。
*/
void accept_cb(evutil_socket_t listenfd, short what, void* arg){
    struct sockaddr_in clientaddr{};
    socklen_t clientaddrlen = sizeof clientaddr;
    //处理一个请求链接，建立通信端点
    int clientfd = accept(listenfd, reinterpret_cast<struct sockaddr*>(&clientaddr), &clientaddrlen);
    printf("new connect created, clientfd %d\n", clientfd);

    //创建监听通信端点的事件
    struct event_base* base = reinterpret_cast<struct event_base*>(arg);
    struct event* ev = event_new(base, clientfd, EV_READ | EV_PERSIST, nullptr, nullptr);
    event_assign(ev, base, clientfd, EV_READ | EV_PERSIST, read_cb, ev);
    event_add(ev, nullptr);
}



/**
 * 创建事件监听listenfd是否有连接请求到达，如果有，则调用回调函数处理。
*/
void main_loop(evutil_socket_t listenfd){
    // 监听listenfd是否有请求到达
    // 创建event_base， 创建event, 添加事件
    struct event_base *base = event_base_new();
    struct event* ev = event_new(base, listenfd, EV_READ | EV_PERSIST, accept_cb, base);
    event_add(ev, nullptr);

    fprintf(stdout, "server begin listening....\n");
    // 开始监听、处理事件
    event_base_dispatch(base);

    //释放event, 释放event_base
    event_free(ev);
    event_base_free(base);
}


int net_startup(uint16_t port){
    // 构造服务器地址
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 创建socket, 绑定地址，监听
    int optval = 1;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *)(&optval), sizeof(int));
    bind(serverfd, reinterpret_cast<struct sockaddr*>(&serveraddr), sizeof(serveraddr));
    listen(serverfd, 100);
    return serverfd;
}

int main(int argc, char* argv[]){
    //判断参数是否正确
    if(argc != 2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(-1);
    }
    // 创建服务请求监听描述符，监听服务请求监听描述符
    int port = atoi(argv[1]);
    int serverfd = net_startup(port);
    main_loop(serverfd);
    close(serverfd);
    return 0;
}



