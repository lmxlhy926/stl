
"""
类：
    类的定义
    成员变量、成员函数
    权限访问符

    构造器、构造器
    运算符重载


"""

class student:
    #成员变量
    name=None
    grade=None
    hobby=None
    __sex=None

    #构造器
    def __init__(self, name:str = "defaultName", grade:int = 3, hobby:list = None, __sex:str = None) -> None:
        self.name = name
        self.grade = grade
        self.hobby = hobby
        self.__sex = __sex

    #析构器
    def __del__(self):
        print("------析构-------")

    #公共成员函数
    def show(self):
        print("---------")
        print("--name: {}".format(self.name))
        print("--grade: {}".format(self.grade))
        print("--hobby: {}".format(self.hobby))
        print("--__sex: {}".format(self.__sex))
        print("---------")

    #私有成员函数
    def __show_private(self):
        print("---------")
        print("--__sex: {}".format(self.__sex))
        print("---------")


    #运算符重载：将运算符与类方法关联起来，每个运算符对应一个指定的内置方法
    #python通过重写一些内置方法，实现了运算符的重载功能

    def __add__(self, other):
        print("__add__")
        pass

    def __sub__(self, other):
        print("__sub__")
        pass

    def __mul__(self, other):
        print("__mul__")
        pass

    def __truediv__(self, other):
        print("__truediv__")
        pass

    def __floordiv__(self, other):
        print("__floordiv__")
        pass

    def __mod__(self, other):
        print("__mod__")
        pass

    def __pow__(self, other:int):
        print("__pow__")
        pass


    #打印重载：只有print()、str()方法可以调用__str__()方法转换
    def __str__(self):
        return "**{}-{}-{}-{}**".format(self.name, self.grade, self.hobby, self.__sex)
    
    #__repr__()方法在多种操作下都能将对象转换为自定义的字符串形式
    def __repr__(self):
        return "__repr__()"
    
    # >=
    def __ge__(self, other):
        print("__ge__")

    
    """
        _getitem__()方法
            用于索引、切片操作,在对象执行索引、切片或者 for 迭代操作时,会自动调用该方法。
        __setitem__()方法
            索引赋值,在通过赋值语句给索引或者切片赋值时,可调用__setitem__()方法实现对序列对象的修改。
        __delitem__()方法
            当使用 del 关键字删除对象时,实质上会调用__delitem__()方法实现删除操作。
    """
    def __getitem__(self, data):
        print("__getitem__")

    def __setitem__(self, index, value):
        print("__setitem__")

    def __delitem__(self, index):
        print("__delitem__")



stu1 = student("stu1", 1, [1], "male")
stu2 = student("stu2", 2, [2], "female")

stu1 + stu2
stu1 - stu2
stu1 * stu2
stu1 / stu2
stu1 // stu2
stu1 % stu2
stu1 ** 2

print("-------------------------------")
print(stu1)
print(stu2)
str = repr(stu1)
print(str)
stu1 >= stu2

print("-------------------------------")

stu1[:]
stu1[2]
stu1[2] = 100


