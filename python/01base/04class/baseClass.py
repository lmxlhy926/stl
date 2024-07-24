
'''
    面向对象：
        类：
            类定义
            实例变量
            方法（方法里包含局部变量）
            
        对象: 类的实例化

        继承：实现代码复用

        方法重写：实现差异化


    类对象：
        类对象支持2种操作: 属性引用和实例化

        实例化：
            实例化时自动调用__init__()方法

        属性引用：
            obj.name
            类对象创建后，类命名空间中所有的命名都是有效属性名
            私有属性: __private_attrs
        

    类方法：
        在类内部, 使用def关键字来定义一个方法, 与一般函数定义不同, 类方法必须包括参数self, 且为第一个参数
        self代表的是类的实例
        私有方法: __private_method

    单继承：
        class DerivedClassName(BaseClassName):
            <stament-N>

    多继承：
        class DerivedClassName(Base1, Base2, Base3):
            <stament-N>

    方法重写：
        如果父类方法的功能不能满足你的要求, 你可以在子类中重写你父类的方法

    类的专有方法：
        __init__    : 构造函数
        __del__     : 析构函数
        __repr__    : 打印、转换
        __len__     : 获得长度
        __cmp__     : 比较运算
        __call__    : 函数调用
        __add__     : 加运算
        __sub__     : 减运算
        __mul__     : 乘运算
        __truediv__ : 除运算
        __mod__     : 求余运算
        __pow__     : 乘方
'''


"""
    类函数的使用：
        本质是第一个参数为类对象且命名空间为类名的函数
"""

class student:
    #公共成员变量
    grade=None
    hobby=None
    #私有成员变量
    __count=None

    #构造器
    def __init__(self, grade:int = 0, hobby:list = None, count:int = 1) -> None:
        self.grade = grade
        self.hobby = hobby
        self.__count = count

    #析构器
    def __del__(self):
        print("------析构-------")

    #公共成员函数
    def show(self):
        print("*********")
        print("--grade: {}".format(self.grade))
        print("--hobby: {}".format(self.hobby))
        print("--__count: {}".format(self.__count))
        print("*********")

    #私有成员函数
    def __show_private(self):
        print("*********")
        print("--__count: {}".format(self.__count))
        print("*********")


    #运算符重载：将运算符与类方法关联起来，每个运算符对应一个指定的内置方法
    #python通过重写一些内置方法，实现了运算符的重载功能
    def __add__(self, other):
        print("__add__")
        hobbys = self.hobby + other.hobby
        return student(self.grade + other.grade, hobbys, self.__count + other.__count)

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
    
    def __ge__(self, other):
        print("__ge__")


    #打印重载：只有print()、str()方法可以调用__str__()方法转换
    def __str__(self):
        return "**{}-{}-{}**".format(self.grade, self.hobby, self.__count)
    
    #__repr__()方法在多种操作下都能将对象转换为自定义的字符串形式
    def __repr__(self):
        return "__repr__()"
    
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
        print("self: {}, data: {}".format(self, data))

    def __setitem__(self, index, value):
        print("__setitem__")
        print("self:{}, index: {}, value: {}".format(self, index, value))

    def __delitem__(self, index):
        print("__delitem__")
        print("self: {}, index: {}".format(self, index))



stu1 = student(100, ["baseball"])
stu2 = student(200, ["basketball"])
print("-------------------------------")

#add
stu1 + stu2 
#sub
stu1 - stu2
#mul
stu1 * stu2
#truediv
stu1 / stu2
#floordiv
stu1 // stu2
#mod
stu1 % stu2
#pow
stu1 ** 2
#ge
stu1 >= stu2
print("-------------------------------")

print(stu1)
print(stu2)
str = repr(stu1)
print(str)
print("-------------------------------")

#__getitem__
stu1[1:3:2]
#__getitem__
stu1[2]
#__setitem__
stu1[2] = 100
#__delitem__
del stu1[2]
print("-------------------------------")


"""
    类函数的调用
"""
#匿名对象调用
student(1, ["base"]).show()
#对象调用
s = student(2, ["gitar"])
s.show()
#本质调用：函数 + 对象
student.show(s)
print("-------------------")


"""
类函数作为普通参数
    使用lambda转换参数个数
"""
def handleNoArg(func):
    func()

def handleMultiArg(func, arg):
    func(arg)


handleNoArg(lambda: s.show())
handleMultiArg(student.show, s)


