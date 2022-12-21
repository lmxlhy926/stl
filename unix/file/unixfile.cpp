
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

/*
 * oflag参数：
 *      必选且只能选定一个：O_RDONLY, O_WRONLY, O_RDWR, O_EXEC
 *      常用可选项：O_TRUNC, O_APPEND, O_CREAT,
 * 读取文件所有内容并打印
 * oflag:
 *      O_TRUNC : 打开时将文件长度截断为0
 *      O_APPEND：在每次写操作之前，将文件偏移量设置在文件的当前结尾处
 *      都没有指定，则打开文件，文件偏移量指示在文件开头处，每次从当前偏移量处开始进行操作。
 */
void file1(){
    int fd = open("/home/lhy/test/a.txt", O_RDWR | O_CREAT, 0666);
    off_t size = lseek(fd, 0, SEEK_END);    //定位到尾端确定文件长度
    lseek(fd, 0, SEEK_SET);     //定位到开头准备读取文件
    if(size > 0){
        char buf[size + 1];
        ssize_t nRead = read(fd, buf, size);
        std::cout << string(buf, nRead) << std::endl;
    }
    close(fd);
}


/*
 * 文件偏移量：读、写操作都从当前文件偏移量处开始，并使偏移量增加多读写的字节数。
 * 开始位置在0，读取5个字节数据，位置在5；从位置5开始写入5个字节数据，位置指向10
 */
void file2(){
    char readBuf[1024];
    int fd = open("/home/lhy/test/a.txt", O_RDWR | O_CREAT, 0666);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    read(fd, readBuf, 5);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    write(fd, "world", 5);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    close(fd);
}


/*
 * O_APPEND: 写操作之前，文件偏移量移动到文件末尾
 */
void file3(){
    int fd = open("/home/lhy/test/a.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    write(fd, "hello", 5);  //先定位到文件末尾，然后开始写操作
    printf("pos: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    lseek(fd, 0, SEEK_SET);     //定位到文件起始处
    char buf[1024];
    ssize_t nRead = read(fd, buf, 5);   //从文件起始处读
    std::cout << string(buf, nRead) << std::endl;
    close(fd);
}


int main(int argc, char* argv[]){
    file3();
    return 0;
}