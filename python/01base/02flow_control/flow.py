

"""
    if
"""
if True:
    pass


if True:
    pass
else:
    pass


if True:
    pass
elif True:
    pass
else:
    pass


"""
    while
"""
i = 1
while i < 10 :
    print("i: {}".format(i))
    i += 1
else:
    print("比较条件不满足, 正常导致的结束时, 会执行。如果循环里面break导致跳出循环, 则不会执行")



"""
    for
"""
list = [1, 2, 3, 4]
for listelement in list :
    print("listelement: {}".format(listelement))
else :
    print("list search end ...")


for i in range(5):
    print("i: {}".format(i))

for t in range(0, 5):
    print("t: {}".format(t))

for h in range(10, 1, -3) :
    print("h: {}".format(h))
