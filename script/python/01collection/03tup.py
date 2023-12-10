'''
    1、与字符串一样,元组的元素不能修改。
    2、元组也可以被索引和切片,方法一样。
    3、注意构造包含 0 或 1 个元素的元组的特殊语法规则。
    4、元组也可以使用+操作符进行拼接。
'''
tup1 = ()                   #0个元素
tup2 = (20,)                #1个元素
tup3 = (1, 2, 3, 4, 5, 6)   #多个元素
print(type(tup1))
print(tup1)
print(tup2)
print(tup3)
print('----------------------------------------')

# 元组中的元素值是不允许修改的

# 元组运算符
a = (1, 2, 3)
b = (4, 5, 6, 7)
print("length of a is {}".format(len(a)))
c = a + b
print("c: {}".format(c))
for x in b:
    print(x, end=" ")


# 元组内置函数
print("len: {}".format(len((1, 2, 3))))
print("max: {}".format(max((1, 2, 3))))
print("min: {}".format(min(1, 2, 3)))

# 将可迭代系列转换为元组
list1 = [1, 2, 3, 4]
tuple1 = tuple(list1)
print(type(tuple1))
print("tuple1: {}".format(tuple1))

# 元组是不可以变类型
# 给元组重新赋值后，元组指向的变量发生变化。
tuple1 = (1, 2, 3)
print("id(tuple1): {}".format(id(tuple1)))
tuple1 = (2, 3, 4)
print("id(tuple1): {}".format(id(tuple1)))
