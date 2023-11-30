'''
    集合(set)是一个无序的不重复元素序列
    集合中的元素不会重复，并且可以进行交集、并集、差集等常见的集合操作
    可以使用大括号{}创建集合，元素之间用,分隔，或者也可以使用set()函数创建集合
'''

set1 = {1, 2, 3}

# 添加元素
set1.add(0)             #添加一个元素
set1.update([3, 4])     #参数可以是列表、元组、字典等
set1.update((5, 6))
print("set1: {}".format(set1))

# 移除元素
set1.remove(1)      # 如果元素不存在，则会发生错误
set1.discard(0)     # 如果元素不存在，不会发生错误
print("set1: {}".format(set1))

# 随机移除元素
set1.pop()
set1.pop()
print("set1: {}".format(set1))


