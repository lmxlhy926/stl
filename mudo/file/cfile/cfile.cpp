//
// Created by 78472 on 2021/12/11.
//

#include "cfile.h"
#include <cstdarg>


/*
 * 功能: 读取任意数量的字符
 * 返回: 返回实际读取到的字节数
 */
size_t cfile::readBytes(void *ptr, size_t size) {
    //fread返回实际从文件中读取到的元素数，元素数大于等于0.
    //如果返回0，则可能遇到文件结尾或者读取错误，需要使用feof,ferror单独来判断
    return fread(ptr, 1, size, stream);
}


/*
 * 功能: 读取一个字符
 * 返回: 读取是否成功
 */
bool cfile::readChar(char &c) {
    //读取成功：以无符号char强制转换为int的形式返回读取的字符
    //读取失败；如果达到文件末尾或发生错误，则返回EOF
    int retChar;
    if((retChar = fgetc(stream)) == EOF)
        return false;
    else{
        c = static_cast<char>(retChar);
        return true;
    }
}


/*
 * 功能：试图读取一行字符串(读取停止条件：1. 到max-1个字符;  2. 读取到一个newline;  3. 读取到文件末尾)
 * 返回：没有读取到任何字符，或者读取错误则返回 false.
 */
bool cfile::readLine(char *str, int readMaxCount) {
/*
    最多读取(n-1)个字符
    读取到一个newline，或者达到文件末尾时，它会停止，视具体情况而定.
    读取到的newline会被存储到buffer，buffer的最后一个字符后会自动存储一个null byte(\0)

    如果成功，该函数返回相同的str参数
    读取错误或者读取到文件末尾（即没有读取到一个字符），返回nullptr
 */
    if(fgets(str, readMaxCount, stream) == nullptr)  return false;
    return true;
}


/*
 * 功能：写入任意字节数到文件中
 * 返回：实际写入的字节数
 */
size_t cfile::writeBytes(void *ptr, size_t size) {
    //返回实际写入的元素数，如果发生错误则实际写入的元素数和期望写入的元素数不相等
    return fwrite(ptr, 1, size, stream);
}


/*
 * 功能：向文件写入一个字符
 * 返回：是否成功写入
 */
bool cfile::writeChar(char c) {
    //将char c转换为一个unsigned c写入文件流
    //如果没有发生错误，则返回被写入的字符。如果发生错误，则返回EOF，并设置错误标识符。
    if(fputc(c, stream) == EOF)  return false;
    return true;
}


/*
 * 功能：写入一个字符串
 * 返回：是否写入成功
 */
bool cfile::write(string& str) {
    //将字符串（不包含字符串的null byte）写入文件流，
    //成功：该函数返回一个非负值; 失败：返回EOF
    if(fputs(str.c_str(), stream) == EOF) return false;
    return true;
}


/*
 * 功能：向文件写入格式化字符串
 * 返回：写入是否成功
 */
bool cfile::write(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int retSize = vfprintf(stream, format, args);   //成功：实际写入的字节数；失败：负数；
    va_end(args);
    return !(retSize < 0);
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
