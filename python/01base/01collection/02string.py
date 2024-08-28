
'''
    0、python中字符串类型以""或者''标识，两者之间可以相互嵌套、以此避免转义
    1、Python中的字符串有两种索引方式,从左往右以0开始,从右往左以-1开始。
    2、Python中的字符串属于不可变对象
    3、反斜杠可以用来转义,使用r可以让反斜杠不发生转义。
    4、字符串可以用'+'运算符连接在一起，用'*'运算符重复。
'''
#创建字符串
str1 = "0123456789" #单引号
str2 = '0123456789' #双引号
#三引号
str3 = """hello     
world
from"""
str4 = '''hello
    world
from'''
#相互嵌套
str5 = "i'm tom"
str6 = 'i\'m tom'
str7 = 'he said "this is good"'
str8 = "he said \"this is good\""
#连接符
str9 = "hello" \
"world" 
print(f"str1: {str1}")
print(f"str2: {str2}")
print(f"str3: {str3}")
print(f"str4: {str4}")
print(f"str5: {str5}")
print(f"str6: {str6}")
print(f"str7: {str7}")
print(f"str8: {str8}")
print(f"str9: {str9}")
print("-----------------------------\n")


#字符串是不可变类型，相同内容的字符串其实是一个对象，对象的索引ID是相同的
string1 = "helloworld"
string2 = "helloworld"
print("id(string1): {}, id(string2): {}".format(id(string1), id(string2)))
string1 = string1 + "hello"
print("id(string1): {}".format(id(string1)))
print("-----------------------------\n")


str = "0123456789"
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
print(str[::2])             #全范围内，正向前进
print(str[-1::-2])          #全范围内：负向前进
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


"""
    查找子字符串、计数子字符串
"""
mystr = "**** and **** and **** and"
# 查找到则返回相应索引，查找不到返回-1
print("---find 正向查找----")
print(mystr.find("and"))            #全范围查找
print(mystr.find("and", 8, -1))     #指定范围查找
print(mystr.find("ands"))           #查找不到返回-1
# print(mystr.index("ands"))        #查找不到抛异常，其它效果和find一样

print("---find 反向查找---")
print(mystr.rfind("and"))       #反向查找
print(mystr.rfind("ands"))      
# print(mystr.rindex("ands"))     #查找不到则抛异常、其它和rfind效果一样

print("-----count 计数-------")
print(mystr.count("and"))
print(mystr.count("and", 8))



"""
    子字符串替换    replace
    分割字符串      split    
    合并           join
"""
print("-----字符串替换----------")
mystr = "abcd;cdef;ssd;1234"
new_str = mystr.replace(";", "***")     #无限替换
print(new_str)
new_str = mystr.replace(";", "***", 1)  #指定最多替换次数
print(new_str)


print("-----字符换分割----------")
list1 = mystr.split(";")            #全部分割
print("list1: {}".format(list1))
list1 = mystr.split(";", 2)         #指定次数分割
print("list1: {}".format(list1))

print("-----字符换拼接----------")
mylist = ["aa", "bb", "cc"]
new_str = ";".join(mylist)      #将列表的条目通过字符串拼接在一起
print("new_str: {}".format(new_str))



"""
    字符串首字母大写、字符串每个单词首字母大写、小写转大写、大写转小写、删除空格
"""
print("-------字符串常用功能-----------")
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
        以指定字符串开头
        以指定字符串结尾
        是否是全是字母
        是否全是数字
        是否全是字母或数字
        是否为空白
"""
print("--------模式匹配------------")
mystr = "hello world python"
print(mystr.startswith("hello"))
print(mystr.startswith("python"))

print(mystr.endswith("python"))
print(mystr.endswith("pythons"))

print(mystr.isalpha())
print(mystr.isnumeric())
print(mystr.isalnum())
print(mystr.isspace())


