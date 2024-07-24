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


dict1 = {i : i **2 for i in range(5)}
print(f"dict1: {dict1}")


counts = {"hp": 125, "dell": 201, "lenovo": 199}
dict2 = {key: value for key, value in counts.items() if value >= 200}
print(f"dict1: {dict2}")




