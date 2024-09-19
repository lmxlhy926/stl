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


#########################################################################
"""
    扩展类重写基类的同名属性和方法
    
    如果基类和扩展类有同名的方法，创建扩展类对象时，默认调用的是扩展类的方法
    可以通过本质调用来调用基类的方法
"""
class base3():
    def __init__(self) -> None:
        print('base3__init__')
        self.name = 'base3'
    
    def info_print(self):
        print(f"base3_info_print: {self.name}")

    def info_print_base3(self):
        print(f"info_print_base3: {self.name}")


class base4():
    def __init__(self) -> None:
        print("base4__init__")
        self.name = 'base4'

    def info_print(self):
        print(f"base4_info_print: {self.name}")

    def info_print_base4(self):
        print(f"info_print_base4: {self.name}")


class extend34(base3, base4):
    def __init__(self) -> None:
        base3.__init__(self)
        base4.__init__(self)

    def info_print(self):
        print(f"extend34_info_print: {self.name}")

    def info_print_base_base3(self):
        base3.info_print(self)

    def info_print_base_base4(self):
        base4.info_print(self)


#########################################################################
"""
    通过类名调用方法
    super()的用法
"""
class base5():
    def __init__(self) -> None:
        print("base5__init__")

    def info_print(self):
        print("base5_info_print")


class extend5(base5):
    def __init__(self) -> None:
        #3种调用父类构造器的方法
        super().__init__()
        super(extend5, self).__init__()
        base5.__init__(self)


#########################################################################
"""
    多态:
        定义的变量类型和实际传送的变量的类型不同，实际传送的变量类型是定义类型的扩展类
        实际调用的扩展类对象的方法
"""
class inputDevice():
    def read(self):
        pass

class usbDevice(inputDevice):
    def read(self):
        print("usb read")

class diskDevice(inputDevice):
    def read(self):
        print("disk read")

class deviceUtil:
    def readInfoFromDevice(self, device):
        device.read()

def duotaiTest():
    usbDev = usbDevice()
    diskDev = diskDevice()
    util = deviceUtil()
    util.readInfoFromDevice(usbDev)
    util.readInfoFromDevice(diskDev)


#########################################################################
"""
    类方法、类属性
    使用类名修改属性，则该属性相当于静态变量，在所有的类对象中生效
"""
class dog():
    tooth = 100

    # 这里使用类名修改属性，则会在所有的对象中生效
    def changeTooth(self):
        dog.tooth = 200

    @classmethod
    def printTooth(self):
        print(f"tooth: {self.tooth}")

    #静态方法调用不需要创建对象，使用类名即可
    @staticmethod
    def info_print():
        print("这是一个静态方法")


def dogTest():
    d1 = dog()
    d1.printTooth()
    d2 = dog()
    d2.printTooth()

    # 使用类名修改属性，所有的对象中该属性被修改
    d1.changeTooth()

    d1.printTooth()
    d2.printTooth()


