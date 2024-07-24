
tupVar = (1, 2, 3)
listVar = [4, 5, 6]
setVar = {7, 8, 9}
dictVar = {"num": 100, "string": "hello"}

print(f"tuple(list):    {tuple(listVar)}")
print(f"tuple(set):     {tuple(setVar)}")
print(f"tuple(dict):    {tuple(dictVar)}")

print("--------------", end = "\n")

print(f"list(tuple):    {list(tupVar)}")
print(f"list(set):      {list(setVar)}")
print(f"list(dict):     {list(dictVar)}")

print("--------------", end = "\n")

tupString =     "(1, 2, 3)"
listString =    "[1, 2, 3]"
setString =     "{1, 2, 3}"
dictString =    '{"num": 100, "string": "hello"}'

print(f"eval(tupString):    {eval(tupString)},      type: {type(eval(tupString))}")
print(f"eval(listString):   {eval(listString)},     type: {type(eval(listString))}")
print(f"eval(setString):    {eval(setString)},      type: {type(eval(setString))}")
print(f"eval(dictString):   {eval(dictString)},     type: {type(eval(dictString))}")



