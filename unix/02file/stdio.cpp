//
// Created by 78472 on 2023/4/9.
//

#include <iostream>
#include <cstdio>
#include <unistd.h>

//读取字符
void readChar(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[1024]{};
    setvbuf(fp, buffer, _IOFBF, 1024);
    if(fp != nullptr){  //成功打开文件
        int getC;
        while((getC = fgetc(fp)) != EOF){   //读取到文件末尾时返回EOF
            printf("%c", getC);
        }
        if(ferror(fp) != 0){    //读取文件错误
            std::cout << "ferror..." << std::endl;
        }else if(feof(fp) != 0){    //读取到文件末尾
            std::cout << "feof...." << std::endl;
        }
    }
}


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


/*
    行缓冲下输出时机：
        1. 当I/O缓冲区被填满，当下一个字符到来时，冲洗缓冲区。
        2. 当遇到换行符时，冲洗缓冲区
*/
void write2linebuf(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[3]{};
    if(setvbuf(fp, buffer, _IOLBF, 3) == 0){
        printf("---缓冲区设置成功---\n");
    }
    if(fp != nullptr){
        fputc('a', fp);
        fputc('\n', fp);
        fputc('b', fp);
        fputc('c', fp);
        fputc('d', fp);
        fputc('e', fp);
        fputc('f', fp);
        fputc('g', fp);
        fputc('a', fp);
        fputc('\n', fp);
    }
}


//行缓冲：还是试图读满缓冲区
void readFromLinebuf(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[3]{};
    if(setvbuf(fp, buffer, _IOLBF, 3) == 0){
        printf("---setvbuf successfully---\n");
    }
    if(fp != nullptr){
        char buf[1024];
        while(true){
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


//输入换行符后，才会进行读取
void readFromLineBufStdin(){
    char buffer[1024]{};
    setvbuf(stdin, buffer, _IOLBF, 1024);
    char buf[1024]{};
    while(fgets(buf, 1024, stdin) != nullptr){
        printf("readStr: %s\n", buf);
        fflush(stdout);
    }
}


void readWriteFromLineBuf(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[1024]{};
    setvbuf(fp, buffer, _IOLBF, 1024);
    if(fp != nullptr){
        fputc('a', fp);
        fputc('b', fp);
        fputc('c', fp);
        fprintf(stdout, "filePosition: %ld\n", ftell(fp));
        fseek(fp, 0, SEEK_CUR);     //读取转换时要加定位操作，定位操作会刷新缓冲区
        fprintf(stdout, "filePosition: %ld\n", ftell(fp));
        char buf[1024]{};
        if(fgets(buf, 1024, fp) != nullptr){
            fprintf(stdout, "readStr: %s", buf);
            fflush(stdout);
        }
    }
}


//全缓冲区输出：
//      1. 当输出字符填满标准I/O缓冲区后，下一个输入字符会触发缓冲区内容写入文件
//      2. fflush函数主动冲刷缓冲区
void write2fullbuf(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[100]{};
    setvbuf(fp, buffer, _IOFBF, 100);
    if(fp != nullptr){
        //输出100个字符到缓冲区中，由于缓冲区没有被填满，所以输出内容没有被写入到文件中
        for(int i = 0; i < 100; ++i){
            if(i < 99){
                fputc('a', fp);
            }else if(i == 99){
                fputc('a', fp);
            }
        }
        fputc('\n', fp);
    }
    //关闭文件时会冲洗缓冲区，此时缓冲区内容才会被写入到文件中。
    fflush(fp);
    fclose(fp);
}


/*
    读取过程：
        标准库尝试从文件读取内容填满I/O缓冲区buffer，如果读取到文件末尾则返回。
        fgets函数从buffer中读取内容，如果读到换行符或者超过fgets缓冲区大小，则fgets函数返回。
        否则继续上述读取操作
*/
void readFromFullbuf(){
    FILE* fp = fopen("/home/lhy/project/stl/unix/02file/a.txt", "r+");
    char buffer[5]{};
    setvbuf(fp, buffer, _IOFBF, 5); //设置全缓冲测试缓冲区
    if(fp != nullptr){
        while(true){
            char buf[1024];
            if(fgets(buf, 1024, fp) != nullptr){  //由于I/O缓冲区设置的比较小，fgets函数会导致底层对文件进行多次读取
                printf("readStr: %s", buf);
            }else{
                if(ferror(fp) != 0){
                    printf("---ferror----\n");
                }else if(feof(fp) != 0){
                    printf("---feof----\n");
                }
                break;
            }
        }
    }
    fclose(fp);
}


int main(int argc, char* argv[]){
    write2fullbuf();

    return 0;
}

































