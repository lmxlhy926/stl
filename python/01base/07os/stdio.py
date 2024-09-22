#! /usr/bin/python3

import os
import pickle


"""
    读写文件：
        open(file, mode="r", buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None):
            filename:   文件路径
            mode:       文件打开模式
            buffering:  设置缓冲
            encoding:   一般使用utf8
            errors:     报错级别
            newline:    区分换行符
            closefd:    传入的file参数类型
            opener:     设置自定义开启器, 开启器的返回值必须是一个打开的文件描述符

        file.fileno():
            返回一个整型的文件描述符, 可以用在如os模块的read方法等一些底层操作上。

        f.read([size]):
            读取一定数目的数据，然后作为字符串或字节对象返回。
            size是一个可选的数字类型参数。当size被忽略了或者为负, 那么该文件的所有内容都将被读取并且返回。

        f.readline():
            会从文件中读取单独的一行包括换行符。换行符为'\n'。
            如果返回一个空字符串，说明已经读取到最后一行了。

        f.readlines():
            将返回该文件中包含的所有行
            如果设置可选参数sizehint, 则读取指定长度的字节，并且将这些字节按行分割

        f.write(str):
            将string写入到文件中去, 然后返回写入的字符数

        file.writelines(sequence):
            向文件写入一个序列字符串列表，如果需要换行则要自己加入每行的换行符

        file.truncate([size])
            从文件的首行首字符开始截断, 截断文件为size个字符。
            无size表示从当前位置截断; 截断之后后面的字符被删除。

        f.flush()
            刷新文件内部缓冲，直接把内部缓冲区的数据立刻写入文件, 而不是被动的等待输出缓冲区写入

        f.tell():
            返回文件对象当前所处的位置，它是从文件开头开始算起的字节数
        
        f.seek(offset, from_what):
            0: 表示开头
            1: 表示当前位置
            2: 表示文件的结尾
        
        f.close():
            关闭文件并释放系统资源

        file.isatty():
            如果文件连接到一个终端设备返回true, 否则返回False。
"""

def file_read():
    file = open("a.txt", "r")
    print(f"fileno: {file.fileno()}")
    print(f"current position: {file.tell()}")
    # 读取指定数量的字符
    str = file.read(4)
    print(f"fileContent: {str}")
    print(f"current position: {file.tell()}")
    # 读取当前位置开始，剩余的所有字符
    str = file.read()
    print(f"fileContent: {str}")
    print(f"current position: {file.tell()}")


def file_readline():
    file = open("a.txt", "r+")
    while True:
        # 如果读取到文件末尾，则读取到的字符串为空
        str = file.readline()
        if(len(str) != 0):
            print(f"readLine: {str}")
        else:
            print("end of file ...")
            break

def file_readLines():
    file = open("a.txt", "r+")
    strList = file.readlines()
    print(f"strList: {strList}")


def file_write():
    file = open("a.txt", "r+")
    # 写入一个字符串
    size2Write = file.write("hello")
    print(f"size2Write: {size2Write}")
    # 写入一个字符串序列
    file.writelines(("nihao", "dajiia"))


def file_truncate():
    file = open("a.txt", 'r+')
    file.seek(5, os.SEEK_SET)
    # 无参数时，删除当前位置后的所有字符
    file.truncate()
    # 有参数时，从首行首字符开始截断，截断为指定数量
    file.truncate(2)


"""
    input():
        python提供input()内置函数从标准输入读入一行文本, 默认的标准输入是键盘
"""
def readWithInput():
    while(True) :
        str = input("请输入： ")
        print("你输入的内容是： ", str)


def readFromStdin():
    file = os.fdopen(0, "r")
    while True:
        line = file.readline()
        if(line.startswith('quit')):
            break
        else:
            print(f"readline: {line}")



"""
    python的pickle模块实现了基本的数据序列和反序列化
    通过pickle模块的序列化操作我们能够将程序中运行的对象信息保存到文件中去, 永久存储
    通过pickle模块的反序列化操作, 我们能够从文件中创建上一次程序保存的对象
    基本接口：
        pickle.dump(obj, file [, protocol])
        pickle.load(file)
"""
def pickle_dump_load():
    obj_1 = {"a": [1, 2, 3],
            "b": ("hello", "world"),
            "c": None}
    obj_2 = [1, 2, 3]
    file = open("a.txt", 'wb')
    pickle.dump(obj_1, file)
    pickle.dump(obj_2, file)
    file.close()

    file = open("a.txt", "rb")
    obj1 = pickle.load(file)
    obj2 = pickle.load(file)
    print(str(obj1))
    print(str(obj2))
    file.close()

