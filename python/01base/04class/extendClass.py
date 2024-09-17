#! /usr/bin/python3
"""
继承
覆写
基类、扩展类成员访问
"""
class base:
    name = None
    age = None
    weight = None

    def __init__(self, name, age, weight) -> None:
        print("base__init__")
        self.name = name
        self.age = age
        self.weight = weight
        pass

    def __del__(self):
        print("base__del__")

    def show_base(self):
        print(f"base_show -> name: {self.name}, age: {self.age}, weight: {self.weight}")

    def showNameAttribute_base(self):
        print(f"id(name): {id(self.name)}")

    def show(self):
        print('base_show()')


class extend(base):
    name = None
    grade = None

    def __init__(self, name, age, weight, grade) -> None:
        print("extend__init__")
        super().__init__(name, age, weight)
        self.name = self.name + "_extend"
        self.grade = grade

    def __del__(self):
        base.__del__(self)
        print("extend__del__")

    def show_extend(self):
        print(f"extend_show -> name: {self.name}, age: {self.age}, weight: {self.weight}, grade: {self.grade}")

    def showNameAttribute_extend(self):
        print(f"id(name): {id(self.name)}")

    def show(self):
        print("extend_show()")
        base.show(self)
    

def test1():
    e = extend("peter", 25, 100, 3)
    e.showNameAttribute_base()
    e.showNameAttribute_extend()
    e.show_base()
    e.show_extend()
    e.show()


class Master():
    def __init__(self) ->None:
        print("Master__init__")
        self.kongfu = "Master"
    
    def showInfo(self):
        print(f"master_kongfu: {self.kongfu}")

class School():
    def __init__(self):
        print("School__init__")
        self.kongfu = "School"
    
    def showInfo(self):
        print(f"school_kongfu: {self.kongfu}")


"""
  继承类没有构造器，继承多个基类，默认调用第一个基类的构造器
"""
class Prentice(School, Master):
    pass


"""
    继承类有构造器，继承多个基类，默认不调用基类的构造器
"""
class Prentice_one(School, Master):
    def __init__(self) -> None:
        print("Prentice_one__init__")
        self.kongfu = "Prentice_one"

    def printInfo(self):
        print(f"Prentice_one_kongfu: {self.kongfu}")


class Prentice_two(School, Master):
    def __init__(self) -> None:
        print("Prentice_two__init__()")
        self.kongfu = "Prentice_two"

    def printInfo(self):
        self.__init__()
        print(f"运用<{self.kongfu}>制作煎饼果子---Prentice_two")

    """
        基类方法的几种调用方式
    """
    def make_school_cake(self):
        #1: 类名 + 函数 + 对象
        School.__init__(self)
        School.make_cake(self)

        #2: 类名 + 函数 + 对象  --- super(当前类名，self).函数()
        super(Prentice_two, self).__init__()
        super(Prentice_two, self).make_cake()

        #3: 函数：  如果是多继承，则这里的基类默认是继承的第一个基类
        super().__init__()
        super().make_cake()
    
    def make_master_cake(self):
        Master.__init__(self)
        Master.make_cake(self)


"""
    使用更本质的调用格式：函数名 + 对象
    首先创建一个对象，然后使用"函数名 + 对象"的调用格式
"""
def test3():
    p = Prentice_two()
    School.make_cake(p)
    Master.make_cake(p)


