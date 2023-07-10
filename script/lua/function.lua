#! /usr/local/bin/lua

-- 在lua中，函数是对语句和表达式进行抽象的主要方法

-- 语句结尾不需要分号，函数不需要用大括号括起来
function max(num1, num2)
    if(num1 > num2)
    then
        local result = num1
    else
        local result = num2
    end
    return result
end


-- 变量的类型为函数
-- 匿名函数赋值给变量
myprint = function(param)
    print("这是打印函数 - ##", param, "##")
end

-- 函数作为函数的参数
function add(num1, num2, functionPrint)
    local result = num1 + num2
    functionPrint(result)
end

-- 函数传参时使用匿名函数
add(22, 33, function(param)
    print("这是打印函数 - ##", param, "##")
end
)
add(2, 5, myprint)


-- 多返回值： lua函数中，在return后列出要返回的值的列表即可返回多值
function maxIndexNum(a)
    local index = 1
    local max = a[index]
    for i, val in ipairs(a) do
        if val > max then
            max = val
            index = i
        end
    end
    return index, max
end
print(maxIndexNum({5, 6, 7}))


-- 可变参数
function addResult(...)
    local result = 0
    for index, val in ipairs({...}) do
        result = result + val
    end
    return result
end

-- print(addResult(1, 2, 3))

function average(...)
    local result = 0
    local arg = {...}
    for index, val in ipairs(arg) do
        result = result + val
    end
    print("总共传入 " .. #arg .. " 个数")
    return result / #arg
end
-- print("平均值为： ", average(1, 2, 3))


-- select('#', ...) 返回可变参数的长度
-- select(n, ...)   返回从起点n开始到结束位置的所有参数列表

function selectTest(...)
    local a = select(3, ...)    -- 变量a对应右边变量列表的第一个参数
    print(a)
    print(select(3, ...))       -- 打印所有列表参数
end

selectTest(1, 2, 3, 4, 5, 6)


function printArgs(...)
    for i = 1, select("#", ...) do -- 获取参数总数
        local arg = select(i, ...)  -- 读取参数， arg对应的是右边变量列表的第一个参数
        print("arg", arg)
    end
end

-- printArgs(1, 2, 3, 4)




