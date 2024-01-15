#! /home/lhy/bin/python

import math
import os
import pickle

"""
    将输出值转换为字符串
        str():  函数返回一个用户易读的表达形式
        repr(): 产生一个解释器易读的表达形式
"""
def repr_test():
    x = 0.12
    y = 3.45
    s = "x的值为: " + repr(x) + ", y的值为: " + repr(y) + "..."
    print(s)

    #repr()可以保留字符串中的转义字符
    s = "hello\nworld"
    print(s)
    print(repr(s))

    #repr()的参数可以是python的任何对象
    s = (1, 2, ("hello", "world"))
    print(repr(s))


"""
    字符串对象的对齐方法：这些方法并不会写任何东西，它们仅仅返回新的字符串
        rjust()
        ljust()
        center()
"""
def rightLeftCenter_test():
    #右对齐
    print("hello".rjust(10), "world".rjust(10), "nihao".rjust(10))
    print("sshello".rjust(10), "ssworld".rjust(10), "ssnihao".rjust(10))

    #左对齐
    print("hello".ljust(10), "world".ljust(10), "nihao".ljust(10))
    print("sshello".ljust(10), "ssworld".ljust(10), "ssnihao".ljust(10))

    #中间对齐
    print("hello".center(10), "world".center(10), "nihao".center(10))
    print("sshello".center(10), "ssworld".center(10), "ssnihao".center(10))


"""
    str.format():
        括号及其里面的字符(称作格式化字段)将会被format()中的参数替换
        在括号中的数字用于指向传入对象在format()中的位置
        位置和关键字参数可以任意组合, 但是位置参数不能出现在关键字参数之后
"""
def str_format():
    print('{}网址： "{}!"'.format("菜鸟教程", "www.runoob.com"))
    print("常量PI的近似值为: {0:.3f}".format(math.pi))
    print("{0:10} ===> {1:10d}".format("google", 1))

    print("{0}, {1}".format("google", "runoob"))
    print("{1}, {0}".format("google", "runoob"))
    print("{1}, {0}, {name}".format("google", "alibaba", name = "runoob")) 

    # 索引键值
    table = {"goole":1, "runoob":2, "taobao":3}
    print("runoob: {0[runoob]}; goole: {0[goole]}; taobao: {0[taobao]}".format(table))


"""
    input():
        python提供input()内置函数从标准输入读入一行文本, 默认的标准输入是键盘
"""
def input_test():
    while(True) :
        str = input("请输入： ")
        print("你输入的内容是： ", str)


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
def file_test():
    f = open("./test.txt", "r+")
    print("fileno: {}".format(f.fileno()))
    f.write("helloworld")
    print("position: {}".format(f.tell()))
    f.seek(5, os.SEEK_SET)
    print("position: {}".format(f.tell()))
    f.write("hello")
    print("position: {}".format(f.tell()))
    f.writelines(("python", "is", "great"))
    f.close()


def file_test2():
    f = open("./test.txt", "r+")
    print("{}--->{}".format(f.readline(), f.tell()))
    print("{}--->{}".format(f.readlines(), f.tell()))
    f.close()
    pass


def file_test3():
    f = os.fdopen(0, "r")
    print(f.isatty())
    while True:
        str = f.readline()
        print("readStr: {}".format(str))


"""
    python的pickle模块实现了基本的数据序列和反序列化
    通过pickle模块的序列化操作我们能够将程序中运行的对象信息保存到文件中去, 永久存储
    通过pickle模块的反序列化操作, 我们能够从文件中创建上一次程序保存的对象
    基本接口：
        pickle.dump(obj, file [, protocol])
        pickle.load(file)
"""
def pickledump_test():
    data1 = {"a": [1, 2, 3],
            "b": ("hello", "world"),
            "c": None}
    data2 = [1, 2, 3]
    file = open("test.txt", 'wb')
    pickle.dump(data1, file)
    pickle.dump(data2, file)
    file.close()


def pickleload_test():
    file = open("./test.txt", "rb")
    data1 = pickle.load(file)
    data2 = pickle.load(file)
    print(str(data1))
    print(str(data2))
    file.close()


file_test()



