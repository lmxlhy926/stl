"""
    lambda本质上是一个匿名函数
"""

#无参数
fn0 = lambda : 100
print(fn0())
print("---------------------------------------------")


#一个参数
fn1 = lambda a : a
print(fn1("hello world"))
print("---------------------------------------------")


#默认参数
fn2 = lambda a, b, c = 100 : a + b + c
print(fn2(1, 2))
print(fn2(1, 2, 3))
print("---------------------------------------------")


#可变参数
fn3 = lambda *args : args
print(fn3(1))
print(fn3(1, 2))
print(fn3(1, 2, 3))
print("---------------------------------------------")


#可变参数
fn4 = lambda **args : args
print(fn4(name = "python"))
print(fn4(name = "python", age = 30))
print("---------------------------------------------")


students = [
    {"name": "tom", "age": 20},
    {"name": "rose", "age": 19},
    {"name": "jack", "age": 22}
]

students.sort(key = lambda x : x["name"])
print(students)

students.sort(key = lambda x : x["name"], reverse=True)
print(students)

students.sort(key = lambda x : x["age"])
print(students)
print("---------------------------------------------")


def sum_num(a, b, f):
    return f(a) + f(b)

print("abs: {}".format(sum_num(-1, -2, abs)))
print("round: {}".format(sum_num(1.1, 1.5, round)))
print("---------------------------------------------")


list1 = [1, 2, 3, 4, 5]

def func(x):
    return x ** 2

result = map(func, list1)
print(list(result))
print("---------------------------------------------")


import functools

list1 = [1, 2, 3, 4, 5]

def func(a, b) :
    return a + b

# 调用reduce, 作用：功能函数计算的结果和序列的下一个数据做累计计算
result = functools.reduce(func, list1)
print(result)

















