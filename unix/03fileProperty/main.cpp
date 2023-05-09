
#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;


//用st_mode来判断文件类型
void printFileProperty(int argc, char** argv){
    int i ;
    struct stat statbuf{};  //文件属性信息
    string ptr;
    for(int i = 1; i < argc; ++i){
        printf("%s: ", argv[i]);
        if(lstat(argv[i], &statbuf) < 0){
            std::cout << "lstat error...." << std::endl;
            continue;
        }

        if(S_ISREG(statbuf.st_mode)){
            ptr = "regular";
        }else if(S_ISDIR(statbuf.st_mode)){
            ptr = "directory";
        }else if(S_ISCHR(statbuf.st_mode)){
            ptr = "character special";
        }else if(S_ISBLK(statbuf.st_mode)){
            ptr = "block special";
        }else if(S_ISFIFO(statbuf.st_mode)){
            ptr = "fifo";
        }else if(S_ISLNK(statbuf.st_mode)){
            ptr = "symbolic link";
        }else if(S_ISSOCK(statbuf.st_mode)){
            ptr = "socket";
        }else{
            ptr = "** unknown mode **";
        }
        printf("%s\n", ptr.c_str());
    }
}



int main(int argc, char* argv[]){
    printFileProperty(argc, argv);
    while(true){
        sleep(10);
    }

    return 0;
}









