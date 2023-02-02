
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>


using namespace std;
int main(int argc, char* argv[]){
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

    //设置套接字可以进行广播操作
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
            if(FD_ISSET(serverFd, &readfd)){
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
