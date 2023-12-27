#! /home/lhy/bin/python

import os
import stat

"""
    os.access(path, mode):
        测试指定访问路径(文件或目录)是否具有以下状态：
            F_OK: 存在
            R_OK: 可读
            W_OK: 可写
            X_OK: 可执行
"""
def access_test():
    filePath = "/home/lhy/ownproject/stl/script/python/base/07os/test.txt"
    ret = os.access(filePath, os.F_OK)
    print("F_OK, 返回值: {}".format(ret))

    ret = os.access(filePath, os.R_OK)
    print("R_OK, 返回值: {}".format(ret))

    ret = os.access(filePath, os.W_OK)
    print("W_OK, 返回值：{}".format(ret))

    ret = os.access(filePath, os.X_OK)
    print("X_OK, 返回值: {}".format(ret))



"""
    os.getcwd()
        获取当前工作目录

    os.chdir(path)
        修改当前工作目录

    os.fchdir(fd)
        通过文件描述符改变当前工作目录
"""
def chdir_test():
    path = "/home/lhy/ownproject/stl/script/python/base"
    print("当前工作目录为：{}".format(os.getcwd()))
    os.chdir(path)
    print("目录修改为: {}".format(os.getcwd()))
    fd = os.open("/home/lhy/ownproject/stl/script/python", os.O_RDONLY) #打开一个目录
    os.fchdir(fd)
    print("目录被修改为：{}".format(os.getcwd()))



"""
    os.chmod(path, mode) : 文件由路径指定
    os.fchmod(path, mode): 文件由参数fd指定
"""
def chmod_test():
    path = "/home/lhy/ownproject/stl/script/python/base/07os/test.txt"
    os.chmod(path, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)  #所有者
    os.chmod(path, stat.S_IRWXU)
    os.chmod(path, stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP)  #所属组
    os.chmod(path, stat.S_IRWXG)
    os.chmod(path, stat.S_IROTH | stat.S_IWOTH | stat.S_IXOTH)  #其它
    os.chmod(path, stat.S_IRWXO)

    #执行此文件其进程有效用户ID为文件所有者ID
    os.chmod(path, stat.S_ISUID)
    #执行此文件其进程有效组ID为文件所属组ID
    os.chmod(path, stat.S_ISGID)
    #设置文件粘滞位
    os.chmod(path, stat.S_ISVTX)

    fd = os.open(path, os.O_RDONLY)
    os.fchmod(fd, stat.S_IREAD | stat.S_IWRITE | stat.S_IEXEC)  #设置所有者权限
    os.fchmod(fd, stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP)



"""
    os.chown(path, uid, gid):
        用于修改文件所有者和组所有者, 如果不修改可以设置为-1。
        只支持在unix中使用

    os.fchown(fd, uid, gid)
        文件由文件描述符fd指定
        在unix上可用
"""
def chown_test():
    path = "/home/lhy/ownproject/stl/script/python/base/07os/test.txt"
    os.chown(path, 1000, 1000)
    fd = os.open(path, os.O_RDONLY)
    os.fchown(fd, 0, 0)
    os.close(fd)



"""
    os.chroot(path)
        更改当前进程的目录为指定的目录，使用该函数需要管理员权限
        在unix中有效
"""
def chroot_test():
    print("before chroot, 当前工作目录为：{}".format(os.getcwd()))
    os.chroot("/home/lhy/ownproject/stl/script/python/base/07os")
    print("after chroot, 当前工作目录为：{}".format(os.getcwd()))



"""
    os.open(path, mod)  : 打开文件
    os.close(fd)        : 关闭文件描述符
"""
def openclose_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    os.write(fd, "this is a test file\n".encode(encoding="utf-8"))
    os.close(fd)



"""
    os.dup(fd)      : 复制文件描述符
"""
def fd_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    d_fd = os.dup(fd)
    os.write(fd, "this is a test file\n".encode(encoding="utf-8"))
    os.write(d_fd, "second test file".encode())
    os.close(fd)
    os.close(d_fd)
    print("关闭文件成功")



"""
    os.dup(fd, dd2):  用于将一个文件描述符fd复制到另一个fd2
        fd:  要被复制的文件描述符
        fd2: 复制的文件描述符
"""
def dup2_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    os.dup2(fd, 1)  #标准输出指向文件
    os.close(fd)    #关闭文件
    #输出定向到文件中
    print("hello")
    print("world")


"""
    os.fdatasync():
        用于强制将文件写入磁盘, 该文件由文件描述符fd指定, 但是不强制更新文件的状态信息。
        如果你需要刷新缓冲区可以使用该方法
"""
def fdatasync_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    os.write(fd, "this is test".encode())
    os.fdatasync(fd)    # 将缓存的文件内容写入磁盘

    os.lseek(fd, 0, 0)
    str = os.read(fd, 100)
    print("读取的字符串是：{}".format(str))
    os.close(fd)
    print("文件关闭")



"""
    os.fdopen(fd, [, mode[, bufsize]]):
        通过文件描述符fd创建一个文件对象, 并返回这个文件对象
        bufsize: 指定返回的文件对象是否带缓冲：
                        0:  不带缓冲
                        1:  行缓冲
                      正数： 全缓冲, 正数为缓冲区的大小
                       -1:  使用系统默认的缓冲设定
"""
def fdopen_test():
    fd = os.open("foo.txt", os.O_RDWR | os.O_CREAT)
    fo = os.fdopen(fd, "w+")


fdopen_test()