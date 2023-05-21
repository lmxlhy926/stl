
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;


//实际用户没有访问权限，但是有效用户可能有访问权限
void accessTest(const char* fileName){
    if(access(fileName, R_OK) < 0){     //进程以进程的实际用户ID和实际组ID来测试指定文件的指定访问权限
        perror("access error");
    }else{
        printf("access ok...\n");
    }

    int fd = open(fileName, O_RDONLY);  //进程以进程的有效用户ID和有效组ID来进行文件的访问权限测试
    if(fd < 0){
        perror("open error");
    }else{
        printf("open successfully...\n");
    }

    close(fd);
}

//设置进程屏蔽字测试
void umaskTest(const char* fileName){
    #define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
    umask(0);   //清除进程屏蔽字
    if(creat(fileName, RWRWRW) < 0){
        printf("%s create failed...\n", fileName);
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);   //设置进程屏蔽字
    string fileName_bak = fileName + string("_bak");
    if(creat(fileName_bak.c_str(), RWRWRW) < 0){
        printf("%s create failed...\n", fileName_bak.c_str());
    }
}

//权限测试
void chmodTest(const char* fileName1, const char* fileName2){
    struct stat statbuf;
    if(stat(fileName1, &statbuf) < 0){
        printf("%s stat error\n", fileName1);
    }
    //在原来文件权限位的基础上关闭组执行，打开设置组ID位
    if(chmod(fileName1, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0){
        printf("%s chmod error...\n", fileName1);
    }

    //将文件权限位设置为用户读、用户写、组读、组写
    if(chmod(fileName2, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0){
        printf("%s chmod error...", fileName2);
    }
    exit(0);
}

//用不同的方式设置权限位
void chmodTest1(const char* fileName, int mode){
    switch (mode){
    case 1:
        chmod(fileName, 0777);  //用户、组、其它的读、写、执行
        break;
    case 2:
        chmod(fileName, 07777); //加上设置用户ID位、设置组ID位、黏着位
        break;
    case 3:
        chmod(fileName, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
        break;
    case 4:
        chmod(fileName, S_IRWXU | S_IRWXG | S_IRWXO);   //用户、组、其它读、写、执行合集
        break;
    case 5:
        chmod(fileName, S_ISUID | S_ISGID | S_ISVTX);   //设置用户ID位、设置组ID位、黏着位设置
        break;
    default:
        break;
    }

    fchmodat(1, "", 0, AT_SYMLINK_FOLLOW);
}


int main(int argc, char* argv[]){
    chmodTest1(argv[1], atoi(argv[2]));
   
    return 0;
}









