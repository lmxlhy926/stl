#! /usr/bin/python3
"""
    单继承：
        扩展类没有显示指定构造器，生成一个默认的空参构造器。且其会调用基类的空参构造器；
        扩展类如果指定了构造器，则不会生成默认的空参构造器
"""
class base():
    def __init__(self, num) -> None:
        print("base__init__(self, num)")
        self.num = num
        pass

    def __init__(self) -> None:
        print("base__init__(self)")
        self.num = 1
        pass
    
    def info_print(self):
        print(f"info_print: {self.num}")


# 没有指定构造器，默认会生成一个空参构造器；且其会调用基类的空参构造器
class extend(base):
    def __init__(self, num) -> None:
        super().__init__(self, num)
    pass

#########################################################################

"""
    多继承：
        扩展类没有显示指定构造器，会生成一个空参构造器，其默认调用第一个基类的空参构造器；
        多个基类含有同名方法，默认继承第一个基类的方法
"""
class base1():
    def __init__(self) -> None:
        print("base1__init__")
        self.str = 'base1'
        pass

    def info_print(self) -> None:
        print(f"base1: {self.str}")


class base2():
    def __init__(self) -> None:
        print("base2__init__")
        self.str = "base2"
        pass

    def info_print(self) -> None:
        print(f"base2: {self.str}")
        pass

class extend12(base1, base2):
    def __init__(self) -> None:
        base1.__init__(self)
        base2.__init__(self)
    pass







