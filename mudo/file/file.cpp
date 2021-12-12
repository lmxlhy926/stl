//
// Created by 78472 on 2021/12/11.
//

#include "file.h"
#include <cstdarg>
#define LINEMAX 1024


/*
 * 功能: 读取任意数量的字符
 * 返回: 返回实际读取到的字节数
 */
size_t cfile::read(void *ptr, size_t size) {
    return fread(ptr, 1, size, stream);
}


/*
 * 功能: 读取一个字符
 * 返回: 读取是否成功
 */
bool cfile::read(char &c) {
//以无符号char强制转换为int的形式返回读取的字符，如果达到文件末尾或发生错误，则返回EOF
    c = fgetc(stream);
    if(c == EOF)  return false;
    return true;
}


/*
 * 功能：读取一行字符串(未必是一行，如果超过最大字节数或者遇到文件末尾)
 * 返回：读取一行是否成功
 */
bool cfile::getLine(char *str) {
/*
    当读取(n-1)个字符时，或者读取到换行符时，或者达到文件末尾时，它会停止，视具体情况而定
    如果成功，该函数返回相同的str参数
    如果到达文件末尾或者没有读取到任何字符，str的内容保持不变，并返回一个空指针。读取错误时，也返回一个空指针。
 */
    if(fgets(const_cast<char *>(str), LINEMAX, stream) == nullptr)  return false;
    return true;
}


/*
 * 功能：写入任意字节数到文件中
 * 返回：实际写入的字节数
 */
size_t cfile::write(void *ptr, size_t size) {
    return fwrite(ptr, 1, size, stream);
}


/*
 * 功能：向文件写入一个字符
 * 返回：是否成功写入
 */
bool cfile::write(char &c) {
    //如果没有发生错误，则返回被写入的字符。如果发生错误，则返回EOF，并设置错误标识符。
    if(fputc(c, stream) == EOF)  return false;
    return true;
}


/*
 * 功能：写入一个字符串
 * 返回：是否写入成功
 */
bool cfile::write(const char *str) {
    //该函数返回一个非负值，如果发生错误则返回EOF
    if(fputs(str, stream) == EOF) return false;
    return true;
}


/*
 * 功能：向文件写入格式化字符串
 * 返回：成功：实际写入的字节数；失败：负数；
 */
int cfile::write(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int retSize = vfprintf(stream, format, args);
    va_end(args);
    return retSize;
}


/*
 * 功能：获取文件位置
 * 返回：返回文件位置
 */
long int cfile::getPos() {
    return ftell(stream);
}


/*
 * 功能：设置文件位置
 * 返回：是否成功
 */
bool cfile::setPos(long offset, enum position pos) {
    //如果成功则返回0，否则返回非0值。
    if(fseek(stream, offset, pos) != 0) return false;
    return true;
}


/*
 * 功能：是否到达文件末尾
 * 返回：bool值
 */
bool cfile::eof() {
    //当设置了与流关联的文件结束标识符时，该函数返回一个非0值，否则返回0值。
    return feof(stream);
}


/*
 * 功能：文件操作是否发生错误
 * 返回：bool值
 */
bool cfile::error() {
    //如果设置了与流关联的错误标识符，该函数返回一个非零值，否则返回一个零值。
    return ferror(stream);
}


/*
 * 功能：清除文件错误标志
 * 返回：无返回
 */
void cfile::clearError() {
    clearerr(stream);
}


/*
 * 功能：刷新缓冲区
 * 返回：是否成功
 */
bool cfile::flush() {
    //如果成功，该函数返回零值。如果发生错误，则返回 EOF，且设置错误标识符（即 feof）
    return fflush(stream) == 0;
}
