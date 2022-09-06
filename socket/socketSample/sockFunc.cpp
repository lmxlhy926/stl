//
// Created by 78472 on 2022/9/4.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>


/*
    将<主机名、主机地址>， <服务名、端口号>的字符串表示  <---->   网络字节序的地址结构

    提供<主机名、主机地址>， <服务名、端口号>， 控制结构，返回指向地址列表的指针
    int getaddrinfo(const char *host, const char *service,
                    const struct addrinfo *hints,
                    struct addrinfo **result)

    //释放返回的地址列表
    void freeaddrinfo(struct addrinfo *result);

    //将错误码转换为字符串表示
    const char *gai_strerror(int errcode);

    将网络字节序的套接字地址转换为相应的主机、服务名字符串
    int getnameinfo(const struct sockaddr *sa, socklen_t, salen,
                    char *host, size_t hostlen,
                    char *service, size_t servlen, int flags);

 */

void getaddrinfo_getnameinfo(const char* ip){
    //控制套接字地址列表的返回
    struct addrinfo hint{};
    memset(&hint, 0, sizeof(hint));

    hint.ai_family = AF_INET;           //只返回IPV4地址
    hint.ai_socktype = SOCK_STREAM;     //只返回连接类型
    hint.ai_protocol = 0;
    hint.ai_flags = AI_ADDRCONFIG;      //Only return address types available on this host

    struct addrinfo *p, *listp;
    int rc;
    if((rc = getaddrinfo(ip, nullptr, &hint, &listp)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    //控制地址的转换
    int flags = NI_NUMERICHOST;     //将IP地址转换为点分十进制表示
    for(p = listp; p != nullptr; p = p->ai_next){
        char buf[INET_ADDRSTRLEN];
        //提取网络字节序地址信息，转换为地址、端口字符串
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, INET_ADDRSTRLEN, nullptr, 0, flags);
        printf("%s\n", buf);
    }

    freeaddrinfo(listp);
}

