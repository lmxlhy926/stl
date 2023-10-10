
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
 * int main(int argc, char* argv[])
 * argc是命令行参数的数目，argv是指向参数的各个指针所构成的数组。
 * 
 * 当内核执行C程序时(使用一个exec函数)，在调用main前先调用一个特殊的启动例程。可执行程序文件
 * 将此启动例程指定为程序的起始地址。启动例程从内核取得命令行参数和环境变量值，然后为按上述方式
 * 调用main函数做好安排。
 * 
 * 启动例程是这样编写的，使得从main返回后立即调用exit函数。如果启动例程以C代码形式表示，实际该例程
 * 常常是用汇编语言编写的，则它调用main函数的形式可能是：
 * exit(main(argc, argv))
*/


/**
 *函数exit
 *      进程有5种正常终止和3种异常终止方式
 *      不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。
 * 
 *      常见3种正常终止方式：
 *          1. 在main函数内执行return语句。等效于调用exit。
 *          2. 调用exit函数。此函数由ISO C定义，其操作包括调用各种终止处理程序(终止处理程序在调用atexit函数时登记)，
 *             然后关闭所有标准IO流等。
 *          3. 调用_Exit函数。ISO C定义_Exit，其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法。
 * 
 *          在UNIX系统中,_exit函数和_Exit函数是同义的，并不冲洗标准IO流。_exit函数由exit函数调用，它处理UNIX系统特定细节。
 *          exit(3)是标准C库中的一个函数，而_exit(2)则是一个系统调用。
 * 
 *      常见的异常终止：
 *          1. 调用abort。它产生SIGABRT信号，这是下一种异常终止的一种特例。
 *          2. 当进程接收到某些信号时。信号可由进程自身(如调用abort函数)、其它进程或内核产生。
 *             若进程引用地址空间之外的存储单元、或者除以0，内核就会为该进程产生相应的信号。
 *      
 *      终止进程如何通知其父进程自己是如何终止的？
 *          对于4个终止函数(return、exit、_exit、_Exit)，将其退出状态作为参数传送给函数。在异常终止情况，内核(不是进程本身)产生一个
 *          指示其异常终止原因的终止状态。这里使用了"退出状态"和"终止状态"两个术语，以表示有所区别。在最后调用_exit时，内核将
 *          退出状态转换成终止状态。如果子进程正常终止，则父进程可以获得子进程的退出状态。
 * 
 *      如果父进程在子进程之前终止，又将如何？
 *          对于父进程已经终止的所有进程，它们的父进程都改变为init进程。我们称这些进程由init进程收养。
 *          init被编写成无论何时只要有一个子进程终止，init就会调用一个wait函数取得其终止状态。
 *          这样就防止了在系统中塞满僵死进程。
 * 
 *      如果子进程在父进程之前终止，父进程如何获取子进程的终止状态？
 *          内核为每个终止子进程保存了一定量的信息，所以当终止进程的父进程调用wait或waitpid时，可以得到这些信息。
 *          这些信息至少包括进程ID、该进程的终止状态以及该进程使用的CPU时间总量
 * 
 * 退出函数
 *   #include <stdlib.h>
 *      void exit(int status);
 *      void _Exit(int status);
 *   #include <unistd.h>
 *      void _exit(int status);
 * 
 *      使用不同头文件的原因是exit和_Exit是由ISO C说明的，而_exit是由POSIX.1说明的。
 *      由于历史原因，exit函数总是执行一个标准I/O库的清理关闭操作；对所有打开流调用fclose函数。
 *      main函数返回一个整型值与用该值调用exit是等价的。在main函数中： return(0) == exit(0)
 *  
 *  atexit函数：
 *      按照ISO C规定，一个进程可以登记多至32个函数，这些函数将由exit自动调用。这些函数称为终止处理程序(exit handler)。
 *      exit调用这些函数的顺序与它们登记时候的顺序相反。同一个函数如果登记多次，也会被调用多次。
 *      根据ISO C 和 POSIX.1，exit首先调用各种终止处理程序，然后关闭所有打开流。
 *      POSIX.1扩展了ISO C标准，它说明，如若程序调用exec函数族中的任一函数，则将清除所有已安装的终止处理程序。
 * 
 *  注意，内核使程序执行的唯一方法是调用一个exec函数。进程自愿终止的唯一方法是显示或隐式地(通过调用exit)调用_exit或_Exit。
 *  进程也可非自愿地由一个信号使其终止。
 *      
*/

static void atExitHandler1(){
    printf("-->atExitHandler1....\n");
}

static void atExitHandler2(){
    printf("-->atExitHandler2....\n");
}

void commonFunc(){
    printf("-->commonFunc start...\n");
    // abort();    //进程接收到信号而异常终止，不执行exit函数。
    // exit(0);
    _exit(0);
    printf("-->commonFunc end...\n");
}

/**
 * 终止处理程序是由exit来调用执行的
 * 无论exit在何处调用，只要exit被调用，则exit就会执行终止处理程序.
 * 如果进程是因为接收到某个信号而异常终止，则不会执行exit函数。
 * 
*/
void atexitTest(){
    atexit(atExitHandler1);
    atexit(atExitHandler2);
    atexit(atExitHandler2);
    commonFunc();
}


/**
 * 重定向标准输出到文件，标准输出变为全缓冲，即只有缓冲区满才会flush内容到文件中。
 * 进程因接收到信号而异常终止，因此不会调用exit。
 * 因此进程终止时没有对打开的流调用fclose，流中的内容不会flush到文件中。
 * 
*/
void exitFlushAbort(){
    for(int i = 0; i < 10; ++i){
         printf("hello world\n");
         usleep(100 * 1000);
    }
    abort();
}


/**
 * 重定向标准输出到文件，标准输出变为全缓冲，即只有缓冲区满才会flush内容到文件中。
 * 进程主动调用exit函数，正常结束。
 * exit会自动对所有打开的流调用fclose函数，流中缓存的内容会刷新到文件中。
 * 
*/
void exitFlushExit(){
    for(int i = 0; i < 10; ++i){
         printf("hello world\n");
         usleep(100 * 1000);
    }
    exit(0);
}


/**
 * 输出重定向的文件
 * 直接调用系统API，无缓冲输出。
 * 输出内容会立即交给内核进行输出。
 * 
*/
void exitFlushInstant(){
    sleep(1);
    for(int i = 0; i < 10; ++i){
        write(STDOUT_FILENO, "HELLO ", 6);
        usleep(100 * 1000);
    }
    write(STDOUT_FILENO, "HELLO ", 6);
    abort();
}


/**
 * 命令行参数
 *      当执行一个程序时，调用exec的进程可将命令行参数传递给新程序。
 *      ISO C 和 POSIX.1 都要求argv[argc]是一个空指针。
 *      执行execl传参时，最后一个参数是nullptr。
*/
void printCommandLine(int argc, char* argv[]){
    for(int i = 0; i < argc; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");
    for(int i = 0; argv[i] != nullptr; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}


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


/**
 * 我们能影响的只是当前进程及其后生成和调用的任何子进程环境，但不能影响父进程的环境，这通常是一个shell进程。
 * 
 * 环境表和环境字符串的存储位置：
 *      环境表和环境字符串通常占用的是进程地址空间的顶部。
 * 
 *      如果修改一个现有的name:
 *          a. 新value的长度 <= 现有value的长度：将新字符串复制到原有字符串的空间中。
 *          b. 新value的长度 >  现有value的长度：调用malloc为新字符串分配空间，然后将新字符串复制到该空间中，接着使环境表
 *             中针对name的指针指向新分配区。
 *     
 *      增加一个新的name：
 *          * 首先必须调用malloc为name=value字符串分配空间，然后将字符串分配到此空间中。
 * 
 *          a. 如果这是第一次增加一个新name，则必须调用malloc为新的环境表分配空间。接着，将原来的环境表复制到新分配区，并将
 *             指向新name=value字符串的指针存放在该指针表的表尾，然后将一个空指针存放在其后。最后使environ指向新指针表。但是
 *             环境表中的大部分指针仍指向栈顶之上的各name=value字符串。
 * 
 *          b. 如果这不是第一次增加一个新name，则可知以前已调用malloc在堆中为环境表分配了空间，所以只要调用realloc，以分配比
 *             原空间多存放一个指针的空间。然后将指向新name=value字符串的指针存放在该表表尾，后面跟着一个空指针。
 *
*/

/**
 * 每个程序都接收到一张环境表。环境表也是一个字符指针数组，其中每个指针包含一个以null结束的C字符串地址。
 * 全局变量environ则包含了该指针数组的地址：extern char* *environ;
 * 称environ为环境指针，指针数组为环境表，其中各指针指向的字符串为环境字符串。
 * 环境表的最后一个字符指针指向nullptr.
 * 环境字符串存放在栈顶之上，或者堆中。环境字符串的格式是'name=value'
 *
 * 三个函数定义于<stdlib.h>中
 * char *getenv(const char *name);
 * 成功： 返回环境变量的值； 失败： NULL (name 不存在)
 *
 * int setenv(const char *name, const char *value, int overwrite);
 * 成功： 0； 失败： -1
 * 如果在环境中name已经存在：
 *      参数 overwrite 取值：
 *          0： 不删除其现有定义(name不设置为新的value，而且也不出错)
 *          1： 删除其现有定义，覆盖为新值。
 *
 * int unsetenv(const char *name);
 * 成功： 0； 失败： -1
 * 注意事项： name 不存在仍返回 0(成功)， 当 name 命名为"ABC="时则会出错。
 *
 */


//打印环境表中的每个字符串，环境表中的最后一个指针为nullptr
int printEnviron(){
    for(int i = 0; *(environ + i) != nullptr; ++i){
        printf("%s\n", *(environ + i));
    }
    return 0;
}


//获取环境变量的值, 指定name，返回其value。
//字符串比较，定位到value的地址
char * getEnvTest(const char *name){
    char *p = nullptr;
    for(int i = 0; *(environ + i) != nullptr; ++i){
        p = strstr(*(environ + i), "=");    //定位到'='字符
        int len = p - *(environ + i);       //变量名长度
        if(strncmp(name, *(environ + i), len) == 0){    //比较变量名长度
            return p+1; //返回值的地址地址
        }
    }
    return nullptr;
}


//设置环境变量
void setUnsetEnv(const char* name, const char* value){
    char *ptr = getenv(name);
    if(ptr == nullptr){
        printf("the %s is not set\n", name);
    }
    setenv(name, value, 1);
    setenv(name, "hhhhhhhhhhh", 1);
    printEnviron();
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
    atexitTest();

    return 0;
}







