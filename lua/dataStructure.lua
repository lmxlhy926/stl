#! /usr/bin/lua

function arraytell()
--[[
    数组，就是相同数据类型的元素按照一定顺序排列的集合，可以是一维数组和多维数组。
    Lua数组的索引键值可以使用整数表示，数组的大小不是固定的
--]]

-- 一维数组是最简单的数组，其逻辑结果是线性表。
-- 默认下标从1开始；如果指定的索引没有值则返回nil；
    array = {"lua", "tutorial"}
    for i = 0, 2 do
        print(array[i])
    end
    print("--------------------------")

    array1 = {}
    for i = -2, 2 do
        array1[i] = i
    end
    for i = -2, 2 do
        print(array1[i])
    end
print("--------------------------")

-- 多维数组即数组中包含数组或一维数组的索引键对应一个数组
    array2 = {}
    for i=1, 3 do
        array2[i] = {}
        for j=1, 3 do
            array2[i][j] = i * j
            print(array2[i][j])
        end
    end
end



--[[
    lua变量有3种类型：全局变量、局部变量、表中的域 
    Lua中的变量全是全局变量，哪怕是语句块或是函数里，除非用local显示声明为局部变量。 
    局部变量的作用域从声明位置开始到所在语句块结束
    变量的默认值均为nil；
--]]






