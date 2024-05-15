import arrow

# 获取时间戳
t = arrow.utcnow()
print(t.timestamp)


# 获取时间戳，并格式化为字符串
t = arrow.now()
s1 = t.format()
print(s1)
s2 = t.format("YYYY-MM-DD")
print(s2)


# 字符串转换为arrow，并格式化为其它格式的字符串
t = arrow.get("2019-12-31 11:30", "YYYY-MM-DD HH:mm")
s3 = t.format("YYYYMMDD")
print(s3)
