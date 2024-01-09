#! /home/lhy/bin/python

# 数据类型是不允许改变的，这就意味着如果改变数字类型的值，将重新分配内存空间
# 以下实例在变量赋值时Number对象将被创建
# 整型(int), 浮点型(float)，负数(complex)

number_bin = 0b11111111     #2进制
number_oct = 0o377          #8进制
number_hex = 0xFF           #16进制
print(number_bin)
print(number_oct)
print(number_hex)
print("---------------------")


# 显式转换
numberbin = int("0b11111111", 2)
numberoct = int("0o377", 8)
numberhex = int("0xFF", 16)
print(numberbin)
print(numberoct)
print(numberhex)
print("---------------------")


# 整除
print(7 // 2)
print(7.0 // 2)
print(7 // 2.0)
print("---------------------")


# 乘方
print(2 ** 5)
print("---------------------")


# 数学函数
print(max(1, 2, 3))
print(min(1, 2, 3))

