
'''
1、字典是一种映射类型, 它的元素是键值对。
2、字典的key必须为不可变类型, 如数字、字符串、元组。且不能重复。如果一个键被赋值2次, 后一个值会被记住。
3、创建空字典使用{ }。
'''

# 创建字典
dict = {}   #空字典
tinydict = {'name':'runoob', 'code':1, 'site':'www.runoob.com'}     #创建字典并初始化
print("dict: {}".format(dict))
print("tinydict: {}".format(tinydict))
print("-----------------------------")

#判断键是否在字典中
if "name" in tinydict:
    print("key 'name' is in tinydict...")
print("-----------------------------")

#添加元素
dict['one'] = "1 - 菜鸟教程"    #添加元素
dict[2] = "2 - 菜鸟工具"        #添加元素
print("添加元素：{}".format(dict))
print("-----------------------------")

#索引元素
print("索引元素: dict[2]: {}".format(dict[2]))
print("-----------------------------")

#修改元素
dict[2] = "2 - 菜鸟工具-修改"
print("修改元素: {}".format(dict))
print("-----------------------------")

#删除元素
del dict[2]
print("删除元素: {}".format(dict))
dict.clear()    #清空字典元素
del dict        #删除字典
print("-----------------------------")




"""
    视图对象：
        dict.keys()、dict.values()、dict.items()返回的都是视图对象, 提供了字典实体的动态视图
        这意味着字典改变, 视图也会跟着变化。不能对视图对象进行任何的修改, 因为字典的视图对象都是只读的。
        视图对象不是列表, 不支持索引, 可以使用list()来转换为列表
"""
dict = {}
dict["one"] = "one"           #第一个插入
dict["two"] = "two"           #第二个插入
dict["one"] = "one-修改"
dict["three"] = "three"       #第三个插入
#可遍历视图对象
items = dict.items()
keys = dict.keys()      
values = dict.values()
print("items: {}".format(items))
for item in items:
    print("     item: {}".format(item))
print("keys: {}".format(keys))
print("values: {}".format(values))
#将视图对象转换为列表，按插入的顺序
print("list(keys): {}".format(list(keys)))
print("list(values): {}".format(list(values)))
#视图对象是动态的，受字典变化影响，删除了字典的key，视图对象转换为列表后也跟着变化
del dict["one"]
print("list(keys): {}".format(list(keys)))
print("list(values): {}".format(list(values)))
print("-----------------------------")


"""
    dict.get(key[, value])
        key:    字典中要查找的键
        value:  可选。如果指定键的值不存在时, 返回该默认值。

    dict.setdefault(key, default=None)
        如果key在字典中, 返回对应的值。
        如果键不存在于字典中, 将会添加键并将值设为默认值, 并返回default, default默认值为None。
"""
dict = {"name": {"url":"www.runoob.com"}, "age": 27}
dict.setdefault("sex", "female")
print("age: {}".format(dict.get("age")))
print("sex: {}".format(dict.get("sex")))
print("sex: {}".format(dict.get("sex", "male")))
print("url: {}".format(dict.get("name", {}).get("url")))    #get()方法对嵌套字典的使用方法
print("-----------------------------")


"""
    pop(key[, default]): 从字典中删除key所对应的值, 返回被删除的值。
        key:        要删除的键
        default:    当Key不存在时返回的值

    如果key存在:    删除字典中对应的元素
    如果key不存在:  返回指定的默认值
    如果key不存在且默认值default没有指定:   触发KeyError异常
"""
site = {"1": "first", "2": "second"}
element = site.pop("1")
print("删除元素为：{}".format(element))
element = site.pop("1", "not exist")
print("删除元素为：{}".format(element))
print("-----------------------------")


"""
    popitem():
        返回最后插入键值对(key,value), 按照LIFO顺序规则, 即最末尾的键值对。
        在python3.7之前, popitem()方法删除并返回任意插入字典的键值对
"""
site = {1:2, 3: 4}
result = site.popitem()
print("返回值: {}".format(result))
print("site: {}".format(site))
site["nickname"] = "runoob"     #新插入的元素
result = site.popitem()
print("返回值: {}".format(result))
print("site: {}".format(site))









