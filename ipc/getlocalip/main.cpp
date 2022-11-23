
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <iostream>
#include <string>

#include <sys/unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>


using namespace std;

/*
struct ifaddrs {
	struct ifaddrs  *ifa_next;
	char		*ifa_name;
	unsigned int	 ifa_flags;
	struct sockaddr	*ifa_addr;
	struct sockaddr	*ifa_netmask;
	struct sockaddr	*ifa_dstaddr;
	void		*ifa_data;
};
 */

void func1(){
    struct ifaddrs *addr = nullptr;
    struct ifaddrs *temp_addr = nullptr;

    if (getifaddrs(&addr) == 0) {
        temp_addr = addr;
        while(temp_addr != nullptr) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                printf("name: %s  ip: %s\n",temp_addr->ifa_name, inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr));
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(addr);
}


int main(int argc, char *argv[])
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {     //通过getifaddrs函数得到所有网卡信息
        perror("getifaddrs");            //通过struct ifaddr结构体由指针，将所有信息通过链表连接
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {    //做链表做循环
        if (ifa->ifa_addr == nullptr)                            //判断地址是否为空
            continue;

        family = ifa->ifa_addr->sa_family;                    //得到IP地址的协议族

        if (family == AF_INET || family == AF_INET6) {    //判断协议族是AF_INET还是AF_INET6
            //通过getnameinfo函数得到对应的IP地址。NI_MAXHOST为宏定义，值为1025. NI_NUMERICHOST宏定义，和NI_NUMERICSERV对应，都试一下就知道了。
            s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
                            sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("\taddress: <%s>\n", host);
        }
    }

    freeifaddrs(ifaddr);

    while(true)
        sleep(5);

    return 0;
}


















