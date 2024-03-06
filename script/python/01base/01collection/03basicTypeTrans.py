"""
基础类型之间的互相转化
    int
    float
    string
"""

# 整数和浮点数之间的相互转换
numinta =   int(12.34)
numfloat =  float(100)
print(f"numinta: {numinta}, numfloat: {numfloat}")
print("------------------")


# 整数--->字符串
numinta = 8
print(f"str(numinta): {str(numinta)}")  #普通字符串
print(f"bin(numinta): {bin(numinta)}")  #二进制格式的字符串
print(f"oct(numinta): {oct(numinta)}")  #8进制格式的字符串
print(f"hex(numinta): {hex(numinta)}")  #16进制格式的字符串
print("------------------")

#字符串--->整数
stringdecimal =     "8"
stringbinary =      "0b1000"
stringoct =         "0o10"
stringhex =         "0x8"
print(f"int(strindecimal, 10):  {int(stringdecimal, 10)}")
print(f"int(stringbinary, 2):   {int(stringbinary, 2)}")
print(f"int(stringoct, 8):      {int(stringoct, 8)}")
print(f"int(stringhex, 16):     {int(stringhex, 16)}")
print("------------------")


#浮点数字符串相互转换
numfloat = 12.34
stringfloat = 34.12
print(f"str(numfloat):      {str(numfloat)}")       #浮点数--->字符串
print(f"float(stringfloat): {float(stringfloat)}")  #字符串--->浮点数





