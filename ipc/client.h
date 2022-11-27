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
 *  连接服务器，发送消息，接收一次返回并输出到标准输出中
 */
void simpleClient(string& ip, uint16_t port, string& writeMesage);














#endif //STL_CLIENT_H
