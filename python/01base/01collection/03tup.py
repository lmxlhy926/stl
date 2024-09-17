'''
    1、与字符串一样,元组的元素不能修改。
    2、元组也可以被索引和切片,方法一样。
    3、注意构造包含 0 或 1 个元素的元组的特殊语法规则。
    4、元组也可以使用'+'操作符进行拼接。
'''
#创建元组
tup1 = ()                   #0个元素
tup2 = (20,)                #1个元素
tup3 = (1, 2, 3, 4, 5, 6)   #多个元素
tup4 = (10)         #int类型
tup5 = ("aaa")      #str类型
print(type(tup1))   #打印类型


# 元组是不可以变类型
# 给元组重新赋值后，元组指向的变量发生变化。
tuple1 = (1, 2, 3)
print("id(tuple1): {}".format(id(tuple1)))
tuple1 = (2, 3, 4)
print("id(tuple1): {}".format(id(tuple1)))
print('----------------------------------------')


#索引元组元素
print("tup3[0]: {}".format(tup3[0]))
print("tup3[1]: {}".format(tup3[1]))
print('----------------------------------------')


#子元组
print("tup3[0:2]: {}".format(tup3[0:2]))
print("tup3[1:]: {}".format(tup3[1:]))
print('----------------------------------------')


#元组拼接
a = (1, 2, 3)
b = (4, 5, 6, 7)
print("a * 2: {}".format(a * 2))
print("a + b: {}".format(a + b))
print('----------------------------------------')


#成员运算符
if( 1 in a):
    print(" 1 in a ...")
if(0 not in a):
    print(" 0 not in a ...")
print('----------------------------------------')


#遍历元素
for x in a:
    print(x, end=" ")
print('\n----------------------------------------')


# 将可迭代系列转换为元组
string1 = "hello"
list2 = [1, 2, 3, 4]
tuple1 = tuple(string1)
tuple2 = tuple(list2)


#索引、计数
tuple3 = (10, 20, 30)
print("tuple3.count(10): {}".format(tuple3.count(10)))
print("tuple3.index(40): {}".format(tuple3.index(40)))  #没有会抛异常


# 操作元组的函数
print("len: {}".format(len((1, 2, 3))))
print("max: {}".format(max((1, 2, 3))))
print("min: {}".format(min(1, 2, 3)))
print('----------------------------------------')

