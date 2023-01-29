#include "rio.hpp"
#include <iostream>


/*
 * 不带缓冲区的读
 * 阻塞读取，并且屏蔽信号中断的影响； 如果系统函数是自启动的则无此必要
 * 返回：读取出错返回-1；未读取到任何字符返回0；返回实际读取到的字节数>0
 * 如果连接端点提前关闭，可能会导致实际读取的字节数小于期望的字节数
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n){
    size_t nleft = n;   //待读取字节数，初始时为n
    ssize_t nread = 0;  //执行读取动作后实际读取到的字节数
    char *bufp = reinterpret_cast<char *>(usrbuf);  //指向读取的字符的存储位置

    while(nleft > 0){   //如果还有待读字节数则继续读取
        if((nread = read(fd, bufp, nleft)) < 0){  //读取时出错
            if(errno == EINTR)  //因为信号处理返回而中断，则重新执行读取指令
                nread = 0;
            else
                return -1;     //不是因为信号处理返回而导致的错误

        }else if(nread == 0){   //读到文件末尾（对端socket节点关闭，才会出现这种情况）
            break;
        }

        nleft -= nread;   //更新待读字节数
        bufp += nread;    //更新写入地址
    }

    return n - nleft;   // 最终实际读取到的字节数（>=0）
}


/*
 * 屏蔽信号导致低速系统调用失败的影响
 */
ssize_t rio_writen(int fd, char *usrbuf, size_t n){
    size_t nleft = n;       //待写入字符数
    ssize_t nwrite;         //write操作写入的字符数
    char *bufp = usrbuf;    //指向待写入的字符的地址

    while(nleft > 0){
        if((nwrite = write(fd, bufp, nleft)) <= 0){     //写失败
            if(errno == EINTR)
                nwrite = 0;
            else
                return -1;
        }

        nleft -= nwrite;
        bufp += nwrite;
    }

    return n - nleft;
}


void rio_readinitb(rio_t *rp, int fd){
    rp->rio_fd = fd;
    rp->rio_cnt = 0;                //buffer中当前可被读取的字节数
    rp->rio_bufptr = rp->rio_buf;   //bufptr指针指向buffer开头
}


/*
 * 1. 缓冲区没有数据时，触发一次数据读取动作
 * 2. 从缓冲区读取数据
 */
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n){

    size_t cnt;

    //缓冲区中无数据时，读取数据到缓冲区中
    while(rp->rio_cnt <= 0){    //buffer中无可读取数据，进行一次读取，期望一次读满缓冲区
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if(rp->rio_cnt < 0){   //读取失败返回-1
            if(errno != EINTR)
                return -1;
        }else if(rp->rio_cnt == 0){  //对端socket节点关闭
            return 0;
        }else{
            rp->rio_bufptr = rp->rio_buf;   //重置bufptr指向buffer的首字节
        }
    }   //最终读取数据到buffer中（未必读满），否则会直接返回

    //从缓冲区中读取数据
    cnt = n;
    if(rp->rio_cnt <cnt)    //min(期望读取的字节数，缓冲区中实际有的字节数)
        cnt = rp->rio_cnt;  //实际可以读取到的字节数
    memcpy(usrbuf, rp->rio_bufptr, cnt);

    //更新缓冲区标识
    rp->rio_cnt -= cnt;         //更新缓冲区的可读字节数
    rp->rio_bufptr += cnt;      //更新bufptr指向待读取位置

    return cnt;     //大于0，肯定能读取到数据
}


/*
 *  最多读取maxlen - 1 个字节
 *  返回实际读取到的字节数
 *  保存读取到的换行符
 */
ssize_t rio_readlineb(rio_t *rp, char *usrbuf, size_t maxlen){

    size_t n, rc;   //n：标记第几次读取，rc：读取结果
    char c;         //读取到的字符
    char *bufp = usrbuf;     //指向用户提供的缓冲区的待写入位置

    for(n = 1; n < maxlen; n++){   //读取maxlen -1个字节，最后一个字节为'\0'
        rc = rio_read(rp, &c, 1);

        if(rc < 0){
            return -1;
        }else if(rc == 0){  //此次读取没有读取到数据
            if(n == 1)      //第一次读取，没有读取到任何数据，对端socket节点关闭
                return 0;
            else
                break;      //之前已经读取到一些数据，对端socket节点关闭
        }else if(rc == 1){  //读取到一个字节后，判断是否为换行符
            *(bufp++) = c;
            if(c == '\n'){
                n++;
                break;
            }
        }
    }

    *bufp = 0;      //字符串以'\0'结尾
    return n - 1;
}


/*
 *  1.该函数为带缓冲区的读。
 *  2. 克服rio_read在临界区读时返回不足值得缺点。
 */
ssize_t rio_readnb(rio_t *rp, char *usrbuf, size_t n){

    size_t nleft = n;       //待读取的字节数
    ssize_t nread;          //读取操作读取的字节数
    char *bufp = usrbuf;    //指向用户缓冲区字符待写入位置

    while(nleft > 0){
        nread = rio_read(rp, bufp, nleft);
        if(nread < 0){         //出错返回-1
            return -1;
        }else if(nread == 0){  //对端socket节点关闭
            break;
        }else {
            nleft -= nread;
            bufp += nread;
            std::cout << "nread: " << nread << std::endl;
            std::cout << "nleft: " << nleft << std::endl;
        }
    }

    return n - nleft;
}











