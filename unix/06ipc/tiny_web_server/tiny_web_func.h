//
// Created by 78472 on 2021/10/5.
//

#ifndef HTTPTEST_TINY_WEB_FUNC_H
#define HTTPTEST_TINY_WEB_FUNC_H

#include "rio.hpp"
#include <sstream>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>


using namespace std;

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

#endif //HTTPTEST_TINY_WEB_FUNC_H
