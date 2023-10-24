//
// Created by 78472 on 2023/4/9.
//
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <array>
#include <thread>


/**
 * 常用操作
*/





/**
 * 打开文件，按字节读取文件
*/
void readChar(){
    FILE* fp = fopen("./a.txt", "r+");
    if(fp != nullptr){  //成功打开文件
        char buffer[1024];
        setvbuf(fp, buffer, _IOFBF, 1024);

        int charInt;
        while((charInt = fgetc(fp)) != EOF){   //读取到文件末尾时返回EOF
            printf("%c", charInt);
        }
        fprintf(stdout, "\n");

        if(ferror(fp) != 0){    //读取文件错误
            fprintf(stdout, "fgetc error ...\n");
        }else if(feof(fp) != 0){    //读取到文件末尾
            fprintf(stdout, "end of file ....\n");
            fclose(fp);
        }
    }
}


/**
 * 行缓冲读取：
 *      1. 尝试读满整个缓冲区
 *      2. fgets从缓冲区读取数据，当从缓冲区读取了所有数据但是没有读取到换行符且读取字符数少于最大字符数，
 *         则再次读取数据到缓冲区。
 *      3. 重复上述流程
*/
void read_Linebuf(){
    FILE* fp = fopen("./a.txt", "r+");
    if(fp != nullptr){
        char buffer[1024]{};
        memset(buffer, 0, 1024);
        if(setvbuf(fp, buffer, _IOLBF, 1024) == 0){
            printf("---setvbuf successfully---\n");
        }

        char buf[10];
        if(fgets(buf, 10, fp) != nullptr){
            printf("readStr: %s", buf);
            printf("-------------\n");
            fflush(stdout);
            for(int i = 0; i < 1024; ++i){
                printf("%c", buffer[i]);
            }
        }

        /**
         * 以读写方式打开文件时，在切换读写方式时，需要调用定位函数
         * 从文件读取内容，是把文件内容读取到缓冲区中。
         * 如果文件内容发生改变，但是此时并没有读取文件内容到缓冲区中，则缓冲区中还是以前的内容。
         * 只有再次读取文件内容，缓冲区内容才能反映文件内容变化。
        */
        fseek(fp, 0, SEEK_CUR);
        fputs("aaaaa", fp);
        printf("-------------\n");
        for(int i = 0; i < 1024; ++i){
            printf("%c", buffer[i]);
        }

        // fseek(fp, 0, SEEK_SET);     //读写转换时要加定位操作，定位操作会刷新缓冲区
        // printf("-------------\n");
        // sleep(3);

        // if(fgets(buf, 10, fp) != nullptr){
        //     printf("readStr: %s", buf);
        //     printf("-------------\n");
        //     fflush(stdout);
        //     for(int i = 0; i < 1024; ++i){
        //         printf("%c", buffer[i]);
        //     }
        // }

        fclose(fp);
    }
}


/**
 * 行缓冲下输出时机：
 *      1. 当I/O缓冲区被填满，当下一个字符到来时，冲洗缓冲区。
 *      2. 当遇到换行符时，冲洗缓冲区 
*/
void write_linebuf(){
#define BUFFER_SIZE 8
    char buffer[BUFFER_SIZE];
    if(setvbuf(stdout, buffer, _IOLBF, BUFFER_SIZE) == 0){
        printf("---缓冲区设置成功---\n");
    }
    while(true){
        for(int i = 0; i < BUFFER_SIZE; ++i){
            fputc('a', stdout);
        }
        fputc('-', stdout);     //写入此字符，导致缓冲区被刷新，缓冲区被刷新后填入'_'
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        fputc('b',  stdout);
        fputc('\n', stdout) ;
    }
}


/**
 * 全缓冲读取过程：
 *      底层读取时，尝试读满整个缓冲区
*/
void read_fullbuf1(){
    FILE* fp = fopen("./a.txt", "r+");
    if(fp != nullptr){
        char buffer[1024]{};
        setvbuf(fp, buffer, _IOFBF, 1024);  //设置全缓冲测试缓冲区
        char buf[5];

        if(fgets(buf, 5, fp) != nullptr){  
            printf("readStr: %s", buf);
            printf("-------------\n");
            for(int i = 0; i < 1024; ++i){
                printf("%c", buffer[i]);
            }
            fflush(stdout);
        }else{
            if(ferror(fp) != 0){
                printf("---ferror----\n");
            }else if(feof(fp) != 0){
                printf("---feof----\n");
            }
        }
        fclose(fp);
    }
}



/**
 * 全缓冲读取过程：
 *      1. 标准库尝试从文件读取内容填满I/O缓冲区buffer，如果读取到文件末尾则返回。
 *      2. fgets函数从buffer中读取内容，如果读到换行符或者超过fgets缓冲区大小，则fgets函数返回。
 *      3. 否则继续上述读取操作
*/
void read_fullbuf2(){
    FILE* fp = fopen("./a.txt", "r+");
    if(fp != nullptr){
        char buffer[2]{};
        setvbuf(fp, buffer, _IOFBF, 2);  
        /**
         * 由于I/O缓冲区设置的比较小，fgets函数会导致底层对文件进行多次读取
        */
       char buf[100];
        if(fgets(buf, 100, fp) != nullptr){  
            printf("readStr: %s", buf);
            printf("-------------\n");
            for(int i = 0; i < 2; ++i){
                printf("%c", buffer[i]);
            }
            fflush(stdout);
        }else{
            if(ferror(fp) != 0){
                printf("---ferror----\n");
            }else if(feof(fp) != 0){
                printf("---feof----\n");
            }
        }
        fclose(fp);
    }
}



/**
 * 全缓冲区输出：
 *    1. 当输出字符填满标准I/O缓冲区后，下一个输入字符会触发缓冲区内容写入文件
 *    2. fflush函数主动冲刷缓冲区
*/
void write2fullbuf(){
    FILE* fp = fopen("./a.txt", "r+");
    if(fp != nullptr){
        char buffer[100]{};
        setvbuf(fp, buffer, _IOFBF, 100);
        //输出100个字符到缓冲区中，由于缓冲区没有被填满，所以输出内容没有被写入到文件中
        for(int i = 0; i < 99; ++i){
            fputc('a', fp);
        }
        fputc('\n', fp);
        sleep(2);
        fputc('*', fp);
        sleep(2);
        //关闭文件时会冲洗缓冲区，此时缓冲区内容才会被写入到文件中。
        fflush(fp);
        fclose(fp);
    }
}


/**
 * 定位函数和缓冲区刷新
*/


//压送字符
void putbackChar(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[1024]{};
    setvbuf(fp, buffer, _IOFBF, 1024);
    if(fp != nullptr){
        int getC = fgetc(fp);
        if(getC == EOF){    
            printf("---end of file---\n");
            //读取到文件末尾后，压送字符后，可以再次读取
            //一次成功的ungetc调用会清除该流的文件结束标志。
            ungetc('\n', fp);
        }
        //ungetc压送回字符时，并没有将它们写到底层文件或设备上，只是将它们写回标准I/O库的流缓冲区中。
        //ungetc压送回的字符，存储在单独的缓冲区中
        ungetc('\n', fp);
        ungetc('a', fp);
        ungetc('b', fp);
        ungetc('c', fp);
        ungetc('\n', fp);
        ungetc('d', fp);
        ungetc('e', fp);
        ungetc('f', fp);

        //定位操作，会导致不读取压送字符所在的缓冲区。
        //如果没有定位操作，先读取压送字符所在的缓冲区，后读取I/O缓冲区
        rewind(fp); 
        while(true){
            char buf[1024]{};
            if(fgets(buf, 1024, fp) != nullptr){
                printf("readStr: %s", buf);
                fflush(stdout);
            }else{
                break;
            }
        }

        //回到文件开头
        rewind(fp);
        while(true){
            char buf[1024]{};
            if(fgets(buf, 1024, fp) != nullptr){
                printf("readStr: %s", buf);
                fflush(stdout);
            }else{
                break;
            }
        }
    }
    fclose(fp);
}


//押送字符回压送字符缓冲区并不改变当前位置位置。
void filePosWithUngetc(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    if(setvbuf(fp, nullptr, _IOFBF, 0) == 0){
        printf("--->缓冲设置成功....\n");
    }
    if(fp != nullptr){
        int i = 0;
        char buf[1024];
        printf("filePosition: %ld\n", ftell(fp));
        while(fgets(buf, 1024, fp) != nullptr){
            printf("filePosition now: %ld\n", ftell(fp));
            printf("-->readStr: %s", buf);
            if(i == 0){
                //压送字符到压送字符缓冲区，并不会影响当前文件指示位置
                ungetc('\n', fp);
                ungetc('a', fp);
                ungetc('b', fp);
                ++i;
            }
        }
        printf("-------------------\n");
        rewind(fp); //定位到文件开头
        printf("filePosition: %ld\n", ftell(fp));
        while(fgets(buf, 1024, fp) != nullptr){
            printf("filePosition: %ld\n", ftell(fp));
            printf("-->readStr: %s", buf);
        }
    }
    fclose(fp);
}



/**
 * freopen：在一个指定的流上打开一个指定的文件
 *      若该流已经打开，则先关闭该流。若该流已经定向，则使用freopen清除该定向。
 *      此函数一般用于将一个指定的文件打开为一个预定义的流：stdin, stdout, stderr。
 *      可以用于I/O重定向
*/
void freopen_test(){
    FILE* fp = freopen("./a.txt", "w+", stdout);
    setvbuf(stdout, nullptr, _IOLBF, 1024);
    if(fp != nullptr){
        while(true){
            char buf[1024]{};
            if(fgets(buf, 1024, stdin) != nullptr){
                fprintf(stdout, "readStr: %s", buf);
                fflush(stdout);
            }else{
                break;
            }
        }
    }
}


/**
 * fdopen函数取一个已有的文件描述符，并使一个标准的I/O流与该描述符相结合。
 * 对于fdopen, type参数的意义稍有区别，因为该描述符已经被打开。
*/
void fdopen_test(){
    FILE* stdinfp = fdopen(0, "r");
    FILE* stdoutfp = fdopen(1, "w+");
    while(true){
        char buf[1024]{};
        if(fgets(buf, 1024, stdinfp)){
            fprintf(stdoutfp, "readStr: %s", buf);
        }else{
            break;
        }
    }
}


/*
    snprintf:
        最多容纳n-1个字符，最后一位一定是'\0'
        多余的输出字符被丢弃
*/
void snprintfTest(){
    char buf[5]{};
    snprintf(buf, 5, "hello");
    sleep(1);
}
 


/*
    格式化输入：
        0. 接收的内容是文本字符
        1. 格式化字符串内指定解释的类型
        2. 读取文本字符内容，依序将文本字符转换为相应的解释类型
*/
void scanfTest(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    int a, b;
    char buf[100]{};
    fscanf(fp, "%d %d %s", &a, &b, buf);
    fprintf(stdout, "a==%d, b==%d, c==%s\n", a, b, buf);

    std::array<char, 10> Array{'1', '2', ' ', '3', '4', ' ', '5', '6', '7'};
    sscanf(Array.data(), "%d %d %s", &a, &b, buf);
    fprintf(stdout, "a==%d, b==%d, c==%s\n", a, b, buf);
}

int main(int argc, char* argv[]){
    read_fullbuf2();

    return 0;
}
