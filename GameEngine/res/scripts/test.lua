--v = transform:getPosition()
--
--function update()
--	v:setX(v:getX() + 0.1)
--	transform:setPosition(v)
--end

local aor1 = Math.Vector3(1, 0, 0)
function update()
	--print("I am test.lua")
	transform:rotate(aor1, (1 * 3.14159 / 180))
end