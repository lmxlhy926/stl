
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event2/event.h>

void read_cb(evutil_socket_t clientfd, short what, void* arg){
    // 读取服务器端点发送的数据
    char buf[1024];
    int nRead = read(clientfd, buf, sizeof buf);
    // 如果对端服务器端点关闭，或者读错误，结束监听
    if(nRead <= 0){
        close(clientfd);
        event_base_loopexit(reinterpret_cast<struct event_base*>(arg), nullptr);
    }
    // 处理接收到的服务器发送的数据
    write(STDOUT_FILENO, buf, nRead);
}

// 从标准输入读取内容，发送到服务器
void cmd_msg_cb(evutil_socket_t stdinfd, short what, void* arg){
    printf("get msg from stdin....\n");
    char buf[1024]{};
    int nRead = read(stdinfd, buf, sizeof buf);
    int clientfd = *(reinterpret_cast<int*>(arg));
    write(clientfd, buf, nRead);
}


/**
 * 客户端逻辑：
 *      * 接收标准输入内容，执行回调函数处理
 *      * 监听客户端端点，调用回调处理服务器返回的内容
*/
void main_loop(const int clientfd){
    // 创建监听事件
    int *clientfdPtr = new int();
    *clientfdPtr = clientfd;
    struct event_base *base = event_base_new();
    struct event* ev_stdin = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST,
                                       cmd_msg_cb, clientfdPtr);
    struct event* ev_socket = event_new(base, clientfd, EV_READ | EV_PERSIST,
                                        read_cb, base);
    event_add(ev_stdin, nullptr);
    event_add(ev_socket, nullptr);
    
    // 开始监听
    event_base_dispatch(base);

    // 释放event、event_base
    fprintf(stdout, "event free and exit....\n");
    event_free(ev_stdin);
    event_free(ev_socket);
    event_base_free(base);
}


int main(int argc, char* argv[]){
    // 命令行参数处理
    if(argc < 3){
        fprintf(stderr, "usage: %s ipaddress port\n", argv[0]);
        exit(-1);
    }
    const char* serverIp = argv[1];
    const short port = atoi(argv[2]);

    // 构造服务器地址
    struct sockaddr_in serveraddr{};
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(port);

    //  创建连接端点，和服务器建立连接
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    int conRet = connect(clientfd, reinterpret_cast<struct sockaddr*>(&serveraddr), sizeof(serveraddr));
    if(conRet < 0){
        printf("connect error....\n");
        exit(-1);
    }

    // 执行客户端逻辑
    main_loop(clientfd);

    return 0;
}


