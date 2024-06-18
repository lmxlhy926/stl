#! /home/lhy/bin/python



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

