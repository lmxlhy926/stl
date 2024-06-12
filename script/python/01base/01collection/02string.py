
'''
    1、Python中的字符串有两种索引方式,从左往右以0开始,从右往左以-1开始。
    2、Python中的字符串不能改变。
    3、反斜杠可以用来转义,使用r可以让反斜杠不发生转义。
    4、字符串可以用'+'运算符连接在一起，用'*'运算符重复。
'''

#创建字符串
str = "0123456789"
print(str)                  #所有字符
print("-----------------------------\n")


#字符串是不可变类型，相同内容的字符串其实是一个对象，对象的索引ID是相同的
string1 = "helloworld"
string2 = "helloworld"
print("id(string1): {}".format(id(string1)))
print("id(string2): {}".format(id(string2)))
string1 = string1 + "hello"
print("id(string1): {}".format(id(string1)))
print("-----------------------------\n")


#单个字符索引
print(str[0])               #第一个字符
print(str[-1])              #最后一个字符
print("-----------------------------\n")


#子字符串
print(str[0:-1])            #指定范围，前闭后开
print(str[2:5])             #指定范围，前闭后开
print(str[2:])              #指定开头位置，默认延伸到最后
print(str[:8])              #指定结尾位置，默认延伸到最前
print(str[:])               #所有字符
print(str[1:5:2])           #指定前闭后开范围 + 步长
print("-----------------------------\n")


#字符串拼接
print(str + "你好")         #字符串拼接
print(str * 2)              #用*运算符指定重复次数
print("-----------------------------\n")


#转义、raw
print("hello\nrunoob")      #转义
print(r"hello\nrunoob")     #raw string(去除转义)
print("-----------------------------\n")


#字符串遍历
string1 = "helloworld"
for c in string1:
    print(" char: {}".format(c))
print("-----------------------------\n")


# 成员运算符in，not in : 判断子字符串是否在整体字符串中
if ('he' in string1) :
    print("he in string1")
if ("a" not in string1) :
    print("a not in string1")
print("-----------------------------\n")


# f-string
name = "runoob"
print(f"hello {name}")
print(f"hello {1 + 2}")


#字符串<----整数：指定转换的字符串格式
number_bin = bin(255)
number_oct = oct(255)
number_hex = hex(255)
print(number_bin)
print(number_oct)
print(number_hex)
print("-----------------------------\n")


