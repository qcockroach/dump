lib1 = {}

-- as table field
lib1.foo = function(a, b)
    return a + b
end
lib1.goo = function(a, b)
    return a - b
end


-- as constructoro
lib2 = {
    foo = function(a, b)
        return a + b
    end,
    goo = function(a, b)
        return a - b
    end
}

-- another way to define function
lib3 = {}

function lib3.foo(a, b)
    return a + b;
end

function lib3.goo(a, b)
    return a - b
end




local fprt = function(a, b)
    return a * b
end

var = fprt(20, 4)
print(var)


local function func1(a, b)
    return a * b
end

var = func1(20, 4)
print(var)




