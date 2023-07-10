//
// Created by 78472 on 2021/12/11.
//

#ifndef STL_CFILE_H
#define STL_CFILE_H

/*
#打开、关闭文件
    FILE *fopen(const char *filename, const char *mode);
    int fclose(FILE *stream);


#读取文件内容到数组，输出数组内容到文件
    size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
#从文件读取字符、输出字符
    int fgetc(FILE *stream);
    int getc(FILE *stream);
    int getchar(void);
    int putc(int char, FILE *stream);
    int fputc(int char, FILE *stream);
    int putchar(int char);
#从文件读取字符串、输出字符串
    char *fgets(char *str, int n, FILE *stream);
    char *gets(char *str);
    int fputs(const char *str, FILE *stream);
    int puts(const char *str);
#字符串打印、文件打印
    int sprintf(char *str, const char *format, ...);
    int snprintf(char * str, size_t size, const char * format, ... );
    int vsprintf(char *str, const char *format, va_list arg);

    int fprintf(FILE *stream, const char *format, ...);
    int printf(const char *format, ...);
    int vfprintf(FILE *stream, const char *format, va_list arg);


#是否到达文件末尾、是否发生错误、打印错误信息、清除错误信息、刷新缓冲区
    int feof(FILE *stream);
    int ferror(FILE *stream);
    void perror(const char *str);
    void clearerr(FILE *stream);
    int fflush(FILE *stream);


#文件位置操作
    int fgetpos(FILE *stream, fpos_t *pos);
    int fsetpos(FILE *stream, const fpos_t *pos);
    long int ftell(FILE *stream);
    int fseek(FILE *stream, long int offset, int whence);
    void rewind(FILE *stream);


#设置缓冲区
    void setbuf(FILE *stream, char *buffer);
    int setvbuf(FILE *stream, char *buffer, int mode, size_t size);


#格式化读取from字符串、文件
    int sscanf(const char *str, const char *format, ...);
    int fscanf(FILE *stream, const char *format, ...);
    int scanf(const char *format, ...);

 */

#include <iostream>
#include <string>
#include <cstdio>
#include <exception>
#include <array>

using namespace std;


/*
 * r： 读取，  文件必须存在，  读取位置在文件开头
 * r+：读和写， 文件必须存在， 读写位置在文件开头
 *
 * w： 清空而后涂写，有必要时创建， 读写位置在开头
 * w+：先清空再读写，有必要才创建， 读写位置在开头
 *
 * a： 追加，有必要才创建，写位置在尾端
 * a+：有必要才创建. 尾端更新，即刚开始读写位置都在尾端
 */
class cfile{
private:
    const string _fileName;
    FILE *stream = nullptr;

public:
    enum position{
        start = SEEK_SET,
        current = SEEK_CUR,
        end = SEEK_END
    };

public:
    explicit cfile(const string& filename, const string& openMode = "r+") : _fileName(filename){
        stream = fopen(_fileName.c_str(), openMode.c_str());
        if(stream == nullptr){
            perror("cfile open error");
            throw std::exception();
        }
    }

    ~cfile(){
        fclose(stream);
    }

public:
    /*
     * 从文件读取size个字节的数据到指定的buffer中，返回实际读取到的字节数
     * 读取错误或者读取一开始就遇到end-of-file，则返回0
     */
    size_t readBytes(void *ptr, size_t size);

    /*
     * 从文件读取一个字符，如果读取成功则返回true, c中为读取到的字符，
     * 如果读取失败则返回false，c中的内容不改变
     *
     * 读取失败：读取操作失败或者读取一开始就遇到了end-of-file
     */
    bool readChar(char &c);

    /*
     * 从文件读取一行字符串, 读取但是不存储newline, 最多读取readMaxCount - 1个字符，自动添加'\0'结束符
     * 如果没有读取到字符（读取错误或遇到EOF）则返回false。
     */
    bool readLine(char *str, int readMaxCount);

    /*
     * 向文件写入指定的字节数
     * 返回值为实际写入的字节数，如果写入操作失败，则返回的值和实际写入的值不相等
     */
    size_t writeBytes(void *ptr, size_t size);

    //向文件写入一个字符
    bool writeChar(char c);

    //向文件写入一个string
    bool write(string& str);

    //向文件写入格式化字符串
    bool write(const char* format, ...);

    //获取文件当前位置标识符
    long int getPos();

    //移动文件当前位置标识符到指定的位置
    bool setPos(long int offset, enum position pos);

    //是否到达文件末尾
    bool eof();

    //文件操作是否发生错误
    bool error();

    //清除文件操作导致的错误标志
    void clearError();

    //刷新缓冲区
    bool flush();
};

#endif //STL_CFILE_H
