#! /home/lhy/bin/python

import keyword
import sys

# 关键字
print(keyword.kwlist)


# 命令行参数，搜索路径
for i in sys.argv:
    print(i)
print("\n python 路径为： ", sys.path)


# 多变量赋值
a, b, c = 1, 2, "runoob"
a = b = c = 1
print(a)
print(b)
print(c)


# python3支持int,float,bool,complex
# type()不会认为子类是一种父类类型。isinstance()会认为子类是一种父类类型
a,b,c,d = 20, 5.5, True, 4+3j
print(type(a), type(b), type(c), type(d))
if type(True) == bool :
    print("type(true) == bool")
if isinstance(True, int):
    print("isinstance(True, int)")


# 类型转换，无法转换则抛异常
# 自动类型转换，python将较小的数据类型转换为较大的数据类型，已避免数据丢失
num_int = 123
num_flo = 1.23
num_new = num_int + num_flo
print("num_int 数据类型为: ", type(num_int))
print("num_flo 数据类型为: ", type(num_flo))
print("num_new 数据类型为: ", type(num_int + num_flo))

# 显示类型转换，用户将对象的数据类型转换为所需的数据类型
x = int(1)
y = int(2.8)
z = int("3")
print("x:", x, ", y:", y, " z:", z)

x = float(1)
y = float(2.8)
z = float("3")
w = float("4.2")


# id()函数用于获取对象地址
# is用于判断2个变量引用的对象是否为同一个，==用于判断引用变量的值是否相等
a = [1, 2]
b = [1, 2]
print(id(a))
print(id(b))
if (a is b):
    print("a is b")
if (a == b):
    print("a == b")

