//
// Created by 78472 on 2021/9/26.
//

#include "test.h"
#include "../../server/rio.hpp"
#include "../../server/socket.hpp"
#include <iostream>
#include <thread>

#define MAXLINE 1024


int func1(const char* host, const char* port){
    int clientfd;
    char buf[MAXLINE];
    rio_t rio;

    if((clientfd = open_clientfd(host, port)) == -1)
        return -1;

    rio_readinitb(&rio, clientfd);

    while(fgets(buf, MAXLINE, stdin) != nullptr){
        rio_writen(clientfd, buf, strlen(buf));
        std::cout << "start read...." << std::endl;
        int nread = rio_readlineb(&rio, buf, 100);
        if(nread == -1)
            return -1;
        fputs(buf, stdout);
    }

    close(clientfd);
    exit(0);
}


int func2(const char *host, const char *port){
    int clientfd;
    char buf[MAXLINE];
    rio_t rio;

    if((clientfd = open_clientfd(host, port)) == -1)
        return -1;

    rio_readinitb(&rio, clientfd);


    for(int i = 0; i < 100; i++)
    {
        std::cout << "start read...." << std::endl;
        rio_readlineb(&rio, buf, 100);
        fputs(buf, stdout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(clientfd);
    exit(0);
}