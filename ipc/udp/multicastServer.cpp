//
// Created by 78472 on 2023/2/3.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>

#define MULTICAST_GROUP ""

using namespace std;
int main(int argc, char* argv[]){
    int servFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(servFd == -1){
        perror("creat socket");
        exit(-1);
    }

    //指定服务器套接字地址
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(6666);

    //服务器套接字绑定地址
    int bindRet = bind(servFd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));
    if(bindRet == -1){
        perror("bind");
        exit(-1);
    }

    //禁止回环许可
    int loopback = 1;
    setsockopt(servFd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback));

    //指定网卡加入指定的多播组
    struct ip_mreq mreq{};
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);              //指定网卡
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);     //指定要加入的多播组
    setsockopt(servFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

    while(true){
        char ipAddr[INET_ADDRSTRLEN];
        char buf[1024];
        struct sockaddr_in from{};
        socklen_t fromLength;
        ssize_t nReceive = recvfrom(servFd, buf, 1024, 0,
                                    reinterpret_cast<struct sockaddr*>(&from), &fromLength);
        if(nReceive == -1){
            perror("recefrom");
            exit(-1);
        }

        printf("receive from [%s] at port[%d]\n",
               inet_ntop(AF_INET, &from.sin_addr, ipAddr, INET_ADDRSTRLEN),
               ntohs(from.sin_port)
               );
        printf("    %s\n", string(buf, nReceive).data());
    }
}













