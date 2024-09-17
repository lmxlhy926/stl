#! /home/lhy/bin/python
'''
    不可变类型: number, strings, tuples
    可变类型:   list, set, dict
'''
# 变量名指向一个对象
# 不可变类型是指，对变量进行操作时，会生成一个新对象赋值给变量。
def nonChange(a):
    print("before> id(a): {}, a: {}".format(id(a), a))
    a += "a"
    print("after> id(a): {}, a: {}".format(id(a), a))


# 可变类型是指: 对变量所指的对象操作时，在原对象上进行操作
def change(a):
    print("before> id(a): {}, a: {}".format(id(a), a))
    a.append(10)
    print("after> id(a): {}, a: {}".format(id(a), a))


'''
调用函数时可使用的正式参数类型
    必需参数
    关键字参数
    默认参数
    不定长参数
'''
#必需参数：以正确的顺序传入函数。调用时的数量必须和声明时的一样
def printCommon(str1, str2) : 
    print("str1: {}".format(str1))
    print("str2: {}".format(str2))
    return
printCommon("hello", "world")


#关键字参数： 允许函数调用时参数的顺序与声明时不一致，因为Python解释器能够用参数名匹配参数值
def printKey(name, age) : 
    print("name: ", name)
    print("age: ", age)
printKey(age = 20, name = "peter")


#默认参数：如果没有传递参数，则会使用默认参数
def printDefault(name, age = 35) :
    print("name: ", name)
    print("age: ", age)
printDefault(name = "tom")
   

#不定长参数
#加了星号*的参数会以元组(tuple)的形式导入，存放所有未命名的变量参数
def printVarTuple(arg1, *vartuple) : 
    print("arg1: {}".format(arg1))
    print("vars: {}".format(vartuple))
printVarTuple(50, 60, 70)


#加了2个星号**的参数会以字典的形式导入
def printVarDict(arg1, **vardict) : 
    print("arg1:  {}".format(arg1))
    print("vars:  {}".format(vardict))
printVarDict(10, name = {1, 2, 3}, age = [4, 5, 6], str = "hello")



"""
    函数作为参数:
        python中函数和变量是一样的
        lambda [arg1 [, arg2, ...argn]] : expression;   匿名函数: python使用lambda来创建匿名函数
"""
def add(a, b): 
    return a + b

def sub(a, b):
    return a - b

def multi(a, b):
    return a * b

# 从函数表中找到对应的函数，执行相应的函数
def handle(operation, table, arg1, arg2):
    if operation in table:
        print("result: {}".format(table.get(operation)(arg1, arg2)))

def funcTableTest():
    funcTable = {"add": add, "sub": sub, "multi": multi}
    funcTable["div"] = lambda arg1, arg2 : arg1 / arg2
    handle("div", funcTable, 100, 200)



"""
    命名空间：
        命名空间是从名称到对象的映射, 大部分的命名空间都是通过Python字典来实现的.
        命名空间提供了在项目中避免名字冲突的一种方法。各个命名空间是独立的，没有任何关系的，所以一个
        命名空间中不能有重名, 但是不同的命名空间是可以重名的而且没有任何影响。

    作用域：
        local:      函数的参数和函数中定义的变量。类中定义的变量。
        enclosing:  非局部非全局变量, 闭包函数外的函数
        global:     全局作用域: 函数、类、其它导入的模块、模块级的变量和常量
        built-in:   内建作用域: python语言内置的名称

    生命周期: 由作用域决定

    python中只有模块、类、以及函数(def、lambda)才会引入新的作用域
"""
#全局变量和局部变量的关系
total = 0   #total:全局变量
def sum(arg1, arg2):
    total = arg1 + arg2     #total:局部变量
    print("局部total: {}".format(total))
    return total

sum(10, 20)
print("全局total: {}".format(total))
print("-------------------------------")


#在函数中索引全局变量
num = 1
print("id(num): {}, num: {}".format(id(num), num))
def fun1():
    global num  # 使用global关键字声明
    num = 123
    print("id(num): {}, num: {}".format(id(num), num))

fun1()
print("id(num): {}, num: {}".format(id(num), num))
print("-------------------------------")


#索引非全局非局部变量
def outer():
    list = [1, 2, 3]
    print("outer--id(num): {}, num: {}".format(id(list), list))
    def inner():
        nonlocal list    #nonlocal关键字声明
        list.append(1) 
        print("inner--id(num): {}, num: {}".format(id(list), list))
    return inner

inner = outer()
inner()
inner()
inner()
print("-------------------------------")



"""
    lambda本质上是一个匿名函数
"""
#无参数
fn0 = lambda : 100
print(fn0())

#一个参数
fn1 = lambda a : a
print(fn1("hello world"))

#默认参数
fn2 = lambda a, b, c = 100 : a + b + c
print(fn2(1, 2))
print(fn2(1, 2, 3))

#可变参数
fn3 = lambda *args : args
print(fn3(1))
print(fn3(1, 2))
print(fn3(1, 2, 3))

#可变参数
fn4 = lambda **args : args
print(fn4(name = "python"))
print(fn4(name = "python", age = 30))

#排序
students = [
    {"name": "tom", "age": 20},
    {"name": "rose", "age": 19},
    {"name": "jack", "age": 22}
]
students.sort(key = lambda x : x["name"])
students.sort(key = lambda x : x["name"], reverse=True)
students.sort(key = lambda x : x["age"])

#函数作为普通参数
def sum_num(a, b, f):
    return f(a) + f(b)
print("abs: {}".format(sum_num(-1, -2, abs)))
print("round: {}".format(sum_num(1.1, 1.5, round)))


#操作可迭代对象
list1 = [1, 2, 3, 4, 5]
def func(x):
    return x ** 2
result = map(func, list1)
print(list(result))



"""
拆包：
    有序: tuple, list
    无序: set
    set: 变量存储的是key值
"""
def return_num():
    return (100, 200)

def return_dict():
    return {"b": "bbb", "a": "aaa"}

a, b = return_num()
print(f"a: {a}, b: {b}")

a, b = return_dict()
print(f"a: {a}, b: {b}")


