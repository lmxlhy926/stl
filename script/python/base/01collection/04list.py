import operator

'''
    1、List写在方括号之间,元素用逗号隔开。
    2、和字符串一样,list可以被索引和切片, 列表被截取后返回一个包含所需元素的新列表
    3、List可以使用'+'操作符进行拼接。
    4、List中的元素是可以改变的。
    5. List截取可以接收第三个参数,参数作用是截取的步长, 如果第三个参数为负数表示逆向读取
    6. List中元素的类型可以不相同,它支持数字，字符串甚至可以包含列表（所谓嵌套)
'''

list = ['abcd', 786, 2.23, 'runoob', 70.2]  #支持多种数据类型

# 索引、切片
print(list[0])              #列表第一个元素
print(list[1 : 3])          #列表第二个到到第三个元素
print(list[2:])             #列表第三个开始的所有元素
print("------------------------")


# 更新
list[0] = 100   #list中元素可被改变
print('id(list): {}, list: {}'.format(id(list), list))
#尾端插入
list.append(101)
print('id(list): {}, list: {}'.format(id(list), list))
# 删除元素
del list[-1]
print('id(list): {}, list: {}'.format(id(list), list))
print("------------------------")


# 表达式
print([1, 2, 3] + [4, 5, 6])    #列表拼接
print(["hi", "hello"] * 4)      #列表重复
print(3 in [1, 2, 3])           #成员判断
for x in [1, 2, 3] : print(x, end=" ")  #遍历数组
print("\n------------------------")


# 列表嵌套
a = [1, 2, 3]
b = [4, 5, 6]
c = [a, b]
print(c)
print("c[0][0]: {}".format(c[0][0]))
print("c[1][0]: {}".format(c[1][0]))
print("------------------------")


# 列表比较
a = [1, 2]
b = [2, 3]
c = [2, 3]
print("id(a): {}".format(id(a)))
print("id(b): {}".format(id(b)))
print("id(c): {}".format(id(c)))
print("operator.eq(a, b): {}".format(operator.eq(a, b)))
print("operator.eq(b, c): {}".format(operator.eq(b, c)))
print("---------------------------------------------")



"""
    list方法:
        属性获取：列表长度，元素计数、获取值对应的索引位置
        添加元素：尾端插入、指定位置插入
        删除元素: 按索引位置删除、按值删除、列表清空
        列表复制、列表排序
"""
list1 = [1, 1, 2, 3]
print("id(list1): {}, list1: {}".format(id(list1), list1))

# 列表长度
print("len(list1): {}".format(len(list1)))

# 元素计数
print("list1.count(1): {}".format(list1.count(1)))

# 尾端插入
list1.append(4)
print("list1.append(4): {}".format(list1))

list1.extend((5, 6, 7))
list1.extend([8, 9])
print("list1.extend((5, 6, 7)): {}".format(list1))

# 指定位置插入
list1.insert(0, 0)
print("list1.insert(0, 0): {}".format(list1))

# 删除指定的索引位置
list1.pop(-1)
print("list1.pop(-1): {}".format(list1))

# 删除匹配的第一个元素
list1.remove(1)
print("list1.remove(1): {}".format(list1))

# 查找匹配的元素的第一个位置
print("list1.index(2): {}".format(list1.index(2)))

#列表翻转
list1.reverse()
print("list1.reverse(): {}".format(list1))

#列表复制
list2 = list1.copy()
print("id(list1): {}, id(list2): {}".format(id(list1), id(list2)))

list.clear()
print("list.clear(): {}".format(list1.clear()))

