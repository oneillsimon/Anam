--v = transform:getPosition()
--
--function update()
--	v:setX(v:getX() + 0.1)
--	transform:setPosition(v)
--end

function printHello()
	print("Hello World")
end

function add(n, m)
	return n + m
end

local aor1 = Math.Vector3(0, 0, -1)

function input(delta)

	if input:getKey(KEY_V) then
		print("V key pressed")
	end
end

function update(delta)
	--printHello()
	--print(add(3, 4))
end