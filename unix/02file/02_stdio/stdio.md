# 标注I/O库

## 引言

不仅是UNIX，很多其它操作系统都实现了标准I/O库，所以这个库由ISO C标准说明。Single UNIX Specification对ISO C标准库进行了扩充，定义了另外一些接口。

## 流和FILE对象

**`围绕文件描述符的I/O函数`**：当打开一个文件时，即返回一个文件描述符，然后该文件描述符就用于后续的I/O操作。**`围绕流的I/O函数`**：当用标准I/O库打开或创建一个文件时，我们已使一个流与一个文件相关联。

对于ASCII字符集，一个字符用一个字节表示。对于国际字符集，一个字符可用多个字节表示。标准I/O文件流可用于单字节或多字节字符集。**流的定向决定了所读、写的字符是单字节还是多字节的**。当一个流最初被创建时，它并没有定向。如若在未定向的流上使用一个多字节I/O函数，则将该流的定向设置为宽定向的。如若在未定向的流上使用一个单字节I/O函数，则将该流的定向设置为字节定向的。

当打开一个流时，标准I/O函数fopen返回一个指向FILE对象的指针。该对象通常是一个结构，它包含了标准I/O库为管理该流需要的所有信息，包括用于**实际I/O的文件描述符、指向用于该流缓冲区的指针、缓冲区的长度、当前在缓冲区中的字符数以及出错标志等**。在本书中，我们称指向FILE对象的指针为文件指针。

## 标准输入、标准输出和标准错误

对一个进程预定义了3个流，并且这3个流可以自动的被进程使用，它们是：标准输入、标准输出和标准错误。这些流引用的文件与文件描述符STDIN_FILENO、STDOUT_FILENO和STDERR_FILENO所引用的相同。这3个标注I/O流通过预定义文件指针stdin、stdout和stderr加以引用。

## 缓冲

```c
#include <stdio.h>
int setvbuf(FILE* fp, char* buf, int mode, size_t size);
	返回值：若成功，返回0；若出错，返回非0。
mode参数：
	_IOFBF：全缓冲，在填满标准I/O缓冲区后才进行实际I/O操作
    _IOLBF：行缓冲，当输入和输出中遇到换行符时，标准I/O库执行I/O操作
    _IONBF：无缓冲，标准I/O库不对字符进行缓冲存储
        
标准I/O库提供缓冲的目的是尽可能减少read和write调用的次数。
很多系统默认使用下列类型的缓冲：
	标准错误是不带缓冲的。
	若是指向终端设备的流，则是行缓冲的；否则是全缓冲的。
```

一般而言，应由系统选择缓冲区的长度，并自动分配缓冲区。在这种情况下关闭此流时，标准I/O库将自动释放缓冲区。

## 打开流

```c
#include <stdio.h>
FILE* fopen(const char* pathname, const char* type);
FILE* freopen(const char* pathname, const char* type, FILE* fp);
FILE* fdopen(int fd, const char* type);
	返回值：若成功，返回文件指针；若出错，返回nullptr。
        
这3个函数的区别：
	*fopen：以指定的打开方式打开指定的文件。
	freopen：在指定的流上打开一个指定的文件，如若该流已经打开，则先关闭该流。若该流已经定向，则使用freopen清除该定向。此函数一般用于将一个指定的文件打开为一个预定义的流：标准输入、标准输出、标准错误。
	fdopen函数取一个已有的文件描述符，并使一个标准的I/O流与该描述符相结合。

```

## 关闭流

```c
#include<stdio.h>
int fflush(FILE* fp);
		返回值：若成功，返回0；若出错，返回EOF。
此函数使该流所有未写的数据都被传送至内核。作为一种特殊情形，如果fp是nullptr，则此函数将导致所有输出流被冲洗。
            
int fclose(FILE *fp);
	返回值： 若成功，返回0；若出错，返回EOF
```

## 结束、出错

```c
#include <stdio.h>
int ferror(FILE *fp);
int feof(FILE *fp);
	返回值：若条件为真，返回非0；否则，返回0；
void clearerr(FILE *fp);
```

## 读写流

### 字符

```c
#include <stdio.h>
int fgetc(FILE *fp);
int getchar(void);
	返回值：若成功，返回下一个字符；若到达文件尾端或出错，返回EOF。
        
int fputc(int c, FILE *fp);
int putchar(int c);
	返回值：若成功，返回c；若出错，返回EOF
```

### 行

```c
#include <stdio.h>
char *fgets(char *restrict buf, int n, FILE *restrict fp);
	返回值：若成功，返回buf；若已到达文件尾端或出错，返回nullptr

int fputs(const char *restrict str, FILE *restrict fp);
	返回值：若成功，返回非负值；若出错，返回EOF
```

### 二进制I/O

```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nobj, FILE *fp);
size_t fwrite(const void *ptr, size_t size, size_t nobj, FILE *fp);
	返回值：读或写的对象数
```

## 定位流

```c
#include <stdio.h>
long ftell(FILE *fp);
	返回值：若成功，返回当前文件位置指示；若出错，返回-1L.
int fseek(FILE *fp, long offset, int whence);
	返回值：若成功，返回0；若出错，返回-1.
void rewind(FILE *fp);
```

## 格式化I/O

```c
#include <stdio.h>
int printf(const char *format, ...);
int fprintf(FILE *fp, const char *format, ...);
int dprintf(int fd, const char* format, ...);
	3个函数返回值：若成功，返回输出字节数；若输出出错，返回负值；
        
int sprintf(char* buf, const char* format, ...);
	返回值：若成功，返回存入数组的字符数；若编码出错，返回负值；
int snprintf(char* buf, size_t n, const char* format, ...);
	返回值：若缓冲区足够大，返回将要存入数组的字符数；若编码出错，返回负值。
```



