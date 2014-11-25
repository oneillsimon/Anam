local aor1 = Math.Vector3(0, 0, 0)

function move(dir, amount)
	-- This is from test.lua
	-- This shouldn't conflict with move in move.lua
end

function final_update(delta)
	-- This is from test.lua
	-- It should have a prefix and not
	-- be copied into the final_update function
end

function printHello()
	print("Hello World")
end

function add(n, m)
	return n + m
end

function update(delta)
	--printHello()
	--print(add(3, 4))
end