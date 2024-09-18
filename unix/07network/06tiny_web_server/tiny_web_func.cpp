//
// Created by 78472 on 2021/10/5.
//

#include "tiny_web_func.h"
#include <cstdio>

void doit(int fd){
    int is_static;      //静态文件内容，还是运行程序产生的动态内容
    struct stat sbuf{};
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE);  //读取请求第一行
    printf("Request headers:\n");
    sscanf(buf, "%s %s %s", method, uri, version);  //解析得到方法、uri、版本号
    if(strcasecmp(method, "GET") != 0){
        clienterror(fd, method, "501", "Not implemented",
                    "Tiny does not implement this method");
        return;
    }

    read_requesthdrs(&rio);   //读取剩余的请求头，因为只处理get请求，不管请求体内容

    is_static = parse_uri(uri, filename, cgiargs);  //从uri获取静态文件或动态程序的文件名，以及动态程序的参数
    if(stat(filename, &sbuf) < 0){
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    }

    if(is_static){
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){  //普通文件且可读
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);

    }else{
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){  //普通文件且可执行
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
}


/*
 * 功能：从uri中提取出filename和cgiargs
 * 参数：uri, 文件名，cgi参数
 * 返回值：动态内容：0;  静态内容：1;
 *
 * uri: /cgi-bin/add?
 *      /home.html
 */
int parse_uri(char *uri, char *filename, char *cgiargs){

    char *ptr;

    if(strstr(uri, "cgi-bin") == nullptr){    //静态内容
        strcpy(cgiargs, "");                     //静态服务则cgi程序参数为空
        strcpy(filename, R"(D:\project\httpLib_socket\example\tiny_web_server\content)");
        strcat(filename, uri);                   //构造静态文件名称
        if(uri[strlen(uri) - 1] == '/')          //补全默认名字
            strcat(filename, "home.html");
        return 1;

    }else{  //动态服务内容
        ptr = strstr(uri, "?");
        if(ptr != nullptr){
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';    //分隔文件名和cgi参数
        }else{
            strcpy(cgiargs, "");
        }

        strcpy(filename, ".");  //构造可执行程序文件名
        strcpy(filename, uri);
        return 0;
    }
}


/*
 * 功能：读取并打印请求头文件
 * 参数：客户端rio_t结构体
 */
void read_requesthdrs(rio_t *rp){

    char buf[MAXLINE];
    rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n") != 0){
        rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }

    return;
}


/*
 * 功能：通过文件名推测文件类型
 * 参数：文件名，文件类型
 */
void get_filetype(char *filename, char *filetype){
    if(strstr(filename, ".html")){
        strcpy(filetype, "text/html");
    }else if(strstr(filename, ".gif")){
        strcpy(filetype, "image/gif");
    }else if(strstr(filename, ".png")){
        strcpy(filetype, "image/png");
    }else if(strstr(filename, ".jpg")){
        strcpy(filetype, "image/jpeg");
    }else
        strcpy(filetype, "text/plain");
}


/*
 * 功能：读取静态服务文件，返回响应报文头和响应报文体
 * 参数：客户端套接字端点，静态服务文件名，静态服务文件大小
 */
void serve_static(int fd, char *filename, int filesize){
    int srcfd;  //静态服务文件描述符
    char *srcp, filetype[MAXLINE], buf[MAXLINE];    //静态服务文件虚拟地址、文件类型、响应报文头

    get_filetype(filename, filetype);   //依据名字推出文件类型
    stringstream resheaders;
    resheaders << "HTTP/1.0 200 OK\r\n";
    resheaders << "Server: Tiny Web Server\r\n";
    resheaders << "Connection: close\r\n";
    resheaders << "Content-length: " << filesize << "\r\n";
    resheaders << "Content-type: " << filetype << "\r\n\r\n";
    strcpy(buf, resheaders.str().c_str());
    rio_writen(fd, buf, strlen(buf));

    srcfd = open(filename, O_RDONLY, 0);    //打开文件
    srcp = static_cast<char *>(mmap(nullptr, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0));  //虚拟地址映射
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}


/*
 * 功能：动态服务，根据文件名执行相应的程序，将执行结果返回给客户端
 * 参数：客户端套接字端点，动态程序文件名，文件参数
 */
void serve_dynamic(int fd, char *filename, char *cgiargs){
    char buf[MAXLINE], *emptylist[] = {nullptr};

    stringstream headers;
    headers << "HTTP/1.0 200 OK\r\n";
    headers << "Server: Tiny Web Server\r\n";
    strcpy(buf, headers.str().c_str());
    rio_writen(fd, buf, strlen(buf));

    if(fork() == 0){
        setenv("QUERY_STRING", cgiargs, 1);  //设置cgi变量
        dup2(fd, STDOUT_FILENO);    //IO重定向，标准输出重定向到客户端套接字
        execve(filename, emptylist, environ);   //执行动态服务程序
    }

    wait(nullptr);  //父进程等待回收子进程
}


void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    rio_writen(fd, buf, strlen(buf));
}
