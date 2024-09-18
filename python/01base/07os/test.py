#! /usr/bin/python3

import os
import stat


"""
# 权限
os.access()         # 文件是否存在、用户权限：读、写、执行
os.stat()	        # 文件属性信息
os.chmod()	        # user、group、other的权限
os.chown()	        # 用户、组ID
os.getcwd()         # 当前工作目录
os.chdir()          # 切换工作目录


# 链接
os.link()               # 创建硬链接
os.symlink()            # 符号链接
os.readlink()           # 读取链接指向的文件名
os.unlink()             # 取消链接
os.remove()             # 删除文件
os.rename()             # 修改文件名称


#目录
os.mkdir()              # 创建目录
os.makedirs()           # 创建父子目录
os.listdir()            # 列出目录下的内容
os.rmdir()              # 删除目录
os.removedirs()         # 删除父子目录


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


# filePath = "a.txt"
# print("current woring directory: {}".format(os.getcwd()))
# print("content: {}".format(os.listdir(os.getcwd())))

# print("file: {} is exist? {}".format(filePath, os.access(filePath, os.F_OK)))
# os.close(os.open(filePath, os.O_RDONLY | os.O_CREAT))
# print("file: {} is exist? {}".format(filePath, os.access(filePath, os.F_OK)))
# os.access(filePath, os.R_OK)
# os.access(filePath, os.W_OK)
# os.access(filePath, os.X_OK)


# #文件属性信息
# info = os.stat(filePath)
# print(f"info.st_ino: {info.st_ino}")
# print(f"info.st_mode: {info.st_mode}")
# print(f"info.st_nlink: {info.st_nlink}")
# print(f"info.st_uid: {info.st_uid}")
# print(f"info.st_gid: {info.st_gid}")
# print(f"info.st_size: {info.st_size}")
# print(f"info.st_atime: {info.st_atime}")
# print(f"info.st_mtime: {info.st_mtime}")
# print(f"info.st_ctime: {info.st_ctime}")


# #文件属性
# os.chmod(filePath, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
# os.chmod(filePath, stat.S_IRWXU)
# os.chmod(filePath, stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP)
# os.chmod(filePath, stat.S_IRWXG)
# os.chmod(filePath, stat.S_IROTH | stat.S_IWOTH | stat.S_IXOTH)
# os.chmod(filePath, stat.S_IRWXO)
# os.chmod(filePath, stat.S_ISUID)
# os.chmod(filePath, stat.S_ISGID)


#所有者
# os.chown(filePath, 1000, 1000)
# os.chdir(os.getcwd() + "/" + "dir")
# print(f"current working director: {os.getcwd()}")


# os.link(filePath, filePath + "_hardlink")
# os.symlink(filePath, filePath + "_softlink")
# print(os.readlink(filePath + "_softlink"))
# os.unlink(filePath)
# os.unlink(filePath + "_hardlink")
# os.unlink("a.txt_softlink")


# os.mkdir("dir", 0o777)
# os.makedirs("dir/a/b", 0o777, True)
# print("os.listdir: {}".format(os.listdir("dir")))
# os.removedirs("dir/a/b")



"""
读取字节数据
将写入的字符串编码为字节类型
将读取到的字节类型解码为字符串
"""

# fd = os.open("a.txt", os.O_RDONLY | os.O_CREAT, 0o777)
# file = os.fdopen(fd, "r", newline="c")
# line = file.readlines()
# print("line: {}".format(line))


print("hello world")