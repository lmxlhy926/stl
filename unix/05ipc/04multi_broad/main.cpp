

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

using namespace std;

#define MULTICAST_GROUP "224.0.0.251"
#define PORT 5354

/*
    发送组播数据：
        向特定的组发送数据，和向特定主机发送数据是一样的，只要ip地址为多播地址即可。
 */
using namespace std;

void client1(){
    int clientFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientFd == -1){
        perror("socket");
        exit(-1);
    }

    //组播地址
    struct sockaddr_in multiAddr{};
    multiAddr.sin_family = AF_INET;
    inet_pton(AF_INET, MULTICAST_GROUP, &multiAddr.sin_addr);
    multiAddr.sin_port = htons(PORT);

    string str = "hello multicast";
    while(true){
        sendto(clientFd, str.data(), str.size(), 0,
               reinterpret_cast<struct sockaddr*>(&multiAddr), sizeof(multiAddr));
        std::cout << "send....." << std::endl;
        sleep(3);
    }
}

void client2(){
    struct sockaddr_in localaddr;
    int confd;
    ssize_t len;
    char buf[BUFSIZ];

    struct ip_mreqn group;                                                  /*组播结构体*/
    confd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&localaddr, sizeof(localaddr));                                   /* 初始化*/
    localaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0" , &localaddr.sin_addr.s_addr);
    localaddr.sin_port = htons(CLIENT_PORT);

    bind(confd, (struct sockaddr *)&localaddr, sizeof(localaddr));

    //指定加入多播组的网卡
    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);                        /* 设置组播组地址*/
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);                      /*使用本地任意IP添加到组播组*/
    group.imr_ifindex = if_nametoindex("eth0");                                  /* 设置网卡名 编号 ip ad */

    //加入多播组
    setsockopt(confd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));    /* 将client加入组播组*/

    while (1) {
        len = recvfrom(confd, buf, sizeof(buf), 0, NULL, 0);
        write(STDOUT_FILENO, buf, len);
    }

    close(confd);
}




/*
    1. 服务器套接字地址中ip地址为地址通配符，表示接收发送到本机所有网卡的数据
    2. 将指定的网卡加入多播组：
            某个网卡加入多播组，意味着该网卡接收发送到该多播地址的数据
            多播地址是一个组标识，没有网络号和主机号，整体作为一个标识，标识一个逻辑组
    3. 通过setsockopt()指定特性协议层的一些属性
 */

void server1(){
    int servFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(servFd == -1){
        perror("creat socket");
        exit(-1);
    }

    //指定服务器套接字地址
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    //组播报文发送地址
    struct sockaddr_in multiAddr{};
    multiAddr.sin_family = AF_INET;
    inet_pton(AF_INET, MULTICAST_GROUP, &multiAddr.sin_addr);
    multiAddr.sin_port = htons(PORT);

    /*
     * 禁止组播数据回环，即发送组播数据时，本机不接收该数据
     * 地址复用
     */
    int loopback = 0;
    int reuseaddr = 1;
    setsockopt(servFd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback));
    setsockopt(servFd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    //指定网卡加入指定的多播组
    struct ip_mreq mreq{};
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);              //指定网卡
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);     //指定要加入的多播组
    setsockopt(servFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

    //服务器套接字绑定地址
    int bindRet = bind(servFd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));
    if(bindRet == -1){
        perror("bind");
        exit(-1);
    }

    printf("receiving......\n");
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

        std::cout << "nReceive: " << nReceive << std::endl;
        printf("    %s\n", string(buf, nReceive).data());
        sleep(3);
    }
}


void server2(){
    int servFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(servFd == -1){
        perror("socket");
        exit(-1);
    }

    //加入多播组
    struct ip_mreq ipMreq{};
    ipMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, "224.0.0.251", &ipMreq.imr_multiaddr);
    setsockopt(servFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipMreq, sizeof(ipMreq));

    while(true){
        char buf[1024];
        struct sockaddr_in from{};
        socklen_t fromLength;
        ssize_t nReceive = recvfrom(servFd, buf, 1024, 0,
                                    reinterpret_cast<struct sockaddr*>(&from), &fromLength);
        std::cout << "nReceive: " << nReceive << std::endl;
    }
}

void server3(){
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[MAXLINE] = "multicast\n";
    struct ip_mreqn group;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);                /*构造用于UDP通信的套接字*/

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                        /* IPv4 */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);         /*本地任意IP INADDR_ANY = 0 */
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);        /*设置组播组的地址*/
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);      /* 本地任意IP 自动分配有效IP*/
    group.imr_ifindex = if_nametoindex("eth0");             /* 给出网卡名，转换为对应编号：eth0 --> 编号         ，，  命令:ip ad */


    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));  /*获取组播权限*/

    bzero(&clientaddr, sizeof(clientaddr));                 /* 构造client 地址 IP+端口号*/
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, GROUP, &clientaddr.sin_addr.s_addr); /* IPv4  239.0.0.2+9000 */
    clientaddr.sin_port = htons(CLIENT_PORT);

    int i = 0;
    while (1) {
        sprintf(buf, "multicast %d\n", i++);
        //fgets(buf, sizeof(buf), stdin);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        sleep(1);
    }
    close(sockfd);
}


/*
    广播：
        广播是由一个主机发向一个网络上所有主机的操作方式。例如在一个局域网内进行广播，同一个子网内的所有主机都可以收到此广播发送的数据。

        IP地址分为左边的网络ID部分以及右边的主机ID部分。广播地址所用的IP地址将表示主机ID的位全部设置为1.
        网卡正确配置以后，可以用ifconfig命令来显示指定的接口信息，如"ifconfig eth0"。

        广播地址255.255.255.255是一种特殊的广播地址，这种格式的广播地址是向全世界进行广播，但是却有更多的限制。一般情况下，这种广播
        类型不会被路由器路由，而一个更为特殊的广播地址，例如192.168.0.255也许会被路由，这取决于路由器的配置。

        通用的广播地址在不同的环境中的语义不同。例如，IP地址255.255.255.255，一些UNIX系统将其解释为在主机的所有网络接口上进行广播，
        而有的UNIX内核只会选择其中一个接口进行广播。当一个主机有多个网卡时，这就会成为一个问题。

        如果必须向每个网络接口广播，程序在广播之前应该执行下面的步骤
            1. 确定第一个或下一个接口名字
            2. 确定接口的广播地址
            3. 使用这个广播地址进行广播
            4. 对于系统中其余的活动网络接口重复执行上述步骤
        在执行完这些步骤后，就可以认为已经对每一个接口进行广播。
 */

int broadcast(){
    //创建数据报套接字
    int serverFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverFd == -1){
        perror("create serverFd");
        exit(-1);
    }

    //获取eth0接口的广播地址
    struct ifreq ifreq{};
    strncpy(ifreq.ifr_name, "eth0", strlen("eth0") + 1);
    if(ioctl(serverFd, SIOCGIFBRDADDR, &ifreq) == -1){
        perror("ioctl error");
        exit(-1);
    }

    //构造套接字地址
    struct sockaddr_in broadcast_addr{};
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr = reinterpret_cast<struct sockaddr_in*>(&ifreq.ifr_addr)->sin_addr;
    broadcast_addr.sin_port = htons(6666);

    //设置套接字选项，使其可以发送广播报文
    int so_broadcast = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));

    string sendContent = "find server";
    for(int times = 0; times < 5; ++times){
        sendto(serverFd, sendContent.data(), sendContent.size(), 0,
              reinterpret_cast<struct sockaddr*>(&broadcast_addr), sizeof(broadcast_addr));

       //阻塞接收返回
        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(serverFd, &readfd);
        int nSelect = select(serverFd + 1, &readfd, nullptr, nullptr, nullptr);
        if(nSelect > 0){
            if(FD_ISSET(serverFd, &readfd)){    //服务器收到返回数据
                char receiveBuf[1024];
                struct sockaddr_in from{};
                socklen_t fromLength;
                ssize_t nReceive = recvfrom(serverFd, receiveBuf, 1024, 0,
                                            reinterpret_cast<struct sockaddr*>(&from), &fromLength);
                printf("receive: %s\n", string(receiveBuf, nReceive).data());
            }
        }
    }

    return 0;
}


int main(int argc, char* argv[]){
    server1();

    return 0;
}