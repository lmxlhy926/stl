#! /home/lhy/bin/python

import keyword
import sys
print(keyword.kwlist)


for i in sys.argv:
    print(i)

print("\n python 路径为： ", sys.path)

a = b = c = 1
a, b, c = 1, 2, "runoob"
print(a)
print(b)
print(c)





