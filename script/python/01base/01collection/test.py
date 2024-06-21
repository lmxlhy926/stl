
dict1 = {"name":"tom", "age": 20, "gender": "male"}

keys = dict1.keys()

for key in keys:
    print("key: {}".format(key))
print("----------------------")


values = dict1.values()
for value in values:
    print("value: {}".format(value))
print("----------------------")


items = dict1.items()
for item in items:
    print("item: {}".format(item))

for key, value in items:
    print(f"{key}={value}")


print("items: {}".format(items))
