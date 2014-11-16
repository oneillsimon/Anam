--v = transform:getPosition()
--
--function update()
--	v:setX(v:getX() + 0.1)
--	transform:setPosition(v)
--end

local aor1 = Math.Vector3(0, 0, 1)
function update()
	--print("I am test.lua")
	transform:rotate(aor1, speed2)
end