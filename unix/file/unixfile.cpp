
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <cstring>


using namespace std;

void test1(){
    const char* buf = "helloworld";
    const char* buf1 = "12345";

    string path = "/home/lhy/test/a.txt";
    int fd = open(path.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
    lseek(fd, 5, SEEK_SET);
    printf("current: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    write(fd, buf1, strlen(buf1));
    printf("current: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    close(fd);
}


int main(int argc, char* argv[]){
    string path = "/home/lhy/test/a.txt";
    int fd = open(path.c_str(), O_RDWR);
    printf("current: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    char readBuf[1024];
    read(fd, readBuf, 5);
    printf("current: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));
    char* writePtr = "world";
    write(fd, writePtr, strlen(writePtr));
    printf("current: %d\n", static_cast<int>(lseek(fd, 0, SEEK_CUR)));

    close(fd);
    return 0;
}