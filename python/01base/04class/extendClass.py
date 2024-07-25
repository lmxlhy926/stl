

class person:
    name = None
    age = None
    weight = None

    def __init__(self, name, age, weight) -> None:
        print("---person 构造---")
        self.name = name
        self.age = age
        self.weight = weight
        print("person: id(name): {}".format(id(self.name)))
    
    def __del__(self):
        print("---person 析构---")

    def show(self):
        print("--person: name:{}, age:{}, weight:{}".format(self.name, self.age, self.weight))
    


class student(person):
    name = None
    grade = None

    def __init__(self, name, age, weight, grade) -> None:
        print("--student 构造")
        super().__init__(name, age, weight)
        # person.__init__(self, name, age, weight)
        self.name = name + "-student"
        self.grade = grade
        print("person: id(name):{}; student: id(name):{}".format(id(super().name), id(self.name)))

    def __del__(self):
        super().__del__()
        # person.__del__(self)
        print("--student 析构")
        

    def show(self):
        print("--student: name:{}, grade:{}".format(self.name, self.grade))


class Master():
    def __init__(self) ->None:
        print("Master: __init__")
        self.kongfu = "Master"
    
    def make_cake(self):
        print(f"运用<{self.kongfu}>制作煎饼果子---Master")

    def addition_func(self):
        print("additon function ...")


class School():
    def __init__(self):
        print("School: __init__")
        self.kongfu = "School"
    
    def make_cake(self):
        print(f"运用<{self.kongfu}>制作煎饼果子---School")

    
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
        print("Prentice_one: __init__()")
        self.kongfu = "Prentice_one"

    def make_cake(self):
        print(f"运用<{self.kongfu}>制作煎饼果子---Prentice_one")



class Prentice_two(School, Master):
    def __init__(self) -> None:
        print("Prentice_two: __init__()")
        self.kongfu = "Prentice_two"

    def make_cake(self):
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



def test1():
    p = Prentice()
    print(f"self.kongfu: {p.kongfu}")
    p.addition_func()


def test2():
    p = Prentice_one()
    print(f"self.kongfu: {p.kongfu}")
    p.addition_func()
    p.make_cake()
    # 打印类的继承关系
    print(Prentice_one.__mro__)


"""
    使用更本质的调用格式：函数名 + 对象
    首先创建一个对象，然后使用"函数名 + 对象"的调用格式
"""
def test3():
    p = Prentice_two()
    School.make_cake(p)
    Master.make_cake(p)




















