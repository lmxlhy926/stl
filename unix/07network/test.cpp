
#include <cstdio>
#include <cstdlib>
#include <thread>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void tcp_client(){
    // 构造服务器地址
    struct sockaddr_in serveraddr = {};
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);

    // 创建端点
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 连接
    if(connect(fd, (sockaddr*)(&serveraddr), sizeof(serveraddr)) == -1){
        perror("connect");
        exit(-1);
    }

    while(true){
        // 发送数据
        send(fd, "hello", 5, 0);
        // 接收数据
        char buf[1024] = {};
        recv(fd, buf, 5, 0);
        printf("client received: %s\n", buf);
        sleep(5);
    }

    // 关闭端点
    close(fd);
}


void tcp_server(){
    // 构造服务器地址
    struct sockaddr_in serveraddr = {};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(9999);

    // 创建端点
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 绑定端点
    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    // 准备接受连接
    listen(listenfd, 5);

    // 等待接受连接
    struct sockaddr_in peerAddress;
    socklen_t size;
    int connfd = accept(listenfd, (sockaddr*)&peerAddress, &size);
    if(connfd == -1){
        perror("accept");
        exit(-1);
    }
    
    while(true){
        char buf[1024] = {};
        recv(connfd, buf, 5, 0);
        printf("server received: %s\n", buf);
        send(connfd, "world", 5, 0);
    }

    close(connfd);
    close(listenfd);
}


void udp_client(){
    // 创建端点
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // 服务器地址
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(9999);

    while(true){
        // 发送数据
        ssize_t nSend = sendto(fd, "hello", 5, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if(nSend == -1){
            perror("sendto");
            exit(-1);
        }

        // 接收数据
        struct sockaddr_in peerAddress = {};
        socklen_t addr_len = sizeof(peerAddress);
        char buf[1024] = {};
        char peerName[INET_ADDRSTRLEN] = {};
        ssize_t nRead = recvfrom(fd, buf, 5, 0, (sockaddr*)&peerAddress, &addr_len);
        printf("read %ld bytes from server<%s:%d>: %s\n", nRead, 
                                                        inet_ntop(AF_INET, &peerAddress.sin_addr.s_addr, peerName, sizeof(peerName)),
                                                        ntohs(peerAddress.sin_port),
                                                        buf);
        sleep(5);
    }

    close(fd);
}


void udp_server(){
    // 创建端点
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    // 绑定地址
    struct sockaddr_in serveraddr = {};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(9999);

    bind(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));

    while(true){
         // 不需要监听连接请求，直接等待接收数据；和客户端不同处在于，客户端的地址是自动分配的，服务器的地址是自己手动指定的。本质上没有什么不同。
        struct sockaddr_in peeraddress = {};
        socklen_t size = sizeof(peeraddress);
        char peername[INET_ADDRSTRLEN] = {};
        char buf[1024] = {};
        ssize_t nRead = recvfrom(fd, buf, 5, 0, (sockaddr*)&peeraddress, &size);
        printf("read %ld bytes from client<%s:%d>: %s\n", nRead, 
                                                        inet_ntop(AF_INET, &peeraddress.sin_addr.s_addr, peername, sizeof(peername)),
                                                        ntohs(peeraddress.sin_port),
                                                        buf);
        sendto(fd, "world", 5, 0, (sockaddr*)&peeraddress, size);
    }

    close(fd);
}



int main(int argc, char* argv[]){

    std::thread server_thread(udp_server);
    sleep(3);
    std::thread client_thread(udp_client);
    
    server_thread.join();
    client_thread.join();


    return 0;
}


