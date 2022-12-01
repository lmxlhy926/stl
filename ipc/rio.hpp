
#ifndef HTTPTEST_RIO_HPP
#define HTTPTEST_RIO_HPP


#include <cstdio>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#define MAXLINE 1024
#define RIO_BUFSIZE 1024 * 8

//缓存读取内容
typedef struct{
    int rio_fd;                 //文件描述符
    ssize_t rio_cnt;            //内部缓冲区的可读字节数
    char *rio_bufptr;           //内部buffer的下一个未读字节地址
    char rio_buf[RIO_BUFSIZE];  //内部buffer
}rio_t;


/*
 * 屏蔽信号中断返回的影响
 * 功能：试图从socket文件中阻塞读取n个字节到usrbuf中，直到成功读取n个或读取出错或对端节点关闭，屏蔽信号中断返回的影响
 * 参数：socket文件描述符，缓冲区首地址，期望读取的字节数
 * 返回：出错返回-1，未读取到任何字符返回0，成功返回实际读取的字节数
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);


/*
 * 功能：期望将缓存中的n个字节写入到socket文件中，屏蔽信号中断引起的错误
 * 参数：socket文件描述符，缓冲区首地址，期望写入的字节数
 * 返回：出错返回-1，成功返回写入的字节数
 */
ssize_t rio_writen(int fd, char *usrbuf, size_t n);


/*
 * 功能：用fd初始化rio_t结构体
 * 参数：rio_t结构体指针，文件描述符
 */
void rio_readinitb(rio_t *rp, int fd);


/*
 * 功能：带缓冲区的读，当缓冲区无数据时，对缓冲区进行读取（一次尝试读取缓冲区大小的字节数）
 * 参数：rio_t结构体指针，缓冲区首地址，期望读取的字节数
 * 返回：出错返回-1，读到文件末尾返回0，否则返回实际读取的字节数
 * 注意：当缓冲区有数据时，但是数据量小于n, 此时只会读取缓冲区容量大小的数据
 */
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);


/*
 * 功能：试图从socket文件中读取一行字符串到缓冲区中
 * 参数：rio_t结构体指针，缓冲区，缓冲区最大容纳量
 * 返回：出错返回-1，读到文件末尾返回0，否则返回读到的字节数
 * 说明：如果读取到maxlen-1个字节但是还未读取到换行符，会直接返回。即返回可能是读取到的一行字符串，也可能是usrbuf缓冲区不够了
 */
ssize_t rio_readlineb(rio_t *rp, char *usrbuf, size_t maxlen);


/*
 * 功能：阻塞读取n个字节到缓冲区中，直到成功读取n个或出错或对端节点关闭，屏蔽信号中断返回的影响
 * 参数：rio_t结构体指针，缓冲区，字节数
 * 返回：出错返回-1，读到文件末尾返回0，否则返回成功读取到的字节数
 * 说明：1.该函数为带缓冲区的读。 2. 克服rio_read在临界区读时返回不足值得缺点。
 */
ssize_t rio_readnb(rio_t *rp, char *usrbuf, size_t n);



#endif //HTTPTEST_RIO_HPP




























