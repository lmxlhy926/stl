#! /usr/bin/python3

import os
import sys
from stat import *

"""
# 文件属性
os.access()         # 文件是否存在、用户权限：读、写、执行
os.stat()	        # 文件属性信息
os.chmod()	        # user、group、other的权限
os.chown()	        # 用户、组ID
os.rename()             # 修改文件名称


# 链接
os.link()               # 创建硬链接
os.symlink()            # 符号链接
os.readlink()           # 读取链接指向的文件名
os.unlink()             # 取消链接
os.remove()             # 删除文件


#目录
os.mkdir()              # 创建目录
os.makedirs()           # 创建父子目录
os.listdir()            # 列出目录下的内容
os.rmdir()              # 删除目录
os.removedirs()         # 删除父子目录
os.getcwd()             # 当前工作目录
os.chdir()              # 切换工作目录


# 文件操作
os.open()               # 打开文件
os.read()               # 读取文件内容
os.write()              # 写文件
os.lseek()              # 定位到文件的指定位置
os.ftruncate()          # 清空文件内容
os.close()              # 关闭文件
os.fdatasync()          # 文件内容更新
os.fsync()              # 文件同步
os.dup()                # 复制文件描述符
os.isatty()             # 是否是tty文件
os.ttyname()
os.fdopen()             #用带缓冲的标准库打开描述符指向的文件


# fifo、管道
os.mkfifo()             # 创建fifo
os.pipe()               # 创建管道
os.popen()              # 管道执行命令
"""


"""
文件属性
    os.access(path, mode):
        测试指定访问路径(文件或目录)是否具有以下状态：
            F_OK: 存在
            R_OK: 可读
            W_OK: 可写
            X_OK: 可执行

    os.stat(path)   :   用于在给定的路径上执行一个系统stat的调用
    os.lstat(path)  :   读取链接文件的信息, 不跳转到链接指向的文件
    os.fstat(fd)    :   返回文件描述符fd的状态, 类似stat()。
        返回的结构:
            st_dev:     设备信息
            st_ino:     文件的i-node值
            st_mode:    文件信息的掩码, 包含文件的权限信息，文件的类型信息
            st_nlink:   硬链接数
            st_uid:     用户ID
            st_gid:     组ID
            st_rdev:    设备ID
            st_size:    文件大小, 以byte为单位
            st_blksize: 系统I/O块大小
            st_blocks:  文件是由多少个I/O块构成
            st_atime:   文件最近的访问时间
            st_mtime:   文件最近的修改时间
            st_ctime:   文件状态信息的修改时间(不是文件内容的修改时间)

    修改文件权限           
        os.chmod(path, mode)    :  文件由路径指定
        os.lchmod(path, mode)   :  用于修改连接文件权限, 即不跳转到连接文件指向的文件
        os.fchmod(path, mode)   :  文件由参数fd指定
    
    修改文件所有者
        os.chown(path, uid, gid)  :  用于修改文件所有者和组所有者, 如果不修改可以设置为-1。只支持在unix中使用
        os.lchown(path, uid, pid) :  用于更改文件所有者, 类似chown, 但是不追踪链接
        os.fchown(fd, uid, gid)   :  文件由文件描述符fd指定.在unix上可用

    修改文件名
        os.rename(src, dst)
                src: 要修改的目录名
                dst: 修改哈的目录名
                用于命名文件或目录, 从src到dst, 如果dst是一个存在的目录, 将抛出OSError。
"""
# 测试文件权限
def access_test():
    filePath = "test.txt" 
    print("F_OK, 返回值: {}".format(os.access(filePath, os.F_OK)))  #文件是否存在
    print("R_OK, 返回值: {}".format(os.access(filePath, os.R_OK)))  #可读
    print("W_OK, 返回值：{}".format(os.access(filePath, os.W_OK)))  #可写
    print("X_OK, 返回值: {}".format(os.access(filePath, os.X_OK)))  #可执行


#获取文件属性
def stat_test(path):
    info =  os.stat(path)
    infoLink = os.lstat(path)
    infoFd = os.fstat(os.open(path, os.O_RDWR))

    print("文件信息: {}"     .format(info))
    # 设备节点
    print("st_dev: {}"      .format(info.st_dev))
    print("st_ino: {}"      .format(info.st_ino))
    print("st_rdev: {}"     .format(info.st_rdev))
    print("st_nlink: {}"    .format(info.st_nlink))
    # 模式、用户名
    print("st_mode: {}"     .format(info.st_mode))
    print("st_mode_str: {}".format(filemode(info.st_mode)))
    print("st_uid: {}"      .format(info.st_uid))
    print("st_gid: {}"      .format(info.st_gid))
    # 大小
    print("st_size: {}"     .format(info.st_size))
    print("st_blocks: {}"   .format(info.st_blocks))
    print("st_blksize: {}"  .format(info.st_blksize))
    # 访问时间
    print("st_atime: {}"    .format(info.st_atime))
    print("st_mtime: {}"    .format(info.st_mtime))
    print("st_ctime: {}"    .format(info.st_ctime))


# 使用stat实现ls -lhi
def stat_ls_lhi(path):
    statInfo = os.stat(path)
    print(f"{statInfo.st_ino} {filemode(statInfo.st_mode)} {statInfo.st_nlink} {statInfo.st_uid} \
{statInfo.st_gid} {statInfo.st_size} {statInfo.st_ctime} {path}")


#修改文件权限
def chmod_test():
    path = "test.txt"
    os.chmod(path, S_IRUSR | S_IWUSR | S_IXUSR)  #所有者
    os.chmod(path, S_IRWXU)
    os.chmod(path, S_IRGRP | S_IWGRP | S_IXGRP)  #所属组
    os.chmod(path, S_IRWXG)
    os.chmod(path, S_IROTH | S_IWOTH | S_IXOTH)  #其它
    os.chmod(path, S_IRWXO)

    # #执行此文件其进程有效用户ID为文件所有者ID
    os.chmod(path, S_ISUID)
    # #执行此文件其进程有效组ID为文件所属组ID
    os.chmod(path, S_ISGID)
    # #设置文件粘滞位
    os.chmod(path, S_ISVTX)

    fd = os.open(path, os.O_RDONLY)
    os.fchmod(fd, S_IREAD | S_IWRITE | S_IEXEC)  #设置所有者权限(同义词)
    os.fchmod(fd, S_IRGRP | S_IWGRP  | S_IXGRP)   #设置文件组权限


#修改文件所有者
def chown_test():
    path = "test.txt"
    os.chown(path, 1000, 1000)
    os.lchown(path, 1000, 1000)
    fd = os.open(path, os.O_RDONLY)
    os.fchown(fd, 0, 0)
    os.close(fd)



"""
创建、读取、删除链接
    os.link(src, dst) : 文件名不同, 但是指向的节点是一致的
        src: 用于创建硬连接的源地址
        dst: 用于创建硬链接的目的地址

    os.symlink(src, dst, target_is_directory=False):
        src: 要创建符号链接的目标文件或目录路径
        dst: 符号链接的路径和名称
        target_is_directory: 指定src是否是一个目录。如果设置为True, dst将被创建为指向目录的符号链接

    os.readlink(path):  返回链接所指向的文件, 可能返回绝对或相对路径

    os.unlink(path):    用于删除文件, 如果文件是一个目录则返回一个错误

    os.remove(path):
        删除指定路径的文件。如果指定的路径是一个目录, 将抛出OSError.
"""
#创建、读取、删除链接
def link_test():
    path = "test.txt"
    os.link(path, path + "_hardlink")           #创建硬链接：源文件名---目的文件名
    os.symlink(path, path + "_softlink")        #创建软链接：源文件名---连接文件名
    print(os.readlink(path + "_softlink"))      #读取链接文件指向的源文件名
    os.unlink(path + "_hardlink")               #删除文件

    

"""
创建、读取、删除文件夹
    os.mkdir(path, mode=0777):  
        path: 要创建的目录路径
        mode: 要为目录设置的权限数字模式
        以数字权限模式创建目录。默认的模式为0777
        如果目录有多级，则创建最后一级,如果最后一级目录的上级目录有不存在的,则会抛出一个OSError

    os.makedirs(name, mode=511, exist_ok=False):    递归创建多层目录
        path:       需要递归创建的目录
        mode:       权限模式, 默认的模式为511
        exist_ok:   是否在目录存在时触发FileExistsError异常

    os.listdir(path)
        返回指定的文件夹包含的文件或文件夹的名字的列表。不包括.和..

    os.rmdir(path)
        用于删除指定路径的目录。仅当这文件夹是空的才可以，否则, 抛出OSError

    os.removedirs(path)
        递归删除目录

    os.getcwd():        
        获取当前工作目录

    os.getcwdb():       
        返回一个字符串(bytestring), 表示当前目录

    os.chdir(path):     
        修改当前工作目录

    os.fchdir(fd):      
        通过文件描述符改变当前工作目录

    os.chroot(path):    
        更改当前进程根目录为指定的目录, 使用该函数需要管理员权限.在unix中有效
"""
#创建、读取、删除文件夹
def dirs_test():
    print("{}".format(os.listdir(os.getcwd())))
    os.makedirs("hello/world", 0o777, True)         #创建上下级目录
    print("{}".format(os.listdir("hello/world")))   
    os.mkdir("hello/world/final", 0o777)            #在上级目录存在的情况下创建子目录
    print("{}".format(os.listdir("hello/world")))
    os.rmdir("hello/world/final")                   #移除最最下级子目录
    print("{}".format(os.listdir("hello/world")))
    os.removedirs("hello/world")                    #移除上下级目录
    print("{}".format(os.listdir(os.getcwd())))


#重命名、删除文件
def nameRemove_test():
    path = "foo.txt"
    os.link(path, path + "_link")       #创建硬连接
    os.remove(path )            #删除指定的文件
    os.unlink(path + "_link")           #删除指定文件


# 修改、读取当前工作目录
def chdir_test():
    print("当前工作目录为：{}".format(os.getcwd()))
    print("当前工作目录为: {}".format(os.getcwdb()))
    os.chdir("/home/lhy/ownproject/stl/script/python/base")     #通过路径修改当前工作目录
    print("1目录修改为: {}".format(os.getcwd()))
    fd = os.open("/home/lhy/ownproject/stl/script/python", os.O_RDONLY)   #打开一个目录
    os.fchdir(fd)       #通过文件描述符修改当前工作目录
    print("2目录被修改为:{}".format(os.getcwd()))
    filefd = os.open("./base/07os/foo.txt", os.O_RDWR | os.O_CREAT)     #使用相对路径
    print("读取到的内容：{}".format(os.read(filefd, 100)))
    chroot_test()


#修改当前进程的根目录为指定目录
def chroot_test():
    print("before chroot, 当前工作目录为：{}".format(os.getcwd()))
    os.chroot("/home/lhy/ownproject/stl/script/python/base/07os")
    print("after chroot, 当前工作目录为：{}".format(os.getcwd()))


"""
    os.pathconf(path, name):    返回一个打开的文件所属于的系统的配置信息.   unix上可用
    os.fpathconf(fd, name)
        fd:     打开的文件描述符
        name:   属性名
    
    os.statvfs(path): 包含指定路径文件的文件系统的信息  
    os.fstatvfs(fd):  包含文件描述符fd指定文件的文件系统信息. unix上可用
        fstatvfs返回的结构
            f_bsize:    文件系统块大小
            f_frsize:   分栈大小
            f_blocks:   文件系统数据块总数
            f_bfree:    可用块数
            f_bavail:   非超级用户可获取的块数
            f_files:    文件节点总数
            f_ffree:    可用文件节点数
            f_favail:   非超级用户的可用文件节点数
            f_fsid:     文件系统标识ID
            f_flag:     挂载标记
            f_namemax:  最大文件长度

    os.major(device)
    os.makedev(major, minor)
    os.minor(device)

"""
def pathconf_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    print("fpathconf names:")
    for name in os.pathconf_names:
        print("     " + name + ":   {}".format(os.fpathconf(fd, name)))
    print("--------------------")
    print("文件名最大长度为： {}".format(os.pathconf("foo.txt", "PC_NAME_MAX")))


def fstatvfs_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    info = os.fstatvfs(fd)
    print("文件信息： {}".format(info))
    print("f_bsize: {}".format(info.f_bsize))
    print("f_frsize: {}".format(info.f_frsize))
    print("f_blocks: {}".format(info.f_blocks))
    print("f_bfree: {}".format(info.f_bfree))
    print("f_bavail: {}".format(info.f_bavail))
    print("f_files: {}".format(info.f_files))
    print("f_ffree: {}".format(info.f_bfree))
    print("f_favail: {}".format(info.f_favail))
    print("f_fsid: {}".format(info.f_fsid))
    print("f_flag: {}".format(info.f_flag))
    print("f_namemax: {}".format(info.f_namemax))


def major_minor_test():
    info = os.lstat("./foo.txt")
    print("info.st_dev: {}".format(info.st_dev))
    major_dnum = os.major(info.st_dev)
    minor_dnum = os.minor(info.st_dev)
    print("major 设备号：{}".format(major_dnum))
    print("minor 设备号：{}".format(minor_dnum))
    # 生成设备号
    dev_num = os.makedev(major_dnum, minor_dnum)
    print("设备号：{}".format(dev_num))



"""
文件操作：
    os.open(path, flags, mode=0777): 打开文件
        flags:
            os.O_RDONLY     :只读
            os.O_WRONLY     :只写
            os.O_RDWR       :读写
            os.O_APPEND     :追加
            os.O_TRUNC      :打开文件时截断文件为0
            os.O_CREAT      :文件不存在时创建
            os.O_EXCL       :如果指定文件存在则返回错误
            os.O_NONBLOCK   :非阻塞方式
            os.O_NOFOLLOW   :不追踪软链接
            os.O_FSYNC      :同步写入
            os.O_SHLOCK     :自动获取共享锁
            os.O_DIRECT     :消除或减少缓存效果

    os.read(fd, n): 
        从文件描述符fd中读取最多n个字节, 返回包含读取字节的字符串.如果读到文件末尾, 则返回一个空字符串

    os.write(fd, str):
        用于写入字符串到文件描述符fd中, 返回实际写入的字符串长度

    os.lseek(fd, pos, how): 
        设置文件描述符fd当前位置

    os.ftruncate(fd, length): 
        裁剪文件描述符fd对应的文件, 它最大不能超过文件大小
            
    os.close(fd): 
        关闭文件描述符

    os.fdatasync():
        用于强制将文件写入磁盘, 该文件由文件描述符fd指定, 但是不强制更新文件的状态信息。
        如果你需要刷新缓冲区可以使用该方法

    os.fsync():
        强制将文件描述符为fd的文件写入硬盘, 在unix中会调用fsync函数, 在windows中调用_commit()函数
        如果你准备操作一个Python文件对象f: 首先调用f.flush(), 然后调用os.fsync(f.fileno())
        确保将与f有关的所有内容都写入了硬盘

    os.dup(fd): 复制文件描述符
    
    os.dup(fd, dd2):  用于将一个文件描述符fd复制到另一个fd2
        fd:  要被复制的文件描述符
        fd2: 复制的文件描述符

    os.isatty():
        用于判断如果文件描述符fd是打开的并且同时和tty-like设备相连, 则返回true, 否则返回false。

    os.ttyname():
        用于返回一个字符串, 它表示与文件描述符fd关联的终端设备。如果fd没有与终端设备关联,则引发一个异常。

    os.fdopen(fd, [, mode[, bufsize]]):
        通过文件描述符fd创建一个文件对象, 并返回这个文件对象
        bufsize: 指定返回的文件对象是否带缓冲：
                        0:      不带缓冲
                        1:      行缓冲
                        正数：   全缓冲, 正数为缓冲区的大小
                       -1:      使用系统默认的缓冲设定

    os.tcgetpgrp(fd):
        用于返回与终端fd(一个由os.open()返回的打开的文件描述符)关联的进程组

    os.tcsetpgrp(fd, pg)
        用于设置与终端fd关联的进程组pg  
"""
def openclose_test():
    # 打开文件
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    # 写入文件
    os.write(fd, "this is a test file\n".encode(encoding="utf-8"))
    # 同步到磁盘
    os.fsync(fd)
    # 定位到开头
    os.lseek(fd, 0, os.SEEK_SET)
    # 读取并输出文件内容
    print("readStr: {}".format(os.read(fd, 100)))
    print("position: {}".format(os.lseek(fd, 0, os.SEEK_CUR)))
    os.close(fd)


def fd_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    d_fd = os.dup(fd)
    os.write(fd, "this is a test file\n".encode(encoding="utf-8"))
    os.write(d_fd, "second test file".encode())
    os.close(fd)
    os.close(d_fd)
    print("关闭文件成功")



def dup2_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    os.dup2(fd, 1)  #标准输出指向文件
    os.close(fd)    #关闭文件
    #输出定向到文件中
    print("hello")
    print("world")


def fdopen_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    fo = os.fdopen(fd, "w+")
    print("current i/o pointer position: {}".format(fo.tell()))
    fo.write("hello")
    fo.close()


def ftruncate_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    os.write(fd, "this is test - this is test".encode())
    os.ftruncate(fd, 12)
    os.close(fd)


def isatty_test():
    fd1 = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    fd2 = os.open("/dev/ttyUSB0", os.O_RDWR)
    print("foo.txt: {}".format(os.isatty(fd1)))
    print("ttyUSB: {}".format(os.isatty(fd2)))


def ttyname_test():
        fd = os.open("/dev/tty", os.O_RDONLY)
        p = os.ttyname(fd)
        print("关联的终端为：{}".format(p))
        print("done")
        os.close(fd)


def tcgetpgrp_test():
    print("当前目录为： {}".format(os.getcwd()))
    fd = os.open("/dev/tty", os.O_RDONLY)
    f = os.tcgetpgrp(fd)
    print("相关进程组： {}".format(f))
    os.close(fd)



"""
    os.mkfifo(path, mode=066): 创建管道, 并设置权限模式。默认的模式为0666。

     os.pipe(): 用于创建一个管道, 返回一对文件描述符(r,w)分别为读和写

     os.popen(command[, mode[, bufsize]])
        command: 使用命令
        mode:    模式权限， 可以是"r"(默认)或"w"
        bufsize: 缓冲区大小
"""
def mkfifo_test():
    os.mkfifo("testfifo", 0o644)


def pipe_test():
    print("the child will write text to pipe")
    print("the parent will read the text written by child")
    r, w = os.pipe()
    processid = os.fork()
    if processid :
        os.close(w)
        r = os.fdopen(r)
        print("Parent reading...")
        str = r.read()
        print("read text: {}".format(str))
        sys.exit(0)
    else:
        os.close(r)
        w = os.fdopen(w, "w")
        print("child writing")
        w.write("text written by child....")
        w.close()
        print("child closing")
        sys.exit(0)

def popen_test():
    file = os.popen("less foo.txt", 'w', 1)



stat_ls_lhi("test.py")