

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

using namespace std;
#define MDNS_PORT 5354

int mdns_socket_setup_ipv4(int sock, const struct sockaddr_in* saddr){
    unsigned char ttl = 1;
	unsigned char loopback = 1;
	unsigned int reuseaddr = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseaddr, sizeof(reuseaddr));
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuseaddr, sizeof(reuseaddr));
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&loopback, sizeof(loopback));

    struct ip_mreq req;
	memset(&req, 0, sizeof(req));
	req.imr_multiaddr.s_addr = htonl((((uint32_t)224U) << 24U) | ((uint32_t)251U));
	if (saddr)
		req.imr_interface = saddr->sin_addr;
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&req, sizeof(req)))
		return -1;

	struct sockaddr_in sock_addr;
	if (!saddr) {
		memset(&sock_addr, 0, sizeof(struct sockaddr_in));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = INADDR_ANY;
	} else {
		memcpy(&sock_addr, saddr, sizeof(struct sockaddr_in));
		setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (const char*)&sock_addr.sin_addr, sizeof(sock_addr.sin_addr));
		sock_addr.sin_addr.s_addr = INADDR_ANY;
	}

	if (bind(sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in)))
		return -1;
	const int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	return 0;
}


int mdns_socket_open_ipv4(const struct sockaddr_in* saddr){
    int sock = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0)
		return -1;
	if (mdns_socket_setup_ipv4(sock, saddr)) {
		close(sock);
		return -1;
	}
	return sock;
}

void mdns_server(){
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(MDNS_PORT);
    int sock = mdns_socket_open_ipv4(&sock_addr);
    if (sock < 0)   return;
    
    send(sock, "hello", 5, 0);


    while (true) {
		int nfds = sock + 1;
		fd_set readfs;
		FD_ZERO(&readfs);
        FD_SET(sock, &readfs);
		
		struct timeval timeout;
		timeout.tv_sec = 20;
		timeout.tv_usec = 100000;

		if (select(nfds, &readfs, 0, 0, &timeout) >= 0) {
			struct sockaddr_in6 addr;
            struct sockaddr* saddr = (struct sockaddr*)&addr;
            socklen_t addrlen = sizeof(addr);
            memset(&addr, 0, sizeof(addr));
            char buffer[1024]{};
            int ret = recvfrom(sock, (char*)buffer, sizeof buffer, 0, saddr, &addrlen);
            if (ret <= 0){
                std::cout << "recvfrom end...." << std::endl;
                return;
            }else{
                std::cout << "received: " << string(buffer) << std::endl;
            }

		} else {
			break;
		}
	}
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
    mdns_server();

    return 0;
}