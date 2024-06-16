#! /home/lhy/bin/python

'''
    面向对象：
        类：
            类变量
            实例变量
            方法（方法里包含局部变量）
            
        对象: 类的实例化

        继承

        方法重写


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

    运算符重载
'''


"""
    类函数的使用：
        本质是第一个参数为类对象且命名空间为类名的函数
"""
class common:
    #public成员变量
    name = None
    age = None
    #private成员变量
    __weight__ = None

    #构造函数
    def __init__(self, name, age, weight):
        print("common __init__ ...")
        self.name = name
        self.age = age
        self.__weight__ = weight

    #析构函数
    def __del__(self):
        print("common __del__ ...")

    #成员函数
    def show(self):
        print("name: {}, age: {}".format(self.name, self.age))

#类函数直接调用
common("niming", None, None).show() #通过匿名对象调用
c = common("peter", 20, 70) 
c.show()        #通过对象调用
common.show(c)  #更本质的调用方法
print("--------------------")


#类函数作为参数传递
def handleNoArgFunc(func):
    func()

def handleOneArgFunc(func, arg):
    func(arg)

handleNoArgFunc(lambda: common.show(c))
handleOneArgFunc(common.show, c)
print("--------------------")


print("init -- id(c): {}".format(id(c)))
x = lambda : print("in lambda id(c): {}".format(id(c)))
c = 100     #此时没有任何变量引用common对象，因为对象被析构
print("after -- id(c): {}".format(id(c)))
x()     #python中lambda没有捕获机制，都是作用域访问
print("--------------------")




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


class student(people):  #单继承
    name = None         #派生类和基类的同名元素，但是不在一个命名空间下
    grade = ""

    def __init__(self, n, a, w, g) :
        people.__init__(self, n, a, w)
        self.name = n + "hello"     #优先从派生类中查找
        self.grade = g
        self.age += 5               #派生类中不存在时从基类中查找
           
    def speak(self) :
        super(student, self).speak()
        people.speak(self)
        print("%s 说：我 %d 岁了，我在读 %d 年级" %(self.name, self.age, self.grade))
    
s = student("name", 10, 20, 3)
s.speak()



class speaker :
    name = ""
    topic = ""
    
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

