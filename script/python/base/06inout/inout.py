#! /home/lhy/bin/python

import math

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

input_test()






