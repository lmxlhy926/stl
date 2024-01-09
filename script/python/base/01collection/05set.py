'''
    集合(set)是一个无序的不重复元素序列
    集合中的元素不会重复，并且可以进行交集、并集、差集等常见的集合操作
    可以使用大括号{}创建集合，元素之间用','分隔, 或者也可以使用set()函数创建集合
    创建一个空集合必须用set()而不是{}, 因为{}是用来创建一个空字典
'''
setempty = set()
print("空集合: setempty(): {}".format(setempty))
print("--------------------------")

set1 = {1, 2, 3}
print("打印元素: set1: {}".format(set1))
print("--------------------------")

# 复制集合
set2 = set1.copy()
print("id(set1): {}, id(set2): {}, set2: {}".format(id(set1), id(set2), set2))
print("--------------------------")

# 添加元素
set1.add(0)                     #添加一个元素，如果添加的元素在集合中已存在，则不进行任何操作。
set1.update((3, 4), (5, 6))     #参数可以是列表、元组、字典等
set1.update([7, 8])
print("添加元素: set1: {}".format(set1))
print("--------------------------")

# 移除元素
set1.remove(1)      # 如果元素不存在，则会发生错误
set1.discard(1)     # 如果元素不存在，不会发生错误
print("删除元素: set1: {}".format(set1))
print("--------------------------")

# 随机移除元素
set1.pop()
set1.pop()
print("随机删除: set1: {}".format(set1))
print("--------------------------")

# 成员测试
if 5 in set1:
    print("成员测试: 5 in set1")
print("--------------------------")

# 清空集合
set1.clear()
print("清空集合：{}".format(set1))
print("--------------------------")


"""
集合内置方法：
    交集
    并集
    差集
    并集 - 交集
    子集合判断
"""
# 交集：多个集合中共有的元素
x = {1, 2, 3}
y = {3, 4, 5}
z = {3, 6, 7}
print("x.intersection(y, z): {}".format(x.intersection(y, z)))
x.intersection_update(y, z)
print("x: {}".format(x))
print("--------------------------")

# 并集：多个集合的元素的汇总，重复的元素只出现一次
x = {1, 2, 3}
y = {3, 4, 5}
z = {3, 6, 7}
print("x.union(y, z): {}".format(x.union(y, z)))
print("--------------------------")


# 差集：a中去除ab交集后剩余的元素集合
x = {1, 2, 3}
y = {3, 4, 5}
print("x.difference(y): {}".format(x.difference(y)))
x.difference_update(y)
print("x: {}".format(x))
print("--------------------------")


# 2个集合中不重复元素的集合
x = {1, 2, 3}
y = {3, 4, 5}
print("x.symmetric_difference(y): {}".format(x.symmetric_difference(y)))
x.symmetric_difference_update(y)
print("x: {}".format(x))
print("--------------------------")


# 子集合
x = {1, 2, 3}
y = (1, 2, 3, 4, 5, 6)
print("x.issubset(y): {}".format(x.issubset(y)))
print("x.issuperset(y): {}".format(x.issuperset(y)))
print("--------------------------")


# 集合是否有相同的元素
x = {1, 2, 3}
y = {3, 4, 5}
print("x.isdisjoint(y): {}".format(x.isdisjoint(y)))



