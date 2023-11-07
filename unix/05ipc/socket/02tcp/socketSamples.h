//
// Created by 78472 on 2022/11/27.
//

#ifndef STL_SOCKUTIL_H
#define STL_SOCKUTIL_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;


/*
 *  getaddrinfo：主机名、主机地址、服务名、服务端口号的字符串表示---> 套接字地址
 *  getnameinfo：套接字地址 ---> 主机名、主机地址、服务名、服务端口号
 *
 *  将域名、服务名转换为点分十进制IP地址以及数字端口号
 */
void domain2decimal(string& host, string& service);


/*
 * 一个简单的客户端段，连接服务器，发送消息，接收一次返回并输出到标准输出中
 */
void simpleTcpClient(string& ip, uint16_t port, string& writeMesage);


/**
 * @param port  服务器监听的端口号
 * 功能：
 *      一个简单的服务器，接收客户端的连接，建立连接后向客户端读取一次数据，然后转换后返回给客户端并关闭和客户端的连接。
 */
void simpleTcpServer(uint16_t port);


/**
 *
 * @param ip        服务器IP
 * @param port      服务器端口号
 * @return          成功返回大于0的文件描述符，失败返回-1
 * 功能：
 *      尝试和指定的服务器建立连接，返回建立连接的文件描述符
 */
int open_clientfd_tcp(string& ip, uint16_t port);


/**
 * @param port   服务器要监听的端口号
 * @return       成功返回大于0的监听描述符，失败返回-1.
 *
 * 功能：
 *     返回监听描述符，监听本机所有网卡接收到的发送到端口port的连接请求
 */
int open_listenfd_tcp(uint16_t port);


/**
 * @param port      服务器监听的端口号
 * 说明：
 *      使用select函数实现的服务器，保存建立的连接，和客户端进行交互
 */
void tcpServer_select(uint16_t port);


/**
 *
 * @param port  服务器监听的端口号
 * 说明：
 *      使用poll函数实现的服务器，保存建立的连接，和客户端进行交互
 */
void tcpServer_poll(uint16_t port);


/**
 * @param port  服务器监听的端口号
 * 说明：
 *      使用epoll函数实现的服务器，保存建立的连接，和客户端进行交互
 */
void tcpServer_epoll(uint16_t port);


void udpClient(string& ip, uint16_t port);


//udp服务器
void udpServer(uint16_t port);











#endif //STL_SOCKUTIL_H
