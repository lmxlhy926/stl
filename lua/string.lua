#! /usr/local/bin/lua



-- 字符串大小写转换
stringUpper = "LUA"
stringLower = "lua"
print(string.upper(stringLower))
print(string.lower(stringUpper))

-- 字符串格式化
-- Lua提供string.format()函数来生成具有特定格式的字符串，由于格式字符串的存在，使得产生的长字符串可读性大大提高了。

-- string.format("%c", 83)                 -- 输出S
-- string.format("%+d", 17.0)              -- 输出+17
-- string.format("%05d", 17)               -- 输出00017
-- string.format("%o", 17)                 -- 输出21
-- string.format("%u", 3.14)               -- 输出3
-- string.format("%x", 13)                 -- 输出d
-- string.format("%X", 13)                 -- 输出D
-- string.format("%e", 1000)               -- 输出1.000000e+03
-- string.format("%E", 1000)               -- 输出1.000000E+03
-- string.format("%6.3f", 13)              -- 输出13.000
-- string.format("%q", "One\nTwo")         -- 输出"One\
--                                         -- 　　Two"
-- string.format("%s", "monkey")           -- 输出monkey
-- string.format("%10s", "monkey")         -- 输出    monkey
-- string.format("%5.3s", "monkey")        -- 输出  mon


print(string.format("%c", 97))              
print(string.format("%d", 17.0))
print(string.format("%+d", 17.0))
print(string.format("%5d", 17) )
print(string.format("%05d", 17) )
print(string.format("%o", 17))
print(string.format("%x", 13.0))
print(string.format("%X", 13))
print(string.format("%e", 1000))
print(string.format("%E", 1000))
print(string.format("%6.3f", 3))
print(string.format("%q", "One\nTwo"))
print(string.format("%q", "monkey"))
print(string.format("%s", "monkey"))
print(string.format("%10s", "monkey"))
print(string.format("%10.3s", "monkey"))
-- a
-- 17
-- +17
--    17
-- 00017
-- 21
-- d
-- D
-- 1.000000e+03
-- 1.000000E+03
--  3.000
-- "One\
-- Two"
-- "monkey"
-- monkey
--     monkey
--        mon














-- 字符串截取


-- 字符串查找与翻转


-- 模式匹配
























