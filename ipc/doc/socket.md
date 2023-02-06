# IP地址表示以及转换

## IP地址

**一个IP地址就是一个32位无符号整数**，将其存放在一个结构体中

IP地址存放在包头中跨过网络被携带，**其大小超过一个字节，统一按照网络字节序存放**。

```c
struct in_addr{
	uint32_t s_addr;		/* 网络字节序地址（大端模式） */
}
```



## 字节序转换

整型、短整型字节序转换

```c
主机字节序 -> 网络字节序
    uint32_t htonl(uint32_t hostlong);
    uint16_t htons(uint16_t hostshort);
```

```c
网络字节序 -> 主机字节序
    uint32_t ntohl(uint32_t netlong);
    uint16_t ntohs(uint16_t netshort);
```



## IP地址表示方式转换

**点分十进制**：点分十进制是一种表示方法，将IP地址的每个字节由它的十进制值表示，并用句点和其它字节间分开，构成一个字符串。例如0x7f000000的点分十进制表示是"127.0.0.1"。

```c
点分十进制字符串表示 --> 网络字节序的地址
int inet_pton(AF_INENT, const char* src, void *dst);
	src：点分十进制字符串表示
	dst：二进制的网络字节序的IP地址。
即将一个点分十进制字符串转换为一个网络字节序的整数。
```

```c
网络字节序地址 --> 点分十进制的字符串表示
const char* inet_ntop(AF_INET, const void* src, char* dst, socklen_t size);
	src：二进制的网络字节序的IP地址
	dst：存储点分十进制字符串；将得到的以null结尾的字符串的最多size个字节复制到dst。
即将一个网络字节序的整数转换为点分十进制的字符串。
```

**"n"代表网络字节序整数，"p"代表点分十进制字符串表示**



## 几个概念

### 整数的存储方式

整数有2种存储方式：**大端存储**、**小端存储**。小端存储将整数的**低有效字节**存储到低地址，大端存储将整数的**高有效字节**存储到低地址。存储方式只影响整数的解析方式，整数表示的数值是不发生变换的。

### 多字节整数在网络中的传输

在网络中传输一个多字节整数时，总是先传输**低地址字节**，再传输**高地址字节**。对端也是先接收低地址字节，再接收高地址字节

### 字节序

基于网络中多字节整数的传输方式，因此必须约定低地址字节代表的是整数的低有效字节还是高有效字节。**TCP/IP中采取网络字节序，即低地址字节代表的是高有效字节**。约定了存储方式，解析方式也就随之确定。字节序的转换就是改变整数的存储方式。

### 点分十进制

点分十进制是IP地址的一种表示方法，其目的是为了人能直观的辨认IP地址。其表示的是IP地址代表的数值，和IP地址的存储方式无关。之前说过，**存储方式只影响解析方式，存储方式不影响所代表的数值**。



# 套接字

## 套接字地址结构

```c
#IPv4套接字地址结构
struct sockaddr_in{
	uint16_t		sin_family;		//协议族：标识IPV4或IPV6
	uint16_t		sin_port;		//端口号：网络字节序, 2字节
	struct in_addr	sin_addr;		//IP地址：网络字节序，4字节
	unsigned char	sin_zero[8];	//填充：补齐至sizeof(struct sockaddr)大小
}
```

```c
#通用套接字地址结构
struct sockaddr{
	uint16_t	sa_family;		//协议族
	char		sa_data[14];	//地址数据
}
```

**当时设计套接字函数时，并不存在void*类型的指针，为了使套接字函数接受与协议相关的套接字地址结构的指针，使其指向通用的sockaddr结构指针。所以要求应用程序将与协议特定的结构的指针强制转换成这个通用结构。**

**套接字**：从linux内核的角度来看，一个套接字就是通信的一个端点。从linux程序的角度来看，套接字就是一个有相应描述符的打开文件。



## 套接字函数

### socket函数

```c
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
	返回：若成功则为非负文件描述符，若出错则为-1。
```

socket返回的clientfd描述符仅是部分打开的，还不能用于读写。如何完成打开套接字的工作，取决于我们是客户端还是服务器。

### connect函数

```c
#include <sys/socket.h>
int connect(int clientfd, const struct sockaddr* addr, socklen_t addrlen);
	返回：若成功则为0，若出错则为-1.
```

**connect函数会阻塞，一直到连接成功建立或是发生错误。如果成功，clientfd描述符即可进行读写**。

### bind函数

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
	返回：若成功则为0，若出错则为-1。
```

bind函数告诉内核将addr指定的服务器套接字地址和套接字描述符sockfd联系起来。

### listen函数

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
	返回：若成功则为0，若出错则为-1。
```

客户端是发起连接请求的**主动实体**。服务器是**等待来自客户端的连接请求的被动实体**。默认情况下，内核会认为socket函数创建的描述符对应于主动套接字，它存在于一个连接的客户端。服务器调用listen函数告诉内核，描述符是被服务器而不是客户端使用的。

listen函数将sockfd从一个**主动套接字**转换为一个 **监听调套接字（listening socket）**，该套接字可以**接受来自客户端的连接请求**。backlog参数暗示了内核在开始拒绝连接请求之前，队列中要排队的未完成的连接请求的数量。

### accept函数

```c
#include <sys/socket.h>
int accept(int listenfd, struct sockaddr* addr, int* addrlen);
	返回：成功则为非负连接描述符，若出错则为-1。
```

accept函数等待来自**客户端的连接请求到达帧听描述符listenfd**，然后在addr中填写客户端的套接字地址，并返回一个**已连接描述符(connected descriptor)**，**这个描述符可被用来利用Unix I/O函数与客户端通信**。

**监听描述符是作为客户端连接请求的一个端点**。他通常被创建一次，并存在于服务器的整个生命周期。**已连接描述符是客户端和服务器之间已经建立起来连接的一个端点**。服务器每次接受连接请求时都会创建一次，它只存在于服务器为一个客户端服务的过程中。

**交互过程如下**：

第一步：服务器调用accept，等待连接请求到达监听描述符。

第二步：客户端调用connect函数，发送一个连接请求到listenfd。

第三步：accept函数收到连接请求后，打开一个新的已连接描述符connfd，在clientfd和connfd之间建立连接，并且随后返回connfd给应用程序。客户端也从connect返回。客户端和服务器就可以分别通过读写clientfd和connfd来回传送数据了。



## 套接字地址和IP地址、端口号的转换

二进制套接字地址结构和主机名、主机地址、服务名、服务端口号的字符串表示之间的相互转换。当和套接字接口一起使用时，这些函数能使我们编写独立于任何特定版本的IP协议的网络程序。

### **getaddrinfo**

addrinfo结构

```c
struct addrinfo{
    int 				ai_flags;		// 指示参数标志
    int 				ai_family;		// first arg to socket funciton
    int 				ai_socktype;	// second arg to socket function
    int 				ai_protocol;	// third arg to socket funciton
    char 				*ai_canonname;	// 官方名字
    size_t 				ai_addrlen;		// size of ai_addr struct
    struct sockaddr		*ai_addr;		// ptr to socket address structure
    struct addrinfo 	*ai_next;		// ptr to next item in linked list
};
```

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
int getaddrinfo(const char* host, const char* service, 
                const struct addrinfo* hints, struct addrinfo** result)
    返回：如果成功则为0，如果错误则为非零的错误代码。
    该函数返回result，result指向addrinfo结构的链表，其中每个结构指向一个对应于host和service的套接字地址结构。
    
void freeaddrinfo(struct addrinfo* result);	
	应用程序必须在最后调用freeaddrinfo，释放该链表。

const char* gai_strerror(int errcode);
	将返回的非零的错误代码转换为消息字符串。

将主机名、主机地址、服务名、服务端口号的字符串表示转换成套接字地址结构。函数可重入，且适用于任何协议  
```

**参数说明**：

host参数可以是域名，也可以是点分十进制IP地址。service参数可以是服务名（如http），也可以是十进制端口号。

可选的参数hints是一个addrinfo结构，**它提供对getaddrinfo返回的套接字地址列表的更好的控制**。只能设置下列字段：`ai_family`, `ai_socktype`, `ai_protocol`, `ai_flags`字段。其它字段必须设置为0。实际中我们用`memset`将整个结构清零，然后有选择的设置一些字段。

- **`ai_family`**：默认返回IPv4和IPv6套接字地址。`AF_INET`可将列表限制为IPv4地址
- **`ai_socktype`**：默认对于host关联的每个地址返回3个addrinfo结构，每个的ai_socktype字段不同。设置此字段可将返回的地址限制为指定的类型。如`SOCK_STREAM`将类型限制为连接类型。
- **`ai_protocol`**：限制指定的协议。
- **`ai_flags`**：**该字段是一个位掩码，可以进一步指导转换的行为**。

​		`AI_NUMERICSERV`：service默认可以是服务名或者端口号。这个标志强制参数service为端口号。

​		`AI_PASSIVE`：这个标志告诉函数，返回的套接字地址可能被服务器用作监听套接字。在这种情况中，参数host应该为nullptr。得到的套接字地址结构中的地址字段会是通配符地址，告诉内核这个服务器会接受发送到该主机所有IP地址的请求。



### getnameinfo

```c
#include <sys/socket.h>
#include <netdb.h>
int getnameinfo(const struct sockaddr* sa, socklen_t salen,
               char* host, size_t hostlen,
               char* service, size_t servlen,
               int flags);
	返回：如果成功则为0，如果错误则为非零的错误代码。
```

**该函数将套接字地址结构sa转换为对应的主机和服务名字符串，并将它们赋值到host和service缓冲区。**
**如果不想要主机名，可以把host设置为nullptr，hostlen设置为0。对服务字段来说一样。但是必须设置一个。**

**flags**：这是一个位掩码，可以用于指导转换的默认行为。

​		`NI_NUMERICHOST`：默认试图返回host对应的域名，设置该标志，会使函数返回一个点分十进制地址字符串。

​		`NI_NUMERICSERV`：默认会检查  `/etc/services`，如果可能，会返回服务名而不是端口号。设置该标志会使函数跳过查找，简单地返回端口号。

















