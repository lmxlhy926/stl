
'''
    1、字典是一种映射类型, 它的元素是键值对。
    2、字典的key必须为不可变类型, 如数字、字符串、元组。且不能重复。如果一个键被赋值2次, 后一个值会被记住。
    3、创建空字典使用{}。
'''
# 创建字典
dictempty = {}   #空字典
tinydict = {'name':'runoob', 'code':1, 'site': {"url": 'www.runoob.com'}}     #创建字典并初始化
print("dictempty: {}".format(dictempty))
print("tinydict: {}".format(tinydict))
print("-----------------------------")


#判断键是否在字典中
if "name" in tinydict:
    print("key 'name' is in tinydict...")
print("-----------------------------")


#索引元素
#get(key):  key不在字典中时，可以返回默认值None或者设置的默认值
#dict[key]: key不在字典中时，会触发KeyError异常
print("索引元素: tinydict['name']: {}".format(tinydict["name"]))
print("name: {}".format(tinydict.get("name")))
print("url: {}".format(tinydict.get("site").get("url")))    #get()方法对嵌套字典的使用方法
print("-----------------------------")


#添加元素
#dict.setdefault(key, default=None)
#      如果key在字典中, 返回对应的值。
#      如果键不存在于字典中, 将会添加键并将值设为默认值, 并返回default, default默认值为None。
tinydict['one'] = "1 - 菜鸟教程"    #添加元素
tinydict[2] = "2 - 菜鸟工具"        #添加元素
tinydict.setdefault(2, "None")
print("添加元素：{}".format(tinydict))
print("-----------------------------")


#修改元素
tinydict[2] = "2 - 菜鸟工具-修改"
print("修改元素: {}".format(tinydict))
print("-----------------------------")


#删除元素
#  pop(key[, default]): 从字典中删除key所对应的值, 返回被删除的值。
#     如果key存在:    删除字典中对应的元素
#     如果key不存在:  指定default则返回指定的默认值; 没有指定default则触发KeyError异常。
del tinydict[2]
print("tinydict.pop('one'): {}".format(tinydict.pop("one")))
print("tinydict.pop('two'): {}".format(tinydict.pop("one", "key two is not existed ....")))
print("删除元素: {}".format(tinydict))


#清空元素
tinydict.clear()    #清空字典元素
del tinydict        #删除字典
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
keys = dict.keys()      
values = dict.values()
items = dict.items()

print("keys: {}".format(keys))
print("values: {}".format(values))
print("items: {}".format(items))
for item in items:
    print("     item: {}".format(item))

#将视图对象转换为列表，按插入的顺序
print("list(keys): {}".format(list(keys)))
print("list(values): {}".format(list(values)))

#视图对象是动态的，受字典变化影响，删除了字典的key，视图对象转换为列表后也跟着变化
del dict["one"]
print("list(keys): {}".format(list(keys)))
print("list(values): {}".format(list(values)))
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



