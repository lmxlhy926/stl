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


