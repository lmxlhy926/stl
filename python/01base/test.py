#! /usr/bin/python3

import sys


#创建字符串
str1 = "hello world"

#字符串单个字符索引
print(str1[0])
print(str1[-1])
print(str1[0:5])

#字符串遍历
for i in str1:
    print(i)

#字符串拼接
print("hello" + "world")
print("hello" * 2)

print("--------------")


#子字符串查找
print("ohello".find('o'))
print("ohello".rfind("lo"))


#子字符串替换
print("oheollo".replace("o", "**", 3))


#子字符串计数
print("ohello".count('o', 1))


#子字符串拆分
listvar = "he;ll;oo".split(";", 2)
print(listvar)


#子字符串拼接
liststr = ["he", "ll", "oo"]
print(";".join(listvar))


print("-------------------------------------------------")

str = "heLLow"

print(str.capitalize())
print(str.casefold())
print(str.center(10, " "))









