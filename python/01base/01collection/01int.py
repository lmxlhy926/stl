#! /home/lhy/bin/python

# 数据类型是不允许改变的，这就意味着如果改变数字类型的值，将重新分配内存空间
# 以下实例在变量赋值时Number对象将被创建
# 整型(int), 浮点型(float)，负数(complex)

# 数字的表示法
number_bin = 0b11111111     #2进制
number_oct = 0o377          #8进制
number_hex = 0xFF           #16进制


# 字符串转换为数字
numberbin = int("0b11111111", 2)
numberoct = int("0o377", 8)
numberhex = int("0xFF", 16)
int(12.34)
float(100)


# 数字的字符串表示
numinta = 8
print(f"str(numinta): {str(numinta)}")  #普通字符串
print(f"bin(numinta): {bin(numinta)}")  #二进制格式的字符串
print(f"oct(numinta): {oct(numinta)}")  #8进制格式的字符串
print(f"hex(numinta): {hex(numinta)}")  #16进制格式的字符串
print("------------------")


# 整除
print(7 // 2)
print(7.0 // 2)
print(7 // 2.0)
print(2 ** 5)

# 数学函数
print(max(1, 2, 3))
print(min(1, 2, 3))

