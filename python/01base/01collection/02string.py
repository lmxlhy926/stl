
'''
    0、python中字符串类型以""或者''标识，两者之间可以相互嵌套、以此避免转义
    1、Python中的字符串有两种索引方式,从左往右以0开始,从右往左以-1开始。
    2、Python中的字符串属于不可变对象
    3、反斜杠可以用来转义,使用r可以让反斜杠不发生转义。
    4、字符串可以用'+'运算符连接在一起，用'*'运算符重复。
'''

#字符串是不可变类型，相同内容的字符串其实是一个对象，对象的索引ID是相同的
string1 = "helloworld"
string2 = "helloworld"
print("id(string1): {}, id(string2): {}".format(id(string1), id(string2)))
string1 = string1 + "hello" # 内容变化，重新分配索引ID
print("id(string1): {}".format(id(string1)))


#创建字符串
str = "0123456789" 
str = '0123456789' 
str = """hello     
world
from"""
str = '''hello
    world
from'''
#相互嵌套
str = "i'm tom"
str = 'i\'m tom'
str = 'he said "this is good"'
str = "he said \"this is good\""
#连接符
str = "hello" \
"world" 


str = "0123456789"
#单个字符索引
print(str[0])               #第一个字符
print(str[-1])              #最后一个字符
#子字符串
print(str[0:-1])            #指定范围，前闭后开
print(str[2:5])             #指定范围，前闭后开
print(str[2:])              #指定开头位置，默认延伸到最后
print(str[:8])              #指定结尾位置，默认延伸到最前
print(str[:])               #所有字符
print(str[1:5:2])           #指定前闭后开范围 + 步长
print(str[::2])             #全范围内，正向前进
print(str[-1::-2])          #全范围内：负向前进


#字符串拼接
print(str + "你好")         #字符串拼接
print(str * 2)              #用*运算符指定重复次数


#转义、raw
print("hello\nrunoob")      #转义
print(r"hello\nrunoob")     #raw string(去除转义)
# f-string
name = "runoob"
print(f"hello {name}")
print(f"hello {1 + 2}")


#字符串遍历
string1 = "helloworld"
for c in string1:
    print(" char: {}".format(c))


# 成员运算符in，not in : 判断子字符串是否在整体字符串中
if ('he' in string1) :
    print("he in string1")
if ("a" not in string1) :
    print("a not in string1")


"""
    查找子字符串    find
    计数子字符串    count
    子字符串替换    replace
    分割字符串      split    
    合并           join
""" 
mystr = "**** and **** and **** and"
# 查找到则返回相应索引，查找不到返回-1
print("---find 正向查找----")
print(mystr.find("and"))            #全范围查找
print(mystr.find("and", 8, -1))     #指定范围查找
print(mystr.rfind("and"))       #反向查找

print("-----count 计数-------")
print(mystr.count("and"))
print(mystr.count("and", 8))

print("-----字符串替换----------")
mystr = "abcd;cdef;ssd;1234"
new_str = mystr.replace(";", "***")     #无限替换
new_str = mystr.replace(";", "***", 1)  #指定最多替换次数

print("-----字符换分割----------")
list1 = mystr.split(";")            #全部分割
list1 = mystr.split(";", 2)         #指定次数分割
print("list1: {}".format(list1))

print("-----字符换拼接----------")
mylist = ["aa", "bb", "cc"]
new_str = ";".join(mylist)      #将列表的条目通过字符串拼接在一起



"""
常用方法
"""
mystr = "hello world world hello"
#字符串首字母大写
print(mystr.capitalize())
#字符串每个单词首字母大写
print(mystr.title())
#转换为大写
print(mystr.upper())
#转换为小写
print(mystr.lower())
mystr = "   hello   "
#删除左侧空白字符
print(mystr.lstrip())
#删除右侧空白字符
print(mystr.rstrip())
#删除所有空白字符
print(mystr.strip())


"""
 模式匹配：
"""
mystr = "hello world python"
print(mystr.startswith("hello"))
print(mystr.startswith("python"))
print(mystr.endswith("python"))
print(mystr.endswith("pythons"))
print(mystr.isalpha())
print(mystr.isnumeric())
print(mystr.isalnum())
print(mystr.isspace())

