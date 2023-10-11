
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


/**
 * 当程序执行时，其main函数是如何被调用的?
 * 命令行参数是如何传递给新程序的?
 * 典型的存储空间布局是什么样式？
 * 如何分配另外的存储空间？
 * 进程如何使用环境变量？
 * 进程的各种不同终止方式？
 * longjmp和setjmp函数以及它们与栈的交互作用。
 * 
*/

/**
 * C程序的虚拟存储空间布局
 * 栈：
 *      自动变量以及每次函数调用时所需保存的信息都存放在此段中。每次函数调用时，其返回地址以及调用者
 *  的环境信息(如某些机器寄存器的值)都存放在栈中。然后，最近被调用的函数在栈上为其自动和临时变量分配
 *  存储空间。通过以这种方式使用栈，C递归函数可以工作。递归函数每次调用自身时，就用一个新的栈帧，因此
 *  一个函数调用实例中的变量集不会影响另一个函数调用实例中的变量。
 * 
 * 未初始化数据段的内容不存放在磁盘程序文件中。
 * 正文段和初始化数据段需要存放在磁盘程序文件中
*/
/**
 * 通过递归来计算阶乘：
 *      1. 自身调用自身，但是传递的参数发生了变化
 *      2. 对于某个参数，必须能返回某个值，即有个调用终止条件，不能无休止的调用。
 *      3. 之所以能工作是因为，虽然调用的是同一个函数，但是函数调用实例中的变量集不同，即每个函数有独立的栈帧。
*/
int factorial(int n){
    if(n < 0){
        return -1;
    }else if( n == 0){  //终止参数
        return 1;
    }else{
        return n * factorial(n - 1);    //递归调用
    }
}


/**
 * 共享库使得可执行文件中不再需要包含公用的库函数，而只需在所有进程都可引用的存储区中保存这种库例程的一个副本。
 * 程序第一次执行或者第一次调用某个库函数时，用动态链接方法将程序和动态库函数相链接。这减少了每个可执行文件的
 * 长度，但增加了一些运行时间开销。这种时间开销发生在该程序第一次被执行时，或者每个共享库函数第一次被调用时。
 * 共享库的另一个优点是可以用库链接的新版本代替老版本而无需对使用该库的程序重新编辑链接(假设参数的数目和类型都没有发生改变)。
 * 
 * size命令报告正文段、数据段、和bss段的长度(以字节为单位)
*/


/**
 * ISO C 说明了3个用于存储空间动态分配的函数
 *      malloc：分配指定字节数的存储区。此存储区中的初始值不确定。
 *      calloc：为指定数量指定长度的对象分配存储空间。该空间的每一位都初始化为0.
 *      realloc：增加或减少以前分配区的长度。当增加长度时，可能需将以前分配区的内容移到另一个足够大的区域，
 *               以便在尾端提供增加的存储区，而新增区域内的初始值则不确定。
 * 
 *  这3个分配函数返回的指针一定是适当对齐的，使其可用于任何数据对象。
 * 
 *  realloc函数最常用的用法是扩充存储空间。如果在该存储空间有足够的空间可供扩充，则可在原存储区位置上向高地址
 *  方向扩充，无需移动任何原先的内容，并返回与传给它相同的指针值。如果原存储区没有足够的存储空间，则realloc分配
 *  另一个足够大的存储区，将现存的元素内容复制到新分配的存储区。然后，释放原存储区，返回新分配区的指针。因为这种
 *  存储区可能会移动位置，所以不应当使任何指针指在该区中。
 * 
 *  大多数实现所分配的存储空间比所要求的要稍大一些，额外的空间用来记录管理信息--分配块的长度、指向下一个分配块的
 *  指针等。这就意味着，如果超过一个已分配区的尾端或者在已分配区起始位置之前进行写操作，则会更改另一块的管理记录
 *  信息。这种类型的错误是灾难性的，但是因为这种错误不会很快就暴露出来，所以也就很难发现。
 * 
 *  如果不调用free函数释放不再使用的空间，那么进程地址空间长度就会慢慢增加，直至不再有空闲空间。此时，由于过度的
 *  换页开销，会造成性能下降。
 * 
 *  动态内存分配器维护着一个进程的虚拟内存区域，称为堆(heap)。堆是一个请求二进制零的区域，它紧接在未初始化的数据
 *  区域后开始，并向上生长。分配器将堆视为一个不同大小的块的集合来维护。每个块就是一个连续的虚拟内存片，要么是已分配
 *  的，要么是空闲的。已分配的块显式地保留为供应用程序使用。空闲块可用来分配。
 *  
 *  动态内存分配器，例如malloc，可以通过使用mmap和munmap函数，显式地分配和释放堆内存。
 *  ptr参数必须指向一个从malloc、calloc或者realloc获得的已分配块的起始位置。否则，free的行为是未定义的。
 * 
*/
/**
 * #include <stdlib.h>
 * void *malloc(size_t size);
 * void *calloc(size_t nobj, size_t size);
 * void *realloc(void* ptr, size_t newsize);
 *      3个函数返回值：若成功，返回非空指针；若出错，返回NULL.
 * void free(void *ptr);
 * 
*/
void multiAlloc(){
    int* mallocIntPtr = reinterpret_cast<int *>(malloc(sizeof(int) * 10));
    for(int i = 0; i < 10; ++i){
        printf("%d\n", *(mallocIntPtr + i));
    }
    free(mallocIntPtr);
    printf("-------------------\n");

    int* callocIntPtr = reinterpret_cast<int *>(calloc(10, sizeof(int)));
    for(int i = 0; i < 10; ++i){
        printf("%d\n",*(callocIntPtr + i));
        *(callocIntPtr + i) = i;
    }
    printf("-------------------\n");

    int* reallocIntPtr = reinterpret_cast<int*>(realloc(callocIntPtr, 15 * sizeof(int)));
    for(int i = 0 ; i < 15; ++i){
        printf("%d\n", *(reallocIntPtr + i));
    }

    printf("---------------end---------------\n");
}


//通过内存映射，可以用操作内存的方式，操作文件内容
void mmapTest(){
    int fd = open("/home/lhy/project/stl/unix/04processEnv/a.txt", O_RDWR);
    char *ptr = static_cast<char *>(mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    for(int i = 0; i < 7; ++i){
        printf("%c\n", *(ptr + i));
        *(ptr + i) = 49 + i;
    }

    FILE* fp = fdopen(fd, "a+");
    char buf[1024]{};
    printf("%s\n", fgets(buf, sizeof(buf), fp));
}




int main(int argc, char* argv[]){

    return 0;
}



