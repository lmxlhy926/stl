#! /usr/bin/lua

-- 字符串大小写转换
print("-------------大小写转换-----------------")
stringUpper = "LUA"
stringLower = "lua"
print(string.upper(stringLower))
print(string.lower(stringUpper))


-- 字符串格式化
-- Lua提供string.format()函数来生成具有特定格式的字符串，由于格式字符串的存在，使得产生的长字符串可读性大大提高了。
-- 为进一步细化格式，可在%号添加参数，参数将以如下的顺序读入
    -- 符号：一个+号表示其后的数字转义符将让正数显示正号，默认情况下只有负数显示符号
    -- 占位符：一个0，在后面指定了字符串宽度时占位用，不填时默认占位符是空格
    -- 对齐标识：在指定了字串宽度时，默认为右对齐，增加-号可以改为左对齐
    -- 宽度数值：指定子串宽度
    -- 小数位数/字符串裁切：'.', 小数位数，字符串裁切

print("-------------格式化输出-----------------")
print(string.format("%c", 97))              --a                 
print(string.format("%d", 17.0))            --17
print(string.format("%+d", 17.0))           --+17
print(string.format("%5d", 17) )            --   17
print(string.format("%05d", 17) )           --00017
print(string.format("%o", 17))              --21
print(string.format("%x", 13.0))            --d
print(string.format("%X", 13))              --D
print(string.format("%e", 1000))            --1.000000e+03
print(string.format("%E", 1000))            --1.000000E+03
print(string.format("%6.3f", 3))            -- 3.000
print(string.format("%q", "monkey"))        --"monkey"
print(string.format("%s", "monkey"))        --monkey
print(string.format("%10s", "monkey"))      --    monkey
print(string.format("%10.3s", "monkey"))    --       mon

--  细化格式符号的使用: 正数添加+号，指定占位符，指定对其方式，指定最小输出宽度，指定小数位数/字符裁切数
print("-------------细化格式符号-----------------")
print(string.format("%+0-10.3f", 10.0))
print(string.format("%+0-10.3f", 1000.0))
print(string.format("%+0-10.3f", 10000.0))
print(string.format("%+0-10.3f", 100000.0))


-- 字符串截取
-- string.sub(s, i [, j])
    -- s：要截取的字符串
    -- i：截取开始位置
    -- j：截取结束位置，默认为-1，代表最后一个字符；最后一个字符的位置为-1，倒数第一个为-2，以此类推；
    -- 位置的前向指示从1开始，反向指示从-1开始；指示时从左指向右；

print("-------------字符串截取子串-----------------")
local sourceString = "helloworld"
-- 指定正向开头
print("1:", string.sub(sourceString, 1))
print("2:", string.sub(sourceString, 2))
print("3:", string.sub(sourceString, 3))

-- 指定反向开头
print("-1:", string.sub(sourceString, -1))
print("-2:", string.sub(sourceString, -2))
print("-3:", string.sub(sourceString, -3))

-- 正向指定子区间
print("1,1:", string.sub(sourceString, 1, 1))
print("1,2:", string.sub(sourceString, 1, 2))
print("1,3:", string.sub(sourceString, 1, 3))

-- 正向反向指定子区间
print("1,-1:", string.sub(sourceString, 1, -1))
print("2,-2:", string.sub(sourceString, 2, -2))
print("3,-3:", string.sub(sourceString, 3, -3))

-- 反向指定子区间
print("-1,-1:", string.sub(sourceString, -1, -1))
print("-2,-1:", string.sub(sourceString, -2, -1))
print("-3,-1:", string.sub(sourceString, -3, -1))

-- 反向正向指定子区间
print("-10,1:", string.sub(sourceString, -10, 1))
print("-9,3:", string.sub(sourceString, -9, 3))
print("-8,4:", string.sub(sourceString, -8, 4))

-- 越界输出完整字符串
print("-100,-1:", string.sub(sourceString, -100, -1))
print("1,100:", string.sub(sourceString, 1, 100))



-- 字符正则匹配
-- string.match(str, pattern, init)
    -- str:     源字符串
    -- pattern: 正则表达式
    -- init：   搜索起点
    -- 成功配对时，函数将返回配对表达式中的所有捕获结果；如果没有设置捕获标记，则返回整个配对字符串；
    -- 当没有成功配对时，返回nil。

-- string.find (str, substr, [init, [plain]])
    -- str:     源字符串
    -- substr： 待查找的子字符串
    -- init：   指定开始查找位置，可以前向指定，反向指定
    -- plain:   简单模式/正则模式
    -- 如果找到一个匹配的子串，则返回这个子串的起始索引和结束索引，不存在则返回nil。

-- string.gmatch(str, pattern)
    -- str:     源字符串
    -- pattern：正则表达式
    -- 返回一个迭代器函数，每一次调用这个函数，返回一个在字符串str找到的下一个符合pattern描述的子串。
    -- 如果参数pattern描述的字符串没有找到，迭代器函数返回nil

-- string.gsub(mainString,findString,replaceString,num)
    -- mainString:      要操作的字符串
    -- findString：     被替换的字符
    -- replaceString：  要替换的字符
    -- num：            替换次数（可以忽略，则全部替换）
    -- 返回替换后的字符串，以及替换的次数

print("-------------字符串正则匹配-----------------")

-- find: 只返回第一个匹配项的起始位置，结束位置；找不到则返回nil；
print("------>1")
local dateString = "Deadline is 30/05/1999, 11/11/1111, firm"
print(string.find("hello lua world", "luab"))
print(string.find(dateString, "%d%d/%d%d/%d%d%d%d"))
print(string.sub(dateString, string.find(dateString, "%d%d/%d%d/%d%d%d%d")))

-- match：执行的操作还是查找；返回第一个匹配项；可以通过分组指定返回的细分项
print("------>2")
print(string.match(dateString, "((%d%d)/(%d%d)/(%d%d%d%d))"))

-- gmatch
print("------>3")
for word in string.gmatch(dateString, "%d%d/(%d%d/%d%d%d%d)") do
    print(word)
end

-- string.gsub：替换所有匹配的项
print("------>4")
print((string.gsub("ss<aaaa>, ss<updown>!", "<%a*>", ".")))
print((string.gsub("Deadline is 30/05/1999, 11/11/1111, firm", "%d%d/%d%d/%d%d%d%d", "<>")))


--[[
正则表达式：
    匹配单个字符：  . 字符类 [abc] 
    锚定位置：      ^ $
    捕获分组：      ()
    指定次数：      ？+ *

    常见的字符类：
        . 数字
        . 小写字母
        . 大写字母
        . 字母
        . 数字字母
        . 空白字符
        . 标点符号字符
        . 可打印字符：数字字母 + 空白字符 + 标点符号字符
        . 控制字符
--]]

print(string.match("123abcse.?$123", "^[%d]+[%l]+%.%?%$[%d]+$"))
print(string.match("\ncba^", "\n[a^bc]*"))





