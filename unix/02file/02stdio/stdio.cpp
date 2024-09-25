//
// Created by 78472 on 2023/4/9.
//
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <array>
#include <thread>


/**
 * 常用操作
 *      设置缓冲区类型
 *          int setvbuf(FILE *fp, char *buf, int mode, size_t size);
 *              返回值：成功返回0；出错返回非0；
 * 
 *      打开流：
 *          FILE *fopen(const char *pathname, const char *type);
 *          FILE *freopen(const char *pathname, const char *type, FILE *fp);
 *          FILE *fdopen(int fd, const char *type);
 *              返回值：若成功返回文件指针；若失败返回nullptr;
 * 
 *      读取字符
 *          int fgetc(FILE *fp);
 *              返回值：若成功，返回下一个字符；若已到达文件尾端或出错，返回EOF。
 *      
 *      读取一行：最多读取n-1个字符，一定以nullptr结尾。
 *          char *fgets(char *buf, int n, FILE *fp);
 *              返回值：若成功，返回buf；若已到达文件尾端或出错，返回nullptr.
 * 
 *       读取任意数据
 *          size_t fread(void *ptr, size_t size, size_t nobj, FILE *fp);
 *              返回值：读或写的对象数
 * 
 * 
 *       输出字符
 *           int fputc(int c, FILE *fp);
 *              返回值：若成功，返回c；若出错，返回EOF。
 * 
 *       输出一行：其实是输出以nullptr结尾的字符串
 *           int fputs(const char *str, FILE *fp);
 *              返回值：若成功，返回非负值；若出错，返回EOF。
 *          
 *       写入任意数据
 *           size_t fwrite(const void *ptr, size_t size, size_t nobj, FILE *fp);
 *               返回值：读或写的对象数
 *          
 *       格式化输出：
 *          int printf(const char *format, ...);
 *          int fprintf(FILE *fp, const char *format, ...);
 *          int dprintf(int fd, const char *format, ...);
 *          int snprintf(char *buf, size_t n, const char *format, ...);     
 *       
 * 
 *       定位流：
 *          long ftell(FILE *fp);
 *                  返回值：若成功，返回当前文件位置指示；若出错，返回-1L.
 *          int fseek(FILE* fp, long offset, int whence);
 *                  返回值：若成功，返回0; 若出错，返回-1.
 *          void rewind(FILE *fp);
 * 
 * 
 *       出错函数：
 *           int ferror(FILE *fp);
 *           int feof(FILE *fp);
 *                  2个函数返回值：若条件为真，返回非0(真)；否则，返回0(假)；
 *           void clearerr(FILE *fp);
 *                  每个流在FILE对象中维护了2个标志：出错标志、文件结束标志。clearerr可以清除这2个标志。
*/



/**
 * 按字符读取
 *      读取字符
 *          int fgetc(FILE *fp);
 *              返回值：若成功，返回下一个字符；若已到达文件尾端或出错，返回EOF。
 *       输出字符
 *           int fputc(int c, FILE *fp);
 *              返回值：若成功，返回c；若出错，返回EOF。
 * 
*/
void readChar(){
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        int charInt;
        while((charInt = fgetc(fp)) != EOF){   //读取到文件末尾或出错返回EOF
           fputc(charInt, stdout);
        }
        //fgetc返回EOF有可能是读到文件末尾或者读取文件出错
        if(ferror(fp)){
            fprintf(stdout, "fgetc error ...\n");
        }else if(feof(fp)){
            fprintf(stdout, "end of file ....\n");
        }
        fclose(fp);
    }
}


/**
 * 按行读取: 
 *      读取一行：读取到换行符或读取到n-1个字符，一定以nullptr结尾。
 *             char *fgets(char *buf, int n, FILE *fp);
 *                  返回值：若成功，返回buf；若已到达文件尾端或出错，返回nullptr.
 * 
 *      输出一行：其实是输出以nullptr结尾的字符串。
 *             int fputs(const char *str, FILE *fp);
 *                  返回值：若成功，返回非负值；若出错，返回EOF。
*/
void readLine(){
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        char buf[1024];
        while(fgets(buf, 1024, fp) != nullptr){
            fputs(buf, stdout);
        }
        //fgets读取到文件末尾或者读取出错返回nullptr
        if(ferror(fp) != 0){    
            fprintf(stdout, "fgets error ...\n");
        }else if(feof(fp) != 0){    
            fprintf(stdout, "end of file ....\n");
        }
        fclose(fp);
    }
}


/**
 * 读取任意字节数
 *      读写任意数据
 *             size_t fread(void *ptr, size_t size, size_t nobj, FILE *fp);
 *             size_t fwrite(const void *ptr, size_t size, size_t nobj, FILE *fp);
 *                      返回值：读或写的对象数
 * 注：
 *      fread函数尝试读取nobj个对象，每个对象的大小为size字节，返回实际读取到的对象数(可能小于期望数)。
 *      
*/
void readNumChar(void){
    FILE *fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        char buf[1024];
        while(true){
            size_t nRead = fread(buf, 7, 7, fp);    //返回读取到的对象数；
            fprintf(stdout, "nRead: %lu\n", nRead);
            for(int i = 0; i < 10; ++i){
                fprintf(stdout, "buf[%d] == %c\n", i, buf[i]);
            }
            if(nRead == 0){   //读取到0个对象，
                break;
            }
        }

        if(ferror(fp)){    
            fprintf(stdout, "fgets error ...\n");
        }else if(feof(fp)){    
            fprintf(stdout, "end of file ....\n");
        }
        fclose(fp);
    }
}


/**
 * 当缓冲区无数据时，从文件读取数据到缓冲区。如果从文件读取不到任何数据到缓冲区，则为读到文件末尾；
 *      1. 设置流为行缓冲读取
 *      2. 调用fgetc读取一个字符时，会尝试读满整个行缓冲区。如果本次读取，读取不到数据，则对流设置文件结束标志
 *      3. 再次调用fetc时，从缓冲区读取数据。
 *      4. 当缓冲区无数据时，再尝试从文件读取数据，如果读取不到数据，则对流设置文件结束标志      
*/
void readChar_linebuf(){
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        // 设置缓冲模式
        char buffer[1024]{};
        if(setvbuf(fp, buffer, _IOLBF, 1024) == 0){
            printf("---setvbuf successfully---\n");
        }
        // 打印fd和FILE的当前偏移量
        fprintf(stdout, "---current file loc: %ld---\n", lseek(fileno(fp), 0, SEEK_CUR));
        fprintf(stdout, "---current FILE loc: %ld---\n", ftell(fp));
        // 读取一个字符，观察缓冲区
        int charInt = fgetc(fp);

        fputs("--buffer content start--\n", stdout);
        for(int i = 0; i < 10; ++i){
            fprintf(stdout, "buf[%d] == %c\n", i, buffer[i]);
        }
        fputs("--buffer content end--\n", stdout);

        fprintf(stdout, "fileLocation: %ld, FILELocation: %ld, readChar: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);
        while((charInt = fgetc(fp)) != EOF){ 
           fprintf(stdout, "fileLocation: %ld, FILELocation: %ld, readChar: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);
        }

        if(ferror(fp)){
            fprintf(stdout, "fgetc error ...\n");
        }else if(feof(fp)){
            fprintf(stdout, "end of file ....\n");
        }
        fclose(fp);
    }
}


/**
 *  1. 设置为行缓冲读取
 *  2. 第一次调用fgets时，尝试读满整个缓冲区。如果第一次读取遇到文件末尾，则返回nullptr并设置流结束标志。
 *  3. 再次调用fgets时，尝试从缓冲区读取数据。如果缓冲区数据不够，则再尝试从文件读取数据到缓冲区。
 *     如果读取到文件末尾，但是没有读取到换行符或者足够的数据，本次读取依然是成功的。下一次读取返回nulllptr，设置流结束标志。
*/
void readLine_Linebuf(){
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        // 设置缓冲区
        char buffer[1024]{};
        memset(buffer, 0, 1024);
        if(setvbuf(fp, buffer, _IOLBF, 1024) == 0){
            printf("---setvbuf successfully---\n");
        }
        // fgets触发缓冲区读取
        char readBuf[10];
        fgets(readBuf, 10, fp);
        // 打印缓冲区内容
        fputs("--buffer content start--\n", stdout);
        for(int i = 0; i < 20; ++i){
            fprintf(stdout, "buffer[%d] == %c\n", i, buffer[i]);
        }
        fputs("--buffer content end--\n", stdout);
        // 观察文件偏移量
        fprintf(stdout, "fileLocation: %ld, FILELocation: %ld, readBuf: %s\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), readBuf);
        while(fgets(readBuf, 10, fp) != nullptr){
             fprintf(stdout, "fileLocation: %ld, FILELocation: %ld, readBuf: %s\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), readBuf);
        }
    }

    if(ferror(fp)){
        fprintf(stdout, "fgetc error ...\n");
    }else if(feof(fp)){
        fprintf(stdout, "end of file ....\n");
    }
    fclose(fp);
}


/**
 * 行缓冲下输出时机：
 *      1. 当I/O缓冲区被填满，当下一个字符到来时，冲洗缓冲区。
 *      2. 当遇到换行符时，冲洗缓冲区 
*/
void write_linebuf(){
    char buffer[8];
    if(setvbuf(stdout, buffer, _IOLBF, 8) == 0){
        printf("---缓冲区设置成功---\n");
    }
    
    for(int i = 0; i < 8; ++i){
        fputc('a', stdout);
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    fputc('-', stdout);     //写入此字符，导致缓冲区被刷新，缓冲区被刷新后填入'_'
    std::this_thread::sleep_for(std::chrono::seconds(3));
    fputc('b',  stdout);
    fputc('\n', stdout) ;
}


/**
 * 全缓冲读取过程：
 *      1. 标准库尝试从文件读取内容填满I/O缓冲区buffer，如果读取到文件末尾则返回。
 *      2. fgets函数从buffer中读取内容，如果读到换行符或者超过fgets缓冲区大小或者读到文件末尾，则fgets函数返回。
 *      3. 否则继续上述读取操作
*/
void read_fullbuf(){
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        char buffer[100]{};
        setvbuf(fp, buffer, _IOFBF, 100);  
        /**
         * 如果I/O缓冲区设置的比较小，fgets函数会导致底层对文件进行多次读取
         * 每次读取都尝试填满缓冲区
         * 只有缓冲区没有数据时才会触发读取数据到缓冲区
        */
       char buf[5];
        if(fgets(buf, 5, fp) != nullptr){  
            printf("readStr: %s", buf);
            printf("--buffer content start--\n");
            for(int i = 0; i < 5; ++i){
                printf("%c", buffer[i]);
            }
            printf("--buffer content end--\n");
            printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));
            fflush(stdout);
        }
       
        if(ferror(fp) != 0){
            printf("---ferror----\n");
        }else if(feof(fp) != 0){
            printf("---feof----\n");
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
    FILE* fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        char buffer[10]{};
        setvbuf(fp, buffer, _IOFBF, 10);
        //输出100个字符到缓冲区中，由于缓冲区没有被填满，所以输出内容没有被写入到文件中
        for(int i = 0; i < 5; ++i){
            fputc('a', fp);
            fputc('\n', fp);
        }
        sleep(2);
        fputc('a', fp);
        sleep(2);
        fputc('*', fp);
        fflush(fp);
        fclose(fp); //关闭文件时会冲洗缓冲区，此时缓冲区内容才会被写入到文件中。
    }
}


/**
 *  1. 读取和写入使用同一个缓冲区？
 *  2. 以读写方式打开文件时，读写之间要使用fseek，重新定位位置。
*/
void readWrite(){
    FILE *fp = fopen("a.txt", "r+");
    if(fp != nullptr){
        char buffer[1024];
        setvbuf(fp, buffer, _IOLBF, 1024);

        fgetc(fp);
        printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));
        fgetc(fp);
        printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));
    
        /**
         * 转换读写方式时调用fseek，定位到当前位置
        */
        fseek(fp, 0, SEEK_CUR);
        printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));

        fputs("hello", fp);
        printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));
        fflush(fp);
        printf("file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));

        fclose(fp);
    }
}


/**
 * 读取到文件末尾后，压送字符后，可以再次读取
 * 一次成功的ungetc调用会清除该流的文件结束标志。
*/
void ungetc_test(){
    FILE* fp = fopen("a.txt", "r+");
    char buffer[1024]{};
    setvbuf(fp, buffer, _IOFBF, 1024);

    if(fp != nullptr){
        int charInt = fgetc(fp);
        if(charInt == EOF){    
            printf("1-file loc: %ld, File loc: %ld, charInt: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);
            /**
             * ungetc压送回字符时，并没有将它们写到底层文件或设备上，只是将它们写回标准I/O库的流缓冲区中。
             * ungetc压送回的字符，存储在单独的缓冲区中
            */
            ungetc('a', fp);
            ungetc('b', fp);
            fputs("--buffer content start--\n", stdout);
            for(int i = 0; i < 2; ++i){
                fprintf(stdout, "buffer[%d] == %c\n", i, buffer[i]);
            }
            fputs("--buffer content end--\n", stdout);
            printf("2-file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));
        }

        /**
         * 定位操作，会导致不读取压送字符所在的缓冲区。
         * 如果没有定位操作，先读取压送字符所在的缓冲区，后读取I/O缓冲区
        */
        fseek(fp, 0, SEEK_CUR);
        printf("3-file loc: %ld, File loc: %ld, charInt: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);

        charInt = fgetc(fp);
        printf("4-file loc: %ld, File loc: %ld, charInt: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);

        charInt = fgetc(fp);
        printf("5-file loc: %ld, File loc: %ld, charInt: %c\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp), charInt);

        fgetc(fp);
        printf("6-file loc: %ld, File loc: %ld\n", lseek(fileno(fp), 0, SEEK_CUR), ftell(fp));

        if(ferror(fp)){    //读取文件错误
            fprintf(stdout, "fgets error ...\n");
        }else if(feof(fp)){    //读取到文件末尾
            fprintf(stdout, "end of file ....\n");
        }
        fclose(fp);
    }
}


/**
 * freopen：在一个指定的流上打开一个指定的文件
 *      若该流已经打开，则先关闭该流。若该流已经定向，则使用freopen清除该定向。
 *      此函数一般用于将一个指定的文件打开为一个预定义的流：stdin, stdout, stderr。
 *      可以用于I/O重定向
*/
void freopen_test(){
    if(freopen("a.txt", "r+", stdin) != nullptr){
        while(true){
            char buf[1024]{};
            if(fgets(buf, 1024, stdin) != nullptr){
                fprintf(stdout, "readStr: %s", buf);
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
    int fd = open("a.txt", O_WRONLY);
    FILE* fp = fdopen(fd, "w");
    setvbuf(fp, nullptr, _IOLBF, 0);
    while(true){
        char buf[1024]{};
        if(fgets(buf, 1024, stdin)){
            fputs(buf, fp);
        }else{
            break;
        }
    }
}


/**
 * snprintf:
 *      最多容纳n-1个字符，最后一位一定是'\0'
 *      多余的输出字符被丢弃
 */
void snprintfTest(){
    char buf[5] = {'a', 'b', 'c', 'd', 'e'};
    snprintf(buf, 5, "%s:%s\n", "hello", "world");
    fputs(buf, stdout);
}
 


int main(int argc, char* argv[]){

    snprintfTest();

    return 0;
}



