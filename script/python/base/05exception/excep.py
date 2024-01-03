#! /home/lhy/bin/python

import sys
import os

"""
    python有2种错误很容易辨认: 语法错误和异常
    python assert用于判断一个表达式, 在表达式条件为false的时候触发异常
    运行期检测到的错误被称为异常, 大多数异常都不会被程序处理, 都以错误信息的形式展现在这里

    assert expression [, arguments]
        用于判断一个表达式, 在表达式条件为false的时候触发异常
        
    raise
    
    try
        执行代码
    except
        发生异常时执行的代码
    else
        没有发生异常时执行的代码
    finally
        不管有没有异常都会执行的代码
"""
def assert_test():
    assert False, "条件不满足"


def trycatch_test():
    try:
        f = open("myfile.txt")
        s = f.readline()
        i = int(s.strip())
    except OSError as err:
        print("OS error: {}".format(err))
    except ValueError:
        print("could not convert data to an integer.")
    except:
        print("unexcepted error: ", sys.exe_info()[0])


def trycatch_test1():
    for arg in sys.argv[1:]:
        try:
            f = open(arg, "r")
        except IOError:
            print("cannot open ", arg)
        else:
            print(arg, 'has ', len(f.readlines()), " lines")
            f.close()
        finally:
            print("alwas execute....")
        

"""
    raise:
        python使用raise语句抛出一个指定的异常
        raise[Exception [, args [, traceback]]]
"""
def trycatch_test2():
    try:
        raise NameError("nameerror")
    except NameError:
        print("an exception flew by")
        raise

"""
    用户自定义异常：
        通过创建一个新的异常类来拥有自己的异常。异常类继承自Exception类, 可以直接继承或间接继承

        当创建一个模块有可能抛出多种不同的异常时, 一种通常的做法是为这个包建立一个基础异常类
        然后基于这个基础类为不同的错误情况创建不同的子类
"""
class Error(Exception):
    pass

class InputError(Error):
    def __init__(self, expression, message):
        self.expression = expression
        self.message = message

class TransitionError(Error):
    def __init__(self, first, second, third):
        self.first = first
        self.second = second
        self.third = third

def ownError_test():
    try:
        raise TransitionError("hello", "world", "the")
    except Error as e:
        print(e.first, e.second, e.third)



assert_test()










