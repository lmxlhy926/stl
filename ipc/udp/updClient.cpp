//
// Created by 78472 on 2023/1/31.
//

#include <cstring>
#include <cstdio>
#include <string>
#include <array>

#include <unistd.h>
#include <arpa/inet.h>

/*
    1. 创建一个套接字s
    2. 填充服务器地址
    3. 向服务器发送数据
    4. 阻塞等待服务器返回
    5. 处理服务器返回的数据
    6. 退出程序
 */

#define SERVERADDR "127.0.0.1"
#define SERVERPORT 6666

using namespace std;
int main(int argc, char* argv[]){

    array<char, 1024> sendContent = {"hello world"};
    char receiveBuf[1024];                  //接收服务器返回的数据
    char ipStrBuffer[INET_ADDRSTRLEN];      //存储数据来源主机的ip地址

    int clientFd = socket(AF_INET, SOCK_DGRAM, 0);

    //指定服务器地址
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERADDR, &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(SERVERPORT);

    while(true){
        //向服务器发送数据
        sendto(clientFd, sendContent.data(), sendContent.size(), 0, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));

        struct sockaddr_in fromAddr{};
        socklen_t fromAddrLength;
        ssize_t nReceive = recvfrom(clientFd, receiveBuf, 1024, 0, reinterpret_cast<struct sockaddr*>(&fromAddr), &fromAddrLength);
        if(nReceive == -1){
            perror("recvfrom");
            break;
        }

        printf("recevie from %s at PORT %d: %s\n",
               inet_ntop(AF_INET, &fromAddr.sin_addr.s_addr, ipStrBuffer, INET_ADDRSTRLEN),
               ntohs(fromAddr.sin_port),
               string(receiveBuf, nReceive).c_str());
        sleep(2);
    }

    close(clientFd);
    return 0;
}