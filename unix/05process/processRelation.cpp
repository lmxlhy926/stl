
#include <iostream>
#include <unistd.h>

/**
 *      进程组以及POSIX.1引入的会话的概念。登录shell(登录时所调用的)和所有从登录shell启动的进程之间的关系
 * 
 * 
 *      在早期的UNIX系统，用户用哑终端(用硬链接连到主机)进行登录。终端或者是本地的(直接连接)或者是远程的(通过调制解调器连接)。
 * 在这两种情况下，登录都经由内核中的终端设备驱动程序。
 * 
 * 
 *      当系统自举时，内核创建进程ID为1的进程，也就是init进程。init进程使用户进入多用户模式。init读取文件/etc/ttys，对每一个
 * 允许登录的终端设备，init调用一次fork，它所生成的子进程则exec getty程序。getty对终端设备调用open函数，以读写方式将终端打开。
 * 如果设备是调制解调器，则open可能会在设备驱动程序中滞留，直到用户拨号调制解调器，并且线路被接通。一旦设备被打开，则文件描述符
 * 0、1、2就被设置到该设备。然后getty输出"login"之类的信息，并等待用户键入用户名。当用户键入用户名后，getty的工作就完成了。
 * 然后以类似于下面的方式调用login程序：execle("/bin/login", "login", "-p", username, (char*)0, envp);
 *      login能处理多项工作。因为它得到了用户名，所以能调用getpwnam取得相应用户的口令文件登录项。然后调用getpass以显示提示
 * "Password: ",接着读用户键入的口令(自然，禁止回显用户键入的口令)。它调用crypt将用户键入的口令加密，并与该用户在阴影
 * 口令文件中登录项的pw_passwd字段相比较。如果用户几次键入的口令都无效，则login以参数1调用exit表示登录失败。父进程(init)
 * 了解到子进程的终止情况后，将再次调用fork，其后又执行了getty，对此终端重复上述过程。
 * 
 *      如果用户正确登录，login将完成如下工作：
 *          * 将当前工作目录更改为该用户的起始目录(chdir)
 *          * 调用chown更改该终端的所有权，使登录用户成为它的所有者
 *          * 将对该终端设备的访问权限改变为"用户读和写"
 *          * 调用setgid及initgroups设置进程的组ID。
 *          * 用login得到的所有信息初始化环境：起始目录(HOME)、shell、用户名、系统默认路径(PATH)
 *          * login进程更改为登录用户的用户ID(setuid)并调用该用户的登录shell。其方式类似于：execl("/bin/sh", "-sh", (char*)0);
 * 
 *      至此，登录用户的登录shell开始运行。其父进程ID是init进程，所以当此登录shell终止时，init会得到通知，它会对该终端重复全部
 *  上述过程。登录shell的文件描述符0、1、2设置为终端设备。现在，登录shell读取其启动文件。这些启动文件通常更改某些环境变量并增加
 *  很多环境变量。当执行完启动文件后，用户最后得到shell提示符，并能加入命令。
 * 
 * 
 * 
*/


/**
 * 进程组是一个或多个进程的集合。
 * 同一进程组的各进程接收来自同一终端的各种信号。每个进程组有一个唯一的进程组ID。
 * 每个进程组有一个组长进程。组长进程的进程组ID等于其进程ID。
 * 进程组组长可以创建一个进程组、创建该组中的进程，然后终止。从进程组创建开始到其中最后一个进程离开为止的时间区间称为进程组的生命期。
 * 
 * 进程调用setpgid可以加入一个现有的进程组或者创建一个新进程组。
 * 一个进程只能为它自己或它的子进程设置进程组ID。在它的子进程调用了exec后，它就不再更改该子进程的进程组ID。
 * 
 * 
 * 会话是一个或多个进程组的集合。
 * 通常是由shell的管道将几个进程编成一组。
 * 
 * 如果调用此函数的进程不是一个进程组的组长，则此函数创建一个新会话。
 *      该进程变成新会话的会话首进程，会话首进程是创建该会话的进程。此时，该进程是新会话中的唯一进程。
 *      该进程成为一个新进程组的组长进程。新进程组ID是该调用进程的进程ID。
 *      该进程没有控制终端。
 * 
*/

/**
 * fork后子进程继承父进程的进程组ID
 * 获取进程组ID，设置进程组ID
*/
void processGrp(){
    std::cout << "parent pid: " << getpid() << std::endl;
    std::cout << "parent gid: " << getpgrp() << std::endl;
    pid_t pid = fork();
    if(pid == 0){
        std::cout << "child pid: " << getpid() << std::endl;
        std::cout << "child gid: " << getpgrp() << std::endl;
        if(setpgid(getpid(), getpid()) == -1){  //设置进程的进程组ID
            std::cout << "setpgid error..." << std::endl;
        }   
        std::cout << "child gid: " << getpgrp() << std::endl;
    }
}


//设置会话ID、获取会话ID
void sessionId(){
    std::cout << "parent pid: " << getpid() << std::endl;
    std::cout << "parent gid: " << getpgrp() << std::endl;
    pid_t pid = fork();
    if(pid == 0){
        std::cout << "child pid: " << getpid() << std::endl;
        std::cout << "child grp: " << getpgrp() << std::endl;
        pid_t grp = setsid();
        std::cout << "grp: " << grp << std::endl;  
        std::cout << "grp id: " << getpgrp() << std::endl;
        std::cout << "session id: " << getsid(0) << std::endl;      
    }
}



int main(int argc, char* argv[]){
    sessionId();


    return 0;
}




















































