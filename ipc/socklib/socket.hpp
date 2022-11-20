#ifndef HTTPTEST_SOCKET_HPP
#define HTTPTEST_SOCKET_HPP

#include "rio.hpp"

#define SA sockaddr
static const int INVALIDFD = -1;
static const int LISTEN_FD_MAX = 100;
static const int LINEMAX = 1024;

/*
 * 功能：创建一个客户端连接端点
 * 参数：hostname: 服务器地址，port: 服务器端口号
 * 返回值：如果建立成功则返回和服务器完全连接成功的clientfd, 失败则返回-1.
 */
int open_clientfd(const char* hostname, const char* port);


/*
 * 功能：用给定的端口号创建一个监听描述符
 * 参数：服务器的端口号
 * 返回：创建失败返回-1，否则返回创建的监听描述符
 */
int open_listenfd(const char* port);

/*
 * 功能：从客户端读取内容，然后向客户端回送读取到的内容
 * 参数：客户端ID
 */
void echo(int fd);


/*
 * 功能：创建服务器端点，接收客户端连接，处理客户端请求
 * 参数：服务器端口号
 */
int test_server(const char *port);




#endif //HTTPTEST_SOCKET_HPP
