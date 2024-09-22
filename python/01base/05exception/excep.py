#! /usr/bin/python3
import sys
import os


"""
assert expression [, arguments]
        用于判断一个表达式, 在表达式条件为false的时候触发异常
"""
def assert_test():
    assert False, "条件不满足"


"""
    try
        执行代码
    except
        发生异常时执行的代码
    else
        没有发生异常时执行的代码
    finally
        不管有没有异常都会执行的代码
"""
def trycatch_test():
    for arg in sys.argv[1:]:
        try:
            f = open(arg, "r")
        except OSError as err:
            print("OSError: {}".format(err))
        except ValueError as err:
            print("ValueError: {}".format(err))
        except Exception as err:
            print(f"exception: {err}")
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
def raise_test():
    try:
        raise NameError("nameerror")
    except Exception as err:
        print(f"{err}")
        raise



"""
    用户自定义异常：
        通过创建一个新的异常类来拥有自己的异常。异常类继承自Exception类, 可以直接继承或间接继承
        当创建一个模块有可能抛出多种不同的异常时, 一种通常的做法是为这个包建立一个基础异常类
        然后基于这个基础类为不同的错误情况创建不同的子类
"""
class Error(Exception):
    def __str__(self) -> str:
        return super().__str__()


class InputError(Error):
    def __init__(self, expression, message):
        self.expression = expression
        self.message = message
    
    def __str__(self):
        return self.expression + ", " + self.message


class TransitionError(Error):
    def __init__(self, first, second, third):
        self.first = first
        self.second = second
        self.third = third

    def __str__(self):
        return "TransitionError: " + self.first + ", " + self.second + ", " + self.third


def ownExceptionError_test():
    try:
        raise TransitionError("hello", "world", "the")
    except Error as e:
        print(f"{e}")


"""
    with
        在处理文件对象时,使用with是一种很好的做法
        with语句实现原理建立在上下文管理器之上, 上下文管理器是一个实现__enter__和__exit__方法的类
        使用with语句确保在嵌套块的末尾调用__exit__方法

    在文件对象中定义了__enter__和__exit__方法,即文件对象也实现了上下文管理器,首先调用__enter__方法,
    然后执行with语句中的代码,最后调用__exit__方法。即使出现错误,也会调用__exit__方法,也就是关闭文件流。
"""
def with_test():
    with open("./myfile.txt", "w") as file:
        file.write("hello world")


trycatch_test()

