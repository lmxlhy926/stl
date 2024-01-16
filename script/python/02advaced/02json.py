import json
"""
    json::loads(): 加载json格式字符串, 转换为Python对象
    json::dumps(): 将python对象转换为json格式字符串
"""

data = {
    "name": "runoob",
    "url": "http://www.runoob.com",
    "int": 100
}
json_str = json.dumps(data)
print("json_str: {}".format(json_str))
print("-------------------")

jsonData = json.loads(json_str)
name = jsonData.get("name")
print("name: {}".format(name))
print("-------------------")


"""
    处理文件
"""
#从文件读取json数据
with open("data.json", "r") as f:
    data = json.load(f)
print("data: {}".format(data))

data["hello"] = 'nihao'
#向文件写入数据
with open("data.json", "w") as f:
    json.dump(data, f)















