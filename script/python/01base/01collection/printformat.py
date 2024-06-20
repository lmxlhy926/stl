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





