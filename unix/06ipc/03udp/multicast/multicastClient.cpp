//
// Created by 78472 on 2022/8/9.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define SERVER_PORT 8000
#define CLIENT_PORT 9000
#define GROUP "239.0.0.2"

int main(int argc, char *argv[])
{
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
    return 0;
}
