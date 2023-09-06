
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

#include <iostream>
#include <unistd.h>


int main(int argc, char* argv[]){
    std::cout << getpid() << std::endl;
    if(setpgid(getpid(), 100) == -1){
        std::cout << "error.." << std::endl;
    }
    std::cout << getpgrp() << std::endl;


    return 0;
}




















































