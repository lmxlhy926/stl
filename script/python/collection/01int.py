#! /home/lhy/bin/python

# 数据类型是不允许改变的，这就意味着如果改变数字类型的值，将重新分配内存空间
# 以下实例在变量赋值时Number对象将被创建
# 整型(int), 浮点型(float)，负数(complex)

number_a = 0xFF     #16进制
number_b = 0o37     #8进制
print(number_a)
print(number_b)

# 显式转换
number = int("0xFF", 16)
print(number)

# 整除
print(7 // 2)
print(7.0 // 2)
print(7 // 2.0)
print(3.0 == 3)

# 乘方
print(2 ** 5)


# 数学函数
print(max(1, 2, 3))
print(min(1, 2, 3))



