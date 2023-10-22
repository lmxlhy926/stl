
# 自动类型转换，python将较小的数据类型转换为较大的数据类型，已避免数据丢失
num_int = 123
num_flo = 1.23
num_new = num_int + num_flo
print("num_int 数据类型为: ", type(num_int))
print("num_flo 数据类型为: ", type(num_flo))
print("num_new 数据类型为: ", type(num_int + num_flo))

# 显示类型转换，用户将对象的数据类型转换为所需的数据类型
x = int(1)
y = int(2.8)
z = int("3")
print("x:", x, ", y:", y, " z:", z)

x = float(1)
y = float(2.8)
z = float("3")
w = float("4.2")











