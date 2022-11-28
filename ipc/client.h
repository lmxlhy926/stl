//
// Created by 78472 on 2022/11/27.
//

#ifndef STL_CLIENT_H
#define STL_CLIENT_H

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
 */
void getaddrinfo_getnameinfo(string& host, string& service);

/*
 *  连接服务器，发送消息，接收一次返回并输出到标准输出中
 */
void simpleClient(string& ip, uint16_t port, string& writeMesage);


// 使用getaddrinfo来获取服务器套接字地址
void simpleClient_1(string& ip, uint16_t port, string& writeMesage);












#endif //STL_CLIENT_H
