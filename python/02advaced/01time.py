
import time
"""
    获取时间戳：
        time.time(): 返回当前时间的时间戳

    时间戳--->时间元组
        time.localtime([secs]): 接收时间戳, 并返回当地时间下的时间元组t

    时间元组-->时间戳
        time.mktime(tupletime): 接受时间元组并返回时间戳

    时间元组->固定格式的时间字符串表示
        time.asctime([tupletime]): 接受时间元组并返回一个可读形式的字符串
        time.ctime([secs]): 作用相当于time.asctime(localtime(secs)), 未给参数相当于asctime()。

    时间元组--->指定格式的时间字符串表示
        time.strftime(fmt[, tupletime]): 接受格式字符串，时间元组, 返回可读字符串表示的当地时间。

    推迟调用线程的运行
        time.sleep(secs)
    
    返回计时器的精准时间(系统的运行时间), 包含整个系统的睡眠时间。结果之间的差才有效
        time.perf_counter()

    返回当前进程执行cpu的时间总和,不包含睡眠时间。结果之间的差才有效
        time.process_time()
"""

#获取时间戳
ticks = time.time()
print("当前时间戳为：{}".format(ticks))
print("------------------------")


#时间戳转换为本地时间
localtime = time.localtime(ticks)
print("time.localtime(ticks): {}".format(localtime))
print("time.localtime(): {}".format(time.localtime()))
print("------------------------")


#转换为特定的时间格式
print("time.asctime(localtime): {}".format(time.asctime(localtime)))
print("time.asctime(): {}".format(time.asctime()))
print("time.ctime(ticks): {}".format(time.ctime(ticks)))
print("time.ctime(): {}".format(time.ctime()))
print("------------------------")


#指定的时间格式
print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
print("------------------------")


system_time_start = time.perf_counter()
process_time_start = time.process_time()
time.sleep(5)
system_time_end = time.perf_counter()
process_time_end = time.process_time()
print("systime: {}".format(system_time_end - system_time_start))
print("processtime: {}".format(process_time_end - process_time_start))








