#! /home/lhy/bin/python
'''
不可变类型: number, strings, tuples
可变类型:   list, set, dict
'''

# 变量名指向一个对象
# 不可变类型是指，对变量进行操作时，会生成一个新对象赋值给变量。
def nonChange(a):
    print(id(a))
    a += "a"
    print(id(a))
    print("after: {}".format(a))

def nonChangeTest():
    a = "hello"
    print(id(a))
    print("before: {}".format(a))
    nonChange(a)

# 可变类型是指: 对变量所指的对象操作时，在原对象上进行操作
def change(a):
    a.append(10)
    print("in call: {}".format(id(a)))
    print("in call a: {}".format(a))

def changeTest():
    a = [1, 2, 3]
    print("before call: {}".format(id(a)))
    change(a)
    print("after call: {}".format(id(a)))
    print("after change: {}".format(a))


'''
调用函数时可使用的正式参数类型
    必需参数
    关键字参数
    默认参数
    不定长参数
'''

#必需参数：以正确的顺序传入函数。调用时的数量必须和声明时的一样
def printme(str) : 
    print(str)
    return

printme((1, 2, 3))

# 关键字参数： 允许函数调用时参数的顺序与声明时不一致，因为Python解释器能够用参数名匹配参数值
def printinfo(name, age) : 
    print("name: ", name)
    print("age: ", age)

printinfo(age = 20, name = "peter")


#默认参数：如果没有传递参数，则会使用默认参数
def printInfo(name, age = 35) :
    print("name: ", name)
    print("age: ", age)

printInfo(name = "tom")
    

#不定长参数
#加了星号*的参数会以元组(tuple)的形式导入，存放所有未命名的变量参数
def printInfoVarTuple(arg1, *vartuple) : 
    print("output: ")
    print("  {}".format(arg1))
    print("  {}".format(vartuple))

printInfoVarTuple(50, 60, 70)


#加了2个星号**的参数会以字典的形式导入
def printInfoVarDict(arg1, **vardict) : 
    print("output: ")
    print("  {}".format(arg1))
    print("  {}".format(vardict))

printInfoVarDict(10, name = 'hello', age = "world")


#匿名函数: python使用lambda来创建匿名函数
# lambda [arg1 [, arg2, ...argn]] : expression
a = 100
x = lambda arg1, arg2 : arg1 + arg2 + 10 + a
print(x(1, 2))


# 定义处理函数
def add(a, b) : 
    return a + b + 100

# 函数作为参数: 
def handleData(func, a, b) :
    return func(a, b)

# 在Python中函数和变量一样
print(handleData(add, 2, 3))
print(handleData(x, 2, 3))
