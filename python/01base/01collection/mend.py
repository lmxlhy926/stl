"""
可迭代对象的转换
"""
tupVar = (1, 2, 3)
listVar = [4, 5, 6]
setVar = {7, 8, 9}
dictVar = {"num": 100, "string": "hello"}

tuple(listVar)
tuple(setVar)
tuple(dictVar)

list(tupVar)
list(setVar)
list(dictVar)

tupString =     "(1, 2, 3)"
listString =    "[1, 2, 3]"
setString =     "{1, 2, 3}"
dictString =    '{"num": 100, "string": "hello"}'
print(f"eval(tupString):    {eval(tupString)},      type: {type(eval(tupString))}")
print(f"eval(listString):   {eval(listString)},     type: {type(eval(listString))}")
print(f"eval(setString):    {eval(setString)},      type: {type(eval(setString))}")
print(f"eval(dictString):   {eval(dictString)},     type: {type(eval(dictString))}")



"""
list, set, dict的推导式
"""
#简单推导式
list1 = [i for i in range(10)]
print(f"list1: {list1}")

#加条件判断
list2 = [i for i in range(10) if i % 2 == 0]
print(f"list2: {list2}")

#多个for循环推导式等同于for循环嵌套
list3 = [(i, j) for i in range(1, 3) for j in range(1, 4)]
print(f"list3: {list3}")

#字典
dict1 = {i : i **2 for i in range(5)}
print(f"dict1: {dict1}")

counts = {"hp": 125, "dell": 201, "lenovo": 199}
dict2 = {key: value for key, value in counts.items() if value >= 200}
print(f"dict1: {dict2}")



"""
    格式化符号输出数据
"""
#使用格式说明符
age = 18
name = 'TOM'
weight = 75.5
id1 = 1
id2 = 1000
print("age: %d" % age)
print("age: %d, name: %s, weight: %f, weight: %.3f" % (age, name, weight, weight))
print("id1: %d, id1: %06d, id2: %6d" % (id1, id1, id2))

# f'{表达式}'
print(f"age:{age}, name:{name}, weight{weight}")

# 使用.format格式
print("age: {}, name: {}, weight: {}".format(age, name, weight))

# print的末尾输出
print('hello', end="\n")
print('world', end="\t")
print('hello', end="...")
print('Python')

