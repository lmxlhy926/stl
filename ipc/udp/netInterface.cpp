
#include <cstdio>

#include <unistd.h>
#include <sys/socket.h>
#include <linux/if.h>       //网络接口相关函数
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;
int main(int argc, char* argv[]){
#define BUFSIZE 1024
    char buf[BUFSIZE];
    struct ifconf ifconf{};     //存储所有的网络接口信息
    ifconf.ifc_buf = buf;
    ifconf.ifc_len = BUFSIZE;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ioctl(sockfd, SIOCGIFCONF, &ifconf);
    printf("ifc_len: %d\n", ifconf.ifc_len);

    for(int i = 0; i < ifconf.ifc_len / sizeof(ifconf); ++i){
        struct ifreq *ifreq = ifconf.ifc_req + i;   //存储单个网络接口信息
        char sockbuf[INET_ADDRSTRLEN];
        printf("name = [%s]\n", ifreq->ifr_name);
        printf("local address = [%s]\n", inet_ntop(AF_INET, &ifreq->ifr_addr, sockbuf, INET_ADDRSTRLEN));
    }

    close(sockfd);

    return 0;
}