import operator

'''
    1、List写在方括号之间,元素用逗号隔开。
    2、和字符串一样,list可以被索引和切片, 列表被截取后返回一个包含所需元素的新列表
    3、List可以使用'+'操作符进行拼接。
    4、List中的元素是可以改变的。
    5. List截取可以接收第三个参数,参数作用是截取的步长, 如果第三个参数为负数表示逆向读取
    6. List中元素的类型可以不相同,它支持数字，字符串甚至可以包含列表（所谓嵌套)
'''

#创建数组
list = ['abcd', 786, 2.23, 'runoob', 70.2]  #支持多种数据类型
print("id(list): {}, list: {}".format(id(list), list))
print("------------------------")

#列表复制
listcopy = list.copy()
print("id(list): {}, id(listcopy): {}".format(id(list), id(listcopy)))

#索引
print("list[0]: {}".format(list[0]))    #列表第一个元素
print("list[-1]: {}".format(list[-1]))  #列表最后一个元素
print("------------------------")

#更新
list[0] = 100   #list中元素可被改变
print('id(list): {}, list: {}'.format(id(list), list))

#插入
list.append(101)        #append函数追加数据的时候，如果数据是一个序列，则追加整个序列到列表结尾。
list.extend((1, 2))     #extend追加数据是一个序列，把数据序列里面的数据拆分，然后逐一追加到列表的结尾
list.extend([2, 3])
list.insert(0, 0)
print('id(list): {}, list: {}'.format(id(list), list))

#删除元素
del list[0]     #按索引删除
list.pop(-1)    #按索引删除, 返回删除的值
list.remove(2)  #按元素值删除（第一个匹配之的位置）
print('id(list): {}, list: {}'.format(id(list), list))
print("------------------------")


#子数组
print("list[1:3]: {}".format(list[1 : 3]))     #前闭后开
print("list[2:]: {}".format(list[2:]))       
print("------------------------")


#列表清空
list.clear()
print("list.clear(): {}".format(list.clear()))


#列表拼接
print(["hi", "hello"] * 4)      #列表重复
print([1, 2, 3] + [4, 5, 6])    #列表拼接
print("------------------------")


#成员属性判断符
if(1 in [1, 2, 3]):
    print("1 in [1, 2, 3]")
print("------------------------")


#遍历
for x in [1, 2, 3] :
   print("  element: {}".format(x))
print("\n------------------------")


# 列表嵌套
a = [1, 2, 3]
b = [4, 5, 6]
c = [a, b]
print("c: {}".format(c))
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
"""
list1 = [1, 1, 2, 3]
print("id(list1): {}, list1: {}".format(id(list1), list1))

#列表长度
print("len(list1): {}".format(len(list1)))

#元素计数
print("list1.count(1): {}".format(list1.count(1)))

#查找值所在的索引 （第一个匹配的位置）
print("list1.index(2): {}".format(list1.index(2)))

#列表翻转
list1.reverse()
print("id(list1): {}, list1.reverse(): {}".format(id(list1), list1))



