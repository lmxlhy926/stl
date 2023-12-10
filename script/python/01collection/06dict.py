
'''
1、字典是一种映射类型, 它的元素是键值对。
2、字典的关键字必须为不可变类型, 且不能重复。
3、创建空字典使用 { }。
'''
dict = {}
dict['one'] = "1 - 菜鸟教程"
dict[2] = "2 - 菜鸟工具"
tinydict = {'name':'runoob', 'code':1, 'site':'www.runoob.com'}

print(dict['one'])
print(dict[2])
print(tinydict)
print(dict.keys())
print(dict.values())
