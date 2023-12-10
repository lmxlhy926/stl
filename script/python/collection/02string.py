
'''
    1、反斜杠可以用来转义,使用r可以让反斜杠不发生转义。
    2、字符串可以用+运算符连接在一起，用*运算符重复。
    3、Python中的字符串有两种索引方式,从左往右以0开始,从右往左以-1开始。
    4、Python中的字符串不能改变。
'''

str = "0123456789"
print(str)                  #所有字符

# 单个字符
print(str[0])               #第一个字符
print(str[-1])              #最后一个字符

# 切片
print(str[0:-1])            #指定范围，前闭后开
print(str[2:5])             #指定范围，前闭后开
print(str[2:])              #指定开头位置，默认延伸到最后
print(str[:8])              #指定结尾位置，默认延伸到最前
print(str[:])               #所有字符
print(str[1:5:2])           #指定前闭后开范围 + 步长


print(str * 2)              #用*运算符指定重复次数
print(str + "你好")          #字符串拼接
print("hello\nrunoob")      #转义
print(r"hello\nrunoob")     #raw string

string1 = "helloworld"
string2 = "helloworld"
string3 = string2[1:2]
print(id(string1))
print(id(string2))
print(id(string3))
print("-----------------------------\n")


number_bin = bin(255)
number_oct = oct(255)
number_hex = hex(255)

print(bin(255))
print(oct(255))
print(hex(255))


# 成员运算符in，not in
str1 = "helloworld"
if ('h' in str1) :
    print("h in str1")
if ("a" not in str1) :
    print("a not in str1")


# f-string
name = "runoob"
print(f"hello {name}")
print(f"hello {1 + 2}")


