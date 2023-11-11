#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


using namespace std;


/**
 *  getaddrinfo：主机名、主机地址、服务名、服务端口号的字符串表示---> 套接字地址
 *  getnameinfo：套接字地址 ---> 主机名、主机地址、服务名、服务端口号
 *
 *  将域名、服务名转换为点分十进制IP地址以及数字端口号
 *  host默认是域名或者ip，service默认是服务名或者端口号
 *  通过AI_NUMERICHOST, 或AI_NUMERICSERV可将host限制为点分十进制形式的IP，将service限制为数字端口号
 *  说明：
 *      1. 如果是指定的是域名、服务名，则首先需要转换为主机地址、端口号。域名通过DNS服务转换为主机地址，服务名通过映射转换为端口号。
 *      2. 点分十进制字符串地址转换为网络字节序地址
 *      3. 字符串端口号--> 主机字节序端口号--> 网络字节序端口号
*/
void domain2decimal(string& host, string& service){
    struct addrinfo *p, *listp, hints{};    //指向单个结构、指向列表、指示结构
    char hostname[100], port[100];
    int rc, flags;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          //IPv4 only
    hints.ai_socktype = SOCK_STREAM;    //Connections only

    //获取地址
    if((rc = getaddrinfo(host.c_str(), service.c_str(), &hints, &listp)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(-1);
    }

    flags = NI_NUMERICHOST | NI_NUMERICSERV;    //显示数字字符串而非域名和服务名
    for(p = listp; p != nullptr; p = p->ai_next){
        getnameinfo(p->ai_addr, p->ai_addrlen, hostname, 100, port, 100, flags);
        printf("hostname: %s, port: %s\n", hostname, port);
    }

    //释放列表
    freeaddrinfo(listp);
}


int main(int argc, char* argv[]){


    return 0;
}







