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
function update(delta)
	a = add(2, 4)
	transform:rotate(aor1, (90 * 3.14 / 180.0) * delta)
end