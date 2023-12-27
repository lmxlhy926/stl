#! /home/lhy/bin/python

'''
面向对象：
    类：
        类变量
        方法（方法里包含局部变量）
        实例变量
    
    对象: 类的实例化

    继承

    方法重写
'''

'''
类对象：
    类对象支持2种操作: 属性引用和实例化
    属性引用：
        obj.name
        类对象创建后，类命名空间中所有的命名都是有效属性名
        私有属性: __private_attrs
    实例化：
        实例化时自动调用__init__()方法

类方法：
    在类内部, 使用def关键字来定义一个方法, 与一般函数定义不同, 类方法必须包括参数self, 且为第一个参数
    self代表的是类的实例
    私有方法: __private_method

继承：
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

运算符重载
'''

class people:
    name = ""
    age = 0
    __weight = 0

    def __init__(self, n, a, w) :
        self.name = n
        self.age = a
        self.__weight = w
    
    def speak(self) : 
        print("%s 说： 我 %d 岁。" %(self.name, self.age))

class student(people) :
    grade = ""

    def __init__(self, n, a, w, g) :
        people.__init__(self, n, a, w)
        self.grade = g
    
    def speak(self) :
        print("%s 说：我 %d 岁了，我在读 %d 年级" %(self.name, self.age, self.grade))
    
class speaker :
    topic = ""
    name = ""

    def __init__(self, n, t) :
        self.name = n
        self.topic = t

    def speak(self) :
        print("我叫 %s, 我是一个演说家， 我演讲的主题是 %s" %(self.name, self.topic))

class sample(student, speaker) :
    a = ""
    def __init__(self, n, a, w, g, t) :
        speaker.__init__(self, n, t)
        student.__init__(self, n, a, w, g)

    def __speak(self) :
        print("----__speak(self) test----")


p = people(n = "runoob", a = 10, w = 30)
p.speak()

s = student(n = "ken", a = 10, w = 60, g = 3)
s.speak()

test = sample(n = "tim", a = 25, w = 80, g = 4, t = "python")
test.speak()


